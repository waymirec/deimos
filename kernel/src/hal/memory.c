#include <hal/memory.h>

uint64_t system_memory_size(memory_info_t* memoryInfo)
{
    static uint64_t memorySizeBytes = 0;
    if (memorySizeBytes > 0) return memorySizeBytes;

    uint64_t count = memoryInfo->memory_map_size / memoryInfo->memory_map_descriptor_size;
    for (int i = 0; i < count; i++)
    {
        efi_memory_descriptor_t* desc = (efi_memory_descriptor_t*)((uint64_t)memoryInfo->memory_map + (i * memoryInfo->memory_map_descriptor_size));
        memorySizeBytes += desc->page_count * 4096;
    }

    return memorySizeBytes;
}