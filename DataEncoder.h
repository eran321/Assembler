/*
 ------------------------------------------------------------------------------------
 The data encoder file responsible for converting all the .data and .string lines
 into numbers base 8
 ------------------------------------------------------------------------------------
 */
#ifndef DATA_H_
#define DATA_H_

#include "Types.h"
#include "Helper.h"
#include "Consts.h"
#include "SymbolTable.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
------------------------------------------------------------------------------------
Function that procceses the line according to the type of the line
gets the current pass the label the data type and if its a symbol or not
------------------------------------------------------------------------------------
 */
void first_pass_process_data(pass_data* pass, char* label, char* type, bool is_symbol);

/*
------------------------------------------------------------------------------------
Function that procceses the line according to the type of the line
gets the current pass the label the data type and if its a symbol or not
------------------------------------------------------------------------------------
 */
void process_string(pass_data* pass);

/*
------------------------------------------------------------------------------------
Function that procceses the line according to the type of the line
gets the current pass the label the data type and if its a symbol or not
------------------------------------------------------------------------------------
 */
void process_numbers(pass_data* pass);

/*
------------------------------------------------------------------------------------
Function that gets tge next number
gets the pass and a pointer to a number
------------------------------------------------------------------------------------
 */
bool get_next_number(pass_data* pass, int* number);

/*
------------------------------------------------------------------------------------
Function that gets a data_node_poninter adds it to the data list 
------------------------------------------------------------------------------------
 */
void add_data_node_to_list(data_node_ptr p_new_data);

/*
------------------------------------------------------------------------------------
Function that adds string data to the list
gets a char and the address
------------------------------------------------------------------------------------
 */
bool add_string_data_to_list(char token, unsigned int address);

/*
------------------------------------------------------------------------------------
Function that adds numeric data to the list
gets a number and the address 
------------------------------------------------------------------------------------
 */
bool add_numeric_data_to_list(int number, unsigned int address);

/*
------------------------------------------------------------------------------------
Function that updates the current ic 
------------------------------------------------------------------------------------
 */
void update_data_address(int ic_length);

/*
------------------------------------------------------------------------------------
Function that writes the the date to the output file 
gets the file where the data would be written to
------------------------------------------------------------------------------------
 */
void write_data_to_output_file(FILE* p_file);

/*
------------------------------------------------------------------------------------
Function that frees the memory allocated the list
------------------------------------------------------------------------------------
 */
void free_data_node_list();

#endif /*  end of DATA_H_ */
