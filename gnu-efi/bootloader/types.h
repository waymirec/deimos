#ifndef DEIMOS_TYPES_H
#define DEIMOS_TYPES_H

typedef unsigned long long size_t;

typedef struct {
    void* BaseAddress;
    size_t BufferSize;
    unsigned int HorizontalResolution;
    unsigned int VerticalResolution;
    unsigned int PixelsPerScanLine;
} FrameBuffer;

typedef struct {
    EFI_MEMORY_DESCRIPTOR* MemoryMap;
    UINTN MemoryMapSize;
    UINTN MemoryMapKey;
    UINTN MemoryMapDescriptorSize;
    UINT32 MemoryMapDescriptorVersion;
} MemoryInfo;

unsigned char PSF2_MAGIC[] = {0x72, 0xb5, 0x4a, 0x86};

// https://en.wikipedia.org/wiki/PC_Screen_Font
#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

typedef struct {
    unsigned char magic[2];
    unsigned char mode;
    unsigned char charSize;
} PSF1_HEADER;

typedef struct {
    PSF1_HEADER *header;
    void *glyphBuffer;
} PSF1_FONT;

typedef struct {
    uint32_t Count;
    uint32_t Size;
    uint32_t Height;
    uint32_t Width;
    void* Buffer;
} Font;

typedef struct {
    FrameBuffer* FrameBuffer;
    PSF1_FONT* Font;
    MemoryInfo* MemoryInfo;
} BootInfo;

#endif //DEIMOS_TYPES_H
