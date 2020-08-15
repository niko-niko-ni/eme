#include "statement_parser.h"
#include <iostream>

#include "symbol.h"
#include "token.h"



bool parse_one_statement(Token *result, Token_Linked_List *remaining_tokens_list) {
  Token *current_token = remaining_tokens_list->first;

  while(current_token->type != token_eol) {
    printf("starting iteration...\n");
    if(current_token->type == token_syntax && current_token->data.syntax == ';') {
      Token_Linked_List sub_tokens;
      sub_tokens.first = remaining_tokens_list->first;
      remaining_tokens_list->first = current_token->next;
      result->next = current_token->next;

      current_token->character = -1;
      current_token->line = -1;
      current_token->type = token_eol;

      sub_tokens.last = current_token;

      result->character = sub_tokens.first->character;
      result->line = sub_tokens.first->line;
      result->type = token_statement;


      result->data.sub_tokens = sub_tokens;
      printf("no parentheses. returning to parse_statements\n");
      return true;
    } else if (current_token->type == token_syntax && (current_token->data.syntax == '(' || (current_token->data.syntax == '{' || current_token->data.syntax == '['))) {
      printf("parentheses found. parsing parentheses...\n");
      std::cout << current_token->data.syntax << "\n";
      int nextNum = parse_parentheses(current_token);
      printf("parentheses parsed\n");
      Token *replace_token = current_token;
      std::cout << nextNum << " tokens to skip. skipping tokens... starting at " << current_token->data.syntax << "\n";
      for(int i = 0; i <= nextNum; i++) {
        std::cout << replace_token->data.syntax << " token\n";
        replace_token = replace_token->next;

      }
      printf("next token found. deleting tokens...\n");
      current_token->next = replace_token;
      printf("tokens deleted. continuing loop...\n");
    }

    // @Incomplete: deal with if an end curly brace, bracket, or parentheses is encountered outside of its context
    current_token = current_token->next;
    printf("iteration complete.\n");
  }
  printf("loop complete. returning to parse_statements...\n");
  return false;
}

int parse_parentheses(Token *token) {

    Token *current_token = token->next;
    char parenthType = token->data.syntax;
    int ret = 1;
    Token_Linked_List sub_tokens;

    token->type = token_expr_parentheses; 
    token->data.sub_tokens = sub_tokens;

    Token *eol = new Token();

    eol->character = -1;

    eol->line = -1;

    eol->type = token_eol;

    bool first = true;
    sub_tokens.first = eol;
    while (current_token->type != token_eol) {
      
      bool parenthMatch = (current_token->data.syntax == ')' && parenthType == '(');
      parenthMatch = parenthMatch || (current_token->data.syntax == '}' && parenthType == '{');
      parenthMatch = parenthMatch || (current_token->data.syntax == ']' && parenthType == '[');
      
      if(parenthMatch) {
        current_token->character = -1;
        current_token->line = -1;
        current_token->type = token_eol;
        sub_tokens.last= current_token;
        break;
        
      } else {
        std::cout << "no parenthMatch at " << current_token->data.syntax << "\n";
      }
      std::cout << "inneriterate " << ret << " " << current_token->data.syntax << "\n";
      if (current_token->data.syntax == '(' || (current_token->data.syntax == '[' || current_token->data.syntax == '{')) {
        ret = ret + parse_parentheses(current_token);
      }
      if (first) {
        sub_tokens.first = current_token;
	first = false;
      } else {
        sub_tokens.last->next = current_token;
      }
      sub_tokens.last = current_token;
     
      current_token = current_token->next;
      ret++;
    }

    token->data.sub_tokens = sub_tokens;
    return ret;
}

bool parse_statements(Token_Linked_List *resulting_list, Token_Linked_List tokens) {
  printf("parsing all statements, iterating through statements...\n");
  bool resulting_list_initialized = false;
  Token_Linked_List parsed = tokens;
  while(parsed.first->type != token_eol) {
    Token *result = new Token();
    printf("sending statement to individual statement parser...\n");
    bool succeeded = parse_one_statement(result, &parsed);
    printf("done parsing individual statement\n");

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
  printf("finished iterating through statements. returning to main...\n");
  return true;
}
/*
bool parse_block(Token *result, Token_Linked_List *remaining_tokens_list) {

  if(remaining_tokens_list->first == '}') {
    remaining_tokens_list->first = remaining_tokens_list->first->next;
    result->type = token_eol;
    result->character = -1;
    result->line = -1;
    return true;
  }

  Token *current_token = remaining_tokens_list->first;

  Token_Linked_List statements;
  statements.first = current_token;

  while(current_token->type != token_eol && !(current_token->type == syntax && current_token->data.syntax == '}')) {
  }

  { // remove first token (which is token_eol) from the linked list
    Token *new_first_token = tokens.first->next;
    delete tokens.first;
    tokens.first = new_first_token;
  }

  return true;
}*/
