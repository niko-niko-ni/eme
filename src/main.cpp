#include <unordered_map>
#include <vector>
#include <iostream>
#include "types.h"
#include "symbol.h"
#include "token.h"
#include "lexer.h"
#include "statement_parser.h"

int main() {

  std::vector<Symbol_Data> symbols_by_id;
  std::unordered_map<std::string, Symbol_Data> symbols_by_name;

  try {
    Token_Linked_List tokens = lex_file((char*)"./examples/basicstatements.lang", &symbols_by_id, &symbols_by_name);
    print_all_tokens_after(*tokens.first, &symbols_by_id);

    Token result;
    Token_Linked_List parsed = tokens;
    bool succeeded = parse_statement(&result, &parsed);
    if(succeeded) {
      print_token(result, &symbols_by_id, 0);
    } else {
      printf("FAILED\n");
    }
    printf("PRINTING ALL TOKENS AFTER\n");
    print_all_tokens_after(result, &symbols_by_id, 0);

  } catch(const std::exception& e) {
    printf("Error: %s\n", e.what());
  }
}
