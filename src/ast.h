#pragma once
#include "token.h"

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

    Token_Linked_List sub_tokens;
    Ast_Node_Linked_List sub_nodes;
  } data;
  Ast_Node *next;
};

void print_node(Ast_Node token, int indentation=0);

void print_all_nodes_after(Ast_Node node, int indentation=0);
