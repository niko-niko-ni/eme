#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "types.h"


struct Symbol_Data {
  int id;
  std::string *name;
};

extern std::vector<Symbol_Data> symbols_by_id;
extern std::unordered_map<std::string, Symbol_Data> symbols_by_name;
