/*
--------------------------------------------------------------------------------
 The Helper file contains all the function that help procese the assembly code
---------------------------------------------------------------------------------
 */


#ifndef HELPER_H_
#define HELPER_H_

#include "Consts.h"
#include "Types.h"
#include "Enums.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/*
--------------------------------------------------------------------------------
 Function resposible for validating if the current operand is register
 gets the operand 
 return bool
---------------------------------------------------------------------------------
 */
bool is_register(char *operand);
/*
--------------------------------------------------------------------------------
 Function resposible for validating if the current operand is indirect register
 gets the operand 
 return bool
---------------------------------------------------------------------------------
 */
bool is_in_register(char *operand);

/*
--------------------------------------------------------------------------------
 Function resposible for printing compile errors 
 gets the message and the line info
---------------------------------------------------------------------------------
 */
void print_compiler_error(char *message, line_info *info);

/*
--------------------------------------------------------------------------------
 Function resposible for printing run time error 
 gets the massage that need to be printed 
---------------------------------------------------------------------------------
 */
void print_runtime_error(char *message);

/*
--------------------------------------------------------------------------------
 Function resposible allocating memory for string
 gets string length
---------------------------------------------------------------------------------
 */
char *allocate_string(int string_length);

/*
--------------------------------------------------------------------------------
 Function resposible allocating memory based on number of bytes
 gets number of bytes
---------------------------------------------------------------------------------
 */
void *allocate_memory(int bytes);

/*
--------------------------------------------------------------------------------
 Function resposible for skiping all the spaces
 gets the line info
---------------------------------------------------------------------------------
 */
void skip_all_spaces(line_info *info);

/*
--------------------------------------------------------------------------------
 Function resposible for going to the next word
 gets the current line
---------------------------------------------------------------------------------
 */
char *get_next_word(pass_data *pass);

/*
--------------------------------------------------------------------------------
 Function resposible skipping the label
 gets line_info
---------------------------------------------------------------------------------
 */
void skip_label(line_info *info);

/*
--------------------------------------------------------------------------------
 Function resposible creatring line info 
 gets file name line number and the line
---------------------------------------------------------------------------------
 */
line_info *create_line_info(char *file_name, int line_number, char *line_str);

/*
--------------------------------------------------------------------------------
 boolean function check if the line is empty or comment
 gets the line string
---------------------------------------------------------------------------------
 */
bool is_empty_or_comment(char *line);

/*
--------------------------------------------------------------------------------
 Function resposible valdidating there is no more info in line
 gets line_info
---------------------------------------------------------------------------------
 */
bool is_end_of_data_in_line(line_info *info);

/*
--------------------------------------------------------------------------------
 Function resposible for converting numbers in base 10 to targeted base (8)
gets the number in base 10 the target base and the lenth of the word tha we want
---------------------------------------------------------------------------------
 */
char *convert_base10_to_target_base(unsigned int base10_number, int target_base, int minimal_returned_length);

/*
--------------------------------------------------------------------------------
 Function resposible for validating if the current label is valid of not
 gets the  label
---------------------------------------------------------------------------------
 */
bool is_valid_label(char *str);

/*
--------------------------------------------------------------------------------
 Function resposible for checking if a string is the name of an operationg
 gets the string 
 return bool
---------------------------------------------------------------------------------
 */
bool is_operation_name(char *str);

/*
--------------------------------------------------------------------------------
 Function resposible for validating the line if the line is valid
 gets the line info
 return bool
---------------------------------------------------------------------------------
 */
bool is_valid_is_operation_line(line_info *info);

/*
--------------------------------------------------------------------------------
 Function resposible for replacing a string with a new one 
 gets pointer to curent string and a new string
---------------------------------------------------------------------------------
 */
void replace_content(char **current_string, char *new_string);

/*
--------------------------------------------------------------------------------
 Function resposible  new pass line data
 return the pointer to the new pass_data
---------------------------------------------------------------------------------
 */
pass_data *create_pass_data();

/*
--------------------------------------------------------------------------------
 Function resposible for creating new file 
 gets file name and extension
---------------------------------------------------------------------------------
 */
FILE *create_output_file(char *file_name_without_extension, char *extension);

/*
--------------------------------------------------------------------------------
 Function resposible for validating if end of the line
 gets line info
 return bool
---------------------------------------------------------------------------------
 */
bool is_end_of_line(line_info *info);

/*
--------------------------------------------------------------------------------
 Function resposible for printing the machine code to file
 gets the adress the value and the output file
---------------------------------------------------------------------------------
 */
void print_encoding_to_file(unsigned int ic, unsigned int value, FILE *p_file);

#endif /* HELPER_H_ */
