#pragma once

#include "types.h"
#include "symbol.h"

enum Token_Type {
  // Used to denote the end of a list of tokens
  eol,

  // Tokens outputted by the lexer
  symbol,
  syntax,
  literal_uint,
  literal_int,
  literal_bool,
  literal_string,

  // Meta-tokens which have a tree-like structure (contain in and of themselves another linked list of tokens)
  // These are created by the statement parser
  statement,
  block_curly,
  expr_parentheses,
  expr_bracket

};

struct Token;

struct Token_Linked_List {
  Token *first;
  Token *last;
};

// Linked list of tokens
struct Token {
  int character;
  int line;
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
void print_token(Token token, std::vector<Symbol_Data> *symbols_by_id, int indentation=-1);

void print_all_tokens_after(Token token, std::vector<Symbol_Data> *symbols_by_id, int indentation=-1);
