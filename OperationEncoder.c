/*
--------------------------------------------------------------------------------
 The OperationsEncoder file holds all the functions for encoding operatons 
---------------------------------------------------------------------------------
 */

#include "OperationEncoder.h"


/* Global variable hols the current operation */
operation_information_node_ptr p_operation_head = NULL;

/*
--------------------------------------------------------------------------------
 Function responsible for encoding the operaton in the second pass 
 gets the current line and the files where the operation will be ecoded
---------------------------------------------------------------------------------
 */
void second_pass_process_operation(pass_data *pass, compiler_output_files *p_output_files)
{
	/* Gets all data about the current operation */
	decoded_operation *p_decoded_operation = get_decoded_operation(pass);

	/* Encode the operation */
	encode_operation(pass, p_decoded_operation, p_output_files);
}

/*
--------------------------------------------------------------------------------
 Function responsible for processing the info during the first pass 
 gets the current line the label if it exists and a bool of is it a symbol
---------------------------------------------------------------------------------
 */
void first_pass_process_operation(pass_data *pass, char *label, bool is_symbol)
{
	decoded_operation *p_decoded_operation = NULL;

	/* Checks if the line is valid for an operation */
	if (!is_valid_is_operation_line(pass->current_line_information))
	{
		print_compiler_error("Line exceeds max length", pass->current_line_information);
		pass->is_compiler_error = true;
		return;
	}

	/* Check if there is a symbol in the beginning of line */
	if (is_symbol)
	{
		/* Checks if the symbol was defined in previous lines */
		symbol_node_ptr p_searched_symbol = search_symbol(label);

		if (p_searched_symbol == NULL)
		{

			/* Creates a new symbol and adds it to the symbol table, or exit */
			symbol_node_ptr p_symbol = create_symbol(label, pass->IC, false, false);
			if (p_symbol != NULL)
			{
				add_symbol_to_list(p_symbol);
			}
			else
			{
				pass->is_runtimer_error = true;
				free(label);
				return;
			}
			/* The label was alredy defined */
		}
		else
		{
			print_compiler_error("Each label can be defined only once", pass->current_line_information);
			pass->is_compiler_error = true;
			free(label);
			return;
		}
	}

	/* Gets all data about the current operation */
	p_decoded_operation = get_decoded_operation(pass);

	if (p_decoded_operation != NULL)
	{
		/* Checks if the operands used in the operation are valid */
		if (are_operand_methods_allowed_in_operation(p_decoded_operation))
		{

			/* Calculate the operation's size */
			pass->IC += calculate_operation_size(pass, p_decoded_operation);
		}
		else
		{
			print_compiler_error("Unauthorized address methods", pass->current_line_information);
			pass->is_compiler_error = true;
		}
	}
}

/*
--------------------------------------------------------------------------------
 Function responsible for calculating the amount of memory ("words") each 
 operation needs
 gets the current line and the current opern
---------------------------------------------------------------------------------
 */
int calculate_operation_size(pass_data *pass, decoded_operation *current_operation)
{
	/* We need to encode the operation */
	int size = OPERAION_MIN_WORD_SIZE;

	/* If both operands are registers/indirect they share the same memory word */
	if (((current_operation->source_operand_address_method == DIRECT_REGISTER) || (current_operation->source_operand_address_method == INDIRECT_REGISTER)) &&
		((current_operation->target_operand_address_method == DIRECT_REGISTER) || (current_operation->target_operand_address_method == INDIRECT_REGISTER)))
	{
		size++;
	}
	else
	{
		/* Each operand needs its own memory word */
		size += current_operation->operation->operands_number;
	}

	return size;
}

/*
--------------------------------------------------------------------------------
 Function responsible for retunring all the info about a the current line 
 gets the currennt line
---------------------------------------------------------------------------------
 */
decoded_operation *get_decoded_operation(pass_data *pass)
{
	decoded_operation *current_operation = NULL;
	machine_operation_definition *p_operation_information = NULL;

	/* Get operation name from line or exit */
	char *operation_name = get_operation_name(pass);
	if (pass->is_runtimer_error)
	{
		return NULL;
	}

	/* Find operation in machine operation list */
	p_operation_information = search_machine_operation_in_list(operation_name);

	/* The operation name isn't valid */
	if (p_operation_information == NULL)
	{
		print_compiler_error("Invalid operation name", pass->current_line_information);
		pass->is_compiler_error = true;
		return NULL;
	}
	else
	{

		char *source_operand = NULL;
		char *target_operand = NULL;

		/* Get the operands to encode in the operation */
		if (get_operands(pass, p_operation_information->operands_number, &source_operand, &target_operand))
		{
			/* Gets the address methods to use in the encoding */
			ADDRESS_METHOD source_operand_address_method = get_address_method(pass, source_operand);
			ADDRESS_METHOD target_operand_address_method = get_address_method(pass, target_operand);

			/* No error occurred while trying to get address method */
			if ((source_operand_address_method != INVALID_ADDRESS_METHOD) && (target_operand_address_method != INVALID_ADDRESS_METHOD))
			{

				current_operation = allocate_memory(sizeof(decoded_operation));

				if (current_operation == NULL)
				{
					pass->is_runtimer_error = true;
				}
				else
				{
					/* Initialize the operation according to data extracted from line */
					current_operation->operation = p_operation_information;
					current_operation->source_operand = source_operand;
					current_operation->source_operand_address_method = source_operand_address_method;
					current_operation->target_operand = target_operand;
					current_operation->target_operand_address_method = target_operand_address_method;
				}
			}
		}
	}

	return current_operation;
}

/*
--------------------------------------------------------------------------------
 Function responsible for getting the data about the operands n the curretn line
 gets the current line and the operand count the souurce and target operands
---------------------------------------------------------------------------------
 */
bool get_operands(pass_data *pass, int operands_count, char **source_operand, char **target_operand)
{
	bool is_valid = true;

	switch (operands_count)
	{
	case NO_OPERANDS:
	{
		/* There aren't any operands */
		*source_operand = NULL;
		*target_operand = NULL;
		break;
	}
	case ONE_OPERAND:
	{
		/* Reads the target operand from the line */
		*source_operand = NULL;
		*target_operand = get_next_operand(pass);
		break;
	}
	case TWO_OPERANDS:
	{
		/* Reads the source operand from the line */
		*source_operand = get_next_operand(pass);

		if (pass->is_runtimer_error)
		{
			is_valid = false;
			break;
		}

		skip_all_spaces(pass->current_line_information);

		/* There must be a comma between operands */
		if (pass->current_line_information->line_str[pass->current_line_information->current_index] != OPERAND_SEPERATOR)
		{
			print_compiler_error("Missing comma between two operands", pass->current_line_information);
			pass->is_compiler_error = true;
			is_valid = false;
		}
		else
		{
			/* Reads the target operand from the line */
			pass->current_line_information->current_index++;
			*target_operand = get_next_operand(pass);
		}

		break;
	}
	}
	/* an error has accured */
	if (pass->is_runtimer_error)
	{
		is_valid = false;
	}
	/* Searches for tokens after operation */
	else if (is_valid && !is_end_of_data_in_line(pass->current_line_information))
	{
		print_compiler_error("Found unexpected token on end of operation definition", pass->current_line_information);
		pass->is_compiler_error = true;
		is_valid = false;
	}

	/* The operands aren't valid. Releases memory allocated */
	if (!is_valid)
	{
		if (*source_operand != NULL)
		{
			free(*source_operand);
		}

		if (*target_operand != NULL)
		{
			free(*target_operand);
		}
	}

	return is_valid;
}

/*
--------------------------------------------------------------------------------
 Function responsible for returning the operation name 
 gets the current line returns the the opeartion name in astring of chars
---------------------------------------------------------------------------------
 */
char *get_operation_name(pass_data *pass)
{
	/* The resuls that would be returned */
	char *result_operation_name;
	char *operation_name;
	/* Operation name length */
	int operation_name_length = 0;

	skip_all_spaces(pass->current_line_information);

	operation_name = pass->current_line_information->line_str + pass->current_line_information->current_index;

	/* Searches for the position of the operation inside the line */
	while (isalpha(pass->current_line_information->line_str[pass->current_line_information->current_index]))
	{
		(pass->current_line_information->current_index)++;
		operation_name_length++;
	}

	result_operation_name = allocate_string(operation_name_length);

	if (result_operation_name != NULL)
	{
		/* Copies the operation's name */
		strncpy(result_operation_name, operation_name, operation_name_length);
		result_operation_name[operation_name_length] = END_OF_STRING;
	}
	else
	{
		pass->is_runtimer_error = true;
	}

	return result_operation_name;
}

/*
--------------------------------------------------------------------------------
 Function responsible for returning the address method
 gets the current line and the operand
 returns the the the addres method
---------------------------------------------------------------------------------
 */
ADDRESS_METHOD get_address_method(pass_data *pass, char *operand)
{
	/* The operation has no source or target operand. Use IMMEDIATE method for encoding */
	if (operand == NULL)
	{
		return NO_ADDRESS;
	}
	else
	{
		int operand_length = strlen(operand);

		/* check if valid operand lenth */
		if (operand_length > 0)
		{
			/* Operand starts with a # */
			if (operand[0] == IMMEDIATE_TOKEN)
			{
				int i = 1;

				/* Operand has + or - on its' start */
				if ((operand[1] == MINUS_SIGN) || (operand[1] == PLUS_SIGN))
				{
					i++;

					if (operand_length == 1)
					{
						print_compiler_error("Immediate token # must be followed by a valid number", pass->current_line_information);
						pass->is_compiler_error = true;
						return INVALID_ADDRESS_METHOD;
					}
				}

				/* Check all operand's tokens are valid digits */
				for (; i < operand_length; i++)
				{
					if (!isdigit(operand[i]))
					{
						print_compiler_error("Immediate token # must be followed by a valid number", pass->current_line_information);
						pass->is_compiler_error = true;
						return INVALID_ADDRESS_METHOD;
					}
				}

				return IMMEDIATE;
			}
			/* The operand is equal to * foollowd by r0 -r7 */
			else if (is_in_register(operand))
			{
				return INDIRECT_REGISTER;
			}
			/* The operand is r0-r7 */
			else if (is_register(operand))
			{
				return DIRECT_REGISTER;
			}
			/* The operand is a variable */
			else if (is_valid_label(operand))
			{
				return DIRECT;
			}
			else
			{
				print_compiler_error("Operand isn't a valid label, register, number ", pass->current_line_information);
				pass->is_compiler_error = true;
				return INVALID_ADDRESS_METHOD;
			}
		}
		else
		{
			print_compiler_error("Operand cannot be an empty string", pass->current_line_information);
			pass->is_compiler_error = true;
			return INVALID_ADDRESS_METHOD;
		}
	}
}

/*
--------------------------------------------------------------------------------
 Function that returns a boolean if the method of adressing is right for the 
 current operation
---------------------------------------------------------------------------------
 */
bool are_operand_methods_allowed_in_operation(decoded_operation *current_operation)
{
	/* This are invalid methods */
	if ((current_operation->source_operand_address_method == INVALID_ADDRESS_METHOD) ||
		(current_operation->target_operand_address_method == INVALID_ADDRESS_METHOD))
		return false;

	/*
	 * mov, add, and sub operation
	 * First operand: all methods are valid
	 * Second operand:  direct , indirect direct,direct register are valid
	 */
	if ((strcmp(current_operation->operation->name, MOV_OPERATION) == 0) ||
		(strcmp(current_operation->operation->name, ADD_OPERATION) == 0) ||
		(strcmp(current_operation->operation->name, SUB_OPERATION) == 0))
	{
		return ((current_operation->target_operand_address_method == DIRECT) ||
				(current_operation->target_operand_address_method == DIRECT_REGISTER) ||
				(current_operation->target_operand_address_method == INDIRECT_REGISTER));
	}
	/*
	 * not, clr, inc, dec operation
	 * Only one operand.
	 * First operand:  direct ,indirect register, direct register are valid
	 */
	else if ((strcmp(current_operation->operation->name, NOT_OPERATION) == 0) ||
			 (strcmp(current_operation->operation->name, CLR_OPERATION) == 0) ||
			 (strcmp(current_operation->operation->name, INC_OPERATION) == 0) ||
			 (strcmp(current_operation->operation->name, DEC_OPERATION) == 0))
	{
		return ((current_operation->target_operand_address_method == DIRECT) ||
				(current_operation->target_operand_address_method == DIRECT_REGISTER) ||
				(current_operation->target_operand_address_method == INDIRECT_REGISTER));
	}
	/*
	 * lea operation
	 * First operand: only direct is valid
	 * Second operand: only direct, indirect register, direct register are valid
	 */
	else if (strcmp(current_operation->operation->name, LEA_OPERATION) == 0)
	{
		return ((current_operation->source_operand_address_method == DIRECT) &&
				((current_operation->target_operand_address_method == DIRECT) ||
				 (current_operation->target_operand_address_method == DIRECT_REGISTER) ||
				 (current_operation->target_operand_address_method == INDIRECT_REGISTER)));
	}
	/*
	 * jmp, bne, jsr operation
	 * Only one operand
	 * First operand: direct or indirect register
	 */
	else if ((strcmp(current_operation->operation->name, JMP_OPERATION) == 0) ||
			 (strcmp(current_operation->operation->name, BNE_OPERATION) == 0) ||
			 (strcmp(current_operation->operation->name, JSR_OPERATION) == 0))
	{
		return ((current_operation->target_operand_address_method == DIRECT) ||
				(current_operation->target_operand_address_method == INDIRECT_REGISTER));
	}
	/*
	 * red operation
	 * Only one operand
	 * First operand: direct , indirect register , direct register
	 */
	else if (strcmp(current_operation->operation->name, RED_OPERATION) == 0)
	{
		return ((current_operation->target_operand_address_method == DIRECT) ||
				(current_operation->target_operand_address_method == INDIRECT_REGISTER) ||
				(current_operation->target_operand_address_method == DIRECT));
	}
	/*
	 * cmp, prn, rts, stop operation
	 * The method is irrelevant or no operands given
	 */
	else
	{
		return true;
	}
}

/*
--------------------------------------------------------------------------------
 Function responsible for writing the operation machine code to the ouput piles
 gets the current line , the curretn oper , the output files
 returns true if sucseful 
---------------------------------------------------------------------------------
 */
bool encode_operation(pass_data *pass, decoded_operation *p_decoded_operation, compiler_output_files *output_files)
{
	encoded_operation coded_op;

	/* Initlaizes the encoded operation with its' values */
	coded_op.bits.source_operand_address_method =
		p_decoded_operation->source_operand_address_method;
	coded_op.bits.target_operand_address_method =
		p_decoded_operation->target_operand_address_method;
	coded_op.bits.op_code = p_decoded_operation->operation->code;
	coded_op.bits.are = ABSOLUTE;
	coded_op.bits.rest = 0;

	/* Print the information */

	print_encoding_to_file(pass->IC + ADDRESS_START, coded_op.value, output_files->ob_file);

	pass->IC++;

	if (p_decoded_operation->operation->operands_number > 0)
	{
		bool is_valid = encode_operands(pass, p_decoded_operation, output_files);

		if (!is_valid)
		{
			return is_valid;
		}
	}

	return true;
}

/*
--------------------------------------------------------------------------------
 Function responsible for writing the operands inmachine code to the ouput piles
 gets the current line , the curret oper , the output files
 returns true if sucseful 
---------------------------------------------------------------------------------
 */
bool encode_operands(pass_data *pass, decoded_operation *p_decoded_operation, compiler_output_files *output_files)
{
	bool is_valid;

	/* Both source and target operands are registers or indirect registsers */
	if (((p_decoded_operation->source_operand_address_method == DIRECT_REGISTER) || (p_decoded_operation->source_operand_address_method == INDIRECT_REGISTER)) &&
		((p_decoded_operation->target_operand_address_method == DIRECT_REGISTER) || (p_decoded_operation->target_operand_address_method == INDIRECT_REGISTER)))
	{
		is_valid = encode_registers(
			pass, p_decoded_operation->source_operand,
			p_decoded_operation->target_operand, output_files->ob_file);
		pass->IC++;
	}
	else
	{
		/* The operation has two operands */
		if (p_decoded_operation->operation->operands_number == 2)
		{
			/* Encode the source operand */
			if (p_decoded_operation->source_operand_address_method == DIRECT)
			{
				is_valid = encode_direct(pass, p_decoded_operation->source_operand, output_files);
			}
			else if (p_decoded_operation->source_operand_address_method == DIRECT_REGISTER)
			{
				is_valid = encode_registers(pass, p_decoded_operation->source_operand, NULL, output_files->ob_file);
			}
			else if (p_decoded_operation->source_operand_address_method == INDIRECT_REGISTER)
			{
				is_valid = encode_in_registers(pass, p_decoded_operation->source_operand, NULL, output_files->ob_file);
			}
			else if (p_decoded_operation->source_operand_address_method == IMMEDIATE)
			{
				is_valid = encode_immediate(pass, p_decoded_operation->source_operand, output_files->ob_file);
			}

			pass->IC++;
		}

		/* Encode the target operand */
		if (p_decoded_operation->target_operand_address_method == DIRECT)
		{
			is_valid = encode_direct(pass, p_decoded_operation->target_operand, output_files);
		}
		else if (p_decoded_operation->target_operand_address_method == DIRECT_REGISTER)
		{
			is_valid = encode_registers(pass, NULL, p_decoded_operation->target_operand, output_files->ob_file);
		}
		else if (p_decoded_operation->target_operand_address_method == INDIRECT_REGISTER)
		{
			is_valid = encode_in_registers(pass, NULL, p_decoded_operation->target_operand, output_files->ob_file);
		}
		else if (p_decoded_operation->target_operand_address_method == IMMEDIATE)
		{
			is_valid = encode_immediate(pass, p_decoded_operation->target_operand, output_files->ob_file);
		}

		pass->IC++;
	}

	return is_valid;
}

/*
--------------------------------------------------------------------------------
 Function responsible for writing the direct adress machine code to the ouput piles
 gets the current line , the curretn oper , the output files
 returns true if sucseful 
---------------------------------------------------------------------------------
 */
bool encode_direct(pass_data *pass, char *operand, compiler_output_files *output_files)
{
	/* This a variable. Therefore it must be a symbol */
	symbol_node_ptr p_symbol = search_symbol(operand);

	if (p_symbol == NULL)
	{
		print_compiler_error("Using unknown symbol", pass->current_line_information);
		pass->is_compiler_error = true;
		return false;
	}
	else
	{
		operand_memory_word word;

		word.non_register_address.operand_address = p_symbol->current_symbol.address;

		/* The symbol is external. Encode the location it is used into the ext file */
		if (p_symbol->current_symbol.is_external)
		{
			word.non_register_address.are = EXTERNAL;

			create_extern_output_file_if_needed(output_files, pass->current_line_information->file_name);

			if (output_files->extern_file == NULL)
			{
				pass->is_runtimer_error = true;
				return false;
			}

			write_extern_to_output_file(operand, pass->IC + ADDRESS_START, output_files->extern_file);
		}
		else
		{
			word.non_register_address.are = RELOCATABLE;
		}

		word.non_register_address.rest = 0;

		print_encoding_to_file(pass->IC + ADDRESS_START, word.value, output_files->ob_file);

		return true;
	}
}

/*
--------------------------------------------------------------------------------
 Function responsible for writing registers in the machine code to the ouput files
 also handles indirect registers
 gets the current line , the curretn oper , the output files
 returns true if sucseful 
---------------------------------------------------------------------------------
 */
bool encode_registers(pass_data *pass, char *source_register, char *target_register, FILE *p_file)
{
	operand_memory_word word;

	/* The target register exists */
	if (source_register != NULL)
	{
		/* Encode the register's number */
		if (*source_register == '*')
		{
			word.register_address.source_register_address = atoi(source_register + 2);
		}
		else
		{
			word.register_address.source_register_address = atoi(source_register + 1);
		}
	}
	else
	{
		word.register_address.source_register_address = NO_ADDRESS;
	}

	/* The target register exists */
	if (target_register != NULL)
	{
		if (*target_register == '*')
		{
			word.register_address.target_register_address = atoi(target_register + 2);
		}
		else
		{
			word.register_address.target_register_address = atoi(target_register + 1);
		}
	}
	else
	{
		word.register_address.target_register_address = NO_ADDRESS;
	}

	word.register_address.are = ABSOLUTE;
	word.register_address.rest = 0;

	print_encoding_to_file(pass->IC + ADDRESS_START, word.value, p_file);

	return true;
}

/*
--------------------------------------------------------------------------------
 Function responsible for writing indirect registers in the machine code to the ouput piles
 gets the current line , the curretn oper , the output files
 returns true if sucseful 
---------------------------------------------------------------------------------
 */
bool encode_in_registers(pass_data *pass, char *source_register, char *target_register, FILE *p_file)
{
	operand_memory_word word;

	/* The target register exists */
	if (source_register != NULL)
	{
		/* Encode the register's number */
		word.register_address.source_register_address = atoi(source_register + 2);
	}
	else
	{
		word.register_address.source_register_address = NO_ADDRESS;
	}

	/* The target register exists */
	if (target_register != NULL)
	{
		/* Encode the register's number */
		word.register_address.target_register_address = atoi(target_register + 2);
	}
	else
	{
		word.register_address.target_register_address = NO_ADDRESS;
	}

	word.register_address.are = ABSOLUTE;
	word.register_address.rest = 0;

	print_encoding_to_file(pass->IC + ADDRESS_START, word.value, p_file);

	return true;
}

/*
--------------------------------------------------------------------------------
 Function responsible for writing the imidiate in machine code to the ouput piles
 gets the current line , the curretn oper , the output files
 returns true if sucseful 
---------------------------------------------------------------------------------
 */
bool encode_immediate(pass_data *pass, char *operand, FILE *p_file)
{
	int number;
	operand_memory_word word;

	/* Convert the number into a integer */
	number = atoi(operand + 1);

	word.non_register_address.operand_address = number;
	word.non_register_address.are = ABSOLUTE;
	word.non_register_address.rest = 0;

	print_encoding_to_file(pass->IC + ADDRESS_START, word.value, p_file);

	return true;
}

/*
--------------------------------------------------------------------------------
 Function responsible for writing finding the opeation in the list of operation
 get operation name
 return the pointer to the operation if succesful and null otherwise
---------------------------------------------------------------------------------
 */
machine_operation_definition *search_machine_operation_in_list(char *operation)
{
	operation_information_node_ptr p_current = p_operation_head;

	/* Scan the operation list and search for the operation */
	while (p_current != NULL)
	{
		if (strcmp(p_current->data.name, operation) == 0)
		{
			return &(p_current->data);
		}

		p_current = p_current->next;
	}

	return NULL;
}

/*
--------------------------------------------------------------------------------
 Function responsible for initializiing the operation list 
 return true if succesfull
---------------------------------------------------------------------------------
 */
bool init_operation_list()
{
	bool initialized = true;

	int op_code = 0;

	/* Initializes the operations */
	initialized &= add_operation_to_list(MOV_OPERATION, op_code++, TWO_OPERANDS);
	initialized &= add_operation_to_list(CMP_OPERATION, op_code++, TWO_OPERANDS);
	initialized &= add_operation_to_list(ADD_OPERATION, op_code++, TWO_OPERANDS);
	initialized &= add_operation_to_list(SUB_OPERATION, op_code++, TWO_OPERANDS);
	initialized &= add_operation_to_list(LEA_OPERATION, op_code++, TWO_OPERANDS);
	initialized &= add_operation_to_list(CLR_OPERATION, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(NOT_OPERATION, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(INC_OPERATION, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(DEC_OPERATION, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(JMP_OPERATION, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(BNE_OPERATION, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(RED_OPERATION, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(PRN_OPERATION, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(JSR_OPERATION, op_code++, ONE_OPERAND);
	initialized &= add_operation_to_list(RTS_OPERATION, op_code++, NO_OPERANDS);
	initialized &= add_operation_to_list(STOP_OPERATION, op_code++, NO_OPERANDS);

	return initialized;
}

/*
--------------------------------------------------------------------------------
 Function responsible for adding operation to the list 
 gets the name of oper the code and the operands
 return true if succesfull
---------------------------------------------------------------------------------
 */
bool add_operation_to_list(char *name, unsigned int code, int operands)
{
	bool added = false;

	operation_information_node_ptr p_new =
		(operation_information_node_ptr)allocate_memory(sizeof(operation_information_node));
	/* check allocaton of mem was duccesful */
	if (p_new != NULL)
	{
		p_new->data.name = name;
		p_new->data.code = code;
		p_new->data.operands_number = operands;
		p_new->next = NULL;

		if (p_operation_head == NULL)
		{
			p_operation_head = p_new;
		}
		else
		{
			p_new->next = p_operation_head;
			p_operation_head = p_new;
		}

		added = true;
	}

	return added;
}

/*
----------------------------------------------------------
 Free memory for operations list
----------------------------------------------------------
*/
void free_operation_list()
{
	operation_information_node_ptr p_cleaner_data = p_operation_head;

	while (p_operation_head)
	{
		p_cleaner_data = p_operation_head;
		p_operation_head = p_operation_head->next;
		free(p_cleaner_data);
	}
	return;
}

/*
--------------------------------------------------------------------------------
 Function responsible for getting the next operands
 gets the current line 
 return the operand
---------------------------------------------------------------------------------
 */
char *get_next_operand(pass_data *pass)
{
	char *operand = NULL;
	int i, operand_end_index, operand_start_index, operand_length;

	skip_all_spaces(pass->current_line_information);

	operand_end_index = operand_start_index = i = pass->current_line_information->current_index;

	/* Find operand position */
	for (; i < pass->current_line_information->line_length; i++)
	{
		if (!isspace(pass->current_line_information->line_str[i]) &&
			(pass->current_line_information->line_str[i] != OPERAND_SEPERATOR))
		{
			operand_end_index = i;
		}
		else
		{
			break;
		}
	}

	operand_length = operand_end_index - operand_start_index + 1;

	operand = allocate_string(operand_length);

	if (operand == NULL)
	{
		pass->is_runtimer_error = true;
	}
	else
	{
		/* Copy operand */
		strncpy(operand, pass->current_line_information->line_str + operand_start_index, operand_length);
		operand[operand_length] = '\0';

		pass->current_line_information->current_index = operand_end_index + 1;
	}

	return operand;
}
