#include "token.h"

#include "types.h"
#include "symbol.h"


const uint INDENTATION_INCREASE = 4;

void print_token(Token token, int indentation) {
  bool is_using_indentation = true;
  if(indentation < 0) {
    is_using_indentation = false;
    indentation = 0;
  }

  printf("%*c\bToken at line %i, character %i:\n", indentation, ' ', token.line, token.character);
  switch(token.type) {
    case token_syntax:
      printf("%*c\bSYNTAX %c\n\n", indentation, ' ', token.data.syntax);
      break;
    case token_symbol:
      printf("%*c\bSYMBOL %s\n\n", indentation, ' ', symbols_by_id.at(token.data.symbol).name->c_str());
      break;
    case token_literal_int:
      printf("%*c\bLITERAL_INT %lld\n\n", indentation, ' ', token.data.literal_int);
      break;
    case token_literal_string:
      printf("%*c\bLITERAL_STRING %s\n\n", indentation, ' ', token.data.literal_string->c_str());
      break;
    case token_literal_bool:
      printf("%*c\bLITERAL_BOOL %s\n\n", indentation, ' ', token.data.literal_bool ? "true" : "false");
      break;
    case token_eol:
      printf("%*c\bEOL TOKEN\n\n", indentation, ' ');
      break;
    case token_statement:
      printf("%*c\bSTATEMENT:\n\n", indentation, ' ');

      uint new_indentation = is_using_indentation ? indentation+INDENTATION_INCREASE : -1;
      print_all_tokens_after(*token.data.sub_tokens.first, new_indentation);
  }
}

void print_all_tokens_after(Token token, int indentation) {
  Token *current_token = &token;

  while(current_token->type != token_eol) {
    print_token(*current_token, indentation);
    current_token = current_token->next;
  }
}
