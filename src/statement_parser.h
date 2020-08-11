#pragma once

#include "token.h"


// Returns success or failure
// The resulting parsed statement is placed in 'result', and 'remaining_tokens_list''s first element is updated to contain the element after this statement's semicolon.
bool parse_statement(Token *result, Token_Linked_List *remaining_tokens_list);

bool parse_statements(Token_Linked_List *resulting_list, Token_Linked_List tokens);
