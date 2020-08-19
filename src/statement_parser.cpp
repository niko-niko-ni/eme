#include "statement_parser.h"
#include <iostream>

#include "symbol.h"
#include "token.h"



bool parse_one_statement(Token *result, Token_Linked_List *remaining_tokens_list) {
  Token *current_token = remaining_tokens_list->first;

  while(current_token->type != token_eol) {
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
      return true;
    } else if (current_token->type == token_syntax && (current_token->data.syntax == '(' || (current_token->data.syntax == '{' || current_token->data.syntax == '['))) {
      parse_grouping(current_token);
    }

    // @Incomplete: deal with if an end curly brace, bracket, or parentheses is encountered outside of its context
    current_token = current_token->next;
  }

  return false;
}

void parse_grouping(Token *token) {
  char groupingType = token->data.syntax;
  if(groupingType == '(') {
    token->type = token_expr_parentheses;
  } else if (groupingType == '{') {
    token->type = token_block_curly;
  } else {
    token->type = token_expr_bracket;
  }
  Token *curr = token->next;
  Token_Linked_List sub_tokens;
  Token *eol = new Token();
  eol->character = -1;
  eol->line = -1;
  eol->type = token_eol;
  bool first = true;
  sub_tokens.first = eol;
  int nestNum = 0;
  while (nestNum >= 0) {
    if(curr->data.syntax == '(' || (curr->data.syntax == '{' || curr->data.syntax == '[')) {
      nestNum++;
    } else if (curr->data.syntax == ')' || (curr->data.syntax == '}' || curr->data.syntax == ']')) {
      nestNum--;
      bool parenthMatch = curr->data.syntax == ')' && (groupingType == '(');
      parenthMatch = parenthMatch || (curr->data.syntax == '}' && (groupingType == '{'));
      parenthMatch = parenthMatch || (curr->data.syntax == ']' && (groupingType == '['));
      if (nestNum == -1) {
        if (parenthMatch) {
          token->next = curr->next;
          curr->character = -1;
          curr->line = -1;
          curr->type = token_eol;
          sub_tokens.last->next = curr;
          sub_tokens.last = curr;
          break;
        } else {
          // @Incomplete throw error here. unexpected closing parentheses
        }
      }
      
    }
    if(first) {
      sub_tokens.first = curr;
      first = false;
    } else {
      sub_tokens.last->next = curr;
    }
    sub_tokens.last = curr;

    curr = curr->next;

  }

  Token_Linked_List result_list;
  bool statementsParsed = parse_statements(&result_list, sub_tokens);

  if(statementsParsed) {
    token->data.sub_tokens = result_list;
    std::cout << groupingType << " statements" << "\n";

  } else {
    token->data.sub_tokens = sub_tokens;
    std::cout << groupingType << " no statements" << "\n";
  }
 
}

bool parse_statements(Token_Linked_List *resulting_list, Token_Linked_List tokens) {
  bool resulting_list_initialized = false;
  Token_Linked_List parsed = tokens;
  while(parsed.first->type != token_eol) {

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
