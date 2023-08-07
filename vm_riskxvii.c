/*
Dan Nguyen
kngu7458
510594877
*/
#include <string.h>
#include "stdvmmem.h"
#include "stdvmparse.h"
#include "stdvmop.h"

int term(
    char *error_msg,
    uint32_t data,
    int32_t *REG,
    size_t PC,
    struct block *head)
{
    if (*(error_msg+strlen(error_msg)-1) == '\n')
        printf("%s", error_msg);
    else {
        printf("%s: 0x%08x\n", error_msg, data);
        reg_dump(REG, PC);
    }
    heap_free(head);
    return 1;
}

int main(int argc, char **argv)
{
    uint8_t MEM[
        INST_MEM_BYTESIZE+
        DATA_MEM_BYTESIZE+
        VR_MEM_BYTESIZE+
        PADDING_BYTESIZE+
        (HEAPBANK_BYTESIZE*N_HEAPBANKS)
    ];
    int32_t REG[N_REGS] = { 0x00000000 };
    size_t PC = 0x00000000;
    struct block *heap_head = block_init(
        HEAP_STARTBYTE,
        HEAPBANK_BYTESIZE*N_HEAPBANKS,
        NULL
    );

    FILE *fp;
    fp = fopen(argv[1], "r");
    if (fp == NULL)
        return term("Memory Image File Not Found\n",
                    0x00000000, NULL, 0x00000000, heap_head);
    
    size_t mem_img_bytesize = INST_MEM_BYTESIZE+DATA_MEM_BYTESIZE;
    size_t bytes_read =  fread(MEM, 1, mem_img_bytesize, fp);
    if (bytes_read != mem_img_bytesize)
        return term("Invalid Memory Image Size\n",
                    0x00000000, NULL, 0x00000000, heap_head);
    fclose(fp);

    instruction inst; // Instruction information container
    uint32_t data;    // Instruction encoding to be parsed
    while (1)
    {
        if (!(0x00000000 <= PC && PC < DATA_MEM_STARTBYTE) | (PC % 4 != 0))
            return term("Invalid PC Address Value",
                        data, REG, PC, heap_head);
        
        data = load_bytes(MEM+PC, 4);
        if ((parse(&inst, data)) == 1)
            return term("Instruction Not Implemented",
                        data, REG, PC, heap_head);
        
        switch (execute(inst, MEM, REG, &PC, heap_head))
        {
        case 1:
            return term("Instruction Not Implemented",
                        data, REG, PC, heap_head);
        case 2:
            return term("Illegal Operation",
                        data, REG, PC, heap_head);
        case 3:
            return term("Console Read Failure",
                        data, REG, PC, heap_head);
        case 4:
            // This, in theory, should never be raised; if it is raised,
            // then there is something wrong with the implementation itself
            return term("Heap Initialisation Failure",
                        data, REG, PC, heap_head);
        case 5:
            // HALT virtual routine
            printf("CPU Halt Requested\n");
            heap_free(heap_head);
            return 0;
        default:
            break;
        }
        // Increment PC and reset zero register
        PC += 4;
        REG[0] = 0;
    }
    return 0;
}
