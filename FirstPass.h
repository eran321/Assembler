/*
--------------------------------------------------------------------------------
 The FirstPass file holds the most relevent functions for the firstpass
---------------------------------------------------------------------------------
 */

#ifndef FIRSTPASS_H_
#define FIRSTPASS_H_

#include "Types.h"
#include "Enums.h"
#include "OperationEncoder.h"
#include "Helper.h"
#include "DataEncoder.h"
#include "EntryEncoder.h"
#include "ExternEncoder.h"
#include "SymbolTable.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "Consts.h"



/*
--------------------------------------------------------------------------------
 Function that exectutes the first pass 
 gets the input file IC and DC
---------------------------------------------------------------------------------
 */
bool first_pass_execute(FILE* assembler_input_file, char* file_name_without_extension, unsigned int* IC, unsigned int* DC);

/*
--------------------------------------------------------------------------------
 Function procces the curent line according to the lines type
 gets the currnet line 
---------------------------------------------------------------------------------
 */
void first_pass_process_line(pass_data* pass);

#endif /* FIRSTPASS_H_ */
