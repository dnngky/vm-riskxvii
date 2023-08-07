/*
Dan Nguyen
kngu7458
510594877
*/
#include "stdvmparse.h"

int parse(instruction *inst, uint32_t data)
{
    // Extract opcode
    uint8_t opcode = data & (BIT8_MASK >> 1);
    // Extract func3
    uint8_t func3 =
        (data >>
         (OPCODE_BITSIZE+RD_BITSIZE)) &
        (BIT8_MASK >> 5);
    // Extract func7
    uint8_t func7 =
        (data >>
         (OPCODE_BITSIZE+RD_BITSIZE+FUNC3_BITSIZE+2 * RS_BITSIZE)) &
        (BIT8_MASK >> 1);
    
    int32_t imm1 = 0;
    int32_t imm2 = 0;
    int32_t imm3 = 0;
    int32_t imm4 = 0;
    int type_assigned = 0;

    // Extract register specifiers/immediate values and build instruction.
    // Data is decoded according to their respective opcode.
    switch (opcode)
    {
    case (TYPE_R_OPCODE):
        inst->type = TYPE_R;
        (inst->field).R.func3 = func3;
        (inst->field).R.func7 = func7;
        (inst->field).R.rd =
            (data >>
             OPCODE_BITSIZE) &
            (BIT8_MASK >> 3);
        (inst->field).R.rs1 =
            (data >>
             (OPCODE_BITSIZE+RD_BITSIZE+FUNC3_BITSIZE)) &
            (BIT8_MASK >> 3);
        (inst->field).R.rs2 =
            (data >>
             (OPCODE_BITSIZE+RD_BITSIZE+FUNC3_BITSIZE+RS_BITSIZE)) &
            (BIT8_MASK >> 3);
        break;

    case (TYPE_I1_OPCODE):
        inst->type = TYPE_I1;
        type_assigned = 1;
    case (TYPE_I2_OPCODE):
        if (!type_assigned)
            inst->type = TYPE_I2;
        type_assigned = 1;
    case (TYPE_I3_OPCODE):
        if (!type_assigned)
            inst->type = TYPE_I3;
        (inst->field).R.func3 = func3;
        (inst->field).I.rd =
            (data >>
             OPCODE_BITSIZE) &
            (BIT8_MASK >> 3);
        (inst->field).I.rs1 =
            (data >>
             (OPCODE_BITSIZE+RD_BITSIZE+FUNC3_BITSIZE)) &
            (BIT8_MASK >> 3);
        imm1 =
            (data >>
             (OPCODE_BITSIZE+RD_BITSIZE+FUNC3_BITSIZE+RS_BITSIZE)) &
            (BIT16_MASK >> 4);
        (inst->field).I.imm = SEXT(imm1, 12);
        break;

    case (TYPE_S_OPCODE):
        inst->type = TYPE_S;
        (inst->field).R.func3 = func3;
        (inst->field).S.rs1 =
            (data >>
             (OPCODE_BITSIZE+IMM5_BITSIZE+FUNC3_BITSIZE)) &
            (BIT8_MASK >> 3);
        (inst->field).S.rs2 =
            (data >>
             (OPCODE_BITSIZE+IMM5_BITSIZE+FUNC3_BITSIZE+RS_BITSIZE)) &
            (BIT8_MASK >> 3);
        imm1 =
            (data >>
             OPCODE_BITSIZE) &
            (BIT8_MASK >> 3);
        imm2 =
            (data >>
             (OPCODE_BITSIZE+IMM5_BITSIZE+FUNC3_BITSIZE+2*RS_BITSIZE)) &
            (BIT8_MASK >> 1);
        imm1 = ((imm2 << 5) | imm1);
        (inst->field).S.imm = SEXT(imm1, 12);
        break;

    case (TYPE_SB_OPCODE):
        inst->type = TYPE_SB;
        (inst->field).R.func3 = func3;
        (inst->field).SB.rs1 =
            (data >>
             (OPCODE_BITSIZE+IMM5_BITSIZE+FUNC3_BITSIZE)) &
            (BIT8_MASK >> 3);
        (inst->field).SB.rs2 =
            (data >>
             (OPCODE_BITSIZE+IMM5_BITSIZE+FUNC3_BITSIZE+RS_BITSIZE)) &
            (BIT8_MASK >> 3);
        imm1 =
            (data >>
             (OPCODE_BITSIZE+1)) &
            (BIT8_MASK >> 4);
        imm2 =
            (data >>
             (OPCODE_BITSIZE+IMM5_BITSIZE+FUNC3_BITSIZE+2*RS_BITSIZE)) &
            (BIT8_MASK >> 2);
        imm3 =
            (data >>
             OPCODE_BITSIZE) &
            (BIT1_MASK);
        imm4 =
            (data >>
             (OPCODE_BITSIZE+IMM5_BITSIZE+FUNC3_BITSIZE+2*RS_BITSIZE+6)) &
            (BIT1_MASK);
        imm1 = (imm4 << 12) | (imm3 << 11) | (imm2 << 5) | (imm1 << 1);
        (inst->field).SB.imm = SEXT(imm1, 13);
        break;

    case (TYPE_U_OPCODE):
        inst->type = TYPE_U;
        (inst->field).U.rd =
            (data >>
             OPCODE_BITSIZE) &
            (BIT8_MASK >> 3);
        imm1 =
            (data >>
             (OPCODE_BITSIZE+RD_BITSIZE)) &
            (BIT32_MASK >> 12);
        (inst->field).U.imm =
            imm1 << 12;
        break;

    case (TYPE_UJ_OPCODE):
        inst->type = TYPE_UJ;
        (inst->field).UJ.rd =
            (data >>
             OPCODE_BITSIZE) &
            (BIT8_MASK >> 3);
        imm1 =
            (data >>
             (OPCODE_BITSIZE+RD_BITSIZE+9)) &
            (BIT16_MASK >> 6);
        imm2 =
            (data >>
             (OPCODE_BITSIZE+RD_BITSIZE+8)) &
            (BIT1_MASK);
        imm3 =
            (data >>
             (OPCODE_BITSIZE+RD_BITSIZE)) &
            (BIT8_MASK);
        imm4 =
            (data >>
             (OPCODE_BITSIZE+RD_BITSIZE+19)) &
            (BIT1_MASK);
        imm1 = (imm4 << 20) | (imm3 << 12) | (imm2 << 11) | (imm1 << 1);
        (inst->field).UJ.imm = SEXT(imm1, 21);
        break;

    default:
        return 1;
    };
    return 0;
}
