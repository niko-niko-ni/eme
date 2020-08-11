#include "statement_parser.h"

#include "symbol.h"
#include "token.h"



bool parse_one_statement(Token *result, Token_Linked_List *remaining_tokens_list) {
  Token *current_token = remaining_tokens_list->first;

  while(current_token->type != eol) {
    if(current_token->type == syntax && current_token->data.syntax == ';') {
      Token_Linked_List sub_tokens;
      sub_tokens.first = remaining_tokens_list->first;
      remaining_tokens_list->first = current_token->next;
      result->next = current_token->next;

      current_token->character = -1;
      current_token->line = -1;
      current_token->type = eol;

      sub_tokens.last = current_token;

      result->character = sub_tokens.first->character;
      result->line      = sub_tokens.first->line;
      result->type = statement;

      result->data.sub_tokens = sub_tokens;

      return true;
    }

    // @Incomplete: deal with if an end curly brace, bracket, or parentheses is encountered outside of its context

    current_token = current_token->next;
  }

  return false;
}


bool parse_statements(Token_Linked_List *resulting_list, Token_Linked_List tokens) {
  bool resulting_list_initialized = false;
  Token_Linked_List parsed = tokens;
  while(parsed.first->type != eol) {
    Token *result = new Token();
    bool succeeded = parse_one_statement(result, &parsed);

    if(succeeded) {
      if(resulting_list_initialized) {
        resulting_list->last->next = result;
        resulting_list->last = result;
      } else {
        resulting_list->first = result;
        resulting_list->last = result;
        resulting_list_initialized = true;
      }
    } else {
      return false; // @Incomplete: maybe put an error message here?
    }
  }
  return true;
}
/*
bool parse_block(Token *result, Token_Linked_List *remaining_tokens_list) {

  if(remaining_tokens_list->first == '}') {
    remaining_tokens_list->first = remaining_tokens_list->first->next;
    result->type = eol;
    result->character = -1;
    result->line = -1;
    return true;
  }

  Token *current_token = remaining_tokens_list->first;

  Token_Linked_List statements;
  statements.first = current_token;

  while(current_token->type != eol && !(current_token->type == syntax && current_token->data.syntax == '}')) {
  }

  { // remove first token (which is eol) from the linked list
    Token *new_first_token = tokens.first->next;
    delete tokens.first;
    tokens.first = new_first_token;
  }

  return true;
}*/
