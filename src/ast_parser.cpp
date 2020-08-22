#include "ast_parser.h"
#include "token.h"
#include "ast.h"
#include "errors.h"


// @Leak @Incomplete memory leak here. After statements is used we're done with it and it can be deleted.
Ast_Node *parse_statements_to_ast(Token_Linked_List statements) {
  // Just loops over a linked list of statements and converts each of them into an AST node, then places them into a root AST node and returns that.
  Ast_Node *root = new Ast_Node();
  root->type = node_root;
  root->line = statements.first->line;
  root->character = statements.first->character;
  root->file = statements.first->file;
  Token *current_statement = statements.first;

  bool sub_nodes_initialized = false;
  while(current_statement->type != token_eol) {
    if(current_statement->type != token_statement) {
      print_error_message("Error in parse_statements_to_ast: the list of statements passed contained a token which was not of type 'token_statement'.\n", current_statement->file, current_statement->line, current_statement->character);
      print_token(*current_statement);
      exit(1);
    }
    Ast_Node *result = parse_statement_to_ast(*current_statement);

    if(sub_nodes_initialized) {
      root->data.sub_nodes.last->next = result;
      root->data.sub_nodes.last = result;
    } else {
      root->data.sub_nodes.first = result;
      root->data.sub_nodes.last = result;
      sub_nodes_initialized = true;
    }
    current_statement = current_statement->next;
  }

  Ast_Node *eol = new Ast_Node();
  eol->type = node_eol;

  root->data.sub_nodes.last->next = eol;
  root->data.sub_nodes.last = eol;

  return root;
}

Ast_Node *ast_node_from_tokens(Token_Linked_List tokens, int line, int character, int file) {
  Ast_Node *result = new Ast_Node();
  result->type = node_token_list;
  result->data.sub_tokens = tokens;
  result->line = line;
  result->character = character;
  result->file = file;
  return result;
}

Ast_Node *parse_statement_to_ast(Token statement) {

  int line = statement.line;
  int character = statement.character;
  int file = statement.file;

  Token_Linked_List sub_tokens = statement.data.sub_tokens;
  Token *current_token = sub_tokens.first;

  // Loop through every token that isn't a : or an =
  while(current_token->type != token_syntax || !(current_token->data.syntax == '=' || current_token->data.syntax == ':')) {
    if(current_token->type == token_eol) {
      // If we ran into the end of the sub-token list without encountering an = or :, then the statement is just an expression.
      return ast_node_from_tokens(sub_tokens, line, character, file);
    }

    current_token = current_token->next;
  }


  // If we ran into a : or an =, then:

  // We construct a linked list forming the far left (identifier) portion of the statement, which we'll use in actually constructing the AST statement object once we've processed all the parts of the statement.
  current_token->type = token_eol;
  Token_Linked_List identifier_portion; // the far left part of a statement
  identifier_portion.first = sub_tokens.first;
  identifier_portion.last = current_token;

  if(current_token->data.syntax == ':') {

    current_token = current_token->next;
    // Parsing a declaration
    while(current_token->type != token_syntax || current_token->data.syntax != '=') {
      if(current_token->type == token_eol) {
        // If we ran into the end of the sub-token list without encountering an =, then the statement is just a declaration without a value.
        Token_Linked_List type_portion;
        type_portion.first = identifier_portion.last->next;
        type_portion.last = current_token;

        Ast_Node *decl = new Ast_Node();
        decl->type = node_decl;
        decl->data.decl.identifier = ast_node_from_tokens(identifier_portion, line, character, file);
        decl->data.decl.type = ast_node_from_tokens(type_portion, line, character, file);
        decl->line = line;
        decl->character = character;
        decl->file = file;
        return decl;
      }
      current_token = current_token->next;
    }

    // If we ran into an = then the expression is a declaration which also sets to a value
    current_token->type = token_eol;
    Token_Linked_List type_portion; // the part of a statement in-between the : and the =
    type_portion.first = identifier_portion.last->next;
    type_portion.last = current_token;

    current_token = current_token->next;
    Token_Linked_List value_portion; // the part of a statement in-between the : and the =
    value_portion.first = current_token;
    value_portion.last = sub_tokens.last;

    Ast_Node *decl = new Ast_Node();
    decl->type = node_decl_with_set;
    decl->data.decl_with_set.identifier = ast_node_from_tokens(identifier_portion, line, character, file);
    decl->data.decl_with_set.type = ast_node_from_tokens(type_portion, line, character, file);
    decl->data.decl_with_set.value = ast_node_from_tokens(value_portion, line, character, file);
    decl->line = line;
    decl->character = character;
    decl->file = file;
    return decl;

  } else {
    // Parsing a set statement without a declaration
    Token_Linked_List value_portion;
    value_portion.first = identifier_portion.last->next;
    value_portion.last = sub_tokens.last;

    Ast_Node *statement = new Ast_Node();
    statement->type = node_set_statement;
    statement->data.statement.identifier = ast_node_from_tokens(identifier_portion, line, character, file);
    statement->data.statement.value = ast_node_from_tokens(value_portion, line, character, file);
    statement->line = line;
    statement->character = character;
    statement->file = file;

    return statement;
  }
}
