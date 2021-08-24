/*
--------------------------------------------------------------------------------
 The symbol table file holds the relevent info to the symbol table
---------------------------------------------------------------------------------
 */

#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_


#include "Types.h"
#include "Consts.h"
#include "Helper.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
--------------------------------------------------------------------------------
 Function resposible for addint the symbol pointer to the table
---------------------------------------------------------------------------------
 */
void add_symbol_to_list(symbol_node_ptr p_new_symbol);

/*
--------------------------------------------------------------------------------
 Function resposible for creating a new node in the symbol table
 gets the name , the adress , bool is extern , bool is inst
 return the pointer to the node
---------------------------------------------------------------------------------
 */
symbol_node_ptr create_symbol(char* name, unsigned int address, bool is_extern, bool is_instruction);

/*
--------------------------------------------------------------------------------
 Function resposible for finding the symbol int the table
 gets name of the symbol return the symbol if found
---------------------------------------------------------------------------------
 */
symbol_node_ptr search_symbol(char* symbol_name);

/*
--------------------------------------------------------------------------------
 Function resposible for update the adress of the symbol based on if instruction
---------------------------------------------------------------------------------
 */
void update_symbol_address(int ic_length);

/*
--------------------------------------------------------------------------------
 Function resposible for freeing the memory taken by the list
---------------------------------------------------------------------------------
 */
void free_symbol_list();

#endif /* SYMBOLTABLE_H_ */
