/*
    @author: Sean Hobeck
      @date: 08/26/23
 */
#include "emu.h"

/// @note: For readibility.
#define cmp(str, cmp, ret)                                                     \
  if (!strcmp(str, cmp))                                                       \
    return ret;
/// @note: For readibility.
#define s_alloc(ptr, equ)                                                      \
  ptr = (char *)calloc(1ul, 256ul);                                            \
  strcpy(ptr, equ);

/*                  @section: Data                     */

/// @note: The emulators memory.
unsigned char memory[0x7FFF];
/// @note: The emulators registers.
unsigned long registers[0x7];
/// @note: The emulators flags.
unsigned char flags[0x5];

/*            @section: Utility Functions              */

/// @note: Loading data into a memory location.
void load_memd(unsigned short loc, unsigned char data) {
  assert(&memory != 0);
  memory[loc] = data;
};
/// @note: Getting the data a the memory location.
unsigned char get_memd(unsigned short loc) {
  assert(memory != 0);
  return memory[loc];
};
/// @note: Getting the instruction name from a string.
signed get_inst(const char *p_str) {
  /// String check.
  assert(p_str != 0);

  /// All of the instructions.
  cmp(p_str, "mov", 0);
  cmp(p_str, "lda", 1);
  cmp(p_str, "add", 2);
  cmp(p_str, "sub", 3);
  cmp(p_str, "cmp", 4);
  cmp(p_str, "jc", 8);
  cmp(p_str, "or", 5);
  cmp(p_str, "not", 6);
  cmp(p_str, "and", 7);
  cmp(p_str, "hlt", 9);
  cmp(p_str, "out", 10);
  return -1;
};

/*          @section: Emulator Structure           */

void i_alloc_cpy(struct instruction_t **__restrict ptr,
                 struct instruction_t cpy) {
  // Allocating & copying to the pointer.
  *ptr = (struct instruction_t *)calloc(1ul, sizeof(struct instruction_t));
  **ptr = cpy;
};

/*            @section: External Functions             */

extern void emulate_data(const struct node_t list) {
  /// The returned instruction tree.
  struct instruction_t ll =
      *(struct instruction_t *)calloc(1ul, sizeof(struct instruction_t));
  ll.inst = -1;

  /// The previous line / instruction pointer.
  unsigned long p_ip = 1;

  /// Getting the iterator.
  struct node_t it = list;
  do {
    /// Getting the next instruction enum.
    signed in = -1;
    if ((in = get_inst(it.p_data)) != -1) {
      /// Creating the temporary instruction.
      struct instruction_t t = {.p_next = 0, .inst = in, .data = 0};
      /// 1 Token.
      if (in == HLT) {
        s_alloc(t.data[0], it.p_data);
      }
      /// 2 Tokens.
      else if (in == OUT) {
        s_alloc(t.data[0], it.p_data);
        s_alloc(t.data[1], it.p_next->p_data);
      }
      /// 3 Tokens.
      else {
        s_alloc(t.data[0], it.p_data);
        s_alloc(t.data[1], it.p_next->p_data);
        s_alloc(t.data[2], it.p_next->p_next->p_data);
      }
      /// Adding the instruction to the list of instructions.
      if (ll.inst == -1)
        ll = t;
      else {
        /// Temporary Loop
        struct instruction_t o = ll;
        if (ll.p_next == 0) {
          i_alloc_cpy(&ll.p_next, t);
          goto el;
        }
        struct instruction_t *p = &o;
        while (p->p_next != 0)
          p = p->p_next;
        i_alloc_cpy(&p->p_next, t);
        ll = o;
        goto el;
      }
    }
  el:
    /// Making the next iterator the first token on a new line.
    while (it.m_line == p_ip) {
      if (it.p_next == 0)
        goto ds;
      else
        it = *it.p_next;
    }
    p_ip = it.m_line;
  } while (p_ip != -1);

  /// We are now done sorting the data and we now have to process all of it.
  ds:
  printf("Sorting Complete.\n");

  /// Converting it to an array.
  unsigned long h = 0;
  struct instruction_t array[p_ip], p = ll;
  while(p.p_next != 0) {
    array[h] = p;
    p = *p.p_next;
    h++;
  }
  array[h] = p;

  /// We now process all of the data.
  for (unsigned long i = 0; i < p_ip; i++) {
      /// Run each instruction....
  }

  return;
};