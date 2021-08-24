/*
 ------------------------------------------------------------------------------------
 The data encoder file responsible for converting all the .data and .string lines
 into numbers base 8
 ------------------------------------------------------------------------------------
 */


#include "DataEncoder.h"


/* DataEncoder head and tail */
data_node_ptr p_data_head = NULL;
data_node_ptr p_data_tail = NULL;


/*
------------------------------------------------------------------------------------
Function that gets a data_node_poninter adds it to the data list 
------------------------------------------------------------------------------------
 */
void add_data_node_to_list(data_node_ptr p_new_data) {
	/* In case of first Node create the list */
	if (p_data_head == NULL) {
		p_data_head = p_new_data;
		p_data_tail = p_new_data;
	} else {
		/* Inserts the data to the list */
		p_data_tail->next = p_new_data;
		p_data_tail = p_new_data;
	}
}

/*
------------------------------------------------------------------------------------
Function that adds string data to the list
gets a char and the address
------------------------------------------------------------------------------------
 */
bool add_string_data_to_list(char data, unsigned int address) {
	data_node_ptr p_data = (data_node_ptr)allocate_memory(sizeof(data_node));

	/* if we managed to create a new node, insert values and add to list */
	if (p_data != NULL) {
		p_data->current_data.encoded_data.bits.number = data;
		p_data->current_data.encoded_data.bits.rest = NO_ADDRESS;
		p_data->current_data.address = address;
		p_data->next = NULL;

		add_data_node_to_list(p_data);

		return true;
	} else {
		return false;
	}
}

/*
------------------------------------------------------------------------------------
Function that adds string data to the list
gets a number and the address 
------------------------------------------------------------------------------------
 */
bool add_numeric_data_to_list(int number, unsigned int address) {
	data_node_ptr p_data = (data_node_ptr)allocate_memory(sizeof(data_node));

	/* if we managed to create a new node, insert values and add to list */
	if (p_data != NULL) {
		p_data->current_data.encoded_data.bits.number = number;
		p_data->current_data.encoded_data.bits.rest = NO_ADDRESS;
		p_data->current_data.address = address;
		p_data->next = NULL;

		add_data_node_to_list(p_data);

		return true;
	} else {
		return false;
	}
}

/*
------------------------------------------------------------------------------------
Function that writes the the date to the output file 
gets the file where the data would be written to
------------------------------------------------------------------------------------
 */
void write_data_to_output_file(FILE* output_file) {
	data_node_ptr p_current_data = p_data_head;

	while (p_current_data != NULL) {
		data_definition data = p_current_data->current_data;
		print_encoding_to_file(data.address, data.encoded_data.value, output_file);
		p_current_data = p_current_data->next;
	}
}

/*
------------------------------------------------------------------------------------
Function that updates the current ic 
------------------------------------------------------------------------------------
 */
void update_data_address(int ic_length) {
	data_node_ptr p_current = p_data_head;

	while (p_current != NULL) {
		p_current->current_data.address += ic_length + ADDRESS_START;
		p_current = p_current->next;
	}
}

/*
------------------------------------------------------------------------------------
Function that procceses the line according to the type of the line
gets the current pass the label the data type and if its a symbol or not
------------------------------------------------------------------------------------
 */
void first_pass_process_data(pass_data* pass, char* label, char* data_type, bool is_symbol) {

	/* check if is a symbol */
	if (is_symbol) {
		/* Check if symbol exists in symbol table*/
		symbol_node_ptr p_searched_symbol = search_symbol(label);

		/* If symbol does not exist in symbol table, create it and add it */
		if (p_searched_symbol == NULL) {
			symbol_node_ptr p_symbol = create_symbol(label, pass->DC, false, true);

			if (p_symbol != NULL) {
				add_symbol_to_list(p_symbol);
			} else {
				pass->is_runtimer_error = true;
				free(label);
				return;
			}

		} else {
			print_compiler_error("Each label can be defined only once", pass->current_line_information);
			pass->is_compiler_error = true;
			return;
		}
	}

	skip_all_spaces(pass->current_line_information);

	/*
	 get the data according to label type
	 */
	if (is_end_of_data_in_line(pass->current_line_information)) {
		print_compiler_error("Missing data after .string or .data", pass->current_line_information);
		pass->is_compiler_error = true;
	}
	/* This is a string initialization */
	else if (strcmp(data_type, STRING_OPERATION) == 0) {
		process_string(pass);
	}
	/* This is a numeric data */
	else {
		process_numbers(pass);
	}
}


/*
------------------------------------------------------------------------------------
Function that procceses a string according to all the rules
gets the current pass
------------------------------------------------------------------------------------
 */
void process_string(pass_data* pass) {
	/* Check if the string starts with quotation mark else print error */
	if (pass->current_line_information->line_str[pass->current_line_information->current_index] != QUOTATION) {
		print_compiler_error("A string must start with a '\"' token", pass->current_line_information);
		pass->is_compiler_error = true;
	} else {
		bool success;

		/* Skip quotation mark */
		pass->current_line_information->current_index++;

		/* Keep scanning till end of line */
		while (!is_end_of_line(pass->current_line_information)) {
			char token = pass->current_line_information->line_str[pass->current_line_information->current_index];

			if (token == END_OF_LINE) {
				print_compiler_error("A string must end with a '\"' token", pass->current_line_information);
				pass->is_compiler_error = true;
				break;
			} else if (token == QUOTATION) {
				pass->current_line_information->current_index++;
				break;
			} else if (token != QUOTATION) {
				success = add_string_data_to_list(token, pass->DC);

				if (!success) {
					pass->is_runtimer_error = true;
					return;
				} else {
					pass->DC++;
				}
			}

			pass->current_line_information->current_index++;
		}

		/* Add string to list or exit */
		success = add_string_data_to_list(STRING_DATA_END, pass->DC);
		if (!success) {
			pass->is_runtimer_error = true;
			return;
		}

		pass->DC++;

		if (!is_end_of_data_in_line(pass->current_line_information)) {
			print_compiler_error("Invalid tokens after .string instruction", pass->current_line_information);
			pass->is_compiler_error = true;
		}
	}
}

/*
------------------------------------------------------------------------------------
Function that procceses numbers
gets the current pass 
------------------------------------------------------------------------------------
 */
void process_numbers(pass_data* pass) {
	bool should_process_next_number = true;
	bool success;

	skip_all_spaces(pass->current_line_information);

	/* Make sure .data contains numbers */
	if (is_end_of_data_in_line(pass->current_line_information)) {
		print_compiler_error("Invalid .data definition. Missing numbers.", pass->current_line_information);
		pass->is_compiler_error = true;
	}

	/* Processes all numbers in line */
	while (!is_end_of_line(pass->current_line_information) && should_process_next_number) {
		char* partial_line = NULL;
		int number;

		if (get_next_number(pass, &number)) {
			success = add_numeric_data_to_list(number, pass->DC++);

			if (!success) {
				pass->is_runtimer_error = true;
				return;
			}

			/* Search the next ',' */
			partial_line =
					strchr(pass->current_line_information->line_str +
							pass->current_line_information->current_index, NUMBER_TOKEN_SEPERATOR);

			/* Found ',' and scan another number */
			if (partial_line != NULL) {
				pass->current_line_information->current_index = partial_line - pass->current_line_information->line_str + 1;
				should_process_next_number = true;
			} else {
				skip_all_spaces(pass->current_line_information);
				should_process_next_number = false;
			}
		} else {
			should_process_next_number = false;
		}
	}

	/* If it's not end of line or we need to get another number, throw an error */
	if (!is_end_of_line(pass->current_line_information) || should_process_next_number) {
		print_compiler_error(".data syntax is invalid", pass->current_line_information);
		pass->is_compiler_error = true;
	}
}

/*
------------------------------------------------------------------------------------
Function that frees the memory allocated the list
------------------------------------------------------------------------------------
 */
void free_data_node_list() {
	data_node_ptr p_cleaner_data = p_data_head;

	/* Clean nodes until no more nodes */
	while (p_data_head) {
		p_cleaner_data = p_data_head;
		p_data_head = p_data_head->next;
		free (p_cleaner_data);
	}
	return;
}

/*
------------------------------------------------------------------------------------
Function that gets tge next number
gets the pass and a pointer 
------------------------------------------------------------------------------------
 */
bool get_next_number(pass_data* pass, int* number) {
	skip_all_spaces(pass->current_line_information);

	/* Reaches end of line while expecting a number */
	if (is_end_of_line(pass->current_line_information)) {
		print_compiler_error("Expected number definition", pass->current_line_information);
		pass->is_compiler_error = true;
	} else {
		int number_start_index = pass->current_line_information->current_index;
		int number_end_index = pass->current_line_information->current_index;

		/* First token of the number is a minus or a plus */
		if ((pass->current_line_information->line_str[number_start_index] == MINUS_SIGN) ||
				pass->current_line_information->line_str[number_start_index] == PLUS_SIGN) {
			number_end_index++;
		}

		/* Makes sure the next token is a digit */
		if (!is_end_of_line(pass->current_line_information) &&
				!isdigit(pass->current_line_information->line_str[number_end_index])) {
			print_compiler_error("A number must have digits in it", pass->current_line_information);
			pass->is_compiler_error = true;
		} else if (!is_end_of_line(pass->current_line_information)){
			char* number_string = NULL;

			/* Finds all digits */
			while (isdigit(pass->current_line_information->line_str[number_end_index])) {
				number_end_index++;
			}

			/* Last token wasn't part of the number */
			number_end_index--;

			/* Copying the number from the line into a new memory space, or die */
			number_string = allocate_string(number_end_index - number_start_index + 1);
			if (!number_string) {
				pass->is_runtimer_error = true;
				return false;
			}

			strncpy(number_string, pass->current_line_information->line_str + number_start_index, number_end_index - number_start_index + 1);
			number_string[number_end_index - number_start_index + 1] = END_OF_STRING;
			*number = atoi(number_string);

			free(number_string);

			pass->current_line_information->current_index = number_end_index + 1;

			return true;
		}
	}
	return false;
}
