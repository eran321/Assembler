/*
 ------------------------------------------------------------------------------------
The Assembler file take a number of files that end with "as" tries to "assemble" them
if we encounter no errors 3 files would be created ("ob","ent","ext") acording to the
rules given to us.
 ------------------------------------------------------------------------------------
 */

#include "FirstPass.h"
#include "Types.h"
#include "SecondPass.h"
#include "Helper.h"
#include "Consts.h"
#include "DataEncoder.h"
#include "SymbolTable.h"
#include "OperationEncoder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void file_compilation_memory_clean();
/*
 ------------------------------------------------------------------------------------
The main function, starts the whole processes first the first pass is executed then
if no errors are encounterd the second path will be executed
 ------------------------------------------------------------------------------------
 */
int main(int argc, char* argv[]) {
	int i;
	char* curr_file;
	FILE* p_file;

	/* Check if the amount of args is more then one else print that we exepct arguments and exit */
	if (argc == 1) {
		print_runtime_error("Expected an argument");
		exit(0);
	}
	
	/* Initialize the operation list if theres an error print the error and exit*/
	if (!init_operation_list()) {
		free_operation_list();
		print_runtime_error("Could not initializes operation definitions");
		exit(0);
	}

	/* Run the Assembler on each one of the files */
	for (i=1; i < argc; i++) {
		/* intialize both of the counters*/
		unsigned int ic;
		unsigned int dc;

		/* Create full file path */
		curr_file = allocate_string(strlen(argv[i])+strlen(FILE_EXT));
		strcpy (curr_file, argv[i]);
		strcat (curr_file, FILE_EXT);

		/* Try to open the file if not succesfull exit */
		p_file = fopen(curr_file, READ_ONLY_MODE);

		if (p_file == NULL) {
			print_runtime_error("Could not open a source file");
		} else {
			/* if no errors are encounterd execute the second pass */
			bool should_continue = first_pass_execute(p_file, curr_file, &ic, &dc);

			if (should_continue) {
				rewind(p_file);
				second_pass_execute(p_file, argv[i], ic, dc);
			} 
			
			file_compilation_memory_clean();
			fclose(p_file);
		}

		free(curr_file);
	}
	free_operation_list();

	exit(0);
}
/* ------------------------------------------------------------------------------------
Function to free all the dynmicly allocated memory
   ------------------------------------------------------------------------------------
*/
void file_compilation_memory_clean() {
	free_data_node_list();
	free_symbol_list();
	return;
}