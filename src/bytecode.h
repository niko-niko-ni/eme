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
  op_2_addrs //add more operations here
};
struct BytecodeProcedure; //contains everything
struct BytecodeInstruction; //contains individual instructions
struct ConditionalCommandData;

struct ConditionalCommandData {
  int condition;
  ByteCodeInstruction[] sub_instrs;
}
struct BytecodeInstruction {
  BytecodeCommandType type; //operation type
  union {
    ByteCodeInstruction[] sub_instrs,
    int[] arguments,
    ConditionalCommandData conditional //special case for if statemends
  } sub_data;

}
struct BytecodeProcedure {
  int id;
  Type_Info[] args;
  BytecodeInstruction[] body;
}
