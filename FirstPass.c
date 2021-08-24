/*
--------------------------------------------------------------------------------
 The FirstPass file holds the most relevent functions for the firstpass
---------------------------------------------------------------------------------
 */


#include "FirstPass.h"

/*
--------------------------------------------------------------------------------
 Function that exectutes the first pass 
 gets the input file IC and DC
---------------------------------------------------------------------------------
 */
bool first_pass_execute(FILE* assembler_input_file, char* file_name_without_extension, unsigned int* IC, unsigned int* DC) {
	/* Creates the first pass data */
	pass_data* pass = create_pass_data();
	int line_number = 0;
	bool success = true;

	if (pass == NULL) {
		return false;
	}

	/*  Initialization fo ic and dc */
	pass->IC = 0;
	pass->DC = 0;

	/* Loop that runs until the end of file */
	while (!feof(assembler_input_file) && !pass->is_runtimer_error) {
		char line[MAX_LINE_LENGTH];

		line_number++;

		/* Read new line  */
		if (fgets(line, MAX_LINE_LENGTH + 1, assembler_input_file)) {

			/* This isn't an empty line or a comment */
			if (!is_empty_or_comment(line)) {
				line_info* info = create_line_info(file_name_without_extension, line_number, line);

				/* Start processing the line */
				if (info != NULL) {
					pass->current_line_information = info;

					first_pass_process_line(pass);
					success &= !(pass->is_compiler_error);

					free(info);
				} else {
					pass->is_runtimer_error = true;
				}
			}
		}
	}

	/* No error has occurred during the first pass */
	if (!pass->is_compiler_error && !pass->is_runtimer_error ) {
		/* Changes the data address according to the code length */
		update_data_address(pass->IC);
		update_symbol_address(pass->IC);

		*IC = pass->IC;
		*DC = pass->DC;
	}
	if (pass->IC + pass->DC > MAX_MEM) {
		printf("Out of memory, Please give a shorter file\n");
		return false;
	}

	/* free memory */
	if (pass->prev_operation_operand != NULL) {
		free(pass->prev_operation_operand);
	}

	free(pass);

	return success & !pass->is_runtimer_error;
}

/*
--------------------------------------------------------------------------------
 Function procces the curent line according to the lines type
 gets the currnet line 
---------------------------------------------------------------------------------
 */
void first_pass_process_line(pass_data* pass) {
	char* label = NULL;
	char* line_type = NULL;
	bool is_symbol = false;
	char* partial_line;

	skip_all_spaces(pass->current_line_information);

	/* Find label */
	if ((partial_line = strchr(pass->current_line_information->line_str, LABEL_END_TOKEN)) != NULL) {

		/* Find label's position */
		int start_label = pass->current_line_information->current_index;
		int label_length = partial_line - pass->current_line_information->line_str - start_label;
		label = allocate_string(label_length);
		strncpy(label, pass->current_line_information->line_str + start_label, label_length);
		label[label_length] = END_OF_STRING;

		/* Check label is valid */
		if (is_valid_label(label)) {
			/* Turn symbol flag on */
			is_symbol = true;
			pass->current_line_information->current_index += label_length + 1;
		} else {
			print_compiler_error("Invalid label definition", pass->current_line_information);
			pass->is_compiler_error = true;
			return;
		}
	}

	/* Read line type */
	line_type = get_next_word(pass);

	/* Handle line type */
	if (line_type == NULL) {
		print_compiler_error("Invalid line", pass->current_line_information);
		pass->is_compiler_error = true;
	}
	/* Line is data initialization */
	else if ((strcmp(line_type, DATA_OPERATION) == 0) || (strcmp(line_type, STRING_OPERATION) == 0)) {
		first_pass_process_data(pass, label, line_type, is_symbol);
	}
	/*  Line is extern */
	else if (strcmp(line_type, EXTERN_OPERATION) == 0) {
		first_pass_process_extern(pass);
	}
	/* Line is entry */
	else if (strcmp(line_type, ENTRY_OPERATION) == 0) {
			first_pass_process_entry(pass);
	}
	/* Step 11 - Line is suspected as operation */
	else  {
		pass->current_line_information->current_index -= strlen(line_type);
		first_pass_process_operation(pass, label, is_symbol);
	}

	if (line_type != NULL) {
		free(line_type);
	}
}
