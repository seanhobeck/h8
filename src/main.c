#include "lex/lex.h"

int main(int argc, char **argv) {
  if (argc == 1)
    return -1;

  struct node_t *tokens = get_lex_tokens(argv[1]);
};