#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "symbol.h"
#include "token.h"



Token_Linked_List lex_file(char filename[], std::vector<Symbol_Data> *symbols_by_id, std::unordered_map<std::string, Symbol_Data> *symbols_by_name);

Token_Linked_List lex_string(char str[], std::vector<Symbol_Data> *symbols_by_id, std::unordered_map<std::string, Symbol_Data> *symbols_by_name);
