/*
    @author: Sean Hobeck
      @date: 08/24/23
 */
#include "emu.h"

/// @note: For readibility.
#define strcmp(str, cmp, ret) if (!strstr(str, cmp)) \
                                return ret;



/*                  @section: Data                     */

/// @note: The emulators memory.
extern unsigned char memory[0x7FFF];

/*            @section: Utility Functions              */

/// @note: Loading data into a memory location.
void load_memd(unsigned short loc, unsigned char data) { assert(memory != 0); memory[loc] = data; };
/// @note: Getting the data a the memory location.
unsigned char get_memd(unsigned short loc) { assert(memory != 0); return memory[loc]; };
/// @note: Getting the instruction name from a string.
signed get_inst(const char* p_str)
{
    /// String check.
    assert(p_str != 0);

    /// All of the instructions.
    strcmp(p_str, "mov", MOV); strcmp(p_str, "lda", LDA);
    strcmp(p_str, "add", ADD); strcmp(p_str, "sub", SUB);
    strcmp(p_str, "cmp", CMP); strcmp(p_str, "jc", JC);
    strcmp(p_str, "or", OR); strcmp(p_str, "not", NOT);
    strcmp(p_str, "and", AND);
    strcmp(p_str, "hlt", HLT); strcmp(p_str, "out", OUT);
    return -1;
};

/*            @section: External Functions             */

extern void emulate_data(const struct node_t list)
{
    /// The returned instruction tree.
    struct instruction_t r;
    
    /// Getting the iterator.
    struct node_t it = list;
};