/*
 ------------------------------------------------------------------------------------
 The entry encoder file responsible for converting all the entry lines into numbers 
 base 8
 ------------------------------------------------------------------------------------
 */


#include "EntryEncoder.h"


/*
 ------------------------------------------------------------------------------------
 Function to procces the enrty line in the first pass 
 gets the pass 
 ------------------------------------------------------------------------------------
 */
void first_pass_process_entry(pass_data* pass) {
	char* entry_name = get_next_word(pass);

	/* If entry was found*/
	if (entry_name != NULL) {
		/* Check the label is valid*/
		if (is_valid_label(entry_name)) {
			/* Check if end of line, if so its an error, and if not, finish */
			if (!is_end_of_data_in_line(pass->current_line_information)) {
				print_compiler_error("Invalid tokens in end of entry definition", pass->current_line_information);
				pass->is_compiler_error = true;
			}
		}
		/* Throw error if label is invalid */
		else {
			print_compiler_error("Entry name must be a valid label", pass->current_line_information);
			pass->is_compiler_error = true;
		}
	}
	/* Throw error if there is no entry name*/
	else if (!pass->is_runtimer_error) {
		print_compiler_error("Missing entry name", pass->current_line_information);
		pass->is_compiler_error = true;
	}
}

/*
 ------------------------------------------------------------------------------------
 Function to procces the enrty line in the second pass 
 gets the pass and the files where the entry will be printed
 ------------------------------------------------------------------------------------
 */

void second_pass_process_entry(pass_data* pass, compiler_output_files* output_files) {
	char* entry_name = get_next_word(pass);

	/* Search for the entry inside the symbol table */
	symbol_node_ptr p_symbol = search_symbol(entry_name);

	/* Check if the entry exists if it doesn't print error*/
	if (p_symbol == NULL) {
		print_compiler_error("Invalid entry definition. Label doesn't exists.", pass->current_line_information);
		pass->is_compiler_error = true;
	} else {
		create_entry_output_file_if_needed(output_files, pass->current_line_information->file_name);

		if (output_files->entry_file == NULL) {
			pass->is_runtimer_error = true;
			return;
		}

		write_entry_to_output_file(entry_name, p_symbol->current_symbol.address, output_files->entry_file);
	}
}


/*
 ------------------------------------------------------------------------------------
 Function that creates an output file if needed
 gets the output files needed and the extensios
 ------------------------------------------------------------------------------------
 */
void create_entry_output_file_if_needed(compiler_output_files* output_files, char* file_name_without_extension) {
	/* check if there is no entry file*/
	if (output_files->entry_file == NULL) {
		output_files->entry_file = create_output_file(file_name_without_extension, ENTRY_FILE_EXT);
	}
}

/*
 ------------------------------------------------------------------------------------
 Function that print the entry to the .ent file
 gets the name of the enty the address the the file where the entry will be printed
 ------------------------------------------------------------------------------------
 */
void write_entry_to_output_file(char* entry_name, unsigned int address, FILE* output_file) {
	char* base;

	/* Add entry to output file  */
	fputs(entry_name, output_file);
	fputc(COLUMN_SEPARATOR, output_file);

	/* Add current entry address to output file */
	base = convert_base10_to_target_base(address, BASE_TEN, TARGET_MEMORY_ADDRESS_WORD_LENGTH);
	fputs(base, output_file);
	free(base);

	/* Add new Line*/
	fputc(END_OF_LINE, output_file);
}
