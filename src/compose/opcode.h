//------------------------------------------------------------------------------
// opcode.h - Opcode definitions for compose instruction set
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2016 Lachlan Orr
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//   1. The origin of this software must not be misrepresented; you must not
//   claim that you wrote the original software. If you use this software
//   in a product, an acknowledgment in the product documentation would be
//   appreciated but is not required.
//
//   2. Altered source versions must be plainly marked as such, and must not be
//   misrepresented as being the original software.
//
//   3. This notice may not be removed or altered from any source
//   distribution.
//------------------------------------------------------------------------------

#ifndef GA_OPCODE_H
#define GA_OPCODE_H

#include "corelib/base_types.h"

namespace ga
{

//------------------------------------------------------------------------------
// Instructions are 32 bit and broken down as follows:
//
//  Bits 31 .. 16  | 15 .. 11       | 10 .. 6        | 5 .. 0  
//-----------------------------------------------------------------------
//  ARG2 (16 bit)  | ARG1 (5 bits)  | ARG0 (5 bits)  | Opcode (6 bits)
//
// 
// VM has 32 general puprose registers, as well as a 16 bit IP and a 16 bit SP.
//
// Memory is cell addressable.  A cell is 4 bytes.
// With 16 bit address space, max memory for a VM instances is therefore:
//   2 ** 16 * 4 == 263144 (256k)
//
// Memory is layed out with code section first, followed by global
// data section.  Stack starts at max address and grows down.  There's
// no heap or dynamic memory.
//------------------------------------------------------------------------------

enum eOpcode
{
                       //    ARG0(5)    |    ARG1(5)   |    ARG2(16)    | Description
                       //-----------------------------------------------------------------------------------------------
    eOpcode_SYSCALL,   //               |              |   Sys Func ID  | Call the system function identified in ARG2

    // Load/Store
    eOpcode_LOADI,     //    Reg Dest   |              |    Immediate   | Reg[ARG0] = Immediate       (Load immediate 16 bit value into dest register)
    eOpcode_LOADC,     //    Reg Dest   |              |                | Reg[ARG0] = Cells[IP], IP++ (Load next cell in instruction stream into dest register, increment IP)
    eOpcode_LOAD,      //    Reg Dest   | Reg Src Addr |                | Reg[ARG0] = Cells[Reg[ARG1] & 0xff]  (Load cell at cell address into dest register)
    eOpcode_STORE,     // Reg Dest Addr |   Reg Src    |                | Cells[Reg[ARG0] & 0xff] = Reg[ARG1]   (Store cell in source register into cell address)

    // Bitwise
    eOpcode_COMP,      //    Reg Dest   |   Reg Src    |                | Reg[ARG0] = ~Reg[ARG1]
    eOpcode_AND,       //    Reg Dest   |   Reg Src0   |    Reg Src1    | Reg[ARG0] = Reg[ARG1] &  Reg[ARG2 & 0x1f]
    eOpcode_OR,        //    Reg Dest   |   Reg Src0   |    Reg Src1    | Reg[ARG0] = Reg[ARG1] |  Reg[ARG2 & 0x1f]
    eOpcode_XOR,       //    Reg Dest   |   Reg Src0   |    Reg Src1    | Reg[ARG0] = Reg[ARG1] ^  Reg[ARG2 & 0x1f]
    eOpcode_LSHIFT,    //    Reg Dest   |   Reg Src0   |    Reg Src1    | Reg[ARG0] = Reg[ARG1] << Reg[ARG2 & 0x1f]
    eOpcode_LSHIFTI,   //    Reg Dest   |   Reg Src    |    Immediate   | Reg[ARG0] = Reg[ARG1] << Immediate
    eOpcode_RSHIFT,    //    Reg Dest   |   Reg Src0   |    Reg Src1    | Reg[ARG0] = Reg[ARG1] >> Reg[ARG2 & 0x1f]
    eOpcode_RSHIFTI,   //    Reg Dest   |   Reg Src    |    Immediate   | Reg[ARG0] = Reg[ARG1] >> Immediate

    // Integer 
    eOpcode_NEG,       //    Reg Dest   |   Reg Src    |                | Reg[ARG0] = -Reg[ARG1]
    eOpcode_ADD,       //    Reg Dest   |   Reg Src0   |    Reg Src1    | Reg[ARG0] = Reg[ARG1] + Reg[ARG2 & 0x1f]
    eOpcode_ADDI,      //    Reg Dest   |   Reg Src0   |    Immediate   | Reg[ARG0] = Reg[ARG1] + Immediate
    eOpcode_SUB,       //    Reg Dest   |   Reg Src0   |    Reg Src1    | Reg[ARG0] = Reg[ARG1] - Reg[ARG2 & 0x1f]
    eOpcode_SUBI,      //    Reg Dest   |   Reg Src0   |    Immediate   | Reg[ARG0] = Reg[ARG1] - Immediate
    eOpcode_MUL,       //    Reg Dest   |   Reg Src0   |    Reg Src1    | Reg[ARG0] = Reg[ARG1] * Reg[ARG2 & 0x1f]
    eOpcode_MULI,      //    Reg Dest   |   Reg Src0   |    Immediate   | Reg[ARG0] = Reg[ARG1] * Immediate
    eOpcode_DIV,       //    Reg Dest   |   Reg Src0   |    Reg Src1    | Reg[ARG0] = Reg[ARG1] / Reg[ARG2 & 0x1f]
    eOpcode_DIVI,      //    Reg Dest   |   Reg Src0   |    Immediate   | Reg[ARG0] = Reg[ARG1] / Immediate
    eOpcode_MOD,       //    Reg Dest   |   Reg Src0   |    Reg Src1    | Reg[ARG0] = Reg[ARG1] % Reg[ARG2 & 0x1f]
    eOpcode_MODI,      //    Reg Dest   |   Reg Src0   |    Immediate   | Reg[ARG0] = Reg[ARG1] % Immediate

    // Comparison
    eOpcode_EQ,        //    Reg Dest   |   Reg Src0   |    Reg Src1    | Reg[ARG0] = (Reg[ARG1] == Reg[ARG2 & 0x1f]) ? 0xffffffff : 0
    eOpcode_NEQ,       //    Reg Dest   |   Reg Src0   |    Reg Src1    | Reg[ARG0] = (Reg[ARG1] != Reg[ARG2 & 0x1f]) ? 0xffffffff : 0
    eOpcode_LT,        //    Reg Dest   |   Reg Src0   |    Reg Src1    | Reg[ARG0] = (Reg[ARG1] <  Reg[ARG2 & 0x1f]) ? 0xffffffff : 0
    eOpcode_LTE,       //    Reg Dest   |   Reg Src0   |    Reg Src1    | Reg[ARG0] = (Reg[ARG1] <= Reg[ARG2 & 0x1f]) ? 0xffffffff : 0
    eOpcode_GT,        //    Reg Dest   |   Reg Src0   |    Reg Src1    | Reg[ARG0] = (Reg[ARG1] >  Reg[ARG2 & 0x1f]) ? 0xffffffff : 0
    eOpcode_GTE,       //    Reg Dest   |   Reg Src0   |    Reg Src1    | Reg[ARG0] = (Reg[ARG1] >= Reg[ARG2 & 0x1f]) ? 0xffffffff : 0

    // Branch/Stack/Jump
    eOpcode_BRZ,       //    Reg Src    |              |    Address     | if Reg[ARG0] == 0 then IP = Address
    eOpcode_BRNZ,      //    Reg Src    |              |    Address     | if Reg[ARG0] != 0 then IP = Address
    eOpcode_PUSH,      //    Reg Src    |              |                | Cells[SP] = Reg[ARG0], SP--
    eOpcode_POP,       //    Reg Dest   |              |                | Reg[ARG0] = Cells[SP], SP++
    eOpcode_JUMP,      //               |              |    Address     | IP = Address
    eOpcode_CALL,      //               |              |    Address     | Cells[SP] = IP, SP--, IP = Address
    eOpcode_RET,       //               |              |                | IP = Cells[SP], SP++

    // Floating point
    eOpcode_FNEG,      //    Reg Dest   |   Reg Src    |                | Reg[ARG0] = -(float)Reg[ARG1]
    eOpcode_FADD,      //    Reg Dest   |   Reg Src0   |    Reg Src1    | Reg[ARG0] = (float)Reg[ARG1] + (float)Reg[ARG2 & 0x1f]
    eOpcode_FSUB,      //    Reg Dest   |   Reg Src0   |    Reg Src1    | Reg[ARG0] = (float)Reg[ARG1] - (float)Reg[ARG2 & 0x1f]
    eOpcode_FMUL,      //    Reg Dest   |   Reg Src0   |    Reg Src1    | Reg[ARG0] = (float)Reg[ARG1] * (float)Reg[ARG2 & 0x1f]
    eOpcode_FDIV,      //    Reg Dest   |   Reg Src0   |    Reg Src1    | Reg[ARG0] = (float)Reg[ARG1] / (float)Reg[ARG2 & 0x1f]



    // not an opcode, just the total count of opcodes
    eOpcode_COUNT
};


inline u32 opcode(u32 inst)
{
    return inst & 0x3f;
}

inline u32 arg0(u32 inst)
{
    return (inst >> 6) & 0x1f;
}

inline u32 arg1(u32 inst)
{
    return (inst >> 11) & 0x1f;
}

inline u32 arg2(u32 inst)
{
    return inst >> 16;
}

inline u32 inst(u32 opcode, u32 arg0, u32 arg1, u32 arg3)
{
    GA_ASSERT(opcode < 0x3f);
    GA_ASSERT(arg0   < 0x1f);
    GA_ASSERT(arg1   < 0x1f);
    GA_ASSERT(arg3   < 0x10000);

    u32 instruction = ( (arg3 << 16) |
                        ((arg2 & 0x1f) << 11) |
                        ((arg1 & 0x1f) << 6) |
                        (opcode & 0x3f) );
    return instruction;
}

}

#endif
