/*
    @author: Sean Hobeck
      @date: 08/24/23
 */
#pragma once

/// @uses: struct node_t, etc...
#include "../lex/lex.h"


/// @note: enumerator for all of the instructions.
enum e_instruction : int { MOV = 0x0, LDA,ADD,SUB,CMP,OR,NOT,AND,JC,HLT,OUT };

/// @note: enumerator for all of the registers.
enum e_register : int { R1 = 0x0,R2,R3,R4,R5,AX,IP };

/// @note: enumerator for all of the flags.
enum e_flag : int { ZF = 0x0,CF,GF,LF,SF };

/// @note: Structure for all of the grouping of each instruction.
typedef struct instruction_t
{
    char** p_data;
};

/*          @section: Memory-based Functions           */

/// @note: Loading data into a memory location.
void load_memd(unsigned short loc, unsigned char data);
/// @note: Getting the data from a memory location.
unsigned char get_memd(unsigned short loc);
/// @note: Getting the instruction from a string.
signed get_inst(const char* p_str);

/*            @section: External Functions             */

/// @note: Emulating all of the data in the linked list.
extern void emulate_data(const struct node_t list);