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
  BytecodeInstructionLinkedList subInstructions;
};

struct BytecodeInstruction {
  BytecodeCommandType type; //operation type
  union {
    BytecodeInstructionLinkedList subInstructions;
    int arguments[];
    ConditionalCommandData conditional;
    //special case for if statemends
  } sub_data;
  BytecodeInstruction *next;

};
struct BytecodeProcedure {
  int id;
  BytecodeInstructionLinkedList body;
  Type_Info bytecodeArguments[];
};
