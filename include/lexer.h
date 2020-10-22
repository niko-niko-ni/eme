#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "symbol.h"
#include "token.h"



Token_Linked_List lex_file(char filename[]);

Token_Linked_List lex_string(char str[], int file_id);
