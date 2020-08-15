#include <vector>
#include <iostream>
#include "types.h"
#include "symbol.h"
#include "token.h"
#include "lexer.h"
#include "statement_parser.h"
#include "ast.h"
#include "ast_parser.h"

int main() {
  try {
    printf("main block started. moving on to lexing...\n");
    Token_Linked_List tokens = lex_file((char*)"./examples/basicstatements.eme");
    printf("lexer finished. moving on to declaring list of statements...\n");
    Token_Linked_List statements;
    printf("list of statements declared. Moving on to parsing statements...\n");
    parse_statements(&statements, tokens);


    print_all_tokens_after(*statements.first);

    Ast_Node *root = parse_statements_to_ast(statements);

    print_node(*root);


  } catch(const std::exception& e) {
    printf("Error: %s\n", e.what());
  }
}
