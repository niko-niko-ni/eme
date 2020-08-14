#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "types.h"

// I'm not sure if doing this without #include-ing
struct Declaration_Data;

struct Symbol_Data {
  int id;
  std::string *name;
  std::vector<Declaration_Data> *declarations;
};

extern std::vector<Symbol_Data> symbols_by_id;
extern std::unordered_map<std::string, Symbol_Data> symbols_by_name;

#include "ast.h"
