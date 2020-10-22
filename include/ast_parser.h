#pragma once

#include "ast.h"

Ast_Node *parse_statements_to_ast(Token_Linked_List statements);

// parse_statement_to_ast parses one statement, splits it into its constituent parts, and converts it into an AST form.
// It is NOT recursive; it doesn't convert curly braces and parentheses into expressions, it just creates a statement consisting of three or less linked lists of tokens.
Ast_Node *parse_statement_to_ast(Token statement);
