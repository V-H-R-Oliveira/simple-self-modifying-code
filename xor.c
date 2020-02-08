#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#define SIZE 20

void changeProtect(void *addr, size_t memSize)
{
    int pagesize = getpagesize();
    addr -= ((unsigned long)addr % pagesize);
    
    if (mprotect(addr, memSize, PROT_EXEC | PROT_READ | PROT_WRITE) == -1)
    {
        perror("mprotec error:");
        exit(EXIT_FAILURE);
    }
}

void xor_obfuscation(unsigned char plaintext[])
{
    for (short i = 0; i < SIZE; ++i)
    {
        plaintext[i] ^= 0x1f;
    }
}

void runtime_patching(void *addr, size_t size, int key)
{
    unsigned char *instruction;

    for (unsigned char offset = 0; offset < size; ++offset)
    {
        instruction = (unsigned char *)addr+offset;

        if (offset == 45) {
            unsigned char *toPatch = instruction;
            *toPatch = key;
        }
    }
}

int main(void)
{
    unsigned char plaintext[] = "Self-modifying-code";
    size_t memSize = (void *)runtime_patching - (void *)xor_obfuscation;
    changeProtect(xor_obfuscation, memSize);
    
    puts("Before patching:");
    printf("%s\n", plaintext);
    xor_obfuscation(plaintext);
    printf("%s\n", plaintext);
    runtime_patching(xor_obfuscation, memSize, 0x3f);
    
    puts("After patching:");
    xor_obfuscation(plaintext);
    printf("%s\n", plaintext);
    xor_obfuscation(plaintext);
    printf("%s\n", plaintext);
    runtime_patching(xor_obfuscation, memSize, 0x1f);
    xor_obfuscation(plaintext);
    printf("%s\n", plaintext);
    
    return 0;
}
