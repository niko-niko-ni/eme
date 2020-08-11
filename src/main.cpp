#include <vector>
#include <iostream>
#include "types.h"
#include "symbol.h"
#include "token.h"
#include "lexer.h"
#include "statement_parser.h"

int main() {
  try {
    Token_Linked_List tokens = lex_file((char*)"./examples/basicstatements.eme");

    Token_Linked_List statements;
    parse_statements(&statements, tokens);

    print_all_tokens_after(*statements.first, 0);


  } catch(const std::exception& e) {
    printf("Error: %s\n", e.what());
  }
}
