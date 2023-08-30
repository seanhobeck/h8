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
/// @note: Easy to use.
#define throw(fmt, ...)                                                        \
  fprintf(stderr, fmt, __VA_ARGS__);                                           \
  exit(-1);
/// @note: For simplicity.
#define load_mem(addr, ext, val)                                               \
  if (ext[0] == 'r') {                                                         \
    if (addr > 0x6) {                                                          \
      fprintf(stderr, "invalid register.\n");                                  \
      exit(-1);                                                                \
    } else                                                                     \
      registers[addr] = val;                                                   \
  } else if (ext[0] == 'a') {                                                  \
    if (addr > 0x7FFE) {                                                       \
      fprintf(stderr, "address is out of range.\n");                           \
      exit(-1);                                                                \
    } else                                                                     \
      memory[addr] = val;                                                      \
  }

/*                  @section: Data                     */

/// @note: The emulators memory.
signed char memory[0x7FFF];
/// @note: The emulators registers.
signed long registers[0x7];
/// @note: The emulators flags.
signed char flags[0x5];

/*            @section: Utility Functions              */

/// @note: Getting the instruction name from a string.
signed get_inst(const char *__restrict p_str) {
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
/// @note: Getting the address & ext from a register, flag or address.
signed long get_addr_ext(const char *__restrict p_str,
                           char **__restrict ext) {
  /// ("r?") >> 1 == 1 -> a register.
  s_alloc(*ext, "r");
  if (p_str[0] == 'r')
    return p_str[1] - '1';
  else if (p_str[0] == 'a' && p_str[1] == 'x')
    return 0x5;
  else if (p_str[0] == 'i' && p_str[1] == 'p')
    return 0x6;

  /// ("#?") -> a address or ("$?") -> a absolute
  if (p_str[0] == '#' || p_str[0] == '$') {
    s_alloc(*ext, p_str[0] == '#' ? "a" : "s");
    signed long l = 0;
    while (p_str[l] != 0)
      l++;
    char *p_sub = (char *)calloc(1ul, l);
    for (signed long i = 0; i < l; i++)
      if (i > 0)
        p_sub[i - 1] = p_str[i];
    /// Returning based on the base specified (0b1, 0x1, 01)
    if (p_sub[0] == 'x' || p_sub[1] == 'x')
      return strtoul(p_sub, 0x0, 16);
    else if (p_sub[0] == 'b' || p_sub[1] == 'b')
      return strtoul(p_sub, 0x0, 2);
    else
      return strtoul(p_sub, 0x0, 10);
  }

  /// '?f' -> a flag.
  s_alloc(*ext, "f");
  if (p_str[1] == 'f')
    if (p_str[0] == 'z')
      return 0x0;
    else if (p_str[0] == 'c')
      return 0x1;
    else if (p_str[0] == 'g')
      return 0x2;
    else if (p_str[0] == 'l')
      return 0x3;
    else if (p_str[0] == 's')
      return 0x4;

  /// couldn't evaluate expression.
  throw("could not evaluate expression: %s\n", p_str)
  return -1;
};
/// @note: Getting the absolute value of a string (if it is a register, flag, or
/// address).
signed long get_value(const char *__restrict p_str) {
  char *p_ext;
  signed long ul_addr = get_addr_ext(p_str, &p_ext);
  /// 'r' -> a register.
  if (p_ext[0] == 'r') {
    if (ul_addr > 0x6) {
        throw("invalid register: %s\n", p_str)
    }
    return registers[ul_addr];
  }
  /// 'a' -> a address
  else if (p_ext[0] == 'a') {
    if (ul_addr > 0x7FFE) {
      throw("address is out of range: %s\n", p_str) }
    return memory[ul_addr];
  }
  /// 's' -> a absolute value
  else if (p_ext[0] == 's')
    return ul_addr;
  /// 'f' -> a flag
  else if (p_ext[0] == 'f')
    return flags[ul_addr];
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
  signed long p_ip = 1;

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
  signed long h = 0;
  struct instruction_t array[p_ip], p = ll;
  while (p.p_next != 0) {
    array[h] = p;
    p = *p.p_next;
    h++;
  }
  array[h] = p;

  /// We now process all of the data.
  for (signed long i = 0; i < p_ip; i++) {
    /// Run each instruction....
    struct instruction_t in = array[i];

    /// Setting the instruction pointer.
    registers[0x6] = i + 1;

    /// If the instruction is halting the program.
    if (in.inst == HLT) {
      printf("Program Halted!\n");
      goto pr_end;
    }

    /// If the instruction is printing out to the console.
    else if (in.inst == OUT) {
      printf("Program Output: %lu\n", get_value(in.data[1]));
      continue;
    }

    /// If we are dealing with other instructions...
    /// Getting the data from the src imm8
    char *src_ext, *dst_ext;
    signed long src = get_addr_ext(in.data[1], &src_ext),
                  dst = get_addr_ext(in.data[2], &dst_ext),
                  v = get_value(in.data[1]),
                  s = get_value(in.data[2]);

    /// Any of the other instructions.
    switch (in.inst) {
        /// MOV
        case (0): {
            load_mem(dst, dst_ext, v);
            break;
        }
        /// LDA
        case (1): {
            if (src_ext[0] != 's') {
                throw("you cannot load a address/register effectively into another address,\nplease refer to the 'mov' instruction\n", 0);
            }
            load_mem(dst, dst_ext, v);
            break;
        }
        /// ADD
        case (2): {
            load_mem(dst, dst_ext, v + s);
            break;
        }
        /// SUB
        case (3): {
          load_mem(dst, dst_ext, s - v);
          break;
        }
        /// CMP
        case (4): {
          flags[0] = (v == s) ? 1 : 0;
          flags[2] = (v > s) ? 1 : 0;
          flags[3] = (v < s) ? 1 : 0;
          flags[4] = (v - s) < 0 ? 1 : 0;
          break;
        }
        /// OR
        case (5): {
          load_mem(dst, dst_ext, s | v);
          break;
        }
        /// NOT
        case (6): {
          load_mem(dst, dst_ext, ~s);
          break;
        }
        /// AND
        case (7): {
          load_mem(dst, dst_ext, s & v);
          break;
        }
        /// JC
        case (8): {
          if (v == 1)
            i = s - 2;
          break;
        }
        default: { break; }
    };
  }

/// Printing all of the registers for the user to see.
pr_end:
  printf("\n\nRegisters:\n\tR1: %lu, R2: %lu, R3: %lu, R4: %lu, R5: %lu\n\tAX: "
         "%lu, IP: %lu\n",
         registers[0], registers[1], registers[2], registers[3], registers[4],
         registers[5], registers[6]);
  printf("Flags:\n\tZF: %lu, CF: %lu, GF: %lu, LF: %lu, SF: %lu\n", flags[0],
         flags[1], flags[2], flags[3], flags[4]);
  return;
};