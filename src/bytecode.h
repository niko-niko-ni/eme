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
struct BytecodeInstructionLinkedList;
struct ConditionalCommandData;

struct BytecodeInstructionLinkedList {
  BytecodeInstruction *first;
  BytecodeInstruction *last;
};
struct ConditionalCommandData {
  int condition;
  BytecodeInstructions subInstructions;
};

struct BytecodeInstruction {
  BytecodeCommandType type; //operation type
  union {
    BytecodeInstructions subInstructions,
    int[] arguments,
    ConditionalCommandData conditional //special case for if statemends
  } sub_data;

};
struct BytecodeProcedure {
  int id;
  Type_Info bytecodeArguments[];
  BytecodeInstructions body;
};
