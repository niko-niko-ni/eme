#include "lexer.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "types.h"
#include "symbol.h"
#include "token.h"


bool is_valid_symbol_char(char ch) {
  return isalpha(ch) || isdigit(ch) || ch == '_';
}

bool is_valid_symbol_start_char(char ch) {
  return isalpha(ch) || ch == '_';
}

Token* syntax_token(char ch, int current_line, int current_char) {
  if(ch == '"') {
    printf("Error: parsing a ' \" ' character as a syntax token, likely because it was placed directly next to a symbol or literal.");
  }
  Token *token = new Token();
  token->type = token_syntax; // @Incomplete: handle multiple literal types
  token->line = current_line;
  token->character = current_char;
  token->data.syntax = ch;
  return token;
}

Token_Linked_List lex_stream(std::basic_iostream<char>* stream) { // @Refactor: this function is a little janky, it's a finite state machine but some things are handled weirdly, like how boolean literals start out being processed as a symbol, or how there are some booleans like escaping_character which only matter if others are true.
  Token_Linked_List tokens;
  tokens.first = new Token();
  tokens.first->character = 0;
  tokens.first->line = 0;
  tokens.first->type = token_eol;

  tokens.last = tokens.first;

  std::string *current_string = new std::string(); // contains literals or symbols, is reset after they are finished being read. This is a pointer because sometimes we don't want to delete the string object, such as if we want to store a pointer to it in the symbols_by_name unordered map or use a pointer to it in a string literal token.
  int current_string_start_line = 0;
  int current_string_start_char = 0;
  bool reading_symbol  = false;
  bool reading_literal = false;
  bool reading_string = false;
  bool reading_single_line_comment = false;
  bool escaping_character = false;

  int current_line = 1;
  int current_char = 0;

  char ch;
  while(*stream >> std::noskipws >> ch) {

    if(ch == '\n') {
      current_line++;
      current_char = 0;
      reading_single_line_comment = false;
    } else {
      current_char++;
    }

    if(reading_single_line_comment) continue;



    if(!reading_symbol && !reading_literal && !reading_string) {

      if(is_valid_symbol_start_char(ch)) {
        current_string->push_back(ch);
        current_string_start_line = current_line;
        current_string_start_char = current_char;
        reading_symbol = true;
      } else if(isdigit(ch)) {
        current_string->push_back(ch);
        current_string_start_line = current_line;
        current_string_start_char = current_char;
        reading_literal = true;
      } else if(!isspace(ch)) {
        if(ch == '#') {
          reading_single_line_comment = true;
        } else if(ch == '"') {
          reading_string = true;
          current_string_start_line = current_line;
          current_string_start_char = current_char;
        } else {
          // add token
          Token *new_last_token = syntax_token(ch, current_line, current_char);
          tokens.last->next = new_last_token;
          tokens.last = new_last_token;
        }
      }





    } else if(reading_symbol && !reading_literal && !reading_string) { // reading symbol

      if(is_valid_symbol_char(ch)) {
        current_string->push_back(ch);
      } else {
        reading_symbol = false;

        bool delete_current_string = true;

        // boolean literal
        if(current_string->compare("true") == 0 || current_string->compare("false") == 0) {
          Token *token = new Token();
          token->type = token_literal_bool;
          token->line = current_line;
          token->character = current_string_start_char;
          token->data.literal_bool = current_string->compare("true");

          // add token
          tokens.last->next = token;
          tokens.last = token;
        } else { // add_symbol_token
          Token *token = new Token();
          token->type = token_symbol;
          token->line = current_line;
          token->character = current_string_start_char;
          token->data.symbol = 0;

          std::unordered_map<std::string, Symbol_Data>::const_iterator got = symbols_by_name.find(*current_string);
          if (got == symbols_by_name.end()) { // if the symbol doesn't exist in symbols_by_name

            Symbol_Data symbol_data;
            Symbol symbol_id = symbols_by_id.size();
            token->data.symbol = symbol_id;
            symbol_data.id = symbol_id;

            symbol_data.name = current_string;
            delete_current_string = false;

            symbols_by_id.push_back(symbol_data);

            try {
              symbols_by_name[*current_string] = symbol_data;
            } catch (const std::exception& e) {
              printf("Error in lex_stream: %s\n", e.what());
              exit(1);
            }

          } else {

            Symbol_Data symbol_data = got->second;
            token->data.symbol = symbol_data.id;


          }


          // add token
          tokens.last->next = token;
          tokens.last = token;
        }



        if (delete_current_string) delete current_string;
        current_string = new std::string();
        if(!isspace(ch)) {
          // add token
          Token *new_last_token = syntax_token(ch, current_line, current_char);
          tokens.last->next = new_last_token;
          tokens.last = new_last_token;
        }
      }




    } else if(!reading_symbol && reading_literal && !reading_string) { // reading literal
      if(is_valid_symbol_char(ch)) { // valid symbol characters are the same as valid literal characters
        current_string->push_back(ch);
      } else {
        reading_literal = false;

        { // add_literal_token
          Token *token = new Token();
          token->type = token_literal_int; // @Incomplete: handle multiple literal types
          token->line = current_line;
          token->character = current_string_start_char;
          try {
            token->data.literal_int = std::stoi(*current_string);
          } catch(...) {
            printf("Error converting assumed int literal '%s' to int.\n", current_string->c_str());
          }

          // add token
          tokens.last->next = token;
          tokens.last = token;
        }

        delete current_string;
        current_string = new std::string();
        if(!isspace(ch)) {
          // add token
          Token *new_last_token = syntax_token(ch, current_line, current_char);
          tokens.last->next = new_last_token;
          tokens.last = new_last_token;
        }
      }
    } else if(!reading_symbol && !reading_literal && reading_string) {
      if(ch == '"' && !escaping_character) {
        Token *token = new Token();
        token->character = current_string_start_char;
        token->line = current_string_start_line;
        token->type = token_literal_string;
        token->data.literal_string = current_string;

        tokens.last->next = token;
        tokens.last = token;

        current_string = new std::string();
        reading_string = false;
      } else if(ch == '\\' && !escaping_character) {
        escaping_character = true;
      } else {
        current_string->push_back(ch);
        escaping_character = false;
      }
    }
  }

  { // add token_eol token as last token
    Token *end_token = new Token();
    end_token->type = token_eol;
    end_token->line = -1;
    end_token->character = -1;

    tokens.last->next = end_token;
    tokens.last = end_token;
  }

  { // remove first token (which is token_eol) from the linked list
    Token *new_first_token = tokens.first->next;
    delete tokens.first;
    tokens.first = new_first_token;
  }

  return tokens;
}

Token_Linked_List lex_file(char filename[]) {
  std::fstream fin(filename, std::fstream::in);
  Token_Linked_List result = lex_stream(&fin);
  fin.close();
  return result;
}

Token_Linked_List lex_string(char str[]) {
  std::stringstream s((std::string)str);
  return lex_stream(&s);
}
