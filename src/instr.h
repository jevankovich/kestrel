#ifndef INSTR_H
#define INSTR_H

#include <stdint.h>

/* Instruction format
 * 32 bits
 * 6 bit opcode
 * 8 bit rd (destination)
 * 9 bit rs, rt (sources)
 *
 * 9  bit imm9  in place of rt
 * 18 bit imm18 in place of rs, rt
 * 26 bit imm26 in place of rd, rs, rt
 * 32 bit imm32 instead of next instruction
 *
 * Every one of the 256 registers can contain any C data,
 * including an array, a struct, a pointer, or any integral type
 */

typedef enum opcode {
	OP_ZERO, // R[rd] = 0 or R[rd] = {0} or R[rd] = NULL
	OP_LOADI, // R[rd] = SE(imm18)
	OP_LOADK, // R[rd] = K[imm32]
	OP_RETURN, // return R[rs]
} opcode;

typedef uint32_t instruction;

#define extract(i, w, o) (((i) >> o) & (UINT32_MAX >> (32 - w)))

#define OP_WIDTH 6
#define OP_OFFSET 0
#define op(i) extract(i, OP_WIDTH, OP_OFFSET)

#define RD_WIDTH 8
#define RD_OFFSET 6
#define rd(i) extract(i, RD_WIDTH, RD_OFFSET)

#define RS_WIDTH 9
#define RS_OFFSET 14
#define rs(i) extract(i, RS_WIDTH, RS_OFFSET)

#define RT_WIDTH 9
#define RT_OFFSET 23
#define rt(i) extract(i, RT_WIDTH, RS_OFFSET)

#define IMM9_WIDTH RT_WIDTH
#define IMM9_OFFSET RT_OFFSET
#define imm9(i) extract(i, IMM9_WIDTH, IMM9_OFFSET)

#define IMM18_WIDTH (RS_WIDTH + RT_WIDTH)
#define IMM18_WIDTH RS_OFFSET
#define imm18(i) extract(i, IMM18_WIDTH, IMM18_OFFSET)

#define IMM29_WIDTH (RS_WIDTH + RT_WIDTH + RD_WIDTH)
#define IMM29_WIDTH RD_OFFSET
#define imm29(i) extract(i, IMM29_WIDTH, IMM29_OFFSET)

#endif
