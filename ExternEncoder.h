/*
 ------------------------------------------------------------------------------------
 The extern encoder file responsible for converting all the .extern lines into numbers 
 base 8
 ------------------------------------------------------------------------------------
 */

#ifndef EXTERNENCODER_H_
#define EXTERNENCODER_H_


#include "Types.h"
#include "Consts.h"
#include "Helper.h"
#include "SymbolTable.h"
#include <stdlib.h>
#include <stdio.h>



/*
 ------------------------------------------------------------------------------------
 Function that converts the .extern lines in to base 8 and prints to the .ext file
 gets the pass
 ------------------------------------------------------------------------------------
 */
void first_pass_process_extern(pass_data* pass);

/*
 ------------------------------------------------------------------------------------
 Function that creates an output file if needed
 gets the output files needed and the extensios
 ------------------------------------------------------------------------------------
 */
void create_extern_output_file_if_needed(compiler_output_files* output_files, char* file_name_without_extension);

/*
 ------------------------------------------------------------------------------------
 Function that print the entry to the .ext file
 gets the name of the enty the address the the file where the extern will be printed
 ------------------------------------------------------------------------------------
 */
void write_extern_to_output_file(char* extern_operand, unsigned int current_address, FILE* output_file);

#endif /* EXTERNENCODER_H_ */
