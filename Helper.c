
/*
--------------------------------------------------------------------------------
 The Helper file contains all the function that help procese the assembly code
---------------------------------------------------------------------------------
 */

#include "Helper.h"

/*
--------------------------------------------------------------------------------
 Function resposible for printing compile errors 
 gets the message and the line info
---------------------------------------------------------------------------------
 */
void print_compiler_error(char *message, line_info *info)
{
	fprintf(stderr, "Error: %s, File %s Line %d \n", message, info->file_name, info->line_number);
}

/*
--------------------------------------------------------------------------------
 Function resposible for printing run time error 
 gets the massage that need to be printed 
---------------------------------------------------------------------------------
 */
void print_runtime_error(char *message)
{
	fprintf(stderr, "Error: %s \n", message);
}
/*
--------------------------------------------------------------------------------
 Function resposible for skiping all the spaces
 gets the line info
---------------------------------------------------------------------------------
 */
void skip_all_spaces(line_info *info)
{
	int index = info->current_index;

	/* Scan all spaces */
	while ((index < info->line_length) && isspace(info->line_str[index]))
	{
		index++;
	}

	info->current_index = index;
}

/*
--------------------------------------------------------------------------------
 Function resposible for going to the next word
 gets the current line
---------------------------------------------------------------------------------
 */
char *get_next_word(pass_data *pass)
{
	char *word;
	int i, word_end_index, word_start_index, word_length;

	skip_all_spaces(pass->current_line_information);

	word_end_index = word_start_index = i = pass->current_line_information->current_index;

	for (; i < pass->current_line_information->line_length; i++)
	{
		if (!isspace(pass->current_line_information->line_str[i]))
		{
			word_end_index = i;
		}
		else
		{
			break;
		}
	}

	word_length = word_end_index - word_start_index + 1;

	word = allocate_string(word_length);

	if (word == NULL)
	{
		pass->is_runtimer_error = true;
	}
	else
	{
		/* Copy word */
		strncpy(word, pass->current_line_information->line_str + word_start_index, word_length);
		(word)[word_length] = END_OF_STRING;

		pass->current_line_information->current_index = word_end_index + 1;
	}

	return word;
}

/*
--------------------------------------------------------------------------------
 Function resposible creatring line info 
 gets file name line number and the line
---------------------------------------------------------------------------------
 */
line_info *create_line_info(char *file_name, int line_number, char *line_str)
{
	line_info *info = (line_info *)allocate_memory(sizeof(line_info));

	/* if memory allocation succesful create new line info */
	if (info != NULL)
	{
		info->current_index = 0;
		info->file_name = file_name;
		info->line_number = line_number;
		info->line_str = line_str;
		info->line_length = strlen(line_str);
	}

	return info;
}

/*
--------------------------------------------------------------------------------
 boolean function check if the line is empty or comment
 gets the line string
---------------------------------------------------------------------------------
 */
bool is_empty_or_comment(char *line)
{
	int i;
	int length = strlen(line);

	/* Check's there is a charecter that isn't a space */
	for (i = 0; i < length; i++)
	{
		if (!isspace(line[i]))
		{
			return line[i] == COMMENT;
		}
	}

	return true;
}

/*
--------------------------------------------------------------------------------
 Function resposible for converting numbers in base 10 to targeted base (8)
gets the number in base 10 the target base and the lenth of the word tha we want
---------------------------------------------------------------------------------
 */
char *convert_base10_to_target_base(unsigned int base10_number, int target_base, int minimal_returned_length)
{
	char *result = NULL;
	char *current_token = NULL;
	int result_length;
	int remainder = 0;

	result = allocate_string(1);

	if (result == NULL)
	{
		print_runtime_error("Could not allocate memory. Exit program");
	}
	else
	{
		result[0] = END_OF_STRING;
		result_length = 1;
	}

	/* Convert number to target base by :
	  	Dividing the whole number then
	 	Take the reminder and append it to the result string
	 * */
	while (base10_number != 0)
	{
		int temp;

		temp = base10_number / target_base;
		remainder = base10_number - temp * target_base;
		base10_number = temp;

		current_token = allocate_string(result_length);

		if (current_token != NULL)
		{
			current_token[0] = '0' + remainder;
			current_token[1] = END_OF_STRING;

			strcat(current_token, result);

			free(result);
			result = current_token;
		}
		else
		{
			if (result != NULL)
			{
				free(result);
			}

			return NULL;
		}
	}

	/* Making sure that the returned result is with the right length and if not, add 0 to the right */
	result_length = strlen(result);

	/* Add zeros to the left if necessary */
	while (result_length < minimal_returned_length)
	{
		current_token = allocate_string(result_length);
		current_token[0] = '0';
		current_token[1] = END_OF_STRING;

		strcat(current_token, result);

		free(result);
		result = current_token;
		result_length++;
	}

	return result;
}

/*
--------------------------------------------------------------------------------
 Function resposible allocating memory for string
 gets string length
---------------------------------------------------------------------------------
 */
char *allocate_string(int string_length)
{
	return (char *)allocate_memory(sizeof(char) * (string_length + 1));
}

/*
--------------------------------------------------------------------------------
 Function resposible allocating memory based on number of bytes
 gets number of bytes
---------------------------------------------------------------------------------
 */
void *allocate_memory(int bytes)
{
	void *result = (void *)malloc(bytes);

	if (result == NULL)
	{
		print_runtime_error("Could not allocate memory. Exit program");
	}

	return result;
}

/*
--------------------------------------------------------------------------------
 Function resposible valdidating there is no more info in line
 gets line_info
---------------------------------------------------------------------------------
 */
bool is_end_of_data_in_line(line_info *info)
{
	bool is_end = true;
	int i;

	/* Searches non-space token */
	for (i = info->current_index; i < info->line_length; i++)
	{
		if (!isspace(info->line_str[i]))
		{
			is_end = false;
			break;
		}
	}

	return is_end;
}

/*
--------------------------------------------------------------------------------
 Function resposible skipping the label
 gets line_info
---------------------------------------------------------------------------------
 */
void skip_label(line_info *info)
{
	/* Search the label end token */
	char *partial_line = strchr(info->line_str, LABEL_END_TOKEN);

	if (partial_line != NULL)
	{
		info->current_index = partial_line - info->line_str + 1;
	}
}

/*
--------------------------------------------------------------------------------
 Function resposible for validating if the current label is valid of not
 gets the  label
---------------------------------------------------------------------------------
 */
bool is_valid_label(char *str)
{
	int i;
	int len = strlen(str);

	/*
	 * Make sure that :
	 * 	string start's with a alphabet letter and 
	 * string is shorter then maximum length (excluding the end of label token)
	 */
	if (!isalpha(str[0]) || len > LABEL_MAX_LENGTH)
	{
		return false;
	}

	/* check all other letters are alphanumeric */
	for (i = 1; i < len; i++)
	{
		if (!isalnum(str[i]))
		{
			return false;
		}
	}

	/* all the the test passed now check labek is not register of operation name*/
	return !is_operation_name(str) && !is_register(str) && !is_in_register(str);
}

/*
--------------------------------------------------------------------------------
 Function resposible for checking if a string is the name of an operationg
 gets the string 
 return bool
---------------------------------------------------------------------------------
 */
bool is_operation_name(char *str)
{
	return (strcmp(str, MOV_OPERATION) == 0) || (strcmp(str, CMP_OPERATION) == 0) ||
		   (strcmp(str, ADD_OPERATION) == 0) || (strcmp(str, SUB_OPERATION) == 0) ||
		   (strcmp(str, NOT_OPERATION) == 0) || (strcmp(str, CLR_OPERATION) == 0) ||
		   (strcmp(str, LEA_OPERATION) == 0) || (strcmp(str, INC_OPERATION) == 0) ||
		   (strcmp(str, DEC_OPERATION) == 0) || (strcmp(str, JMP_OPERATION) == 0) ||
		   (strcmp(str, BNE_OPERATION) == 0) || (strcmp(str, RED_OPERATION) == 0) ||
		   (strcmp(str, PRN_OPERATION) == 0) || (strcmp(str, JSR_OPERATION) == 0) ||
		   (strcmp(str, RTS_OPERATION) == 0) || (strcmp(str, STOP_OPERATION) == 0);
}

/*
--------------------------------------------------------------------------------
 Function resposible for validating the line if the line is valid
 gets the line info
 return bool
---------------------------------------------------------------------------------
 */
bool is_valid_is_operation_line(line_info *info)
{

	return info->line_length < OPERATION_LINE_MAX_LENGTH;
}

/*
--------------------------------------------------------------------------------
 Function resposible for replacing a string with a new one 
 gets pointer to curent string and a new string
---------------------------------------------------------------------------------
 */
void replace_content(char **current_string, char *new_string)
{
	char *temp = allocate_string(strlen(new_string));

	if (*current_string != NULL)
	{
		free(*current_string);
	}

	*current_string = temp;
	strcpy(*current_string, new_string);
}

/*
--------------------------------------------------------------------------------
 Function resposible  new pass line data
 return the pointer to the new pass_data
---------------------------------------------------------------------------------
 */
pass_data *create_pass_data()
{
	pass_data *pass = (pass_data *)allocate_memory(sizeof(pass_data));

	/* if succesufl in allocating memory for the pass line initalize it*/
	if (pass != NULL)
	{
		pass->IC = 0;
		pass->DC = 0;
		pass->prev_operation_operand = NULL;
		pass->is_compiler_error = false;
		pass->is_runtimer_error = false;
		pass->current_line_information = NULL;
	}

	return pass;
}

/*
--------------------------------------------------------------------------------
 Function resposible for creating new file 
 gets file name and extension
---------------------------------------------------------------------------------
 */
FILE *create_output_file(char *file_name_without_extension, char *extension)
{
	FILE *p_output_file = NULL;
	int file_name_length = strlen(file_name_without_extension) + strlen(extension);

	char *file_name = allocate_string(file_name_length);

	if (file_name != NULL)
	{
		/* Creates code file output name */
		strcpy(file_name, file_name_without_extension);
		strcat(file_name, extension);
		file_name[file_name_length] = END_OF_STRING;

		p_output_file = fopen(file_name, WRITE_MODE);

		free(file_name);

		if (!p_output_file)
		{
			print_runtime_error("Cannot create output file");
		}
	}

	return p_output_file;
}

/*
--------------------------------------------------------------------------------
 Function resposible for validating if end of the line
 gets line info
 return bool
---------------------------------------------------------------------------------
 */
bool is_end_of_line(line_info *info)
{
	return info->current_index == info->line_length;
}

/*
--------------------------------------------------------------------------------
 Function resposible for validating if the current operand is register
 gets the operand 
 return bool
---------------------------------------------------------------------------------
 */
bool is_register(char *operand)
{
	int digit;
	int length = strlen(operand);

	if ((length != 2) || (operand[0] != REGISTER_FIRST_TOKEN) || (!isdigit(operand[1])))
	{
		return false;
	}

	digit = atoi(operand + 1);

	return (digit < REGISTERS_COUNT) && (digit >= 0);
}
/*
--------------------------------------------------------------------------------
 Function resposible for validating if the current operand is indirect register
 gets the operand 
 return bool
---------------------------------------------------------------------------------
 */
bool is_in_register(char *operand)
{
	int digit;
	int length = strlen(operand);

	if ((length != 3) || (operand[0] != '*') || (operand[1] != REGISTER_FIRST_TOKEN) || (!isdigit(operand[2])))
	{
		return false;
	}

	digit = atoi(operand + 2);

	return (digit < REGISTERS_COUNT) && (digit >= 0);
}

/*
--------------------------------------------------------------------------------
 Function resposible for printing the machine code to file
 gets the adress the value and the output file
---------------------------------------------------------------------------------
 */
void print_encoding_to_file(unsigned int address, unsigned int value, FILE *p_file)
{
	char *base8_value;

	/* Print Address base 10 value */
	base8_value = convert_base10_to_target_base(address, BASE_TEN, TARGET_MEMORY_ADDRESS_WORD_LENGTH);

	fputs(base8_value, p_file);
	fputc(COLUMN_SEPARATOR, p_file);

	free(base8_value);

	/* Print operation base in targeted base value */
	base8_value = convert_base10_to_target_base(value, TARGET_BASE, TARGET_OPERATION_WORD_LENGTH);
	fputs(base8_value, p_file);

	free(base8_value);

	fputc(END_OF_LINE, p_file);
}
