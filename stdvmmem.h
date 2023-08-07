/*
Dan Nguyen
kngu7458
510594877
*/
#ifndef STDVMMEM
#define STDVMMEM

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define INST_MEM_BYTESIZE 1024
#define DATA_MEM_BYTESIZE 1024
#define VR_MEM_BYTESIZE 256
#define PADDING_BYTESIZE 44544
#define HEAPBANK_BYTESIZE 64

#define DATA_MEM_STARTBYTE 0x0400
#define VR_MEM_STARTBYTE 0x0800
#define PADDING_STARTBYTE 0x0900
#define HEAP_STARTBYTE 0xB700
#define END_OF_MEMORY 0xD700

#define N_HEAPBANKS 128
#define N_REGS 32

/*
VM's memory.
*/
extern const uint8_t MEM[];
/*
VM's registers.
*/
extern const int32_t REG[];
/*
VM's program counter.
*/
extern size_t PC;
/*
Block of heap bank(s).
*/
struct block {
    unsigned addr;
    size_t size;
    uint8_t n_banks;
    uint8_t is_alloc;
    struct block *next;
};

/*
Create a block of heap bank(s).
*/
extern struct block *block_init(uint32_t addr, size_t size, struct block *next);
/*
Allocate a block of heap bank(s) with sufficient size. Return -1 if head is
uninitialised. Else, return the starting address of the allocated memory if
allocation is successful. Otherwise, 0 is returned.
*/
extern uint32_t block_malloc(struct block *head, size_t size);
/*
Free a block of heap bank(s) starting at addr. Return 5 if head is
uninitialised. Else, return 2 if provided addr was not allocated or is not
valid, i.e., not a starting address. Otherwise, 0 is returned.
*/
extern int block_free(struct block *head, uint32_t addr);
/*
Check if the block at the current addr has been allocated. Return the size
of memory from addr to the end of block if addr is allocated, 0 otherwise.
*/
extern int is_alloc(struct block *head, uint32_t addr);
/*
Free all heap blocks (allocated or not).
*/
extern void heap_free(struct block *head);

#endif /* STDVMMEM */
