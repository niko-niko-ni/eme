#include "errors.h"
#include <fstream>
#include "types.h"
#include "files.h"


// @Incomplete Possibly add red color to error messages?
void print_error_message(char *message, int file_id, int line, int character) {
  if(file_id >= 0 && file_id < filenames.size()) {
    print_line_in_file(file_id, line);

    if(line >= 0 && character >= 0) {
      printf("Error at line %d, character %d: %s\n\n", line, character, message);
    } else if(line >= 0) {
      printf("Error at line %d: %s\n\n", line, message);
    } else {
      printf("Error at unknown location: %s\n\n", message);
    }
  } else {
    printf("Error at unknown location in unknown file: %s\n\n", message);
  }
}

void print_line_in_file(int file_id, int line) {
  if(file_id >= 0 && file_id < filenames.size()) {
    char *filename = filenames[file_id];
    printf("[In file: %s]\n", filename);

    if(line >= 0) {
      std::ifstream stream (filename);

      stream >> std::noskipws;
      char ch;
      uint current_line = 1;

      while (stream>>ch) {
        if(ch == '\n') {
          current_line++;

          if(current_line == line) {
            printf("%d | ", line);
            while(stream>>ch) {
              printf("%c", ch);
              if(ch == '\n') {
                return;
              }
            }
            printf("\n");
            return;
          }
        }
      }
    }
  } else {
    printf("Error in print_line_in_file: Attempted to print line %d in a file, but the filename associated with file_id %d could not be found.\n", line, file_id);
  }
}
