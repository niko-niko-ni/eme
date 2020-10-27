#pragma once
#include "token.h"
enum Type_Info_Type {
  type_uint,
  type_int,
  type_i8,
  type_i16,
  type_i32,
  type_i64,
  type_u8,
  type_u16,
  type_u32,
  type_u64,

  type_struct,
  type_pointer,
  type_array
};

struct Type_Info {
  bool is_const;
  Type_Info_Type type;
  union {
    // let's not do structs yet.

    struct {
      Type_Info *value_type;
      uint dereference_count;
    } pointer;

    struct {
      Type_Info *element_type;
    } array;
  } data;
};

union Ast_Value {
  u64 _uint;
  i64 _int;
  i8  _i8;
  i16 _i16;
  i32 _i32;
  i64 _i64;
  u8  _u8;
  u16 _u16;
  u32 _u32;
  u64 _u64;
};
enum Ast_Node_Type {
  node_eol,
  node_set_statement,
  node_decl,
  node_decl_with_set,
  node_token_list,
  node_root
};

struct Ast_Node;

struct Ast_Node_Linked_List {
  Ast_Node *first;
  Ast_Node *last;
};

struct Ast_Node {
  Ast_Node_Type type;

  int line = -1;
  int character = -1;
  int file = -1;

  union {
    struct {
      Ast_Node *identifier;
      Ast_Node *value;
    } statement;

    struct {
      Ast_Node *identifier;
      Ast_Node *type;
    } decl;

    struct {
      Ast_Node *identifier;
      Ast_Node *type;
      Ast_Node *value;
    } decl_with_set;

    struct Token_Linked_List sub_tokens;
    Ast_Node_Linked_List sub_nodes;
  } data;
  Ast_Node *next;
};

struct Declaration_Data {
  Ast_Node *block;
  Ast_Node *declaration;
  //Type_Info type;
};

void print_node(Ast_Node token, int indentation=0);

void print_all_nodes_after(Ast_Node node, int indentation=0);
