/*
Dan Nguyen
kngu7458
510594877
*/
#include "stdvmop.h"

op1_t op_R [0b1000][0b10000000] = { NULL };
op2_t op_I1[0b1000]             = { NULL };
op3_t op_I2[0b1000]             = { NULL };
op4_t op_I3[0b1000]             = { NULL };
op3_t op_S [0b1000]             = { NULL };
op4_t op_SB[0b1000]             = { NULL };

void reg_dump(int32_t *R, size_t PC) {
    printf("PC = 0x%08x;\n", (uint16_t)PC);
    for (int i = 0; i < N_REGS; i++)
        printf("R[%u] = 0x%08x;\n", i, R[i]);
}

uint32_t load_bytes(uint8_t *src, uint8_t width) {
    uint32_t value = 0;
    for (int i = 0; i < width; i++)
        value |= *(src+i) << (8*i);
    return value;
}

void store_bytes(uint8_t *dst, int32_t src, uint8_t width) {
    for (int i = 0; i < width; i++)
        *(dst+i) = (src >> (8*i)) & BIT8_MASK;
}

void add(uint8_t rd, uint8_t rs1, uint8_t rs2, int32_t *R) {
    R[rd] = R[rs1] + R[rs2];
}

void addi(uint8_t rd, uint8_t rs1, int32_t imm, int32_t *R) {
    R[rd] = R[rs1] + imm;
}

void sub(uint8_t rd, uint8_t rs1, uint8_t rs2, int32_t *R) {
    R[rd] = R[rs1] - R[rs2];
}

void lui(uint8_t rd, int32_t imm, int32_t *R) {
    R[rd] = imm;
}

void xor(uint8_t rd, uint8_t rs1, uint8_t rs2, int32_t *R) {
    R[rd] = R[rs1] ^ R[rs2];
}

void xori(uint8_t rd, uint8_t rs1, int32_t imm, int32_t *R) {
    R[rd] = R[rs1] ^ imm;
}

void or(uint8_t rd, uint8_t rs1, uint8_t rs2, int32_t *R) {
    R[rd] = R[rs1] | R[rs2];
}

void ori(uint8_t rd, uint8_t rs1, int32_t imm, int32_t *R)  {
    R[rd] = R[rs1] | imm;
}

void and(uint8_t rd, uint8_t rs1, uint8_t rs2, int32_t *R) {
    R[rd] = R[rs1] & R[rs2];
}

void andi(uint8_t rd, uint8_t rs1, int32_t imm, int32_t *R)  {
    R[rd] = R[rs1] & imm;
}

void sll(uint8_t rd, uint8_t rs1, uint8_t rs2, int32_t *R) {
    R[rd] = (uint32_t)R[rs1] << R[rs2];
}

void srl(uint8_t rd, uint8_t rs1, uint8_t rs2, int32_t *R) {
    R[rd] = (uint32_t)R[rs1] >> R[rs2];
}

void sra(uint8_t rd, uint8_t rs1, uint8_t rs2, int32_t *R) {
    R[rd] = R[rs1];
    for (int i = 0; i < R[rs2]; i++) {
        uint8_t bit = R[rd] & 1;
        R[rd] = ((uint32_t)R[rd] >> 1) | (bit << 31);
    }
}

void lb(uint8_t rd, uint8_t rs1, int32_t imm, uint8_t *M, int32_t *R) {
    R[rd] = load_bytes(&M[R[rs1] + imm], 1);
    R[rd] = SEXT(R[rd], 8);
}

void lh(uint8_t rd, uint8_t rs1, int32_t imm, uint8_t *M, int32_t *R) {
    R[rd] = load_bytes(&M[R[rs1] + imm], 2);
    R[rd] = SEXT(R[rd], 16);
}

void lw(uint8_t rd, uint8_t rs1, int32_t imm, uint8_t *M, int32_t *R) {
    R[rd] = load_bytes(&M[R[rs1] + imm], 4);
}

void lbu(uint8_t rd, uint8_t rs1, int32_t imm, uint8_t *M, int32_t *R) {
    R[rd] = load_bytes(&M[R[rs1] + imm], 1);
}

void lhu(uint8_t rd, uint8_t rs1, int32_t imm, uint8_t *M, int32_t *R) {
    R[rd] = load_bytes(&M[R[rs1] + imm], 2);
}

void sb(uint8_t rs1, uint8_t rs2, int32_t imm, uint8_t *M, int32_t *R) {
    store_bytes(&M[R[rs1] + imm], R[rs2], 1);
}

void sh(uint8_t rs1, uint8_t rs2, int32_t imm, uint8_t *M, int32_t *R) {
    store_bytes(&M[R[rs1] + imm], R[rs2], 2);
}

void sw(uint8_t rs1, uint8_t rs2, int32_t imm, uint8_t *M, int32_t *R) {
    store_bytes(&M[R[rs1] + imm], R[rs2], 4);
}

void slt(uint8_t rd, uint8_t rs1, uint8_t rs2, int32_t *R) {
    R[rd] = (R[rs1] < R[rs2]) ? 1 : 0;
}

void slti(uint8_t rd, uint8_t rs1, int32_t imm, int32_t *R) {
    R[rd] = (R[rs1] < imm) ? 1 : 0;
}

void sltu(uint8_t rd, uint8_t rs1, uint8_t rs2, int32_t *R) {
    R[rd] = ((uint32_t)R[rs1] < (uint32_t)R[rs2]) ? 1 : 0;
}

void sltiu(uint8_t rd, uint8_t rs1, int32_t imm, int32_t *R) {
    R[rd] = ((uint32_t)R[rs1] < (uint32_t)imm) ? 1 : 0;
}

void beq(uint8_t rs1, uint8_t rs2, int32_t imm, int32_t *R, size_t *PC) {
    *PC = (R[rs1] == R[rs2]) ? *PC + imm - 4 : *PC;
}

void bne(uint8_t rs1, uint8_t rs2, int32_t imm, int32_t *R, size_t *PC) {
    *PC = (R[rs1] != R[rs2]) ? *PC + imm - 4 : *PC;
}

void blt(uint8_t rs1, uint8_t rs2, int32_t imm, int32_t *R, size_t *PC) {
    *PC = (R[rs1] < R[rs2]) ? *PC + imm - 4 : *PC;
}

void bltu(uint8_t rs1, uint8_t rs2, int32_t imm, int32_t *R, size_t *PC) {
    *PC = ((uint32_t)R[rs1] < (uint32_t)R[rs2]) ? *PC + imm - 4 : *PC;
}

void bge(uint8_t rs1, uint8_t rs2, int32_t imm, int32_t *R, size_t *PC) {
    *PC = (R[rs1] >= R[rs2]) ? *PC + imm - 4 : *PC;
}

void bgeu(uint8_t rs1, uint8_t rs2, int32_t imm, int32_t *R, size_t *PC) {
    *PC = ((uint32_t)R[rs1] >= (uint32_t)R[rs2]) ? *PC + imm - 4 : *PC;
}

void jal(uint8_t rd, int32_t imm, int32_t *R, size_t *PC) {
    R[rd] = *PC + 4;
    *PC = *PC + imm - 4;
}

void jalr(uint8_t rd, uint8_t rs1, int32_t imm, int32_t *R, size_t *PC) {
    R[rd] = *PC + 4;
    *PC = R[rs1] + imm - 4;
}

int execute(
    instruction inst,
    uint8_t *M,
    int32_t *R,
    size_t *PC,
    struct block *head)
{
    // Retrieve operation from their func3/7
    op_R [0b000][0b0000000] = add;
    op_I1[0b000]            = addi;
    op_R [0b000][0b0100000] = sub;
    op_R [0b100][0b0000000] = xor;
    op_I1[0b100]            = xori;
    op_R [0b110][0b0000000] = or;
    op_I1[0b110]            = ori;
    op_R [0b111][0b0000000] = and;
    op_I1[0b111]            = andi;
    op_R [0b001][0b0000000] = sll;
    op_R [0b101][0b0000000] = srl;
    op_R [0b101][0b0100000] = sra;
    op_I2[0b000]            = lb;
    op_I2[0b001]            = lh;
    op_I2[0b010]            = lw;
    op_I2[0b100]            = lbu;
    op_I2[0b101]            = lhu;
    op_S [0b000]            = sb;
    op_S [0b001]            = sh;
    op_S [0b010]            = sw;
    op_R [0b010][0b0000000] = slt;
    op_I1[0b010]            = slti;
    op_R [0b011][0b0000000] = sltu;
    op_I1[0b011]            = sltiu;
    op_SB[0b000]            = beq;
    op_SB[0b001]            = bne;
    op_SB[0b100]            = blt;
    op_SB[0b110]            = bltu;
    op_SB[0b101]            = bge;
    op_SB[0b111]            = bgeu;
    op_I3[0b000]            = jalr;

    op1_t op1;
    op2_t op2;
    op3_t op3;
    op4_t op4;
    
    uint8_t rd;
    uint8_t rs1;
    uint8_t rs2;
    int32_t imm;
    int32_t addr;
    
    int execute_vr = 0;

    /* EXECUTE OPERATION */
    switch (inst.type)
    {
    case (TYPE_R):
        rd = inst.field.R.rd;
        rs1 = inst.field.R.rs1;
        rs2 = inst.field.R.rs2;
        // Catch unknown operation
        if ((op1 = op_R[inst.field.R.func3][inst.field.R.func7]) == NULL)
            return 1;
        // Shift by a value smaller than 0 or greater than 31 is undefined
        if ((op1 == sll || op1 == srl || op1 == sra) &&
            (R[rs2] < 0 || 31 < R[rs2]))
            return 2;
        op1(rd, rs1, rs2, R);
        break;
    
    case (TYPE_I1):
        rd = inst.field.I.rd;
        rs1 = inst.field.I.rs1;
        imm = inst.field.I.imm;
        // Catch unknown operation
        if ((op2 = op_I1[inst.field.R.func3]) == NULL)
            return 1;
        op2(rd, rs1, imm, R);
        break;
    
    case (TYPE_I2):
        rd = inst.field.I.rd;
        rs1 = inst.field.I.rs1;
        imm = inst.field.I.imm;
        addr = R[rs1] + imm;
        // Catch unknown operation
        if ((op3 = op_I2[inst.field.I.func3]) == NULL)
            return 1;
        // Catch out-of-bound memory access
        if ((addr < 0x00000000) ||
            (PADDING_STARTBYTE <= addr && addr < HEAP_STARTBYTE) ||
            (END_OF_MEMORY <= addr))
            return 2;
        // Catch unallocated memory access
        if ((addr >= HEAP_STARTBYTE) && (
            (op3 == lb && is_alloc(head, addr) < 1) ||
            (op3 == lh && is_alloc(head, addr) < 2) ||
            (op3 == lw && is_alloc(head, addr) < 4) ||
            (op3 == lbu && is_alloc(head, addr) < 1) ||
            (op3 == lhu && is_alloc(head, addr) < 2)))
            return 2;
        // Invoke virtual routine
        if (VR_MEM_STARTBYTE <= addr && addr < PADDING_STARTBYTE)
            execute_vr++;
        else
            op3(rd, rs1, imm, M, R);
        break;
    
    case (TYPE_I3):
        rd = inst.field.I.rd;
        rs1 = inst.field.I.rs1;
        imm = inst.field.I.imm;
        // Catch unknown operation
        if ((op4 = op_I3[inst.field.I.func3]) == NULL)
            return 1;
        op4(rd, rs1, imm, R, PC);
        break;
    
    case (TYPE_S):
        rs1 = inst.field.S.rs1;
        rs2 = inst.field.S.rs2;
        imm = inst.field.S.imm;
        addr = R[rs1] + imm;
        // Catch unknown operation
        if ((op3 = op_S[inst.field.S.func3]) == NULL)
            return 1;
        // Catch out-of-bound memory access
        if ((addr < DATA_MEM_STARTBYTE) ||
            (PADDING_STARTBYTE <= addr && addr < HEAP_STARTBYTE) ||
            (END_OF_MEMORY <= addr))
            return 2;
        // Catch unallocated memory access
        if ((addr >= HEAP_STARTBYTE) && (
            (op3 == sb && is_alloc(head, addr) < 1) ||
            (op3 == sh && is_alloc(head, addr) < 2) ||
            (op3 == sw && is_alloc(head, addr) < 4)))
            return 2;
        // Invoke virtual routine
        if (VR_MEM_STARTBYTE <= addr && addr < PADDING_STARTBYTE)
            execute_vr++;
        else
            op3(rs1, rs2, imm, M, R);
        break;
    
    case (TYPE_SB):
        rs1 = inst.field.SB.rs1;
        rs2 = inst.field.SB.rs2;
        imm = inst.field.SB.imm;
        // Catch unknown operation
        if ((op4 = op_SB[inst.field.SB.func3]) == NULL)
            return 1;
        op4(rs1, rs2, imm, R, PC);
        break;
    
    case (TYPE_U):
        rd = inst.field.U.rd;
        imm = inst.field.U.imm;
        lui(rd, imm, R);
        break;
    
    case (TYPE_UJ):
        rd = inst.field.U.rd;
        imm = inst.field.U.imm;
        jal(rd, imm, R, PC);
        break;
    }
    if (!execute_vr)
        return 0;
    
    /* EXECUTE VIRTUAL ROUTINE */
    switch (addr)
    {
    case (CONSOLE_W_CHAR):
        if (inst.type != TYPE_S)
            return 2;
        putchar(R[rs2]);
        break;
    
    case (CONSOLE_W_INT):
        if (inst.type != TYPE_S)
            return 2;
        printf("%d", R[rs2]);
        break;
    
    case (CONSOLE_W_UINT):
        if (inst.type != TYPE_S)
            return 2;
        printf("%x", (uint32_t)R[rs2]);
        break;
    
    case (HALT):
        if (inst.type != TYPE_S)
            return 2;
        return 5;
        break;
    
    case (CONSOLE_R_CHAR):
        if (inst.type != TYPE_I2)
            return 2;
        // Catch reading error
        if ((R[rd] = getchar()) == EOF)
            return 3;
        break;
    
    case (CONSOLE_R_INT):
        if (inst.type != TYPE_I2)
            return 2;
        // Catch reading error
        if (scanf("%d", &R[rd]) == EOF)
            return 3;
        break;
    
    case (DUMP_PC):
        if (inst.type != TYPE_S)
            return 2;
        printf("%x\n", *((int32_t *)PC));
        break;
    
    case (DUMP_REG_BANKS):
        reg_dump(R, *PC);
        break;
    
    case (DUMP_MEM_WORD):
        if (inst.type != TYPE_S)
            return 2;
        uint32_t v = (uint32_t)R[rs2];
        if ((v < 0x00000000) ||
            (PADDING_STARTBYTE <= v && v < HEAP_STARTBYTE) ||
            (END_OF_MEMORY <= v))
            return 2;
        printf("%x\n", load_bytes(&M[v], 4));
        break;
    
    case (MALLOC):
        if (inst.type != TYPE_S)
            return 2;
        if (R[rs2] < 0)
            return 2;
        // Catch malloc error
        if ((R[28] = block_malloc(head, R[rs2])) == -1)
            return 4;
        break;
    
    case (FREE):
        if (inst.type != TYPE_S)
            return 2;
        if (R[rs2] < 0)
            return 2;
        // Catch free error
        int ret_val;
        if ((ret_val = block_free(head, R[rs2])) != 0)
            return ret_val;
        break;

    case (RESERVED):
        break;

    default:
        // Catch out-of-bound memory access
        return 2;
    }
    return 0;
}
