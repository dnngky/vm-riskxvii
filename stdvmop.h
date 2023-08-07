/*
Dan Nguyen
kngu7458
510594877
*/
#ifndef STDVMOP
#define STDVMOP

#include <stdint.h>
#include <stdio.h>
#include "stdvmmem.h"
#include "stdvmparse.h"

#define CONSOLE_W_CHAR 0x0800
#define CONSOLE_W_INT 0x0804
#define CONSOLE_W_UINT 0x0808
#define HALT 0x080C
#define CONSOLE_R_CHAR 0x0812
#define CONSOLE_R_INT 0x0816
#define DUMP_PC 0x0820
#define DUMP_REG_BANKS 0x0824
#define DUMP_MEM_WORD 0x0828
#define MALLOC 0x0830
#define FREE 0x0834
#define RESERVED 0x0850

/*
Function placeholder for type R.
*/
typedef void (*op1_t)(uint8_t, uint8_t, uint8_t, int32_t*);
/*
Function placeholder for type I1.
*/
typedef void (*op2_t)(uint8_t, uint8_t, int32_t, int32_t*);
/*
Function placeholder for types I2 and S.
*/
typedef void (*op3_t)(uint8_t, uint8_t, int32_t, uint8_t*, int32_t*);
/*
Function placeholder for types I3 and SB.
*/
typedef void (*op4_t)(uint8_t, uint8_t, int32_t, int32_t*, size_t*);

// Arithmetic and logic operations

void add(uint8_t rd, uint8_t rs1, uint8_t rs2, int32_t *R);
void addi(uint8_t rd, uint8_t rs1, int32_t imm, int32_t *R);
void sub(uint8_t rd, uint8_t rs1, uint8_t rs2, int32_t *R);
void lui(uint8_t rd, int32_t imm, int32_t *R);
void xor(uint8_t rd, uint8_t rs1, uint8_t rs2, int32_t *R);
void xori(uint8_t rd, uint8_t rs1, int32_t imm, int32_t *R);
void or(uint8_t rd, uint8_t rs1, uint8_t rs2, int32_t *R);
void ori(uint8_t rd, uint8_t rs1, int32_t imm, int32_t *R);
void and(uint8_t rd, uint8_t rs1, uint8_t rs2, int32_t *R);
void andi(uint8_t rd, uint8_t rs1, int32_t imm, int32_t *R);

// Memory access operations

void sll(uint8_t rd, uint8_t rs1, uint8_t rs2, int32_t *R);
void srl(uint8_t rd, uint8_t rs1, uint8_t rs2, int32_t *R);
void sra(uint8_t rd, uint8_t rs1, uint8_t rs2, int32_t *R);
void lb(uint8_t rd, uint8_t rs1, int32_t imm, uint8_t *M, int32_t *R);
void lh(uint8_t rd, uint8_t rs1, int32_t imm, uint8_t *M, int32_t *R);
void lw(uint8_t rd, uint8_t rs1, int32_t imm, uint8_t *M, int32_t *R);
void lbu(uint8_t rd, uint8_t rs1, int32_t imm, uint8_t *M, int32_t *R);
void lhu(uint8_t rd, uint8_t rs1, int32_t imm, uint8_t *M, int32_t *R);
void sb(uint8_t rs1, uint8_t rs2, int32_t imm, uint8_t *M, int32_t *R);
void sh(uint8_t rs1, uint8_t rs2, int32_t imm, uint8_t *M, int32_t *R);
void sw(uint8_t rs1, uint8_t rs2, int32_t imm, uint8_t *M, int32_t *R);

// Program flow operations

void slt(uint8_t rd, uint8_t rs1, uint8_t rs2, int32_t *R);
void slti(uint8_t rd, uint8_t rs1, int32_t imm, int32_t *R);
void sltu(uint8_t rd, uint8_t rs1, uint8_t rs2, int32_t *R);
void sltiu(uint8_t rd, uint8_t rs1, int32_t imm, int32_t *R);
void beq(uint8_t rs1, uint8_t rs2, int32_t imm, int32_t *R, size_t *PC);
void bne(uint8_t rs1, uint8_t rs2, int32_t imm, int32_t *R, size_t *PC);
void blt(uint8_t rs1, uint8_t rs2, int32_t imm, int32_t *R, size_t *PC);
void bltu(uint8_t rs1, uint8_t rs2, int32_t imm, int32_t *R, size_t *PC);
void bge(uint8_t rs1, uint8_t rs2, int32_t imm, int32_t *R, size_t *PC);
void bgeu(uint8_t rs1, uint8_t rs2, int32_t imm, int32_t *R, size_t *PC);
void jal(uint8_t rd, int32_t imm, int32_t *R, size_t *PC);
void jalr(uint8_t rd, uint8_t rs1, int32_t imm, int32_t *R, size_t *PC);

/*
Dump PC and registers to stdout.
*/
void reg_dump(int32_t *R, size_t PC);
/*
Load width-byte of data from src.
*/
extern uint32_t load_bytes(uint8_t *src, uint8_t width);
/*
Store width-byte of data from src to in dst.
*/
extern void store_bytes(uint8_t *dst, int32_t src, uint8_t width);
/*
Execute inst. If a virtual routine is called, this function returns the
address of the corresponding routine. Else, return 0 on success and 1
if an illegal operation is encountered.
*/
extern int execute(
    instruction inst,
    uint8_t *M,
    int32_t *R,
    size_t *PC,
    struct block *head
);

#endif /* STDVMOP */
