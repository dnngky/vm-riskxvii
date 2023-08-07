/*
Dan Nguyen
kngu7458
510594877
*/
#ifndef STDVMPARSE
#define STDVMPARSE

#include <stdint.h>
#include <stdio.h>

#define TYPE_R_OPCODE 0b0110011
#define TYPE_I1_OPCODE 0b0010011
#define TYPE_I2_OPCODE 0b0000011
#define TYPE_I3_OPCODE 0b1100111
#define TYPE_S_OPCODE 0b0100011
#define TYPE_SB_OPCODE 0b1100011
#define TYPE_U_OPCODE 0b0110111
#define TYPE_UJ_OPCODE 0b1101111

#define BIT1_MASK 0x1
#define BIT8_MASK 0xFF
#define BIT16_MASK 0xFFFF
#define BIT32_MASK 0xFFFFFFFF

#define OPCODE_BITSIZE 7
#define FUNC3_BITSIZE 3
#define FUNC7_BITSIZE 7
#define RD_BITSIZE 5
#define RS_BITSIZE 5
#define IMM5_BITSIZE 5

/* Sign extend an n-bit binary value. */
#define SEXT(bin, n) (bin>>(n-1)) ? (bin|(BIT32_MASK>>n<<n)) : bin

/*
Format types supported by the VM.
*/
typedef enum {
    TYPE_R,
    TYPE_I1,
    TYPE_I2,
    TYPE_I3,
    TYPE_S,
    TYPE_SB,
    TYPE_U,
    TYPE_UJ
} ftype;

/*
Instruction parameter container.
*/
typedef struct {
    ftype type;
    union {
        struct {
            uint8_t func3;
            uint8_t func7;
            uint8_t rd;
            uint8_t rs1;
            uint8_t rs2;
        } R;
        struct {
            uint8_t func3;
            uint8_t rd;
            uint8_t rs1;
            int32_t imm;
        } I;
        struct {
            uint8_t func3;
            uint8_t rs1;
            uint8_t rs2;
            int32_t imm;
        } S;
        struct {
            uint8_t func3;
            uint8_t rs1;
            uint8_t rs2;
            int32_t imm;
        } SB;
        struct {
            uint8_t rd;
            int32_t imm;
        } U;
        struct {
            uint8_t rd;
            int32_t imm;
        } UJ;
    } field;
} instruction;

/*
Parse the relevant information from 32-bit encoded data into inst.
Return 0 if parse was successful, 1 otherwise.
*/
extern int parse(instruction *inst, uint32_t data);

#endif /* STDVMPARSE */
