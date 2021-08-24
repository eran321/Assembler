/*
--------------------------------------------------------------------------------
 The OperationsEncoder file holds all the functions for encoding operatons 
---------------------------------------------------------------------------------
 */

#ifndef OPERATIONENCODER_H_
#define OPERATIONENCODER_H_

#include "Consts.h"
#include "Types.h"
#include "Helper.h"
#include "SymbolTable.h"
#include "ExternEncoder.h"
#include "Helper.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/*
--------------------------------------------------------------------------------
 Function responsible for processing the info during the first pass 
 gets the current line the label if it exists and a bool of is it a symbol
---------------------------------------------------------------------------------
 */
void first_pass_process_operation(pass_data* pass, char* label, bool is_symbol);

/*
--------------------------------------------------------------------------------
 Function responsible for encoding the operaton in the second pass 
 gets the current line and the files where the operation will be ecoded
---------------------------------------------------------------------------------
 */
void second_pass_process_operation(pass_data* pass, compiler_output_files* p_output_files);

/*
--------------------------------------------------------------------------------
 Function responsible for retunring all the info about a the current line 
 gets the currennt line
---------------------------------------------------------------------------------
 */
decoded_operation* get_decoded_operation(pass_data* pass);

/*
--------------------------------------------------------------------------------
 Function responsible for returning the operation name 
 gets the current line returns the the opeartion name in astring of chars
---------------------------------------------------------------------------------
 */
char* get_operation_name(pass_data* pass);

/*
--------------------------------------------------------------------------------
 Function responsible for getting the data about the operands n the curretn line
 gets the current line and the operand count the souurce and target operands
---------------------------------------------------------------------------------
 */
bool get_operands(pass_data* pass, int operands_count, char** source_operand, char** target_operand);

/*
--------------------------------------------------------------------------------
 Function responsible for getting the next operands
 gets the current line 
 return the operand
---------------------------------------------------------------------------------
 */
char* get_next_operand(pass_data* pass);

/*
--------------------------------------------------------------------------------
 Function responsible for returning the address method
 gets the current line and the operand
 returns the the the addres method
---------------------------------------------------------------------------------
 */
ADDRESS_METHOD get_address_method(pass_data* pass, char* operand);

/*
--------------------------------------------------------------------------------
 Function that returns a boolean if the method of adressing is right for the 
 current operation
---------------------------------------------------------------------------------
 */
bool are_operand_methods_allowed_in_operation(decoded_operation* current_operation);

/*
--------------------------------------------------------------------------------
 Function responsible for calculating the amount of memory ("words") each 
 operation needs
 gets the current line and the current opern
---------------------------------------------------------------------------------
 */
int calculate_operation_size(pass_data* pass, decoded_operation* current_operation);

/*
--------------------------------------------------------------------------------
 Function responsible for writing the operation machine code to the ouput piles
 gets the current line , the curretn oper , the output files
 returns true if sucseful 
---------------------------------------------------------------------------------
 */
bool encode_operation(pass_data* pass, decoded_operation* p_decoded_operation, compiler_output_files* output_files);

/*
--------------------------------------------------------------------------------
 Function responsible for writing the operands inmachine code to the ouput piles
 gets the current line , the curret oper , the output files
 returns true if sucseful 
---------------------------------------------------------------------------------
 */
bool encode_operands(pass_data* pass, decoded_operation* p_decoded_operation, compiler_output_files* output_files);

/*
--------------------------------------------------------------------------------
 Function responsible for writing the direct adress machine code to the ouput piles
 gets the current line , the curretn oper , the output files
 returns true if sucseful 
---------------------------------------------------------------------------------
 */
bool encode_direct(pass_data* pass, char* operand, compiler_output_files* output_files);

/*
--------------------------------------------------------------------------------
 Function responsible for writing registers in the machine code to the ouput piles
 gets the current line , the curretn oper , the output files
 returns true if sucseful 
---------------------------------------------------------------------------------
 */
bool encode_registers(pass_data* pass, char* source_register, char* target_register, FILE* p_file);

/*
--------------------------------------------------------------------------------
 Function responsible for writing indirect registers in the machine code to the ouput piles
 gets the current line , the curretn oper , the output files
 returns true if sucseful 
---------------------------------------------------------------------------------
 */
bool encode_in_registers(pass_data* pass, char* source_register, char* target_register, FILE* p_file);

/*
--------------------------------------------------------------------------------
 Function responsible for writing the imidiate in machine code to the ouput piles
 gets the current line , the curretn oper , the output files
 returns true if sucseful 
---------------------------------------------------------------------------------
 */
bool encode_immediate(pass_data* pass, char* operand, FILE* p_file);

/*
--------------------------------------------------------------------------------
 Function responsible for writing finding the opeation in the list of operation
 get operation name
 return the pointer to the operation if succesful and null otherwise
---------------------------------------------------------------------------------
 */
machine_operation_definition* search_machine_operation_in_list(char* operation);

/*
--------------------------------------------------------------------------------
 Function responsible for initializiing the operation list 
 return true if succesfull
---------------------------------------------------------------------------------
 */
bool init_operation_list();

/*
--------------------------------------------------------------------------------
 Function responsible for adding operation to the list 
 gets the name of oper the code and the operands
 return true if succesfull
---------------------------------------------------------------------------------
 */
bool add_operation_to_list(char* name, unsigned int code, int operands);

/*
----------------------------------------------------------
 Free memory for operations list
----------------------------------------------------------
*/
void free_operation_list();

#endif /* OPERATIONENCODER_H_ */
