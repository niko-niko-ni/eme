#pragma once

#include "types.h"
#include "ast.h"
#include <string>

enum BytecodeCommandType {
  if_command,
  loop_command,
  break_command,
  return_command,
  cp_command,
  lodarg_command
};
enum BytecodeCommandDataType {
  sub_isntrs,
  op_2_addrs //add more here
};
struct BytecodeProcedure;
struct BytecodeInstruction;

struct BytecodeInstruction {
  BytecodeCommandType type;
  union {
    ByteCodeInstruction[] sub_instrs;
    int[] arguments;
  } sub_data;

}
struct BytecodeProcedure {
  int id;
  Type_Info[] args;
  BytecodeInstruction[] body;
}
