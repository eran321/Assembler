/*
 ------------------------------------------------------------------------------------
 The extern encoder file responsible for converting all the .extern lines into numbers 
 base 8
 ------------------------------------------------------------------------------------
 */
#include "ExternEncoder.h"



/*
 ------------------------------------------------------------------------------------
 Function that converts the .extern lines in to base 8 and prints to the .ext file
 gets the pass
 ------------------------------------------------------------------------------------
 */
void first_pass_process_extern(pass_data* pass) {
	symbol_node_ptr p_symbol = NULL;

	char* extern_name = get_next_word(pass);

	/* If extern was found*/
	if (extern_name != NULL) {
		symbol_node_ptr p_searched_symbol = search_symbol(extern_name);

		/* Extern does not exists  */
		if (!p_searched_symbol ) {
			/* Create new symbol in table or exit */
			p_symbol = create_symbol(extern_name, NO_ADDRESS, true, true);
			if (p_symbol != NULL) {
				add_symbol_to_list(p_symbol);

				/* Make sure that the line does not contain more info that is not needed */
				if (!is_end_of_data_in_line(pass->current_line_information)) {
					print_compiler_error("Invalid tokens after extern definition", pass->current_line_information);
					pass->is_compiler_error = true;
					return;
				}
			}
			/* If could not allocate mem exit */
			else {
				pass->is_runtimer_error = true;
				free(extern_name);
				return;
			}
		}
		/* Extern already exists in table */
		else {
			print_compiler_error("Each extern can be defined only once", pass->current_line_information);
			pass->is_compiler_error = true;
			return;
		}
	}
}

/*
 ------------------------------------------------------------------------------------
 Function that creates an output file if needed
 gets the output files needed and the extensios
 ------------------------------------------------------------------------------------
 */
void create_extern_output_file_if_needed(compiler_output_files* output_files, char* file_name_without_extension) {
	if (output_files->extern_file == NULL) {
		output_files->extern_file = create_output_file(file_name_without_extension, EXTERN_FILE_EXT);
	}
}


/*
 ------------------------------------------------------------------------------------
 Function that print the entry to the .ext file
 gets the name of the enty the address the the file where the extern will be printed
 ------------------------------------------------------------------------------------
 */
void write_extern_to_output_file(char* extern_operand, unsigned int current_address, FILE* output_file) {
	char* base;

	/* Add extern to output file */
	fputs(extern_operand, output_file);
	fputc(COLUMN_SEPARATOR, output_file);

	/* Add current extern address to output file */
	base = convert_base10_to_target_base(current_address, BASE_TEN, TARGET_MEMORY_ADDRESS_WORD_LENGTH);
	fputs(base, output_file);
	free(base);

	/* Add new line */
	fputc(END_OF_LINE, output_file);
}
 