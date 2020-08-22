#pragma once

#include "types.h"

enum Token_Type {
  // Used to denote the end of a list of tokens
  token_eol,

  // Tokens outputted by the lexer
  token_symbol,
  token_syntax,
  token_literal_uint,
  token_literal_int,
  token_literal_bool,
  token_literal_string,

  // Meta-tokens which have a tree-like structure (contain in and of themselves another linked list of tokens)
  // These are created by the statement parser
  token_statement,
  token_block_curly,
  token_expr_parentheses,
  token_expr_bracket

};

struct Token;

struct Token_Linked_List {
  Token *first;
  Token *last;
};

#include "symbol.h"

// Linked list of tokens
struct Token {
  int character = -1;
  int line = -1;
  int file = -1;
  Token_Type type;

  union {
    Symbol symbol;
    char syntax;
    u64  literal_uint;
    i64  literal_int;
    bool literal_bool;
    std::string *literal_string;
    Token_Linked_List sub_tokens;
  } data;

  Token *next;
};

// If indentation is less than 0, don't indent at all, even for curly braces, statements, etc.
// If indentation is anything else, do normal indentation.
void print_token(Token token, int indentation=0);

void print_all_tokens_after(Token token, int indentation=0);
