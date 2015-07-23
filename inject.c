#include <stdio.h>
#include <dlfcn.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

//Set this to 0 if you don't want to see log messages
const int PRINT_INFO = 1;

__attribute__((visibility("default")))
static int hookReplacementFunction() {
    printf("Calling replacement function!\n");
    return 3;
}

//Print the first 16 bytes after the given address
static void printBytes(int64_t *address) {
    for (int i = 0; i < 4; i++) {
        printf("*(address+%d):\t%08x\n", i * 4, htonl((int)*(address+i)));
    }
}

__attribute__((constructor))
static void ctor(void) {
    if (PRINT_INFO)
        printf("Injection dylib constructor called.\n");

    //"If filename is NULL, then the returned handle is for the main program."
    void *mainProgramHandle = dlopen(NULL, RTLD_NOW);

    //Get a pointer to the original function using dlsym
    int64_t *origFunc = dlsym(mainProgramHandle , "hookTargetFunction");
    if (PRINT_INFO)
        printf("Original function address: 0x%llx\n", (int64_t)origFunc);

    //Get a pointer to the replacement function
    int64_t *newFunc = (int64_t*)&hookReplacementFunction;
    if (PRINT_INFO)
        printf("Replacement function address: 0x%llx\n", (int64_t)newFunc);

    //Calculate the relative offset needed for the jump instruction
    //Since relative jumps are calculated from the address of the next instruction,
    //  5 bytes must be added to the original address (jump instruction is 5 bytes)
    int32_t offset = (int64_t)newFunc - ((int64_t)origFunc + 5 * sizeof(char));
    if (PRINT_INFO)
        printf("Offset: 0x%x\n", offset);

    //Make the memory containing the original funcion writable
    //Code from http://stackoverflow.com/questions/20381812/mprotect-always-returns-invalid-arguments
    size_t pageSize = sysconf(_SC_PAGESIZE);
    uintptr_t start = (uintptr_t)origFunc;
    uintptr_t end = start + 1;
    uintptr_t pageStart = start & -pageSize;
    mprotect((void *)pageStart, end - pageStart, PROT_READ | PROT_WRITE | PROT_EXEC);
    
    if (PRINT_INFO) {
        printf("Before replacement: \n");
        printBytes(origFunc);
    }

    //Set the first instruction of the original function to be a jump
    //  to the replacement function.
    //E9 is the x86 opcode for an unconditional relative jump
    //Htonl is used to flip the endian-ness
    *origFunc = htonl(0xe9000000) | offset << 8;

    if (PRINT_INFO) {
        printf("After replacement: \n");
        printBytes(origFunc);
    }
}
