/*
    @author: Sean Hobeck
      @date: 08/24/23
 */
#pragma once

/// @uses: struct node_t, etc...
#include "../lex/lex.h"

/// @uses: bool
#include <stdbool.h>

/// @note: enumerator for all of the instructions.
enum e_instruction : signed { MOV = 0x0, LDA,ADD,SUB,CMP,OR,NOT,AND,JC,HLT,OUT };
/// @note: enumerator for all of the registers.
enum e_register : signed { R1 = 0x0,R2,R3,R4,R5,AX,IP };
/// @note: enumerator for all of the flags.
enum e_flag : signed { ZF = 0x0,CF,GF,LF,SF };

/*          @section: Emulator Functions           */

/// @note: Loading data into a memory location.
void load_memd(unsigned short loc, unsigned char data);
/// @note: Getting the data from a memory location.
unsigned char get_memd(unsigned short loc);
/// @note: Getting the instruction from a string.
signed get_inst(const char* p_str);

/*          @section: Emulator Structure           */

typedef struct instruction_t {
  /// Enumerated Type of instruction.
  signed inst = -1;
  /// Grouped data; xx xx, xx (for a grouped instruction).
  char** data;

  /// Constructor for initialization.
  instruction_t() { data = (char**) calloc(3ul, 256ul); };

  /// The next instruction in the list.
  struct instruction_t* p_next;
};

/*            @section: External Functions         */

/// @note: Emulating all of the data in the linked list.
extern void emulate_data(const struct node_t list);