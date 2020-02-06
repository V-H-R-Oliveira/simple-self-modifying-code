#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#define SIZE 20

void changeProtect(void *addr)
{
    int pagesize = getpagesize();
    addr -= ((unsigned long)addr % pagesize);

    if (mprotect(addr, pagesize, PROT_EXEC | PROT_READ | PROT_WRITE) == -1)
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
            unsigned char *toPatch = (unsigned char *)addr+offset;
            *toPatch = key;
        }
    }
}

int main(void)
{
    unsigned char plaintext[] = "Self-modifying-code";
    
    changeProtect(xor_obfuscation);
    puts("The original string:");
    puts(plaintext);
    
    puts("After the first layer of obfuscation:");
    xor_obfuscation(plaintext);
    puts(plaintext);
    runtime_patching(xor_obfuscation, (void *)runtime_patching - (void *)xor_obfuscation, 0x3f);
    
    puts("After patching the key:");
    xor_obfuscation(plaintext);
    puts(plaintext);
    
    puts("After removing the second layer of obfuscation:");
    xor_obfuscation(plaintext);
    puts(plaintext);
    
    puts("The original string again:");
    runtime_patching(xor_obfuscation, (void *)runtime_patching - (void *)xor_obfuscation, 0x1f);
    xor_obfuscation(plaintext);
    puts(plaintext);
    
    return 0;
}
