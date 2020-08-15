#pragma once


// Given an error message, line, character, and file, prints out the error with a nice display of the line in which it occurred.
// If any of the last 3 values are negative, their values are assumed to not be known the error message adapts to display correctly. (for example, it only displays the file if the line is negative, or it says "unknown location" if the file_id is invalid)
void print_error_message(char *message, int file_id, int line, int character);


// Given a file id and a line number, prints that line out in the below format:

// [In file: $filename]
// $line | $line_contents

// If line is negative, just the [In file: $filename] is printed.
// If file_id is negative, an error is printed.
void print_line_in_file(int file_id, int line);
