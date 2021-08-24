/*
 ------------------------------------------------------------------------------------
 The entry encoder file responsible for converting all the entry lines into numbers 
 base 8
 ------------------------------------------------------------------------------------
 */

#ifndef ENTRYENCODER_H_
#define ENTRYENCODER_H_

#include "Consts.h"
#include "Types.h"
#include "SymbolTable.h"
#include "Helper.h"
#include <stdlib.h>
#include <stdio.h>

/*
 ------------------------------------------------------------------------------------
 Function to procces the entry line in the first pass 
 gets the pass 
 ------------------------------------------------------------------------------------
 */
void first_pass_process_entry(pass_data* pass);

/*
 ------------------------------------------------------------------------------------
 Function to procces the entry line in the second pass 
 gets the pass and the files where the entry will be printed
 ------------------------------------------------------------------------------------
 */
void second_pass_process_entry(pass_data* pass, compiler_output_files* output_files);

/*
 ------------------------------------------------------------------------------------
 Function that print the entry to the .ent file
 gets the name of the enty the address the the file where the entry will be printed
 ------------------------------------------------------------------------------------
 */
void write_entry_to_output_file(char* entry_name, unsigned int address, FILE* output_file);

/*
 ------------------------------------------------------------------------------------
 Function that creates an output file if needed
 gets the output files needed and the extensios
 ------------------------------------------------------------------------------------
 */
void create_entry_output_file_if_needed(compiler_output_files* output_files, char* file_name_without_extension);

#endif /* ENTRYENCODER_H_ */
