/*
    @author: Sean Hobeck
      @date: 08/26/23
 */
#pragma once

/// @uses: struct node_t, etc...
#include "../lex/lex.h"

/// @uses: bool
#include <stdbool.h>

/*                  @section: Data                */

/// @note: enumerator for all of the instructions.
enum e_instruction {
  MOV = 0x0,LDA,ADD,SUB,CMP,OR,NOT,AND,JC,HLT,OUT
};
/// @note: enumerator for all of the registers.
enum e_register {
  R1 = 0x0, R2, R3, R4, R5, AX, IP
};
/// @note: enumerator for all of the flags.
enum e_flag {
  ZF = 0x0, CF, GF, LF, SF
};

/*          @section: Emulator Functions           */

/// @note: Getting the instruction from a string.
signed get_inst(const char *__restrict p_str);
/// @note: Getting the address & ext from a register, flag or address.
signed long get_addr_ext(const char *__restrict p_str, char **__restrict ext);
/// @note: Getting the absolute value of a string (if it is a register, flag, or address).
signed long get_value(const char *__restrict p_str);

/*          @section: Emulator Structure           */

typedef struct instruction_t {
  /// Enumerated Type of instruction.
  signed inst;
  /// Grouped data; xx xx, xx (for a grouped instruction).
  char *data[3];

  /// The next instruction in the list.
  struct instruction_t *p_next;
};

/// @note: Allocating and copying data to the "instruction_t" data structure.
void i_alloc_cpy(struct instruction_t **__restrict ptr,
                 struct instruction_t cpy);

/*            @section: External Functions         */

/// @note: Emulating all of the data in the linked list.
extern void emulate_data(const struct node_t list);