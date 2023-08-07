/*
Dan Nguyen
kngu7458
510594877
*/
#include "stdvmmem.h"

/*
Merge a free block with its next block. Both blocks must be unallocated.
*/
void __block_merge(struct block *block);

struct block *block_init(uint32_t addr, size_t size, struct block *next)
{
    struct block *block = malloc(sizeof (struct block));
    block->addr = addr;
    block->size = size;
    uint8_t n_banks = size/HEAPBANK_BYTESIZE;
    n_banks += (size%HEAPBANK_BYTESIZE > 0) ? 1 : 0;
    block->n_banks = n_banks;
    block->is_alloc = 0;
    block->next = next;
    return block;
}

uint32_t block_malloc(struct block *head, size_t size)
{
    if (head == NULL)
        return -1;
    if (size == 0)
        return 0;
    // Traverse the heap until an unallocated block of sufficient
    // size is found
    while (head != NULL && (head->is_alloc || head->size < size))
        head = head->next;
    // If the end of the heap is reached then no blocks of
    // sufficient size can be found
    if (head == NULL)
        return 0;
    
    size_t n_banks = size/HEAPBANK_BYTESIZE;
    n_banks += (size%HEAPBANK_BYTESIZE > 0) ? 1 : 0;
    // If malloc request does not use up all of the free memory,
    // initialise a new block with the remaining free memory
    if (n_banks < head->n_banks) {
        struct block *free_block = block_init(
            head->addr+(n_banks*HEAPBANK_BYTESIZE),
            (head->n_banks-n_banks)*HEAPBANK_BYTESIZE,
            head->next
        );
        head->next = free_block;
    }
    // Allocate and update the current block
    head->size = size;
    head->n_banks = n_banks;
    head->is_alloc = 1;
    // Return the address of the malloced block
    return head->addr;
}

int block_free(struct block *head, uint32_t addr)
{
    if (head == NULL)
        return 4;
    // If the head block is being freed, then we only need to check
    // for merging with the next block
    if (head->addr == addr) {
        if (!head->is_alloc)
            return 2;
        head->is_alloc = 0;
        if (head->next != NULL && !head->next->is_alloc)
            __block_merge(head);
        return 0;
    }
    // Traverse the heap until the next block is one with the target address
    while (head->next != NULL && head->next->addr != addr)
        head = head->next;
    // If no such block is found, raise Illegal Operation
    if (head->next == NULL)
        return 2;
    // If block is found but is already freed, raise Illegal Operation
    if (!head->next->is_alloc)
        return 2;
    // Else, free the target block and merge with its previous and next block,
    // if either of them is also unallocated
    head->next->is_alloc = 0;
    if (!head->is_alloc)
        __block_merge(head);
    else
        head = head->next;
    if (!head->next->is_alloc)
        __block_merge(head);
    return 0;
}

int is_alloc(struct block *head, uint32_t addr)
{   
    uint32_t block_start;
    uint32_t block_end;
    while (head != NULL)
    {
        if (!head->is_alloc) {
            head = head->next;
            continue;
        }
        // For each block, compute the range of memory it covers
        block_start = head->addr;
        block_end = head->addr + head->size;
        // If target address is within the range, return the size of memory
        // from the target address to the end of the block
        if (block_start <= addr && addr < block_end)
            return block_end - addr;
        head = head->next;
    }
    return 0;
}

void heap_free(struct block *head)
{
    while (head != NULL) {
        struct block *current_block = head;
        head = head->next;
        free(current_block);
    }
}

void __block_merge(struct block *block)
{
    struct block *next_block = block->next;
    block->size = block->n_banks * HEAPBANK_BYTESIZE;
    block->size += next_block->n_banks * HEAPBANK_BYTESIZE;
    block->n_banks += next_block->n_banks;
    block->next = next_block->next;
    free(next_block);
}
