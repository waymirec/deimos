#ifndef DEIMOS_MEMORY_H
#define DEIMOS_MEMORY_H

#include <stdint.h>
#include <hal/efimem.h>

uint64_t system_memory_size(memory_info_t* memoryInfo);

#endif //DEIMOS_MEMORY_H