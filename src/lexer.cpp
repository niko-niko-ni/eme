#include "lexer.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "types.h"
#include "symbol.h"
#include "token.h"
#include "errors.h"
#include "files.h"

// @Incomplete MAJOR PROBLEM: We aren't adding file_id data to tokens yet! This is because it might cause problems interfering with literals WIP meta-token parser. file_id data is already used in error messages, just update token and AST objects to include the data, as well as make sure it is set wherever the line and character are set.

bool is_valid_symbol_char(char ch) {
  return isalpha(ch) || isdigit(ch) || ch == '_';
}

bool is_valid_symbol_start_char(char ch) {
  return isalpha(ch) || ch == '_';
}

Token* syntax_token(char ch, int current_file_id, int current_line, int current_char) {
  if(ch == '"') {
    print_error_message("Incorrectly parsing a ' \" ' character as a syntax token, likely because it was placed directly next to a symbol or literal.", current_file_id, current_line, current_char);
  }
  Token *token = new Token();
  token->type = token_syntax; // @Incomplete: handle multiple literal types
  token->line = current_line;
  token->character = current_char;
  token->data.syntax = ch;
  return token;
}

Token_Linked_List lex_stream(std::istream *stream, int file_id) { // @Refactor: this function is a little janky, it's a finite state machine but some things are handled weirdly, like how boolean literals start out being processed as a symbol, or how there are some booleans like escaping_character which only matter if others are true.
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

  unsigned char ch;
  while(*stream >> std::noskipws >> ch) {

    if(ch == '\n') {
      current_line++;
      current_char = 0;
      reading_single_line_comment = false;
    } else {
      current_char++;
    }

    if(reading_single_line_comment) continue;

    // HANDLE UNICODE
    // All we're going to do here is read through unicode characters. If they are some sort of apostrophe or quotation mark we convert them to their ASCII form, and if they're unrecognized we just print an error and skip them.
    // If the most significant bit is 0, this is already in 7-bit ASCII.
    // Otherwise, it's a multi-byte unicode character.
    if(ch & (1 << 7)) {
      unsigned char utf8_ch1 = ch;

      if((utf8_ch1 & 0xe0) == 0xc0) {
        // if the three most significant bits are 110 then the unicode character is 2 utf-8 characters wide.
        char utf8_ch2;
        *stream >> utf8_ch2;

        char ub1 =  (utf8_ch1 << 3) & 0xe0;
        char ub2 = ((utf8_ch1 << 6) & 0xc0) | (utf8_ch2 & 0x3f);
        u16  u_char = (((u16)ub1)<<8) | ((u16)ub2);

        switch(u_char) {
          case 0x00B4: ch = '\''; break; //	´ - ACUTE ACCENT
          default:
            print_error_message("Unrecognized unicode character.", file_id, current_line, current_char);
            continue;
        }

      } else if((utf8_ch1 & 0xf0) == 0xe0) {
        // if the four most significant bits are 1110 then the unicode character is 3 utf-8 characters wide.
        char utf8_ch2;
        char utf8_ch3;
        *stream >> utf8_ch2;
        *stream >> utf8_ch3;

        char ub1 = ((utf8_ch1 << 4) & 0xf0) | ((utf8_ch2 >> 2) & 0x0f);
        char ub2 = ((utf8_ch2 << 6) & 0xc0) | (utf8_ch3 & 0x3f);
        u16  u_char = (((u16)ub1)<<8) | ((u16)ub2);

        switch(u_char) {
          case 0x2018: ch = '\''; break; // ‘	- LEFT SINGLE QUOTATION MARK
          case 0x2019: ch = '\''; break; // ’	- RIGHT SINGLE QUOTATION MARK
          case 0x201C: ch = '"';  break; // “	- LEFT DOUBLE QUOTATION MARK
          case 0x201D: ch = '"';  break; // ”	- RIGHT DOUBLE QUOTATION MARK
          default:
            print_error_message("Unrecognized unicode character.", file_id, current_line, current_char);
            continue;
        }

      } else if((utf8_ch1 & 0xf8) == 0xf0) {
        // if the five most significant bits are 11110 then the unicode character is 4 utf-8 characters wide.
        char utf8_ch2;
        char utf8_ch3;
        char utf8_ch4;
        *stream >> utf8_ch2;
        *stream >> utf8_ch3;
        *stream >> utf8_ch4;
        print_error_message("Unrecognized unicode character.", file_id, current_line, current_char);
        continue;
      } else {
        print_error_message("Unrecognized unicode character start.", file_id, current_line, current_char);
        continue;
      }
    }
    // Finished handling unicode, now onto actually processing the character.


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
          Token *new_last_token = syntax_token(ch, file_id, current_line, current_char);
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
              printf("Compiler internal error in lex_stream: %s", e.what());
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
          Token *new_last_token = syntax_token(ch, file_id, current_line, current_char);
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
            printf("Error converting assumed int literal (because it starts with a digit) to int.", file_id, current_line, current_string_start_char);
          }

          // add token
          tokens.last->next = token;
          tokens.last = token;
        }

        delete current_string;
        current_string = new std::string();
        if(!isspace(ch)) {
          // add token
          Token *new_last_token = syntax_token(ch, file_id, current_line, current_char);
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
  filenames.push_back(filename);
  int file_id = filenames.size()-1;
  std::fstream fin(filename, std::fstream::in);
  Token_Linked_List result = lex_stream(&fin, file_id);
  fin.close();
  return result;
}

Token_Linked_List lex_string(char str[], int file_id) {
  std::stringstream s((std::string)str);
  return lex_stream(&s, file_id);
}
