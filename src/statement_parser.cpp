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
      int skip = parse_grouping(current_token);
      std::cout << "skipping " << skip << " tokens\n";
      Token *replace_token = current_token->data.sub_tokens.last->next;
      //for(int i = 0; i < skip; i++) {
        //replace_token = replace_token->next;
      //  std::cout << "skipping to " << replace_token->line << "\n";
      //}
      std::cout << "skipped to " << replace_token->line << "\n";

      current_token->next = replace_token;
    }

    // @Incomplete: deal with if an end curly brace, bracket, or parentheses is encountered outside of its context
    current_token = current_token->next;
  }

  return false;
}

int parse_grouping(Token *token) {
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
  int ret = 1;
  while (curr->type != token_eol) {
    //First, check for nested parentheses
    if (curr->type == token_syntax && (curr->data.syntax == '(' || (curr->data.syntax == '{' || curr->data.syntax == '['))) {
      nestNum++;
    } else if (curr->type == token_syntax && (curr->data.syntax == ')' || (curr->data.syntax == '}' || curr->data.syntax == ']'))) {
      bool closeGrouping = (curr->data.syntax ==')') && (groupingType == '(');
      closeGrouping = closeGrouping || ((curr->data.syntax =='}') && (groupingType == '{'));
      closeGrouping = closeGrouping || ((curr->data.syntax ==']') && (groupingType == '['));
      if(closeGrouping) {
        nestNum--;
        if(nestNum < 0) { //closing of main grouping;
          ret++;
          curr->character = -1;
          curr->line = -1;
          curr->type = token_eol;
          sub_tokens.last->next = curr;
          sub_tokens.last = curr;
          break;
        }
      } else if (nestNum == 0) { //if no nested grouping and an unexpected closed grouping is found
        // @Incomplete give error here
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
    ret++;
  }
  //Now need to give sub_tokens to parse_statements
  Token_Linked_List result_list;

  bool statementsParsed = parse_statements(&result_list, sub_tokens);

  if(statementsParsed) {
    token->data.sub_tokens = result_list;
    std::cout << groupingType << " statements" << "\n";

  } else {
    token->data.sub_tokens = sub_tokens;
    std::cout << groupingType << " no statements" << "\n";
  }
  return ret;

}

bool parse_statements(Token_Linked_List *resulting_list, Token_Linked_List tokens) {
  printf("recurse\n");
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
      printf("false\n");
      return false; // @Incomplete: maybe put an error message here?
    }
  }

  printf("compleet\n");
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
