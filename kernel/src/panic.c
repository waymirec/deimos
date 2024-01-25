#include <panic.h>
#include <hal/console.h>
#include <stdio.h>

void panic(char *message)
{
    console_clear();
    printf("KERNEL PANIC: %s\n", message);
}