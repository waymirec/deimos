#include <efi.h>
#include <efilib.h>
#include <elf.h>
#include "types.h"

EFI_FILE_HANDLE GetVolume(EFI_HANDLE);
EFI_FILE* LoadFile(EFI_FILE*, CHAR16*, EFI_HANDLE);
int LoadElf(EFI_FILE*, Elf64_Ehdr*);
PSF1_FONT* LoadConsoleFont(EFI_FILE*, CHAR16*, EFI_HANDLE, EFI_SYSTEM_TABLE*);
EFI_FILE* LoadKernel(Elf64_Ehdr*, EFI_HANDLE);

int memcmp(const void*, const void*, size_t);
int VerifyElfHeader(Elf64_Ehdr*);
FrameBuffer* GetFrameBuffer(EFI_SYSTEM_TABLE *systemTable);
MemoryInfo* GetMemoryInfo(EFI_SYSTEM_TABLE *systemTable);

EFI_STATUS efi_main (EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable) {
    InitializeLib(imageHandle, systemTable);

    Elf64_Ehdr header;
    EFI_FILE* kernel = LoadKernel(&header, imageHandle);
    if (kernel == NULL) {
        return EFI_LOAD_ERROR;
    }

    PSF1_FONT* font = LoadConsoleFont(NULL, L"console.psf", imageHandle, systemTable);
    if (font == NULL) {
        return EFI_LOAD_ERROR;
    }

    FrameBuffer* frameBuffer = GetFrameBuffer(systemTable);
    if (frameBuffer == NULL) {
        Print(L"Unable to obtain framebuffer");
        return EFI_LOAD_ERROR;
    }

    MemoryInfo* memoryInfo = GetMemoryInfo(systemTable);
    
    BootInfo bootInfo;
    bootInfo.FrameBuffer = frameBuffer;
    bootInfo.MemoryInfo = memoryInfo;
    bootInfo.Font = font;

    Print(L"Calling kernel...\n\r");

    systemTable->BootServices->ExitBootServices(imageHandle, memoryInfo->MemoryMapKey);
    systemTable->RuntimeServices->SetVirtualAddressMap(memoryInfo->MemoryMapSize,
                                                       memoryInfo->MemoryMapDescriptorSize,
                                                       memoryInfo->MemoryMapDescriptorVersion,
                                                       memoryInfo->MemoryMap);

    void (*KernelStart)(BootInfo*) = (__attribute__((sysv_abi)) void (*)(BootInfo*)) header.e_entry;
    KernelStart(&bootInfo);

    //Print(L"Kernel returned\n\r");

    return EFI_SUCCESS; // Exit the UEFI application
}

EFI_FILE_HANDLE GetVolume(EFI_HANDLE imageHandle)
{
    EFI_LOADED_IMAGE *loadedImage = NULL;                   /* image interface */
    EFI_GUID lipGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;      /* image interface GUID*/
    EFI_FILE_IO_INTERFACE *ioVolume;                        /* file system interface */
    EFI_GUID fsGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID; /* file system interface GUID */
    EFI_FILE_HANDLE volume;

    /* get the loaded image protocol interface for our "image" */
    BS->HandleProtocol(imageHandle, &lipGuid, (void **) &loadedImage);
    BS->HandleProtocol(loadedImage->DeviceHandle, &fsGuid, (void*) &ioVolume);
    ioVolume->OpenVolume(ioVolume, &volume);

    return volume;
}

int LoadElf(EFI_FILE* file, Elf64_Ehdr* header)
{
    UINTN FileInfoSize;
    EFI_FILE_INFO* FileInfo;
    file->GetInfo(file, &gEfiFileInfoGuid, &FileInfoSize, NULL);
    BS->AllocatePool(EfiLoaderData, FileInfoSize, (void **) &FileInfo);
    file->GetInfo(file, &gEfiFileInfoGuid, &FileInfoSize, (void **) &FileInfo);
    UINTN size = sizeof(Elf64_Ehdr);
    file->Read(file, &size, header);

    if (!VerifyElfHeader(header))
    {
        Print(L"ERROR: Elf header is malformed\n\r");
        return 1;
    }

    UINTN headerTableSize = header->e_phnum * header->e_phentsize;
    Elf64_Phdr* pHeaders;
    {
        file->SetPosition(file, header->e_phoff);
        BS->AllocatePool(EfiLoaderData, headerTableSize, (void **)&pHeaders);
        file->Read(file, &headerTableSize, pHeaders);
    }


    Elf64_Phdr* pHeader;
    for (Elf64_Half i = 0; i < header->e_phnum; i++)
    {
        pHeader = (Elf64_Phdr *)((char *)pHeaders + (header->e_phentsize * i));
        switch(pHeader->p_type)
        {
            case PT_LOAD:
            {
                uint64_t pages = (pHeader->p_memsz + 0x1000 -1) / 0x1000;
                Elf64_Addr segment = pHeader->p_paddr;
                BS->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);
                file->SetPosition(file, pHeader->p_offset);
                UINTN size = pHeader->p_filesz;
                file->Read(file, &size, (void *)segment);
                break;
            }
        }
    }
    
    return 0;
}

EFI_FILE* LoadKernel(Elf64_Ehdr* header, EFI_HANDLE imageHandle)
{
    EFI_FILE* kernel = LoadFile(NULL, L"kernel.elf", imageHandle);
    if (kernel == NULL)
    {
        Print(L"Error: Unable to open kernel.\r\n");
        return NULL;
    }
    Print(L"Kernel opened successfully.\r\n");

    int result = LoadElf(kernel, header);
    if (result != 0)
    {
        Print(L"Error: unable to load kernel.\r\n");
        return NULL;
    }
    
    Print(L"Kernel loaded.\n\r");
    return kernel;
}

PSF1_FONT* LoadConsoleFont(EFI_FILE* directory, CHAR16* path, EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
    EFI_FILE* file = LoadFile(directory, path, imageHandle);
    if (file == NULL) {
        return NULL;
    }

    PSF1_HEADER* header;
    UINTN headerSize = sizeof(PSF1_HEADER);
    systemTable->BootServices->AllocatePool(EfiLoaderData, headerSize, (void **)&header);
    file->Read(file, &headerSize, header);

    if (header->magic[0] != PSF1_MAGIC0 || header->magic[1] != PSF1_MAGIC1)
    {
        Print(L"Invalid Font File\n\r");
        return NULL;
    }

    UINTN count = header->mode == 1 ? 512 : 256;
    UINTN bufferSize = header->charSize * count;
    void *buffer;
    {
        file->SetPosition(file, headerSize);
        systemTable->BootServices->AllocatePool(EfiLoaderData, bufferSize, (void **)&buffer);
        file->Read(file, &bufferSize, buffer);
    }

    PSF1_FONT* font;
    systemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_FONT), (void **)&font);
    font->header = header;
    font->glyphBuffer = buffer;

    Print(L"  Font: %s\n\r", path);
    Print(L"  Size: %d\n\r", font->header->charSize);
    return font;

    /*
    Font* font;
    BS->AllocatePool(EfiLoaderData, sizeof(Font), (void **)&font);
    font->Count = count;
    font->Size = header->charSize;
    font->Height = header->charSize;
    font->Width = 8;
    font->Buffer = buffer;

    Print(L"  Font: %s\n\r", path);
    Print(L" Width: %d\n\r", font->Width);
    Print(L"Height: %d\n\r", font->Height);
    Print(L"  Size: %d\n\r", font->Size);
    return font;
    */
}

EFI_FILE* LoadFile(EFI_FILE* directory, CHAR16* path, EFI_HANDLE imageHandle)
{
    EFI_FILE* loadedFile;
    EFI_LOADED_IMAGE_PROTOCOL* loadedImage;
    BS->HandleProtocol(imageHandle, &gEfiLoadedImageProtocolGuid, (void **)&loadedImage);

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* fileSystem;
    BS->HandleProtocol(loadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void **) &fileSystem);
    if (directory == NULL)
    {
        fileSystem->OpenVolume(fileSystem, &directory);
    }

    EFI_STATUS fileLoadStatus = directory->Open(directory, &loadedFile, path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
    if (EFI_ERROR(fileLoadStatus))
    {
        return NULL;
    }
    return loadedFile;
}

int VerifyElfHeader(Elf64_Ehdr* header)
{
    int result = (memcmp(&header->e_ident[EI_MAG0], ELFMAG, SELFMAG) == 0);
    result = result && header->e_ident[EI_CLASS] == ELFCLASS64;
    result = result && header->e_ident[EI_DATA] == ELFDATA2LSB;
    result = result && header->e_type == ET_EXEC;
    result = result && header->e_machine == EM_X86_64;
    result = result && header->e_version == EV_CURRENT;
    return result;
}


int memcmp(const void* ptr1, const void* ptr2, size_t length)
{
    const unsigned char* p1 = ptr1;
    const unsigned char* p2 = ptr2;
    for(size_t i = 0; i < length; i++)
    {
        if (p1[i] < p2[i]) return -1;
        if (p1[i] > p2[i]) return 1;
    }
    return 0;
}

FrameBuffer* GetFrameBuffer(EFI_SYSTEM_TABLE *systemTable)
{
    Print(L"Initializing graphics output\n\r");
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
    EFI_STATUS status;

    status = BS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (void **)&gop);
    if (EFI_ERROR(status))
    {
        Print(L"ERROR: Unable to initialize graphics.\n\r");
        return NULL;
    }

    Print(L"Successfully initialized graphics output\n\r");

    Print(L"Framebuffer:\n\r");
    Print(L"-- Base: 0x%x\n\r", (void *)gop->Mode->FrameBufferBase);
    Print(L"-- Size: %d\n\r", gop->Mode->FrameBufferSize);
    Print(L"-- Width: %d\n\r", gop->Mode->Info->HorizontalResolution);
    Print(L"-- Height: %d\n\r", gop->Mode->Info->VerticalResolution);
    Print(L"-- PPSL: %d\n\r", gop->Mode->Info->PixelsPerScanLine);

    FrameBuffer* frameBuffer;
    systemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(FrameBuffer), (void **)&frameBuffer);
    frameBuffer->BaseAddress = (void *)gop->Mode->FrameBufferBase;
    frameBuffer->BufferSize = gop->Mode->FrameBufferSize;
    frameBuffer->HorizontalResolution = gop->Mode->Info->HorizontalResolution;
    frameBuffer->VerticalResolution = gop->Mode->Info->VerticalResolution;
    frameBuffer->PixelsPerScanLine = gop->Mode->Info->PixelsPerScanLine;
    return frameBuffer;
}

MemoryInfo* GetMemoryInfo(EFI_SYSTEM_TABLE *systemTable)
{
    EFI_MEMORY_DESCRIPTOR* map = NULL;
    UINTN size, key, descriptorSize;
    UINT32 descriptorVersion;
    {
        systemTable->BootServices->GetMemoryMap(&size, map, &key, &descriptorSize, &descriptorVersion);
        systemTable->BootServices->AllocatePool(EfiLoaderData, size, (void **)&map);
        systemTable->BootServices->GetMemoryMap(&size, map, &key, &descriptorSize, &descriptorVersion);
    }
    
    MemoryInfo* memoryInfo;
    systemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(MemoryInfo), (void **)&memoryInfo);
    memoryInfo->MemoryMap = map;
    memoryInfo->MemoryMapSize = size;
    memoryInfo->MemoryMapKey = key;
    memoryInfo->MemoryMapDescriptorSize = descriptorSize;
    memoryInfo->MemoryMapDescriptorVersion = descriptorVersion;
    return memoryInfo;
}

