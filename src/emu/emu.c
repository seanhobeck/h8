/*
    @author: Sean Hobeck
      @date: 08/24/23
 */
#include "emu.h"



/*                  @section: Data                     */

/// @note: The emulators memory.
extern unsigned char memory[65536];

/*            @section: Utility Functions              */

/// @note: Loading data into a memory location.
void load_memd(unsigned short loc, unsigned char data) { assert(memory != 0); memory[loc] = data; };
/// @note: Getting the data a the memory location.
unsigned char get_memd(unsigned short loc) { assert(memory != 0); return memory[loc]; };

/*            @section: External Functions             */

extern void emulate_data(struct node_t* list)
{
    
};