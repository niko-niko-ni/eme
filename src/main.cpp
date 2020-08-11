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
    //print_all_tokens_after(*tokens.first, &symbols_by_id);

    Token_Linked_List resulting_list;

    {
      bool resulting_list_initialized = false;
      Token_Linked_List parsed = tokens;
      while(parsed.first->type != eol) {
        Token *result = new Token();
        bool succeeded = parse_statement(result, &parsed);

        if(succeeded) {
          if(resulting_list_initialized) {
            resulting_list.last->next = result;
            resulting_list.last = result;
          } else {
            resulting_list.first = result;
            resulting_list.last = result;
            resulting_list_initialized = true;
          }
        } else {
          printf("Failed to parse statement.\n"); // @Incomplete: put a better error message here and exit
        }
      }
    }


    printf("PRINTING ALL TOKENS AFTER\n");
    print_all_tokens_after(*resulting_list.first, &symbols_by_id, 0);


  } catch(const std::exception& e) {
    printf("Error: %s\n", e.what());
  }
}
