/*
    @author: Sean Hobeck
      @date: 08/22/23
 */
#include "lex.h"

/// @note: Filtering out certain characters in a string.
char *filter(char *__restrict str) {
  /// Returning the string by filtering out , & \n
  unsigned long l = 0;
  while (str[l] != 0) {
    if (str[l] == ',' || str[l] == '\n')
      str[l] = 0;
    l++;
  }
  return str;
};

/// @note: calloc + strcpy in one function.
void s_alloc_cpy(char **__restrict dst, char *__restrict src) {
  *dst = (char *)calloc(1ul, 256ul);
  if (src != 0)
    strcpy(*dst, src);
};
void n_alloc_cpy(struct node_t **__restrict dst, struct node_t src) {
  *dst = (struct node_t *)calloc(1ul, sizeof(struct node_t));
  if (&src != 0)
    **dst = src;
};



/*            @section: Extern Functions             */



extern struct node_t *get_lex_tokens(const char *__restrict p_filename) {
  /// Attempting to open the file.
  FILE *fptr = fopen(p_filename, "r");
  assert(fptr != NULL);

  /// Creating the linked list.
  struct node_t list = {.m_line = -1};

  /// Reading by each line.
  char *s = 0;
  unsigned long l = 1, n = 0;
  while (getline(&s, &n, fptr) != -1) {
    /// Splitting string by space.
    char *t = strtok(s, " ");
    while (t != 0) {
      /// Creating temporary nodes.
      struct node_t node = { .m_line = l, .p_next = 0 };
      s_alloc_cpy(&node.p_data, filter(t));

      /// Adding to our linked list.
      if (list.m_line == -1)
        list = node;
      else {
        /// Finding the last node.
        struct node_t o = list;
        if (list.p_next == 0) {
          n_alloc_cpy(&list.p_next, node);
          goto el;
        }
        struct node_t *u = &o;
        while (u->p_next != 0) {
          u = u->p_next;
          if (u->p_next == 0) {
            n_alloc_cpy(&u->p_next, node);
            list = o;
            break;
          }
        }
      }
    el:
      /// Getting the split string.
      t = strtok(0, " ");
    }
    l++;
  };
  return 0;
};