#include "ast.h"
#include "token.h"
#include "types.h"

const uint INDENT_INCREASE = 4;
const uint HALF_INCREASE = 2;

void print_node(Ast_Node node, int indentation) {
  printf("%*c\bAt line %i, character %i, file %i:\n", indentation, ' ', node.line, node.character, node.file);
  switch(node.type) {
    case node_set_statement:
      printf("%*c\bAST node set_statement:\n\n", indentation, ' ');
      printf("%*c\bidentifier:\n\n", indentation+HALF_INCREASE, ' ');
      print_node(*node.data.statement.identifier, indentation+INDENT_INCREASE);

      printf("%*c\bvalue:\n\n", indentation+HALF_INCREASE, ' ');
      print_node(*node.data.statement.value, indentation+INDENT_INCREASE);
      break;

    case node_decl:
      printf("%*c\bAST node decl:\n\n", indentation, ' ');
      printf("%*c\bidentifier:\n\n", indentation+HALF_INCREASE, ' ');
      print_node(*node.data.decl.identifier, indentation+INDENT_INCREASE);

      printf("%*c\btype:\n\n", indentation+HALF_INCREASE, ' ');
      print_node(*node.data.decl.type, indentation+INDENT_INCREASE);
      break;

    case node_decl_with_set:
      printf("%*c\bAST node decl_with_set:\n\n", indentation, ' ');
      printf("%*c\bidentifier:\n\n", indentation+HALF_INCREASE, ' ');
      print_node(*node.data.decl_with_set.identifier, indentation+INDENT_INCREASE);

      printf("%*c\btype:\n\n", indentation+HALF_INCREASE, ' ');
      print_node(*node.data.decl_with_set.type, indentation+INDENT_INCREASE);

      printf("%*c\bvalue:\n\n", indentation+HALF_INCREASE, ' ');
      print_node(*node.data.decl_with_set.value, indentation+INDENT_INCREASE);
      break;

    case node_token_list:
      printf("%*c\bAST node token_list:\n\n", indentation, ' ');
      print_all_tokens_after(*node.data.sub_tokens.first, indentation+INDENT_INCREASE);
      break;

    case node_root:
      printf("%*c\bAST node root:\n\n", indentation, ' ');
      print_all_nodes_after(*node.data.sub_nodes.first, indentation+INDENT_INCREASE);
      break;
  }
}

void print_all_nodes_after(Ast_Node node, int indentation) {
  Ast_Node *current_node = &node;
  while(current_node->type != node_eol) {
    print_node(*current_node, indentation);
    current_node = current_node->next;
  }
}
