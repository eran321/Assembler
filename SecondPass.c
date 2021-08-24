/*
--------------------------------------------------------------------------------
 The SecondPassf ile holds all the relevant functions to the second pass through 
 the assembly code
---------------------------------------------------------------------------------
 */

#include "SecondPass.h"

/*
--------------------------------------------------------------------------------
 Function that executes the second pass
 gets the source file, the name of file , the previous ic and dc
---------------------------------------------------------------------------------
 */
void second_pass_execute(FILE *pFile, char *file_name_without_extension, unsigned int previous_pass_ic, unsigned int previous_pass_dc)
{
	compiler_output_files output_files;
	int line_number = 0;

	/* Creates pass first data */
	pass_data *pass = create_pass_data();

	if (pass == NULL)
	{
		return;
	}

	/* Create the ob file */
	output_files.ob_file = create_output_file(file_name_without_extension, CODE_FILE_EXT);

	if (output_files.ob_file == NULL)
	{
		return;
	}

	output_files.entry_file = NULL;
	output_files.extern_file = NULL;

	/* Write first line to ob file */
	write_code_and_data_size_to_output_file(previous_pass_ic, previous_pass_dc, output_files.ob_file);

	/* Initializes IC to zero*/
	pass->IC = 0;

	/* loop that goes until the end file */
	while (!feof(pFile) && !pass->is_runtimer_error)
	{
		char line[MAX_LINE_LENGTH];

		line_number++;

		/* Read next line*/
		if (fgets(line, MAX_LINE_LENGTH + 1, pFile))
		{
			/* This isn't an empty line or a comment */
			if (!is_empty_or_comment(line))
			{
				pass->current_line_information =
					create_line_info(file_name_without_extension, line_number, line);

				/* Process line */
				if (pass->current_line_information != NULL)
				{
					second_pass_process_line(pass, &output_files);

					free(pass->current_line_information);
				}
				else
				{
					pass->is_runtimer_error = true;
				}
			}
		}
	}

	/* No error has occurred */
	if (!pass->is_runtimer_error && !pass->is_compiler_error)
	{
		/* Write data initialization section into ob file */
		write_data_to_output_file(output_files.ob_file);

		/* Close files */
		if (output_files.ob_file != NULL)
		{
			fclose(output_files.ob_file);
		}

		if (output_files.extern_file != NULL)
		{
			fclose(output_files.extern_file);
		}

		if (output_files.entry_file != NULL)
		{
			fclose(output_files.entry_file);
		}
	}
	else
	{
		/* Close and delete  ob file */
		if (output_files.ob_file != NULL)
		{
			char *full_name = allocate_string(strlen(file_name_without_extension) + strlen(CODE_FILE_EXT));

			fclose(output_files.ob_file);
			if (full_name != NULL)
			{
				strcpy(full_name, file_name_without_extension);
				strcat(full_name, CODE_FILE_EXT);

				remove(full_name);

				free(full_name);
			}
			else
			{
				print_runtime_error("Couldn't delete compilation files");
			}
		}

		/* Close and delete ext file */
		if (output_files.extern_file != NULL)
		{
			char *full_name = allocate_string(strlen(file_name_without_extension) + strlen(EXTERN_FILE_EXT));

			fclose(output_files.extern_file);

			if (full_name != NULL)
			{
				strcpy(full_name, file_name_without_extension);
				strcat(full_name, EXTERN_FILE_EXT);

				remove(full_name);

				free(full_name);
			}
			else
			{
				print_runtime_error("Couldn't delete compilation files");
			}
		}

		/* Close and delete ent file */
		if (output_files.entry_file != NULL)
		{
			char *full_name = allocate_string(strlen(file_name_without_extension) + strlen(ENTRY_FILE_EXT));

			fclose(output_files.entry_file);
			if (full_name != NULL)
			{
				strcpy(full_name, file_name_without_extension);
				strcat(full_name, ENTRY_FILE_EXT);

				remove(full_name);
				free(full_name);
			}
			else
			{
				print_runtime_error("Couldn't delete compilation files");
			}
		}
	}
}

/*
--------------------------------------------------------------------------------
 Function that proceces a line in the second pass
 gets the current line the output files 
---------------------------------------------------------------------------------
 */
void second_pass_process_line(pass_data *pass, compiler_output_files *output_files)
{
	char *type = NULL;
	int index;

	/* Skips label if exists */
	skip_label(pass->current_line_information);

	index = pass->current_line_information->current_index;

	/* Read line type */
	type = get_next_word(pass);

	/* Handle line type */
	if (type == NULL)
	{
		print_compiler_error("Invalid line", pass->current_line_information);
		pass->is_compiler_error = true;
	}
	/* Line is data initialization - Ignores it */
	else if ((strcmp(type, DATA_OPERATION) == 0) || (strcmp(type, STRING_OPERATION) == 0))
	{
		/* do nothing*/
	}
	/* Line is extern */
	else if (strcmp(type, EXTERN_OPERATION) == 0)
	{
		create_extern_output_file_if_needed(output_files, pass->current_line_information->file_name);

		if (output_files->extern_file == NULL)
		{
			pass->is_runtimer_error = true;
		}
	}
	else if (strcmp(type, ENTRY_OPERATION) == 0)
	{
		/* Process entry */
		second_pass_process_entry(pass, output_files);
	}
	else
	{
		/* Process operation */
		pass->current_line_information->current_index = index;
		second_pass_process_operation(pass, output_files);
	}

	if (type != NULL)
	{
		free(type);
	}
}

/*
--------------------------------------------------------------------------------
 Function that resposible for printing the the ic and dc to the output file
---------------------------------------------------------------------------------
 */
void write_code_and_data_size_to_output_file(unsigned int ic, unsigned int dc, FILE *output_file)
{
	char *number;

	/* Write ic to file */
	number = convert_base10_to_target_base(ic, BASE_TEN, 0);
	fputs(number, output_file);
	free(number);

	fputc(HEADER_SEPARATOR, output_file);

	/* Write dc to file */
	number = convert_base10_to_target_base(dc, BASE_TEN, 0);
	fputs(number, output_file);
	free(number);

	fputc(END_OF_LINE, output_file);
}
