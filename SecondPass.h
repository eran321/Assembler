/*
--------------------------------------------------------------------------------
 The SecondPass file holds all the relevant functions to the second pass through 
 the assembly code
---------------------------------------------------------------------------------
 */
#ifndef SECONDPASS_H_
#define SECONDPASS_H_

#include "Types.h"
#include "Enums.h"
#include "Consts.h"
#include "SymbolTable.h"
#include "Helper.h"
#include "OperationEncoder.h"
#include "DataEncoder.h"
#include "EntryEncoder.h"
#include "ExternEncoder.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
--------------------------------------------------------------------------------
 Function that executes the second pass
 gets the source file, the name of file , the previous ic and dc
---------------------------------------------------------------------------------
 */
void second_pass_execute(FILE *pFile, char *file_name_without_extension, unsigned int previous_pass_ic, unsigned int previous_pass_dc);

/*
--------------------------------------------------------------------------------
 Function that proceces a line in the second pass
 gets the current line the output files 
---------------------------------------------------------------------------------
 */
void second_pass_process_line(pass_data *pass, compiler_output_files *output_files);

/*
--------------------------------------------------------------------------------
 Function that resposible for printing the the ic and dc to the output file
---------------------------------------------------------------------------------
 */
void write_code_and_data_size_to_output_file(unsigned int ic, unsigned int dc, FILE *output_file);

#endif /* SECONDPASS_H_ */
