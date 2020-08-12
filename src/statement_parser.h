#pragma once

#include "token.h"


// Returns true for success, false for failure
// The resulting parsed statement is placed in 'result', and 'remaining_tokens_list''s first element is updated to contain the element after this statement's semicolon.
bool parse_one_statement(Token *result, Token_Linked_List *remaining_tokens_list);

//Recrusive function used to parse parentheses tokens
int parse_parentheses(Token *token);

// Parses a list of semicolon-separated statements into a list of statement tokens.
// Returns true for success, false for failure.
// The resulting list of statement tokens is placed in 'resulting_list'.
bool parse_statements(Token_Linked_List *resulting_list, Token_Linked_List tokens);

