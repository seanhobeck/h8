/*
    @author: Sean Hobeck
      @date: 08/22/23
 */
#pragma once

/// @uses: c features
#include "../incl.h"

/// @note: Type definition for the lexed nodes.
typedef struct node_t {
  /// @note: Union for lexed token data.
  char *p_data;
  unsigned long m_line;

  /// @note: The next node in the linked list.
  struct node_t *p_next;
};

/// @return: A filtered out version of the original string.
char *filter(char *__restrict str);
/// @note: Allocates the destination and then copys the source data.
void s_alloc_cpy(char **__restrict dst, char *__restrict src);
void n_alloc_cpy(struct node_t *__restrict dst, struct node_t src);

/*            @section: External Functions             */

/// @note: Getting the linked list of lexed tokens in a file.
extern struct node_t *get_lex_tokens(const char *__restrict p_filename);