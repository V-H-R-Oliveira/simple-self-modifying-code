# A very simple self-modifying code.

- It should be compiled with -O0.
- It patches the xor key in runtime.

To build:
- gcc -O0 -s -o xoring xor.c
