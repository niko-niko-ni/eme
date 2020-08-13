#include <vector>
#include <iostream>
#include "types.h"
#include "symbol.h"
#include "token.h"
#include "lexer.h"
#include "statement_parser.h"

int main() {
  try {
    printf("main block started. moving on to lexing...\n");
    Token_Linked_List tokens = lex_file((char*)"./examples/basicstatements.eme");
    printf("lexer finished. moving on to declaring list of statements...\n");
    Token_Linked_List statements;
    printf("list of statements declared. Moving on to parsing statements...\n");
    parse_statements(&statements, tokens);
    printf("statements parsed. moving onto printing all tokens...\n");
    print_all_tokens_after(*statements.first, 0);
    printf("all tokens printed \n");

  } catch(const std::exception& e) {
    printf("Error: %s\n", e.what());
  }
}
