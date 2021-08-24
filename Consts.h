/*
 ------------------------------------------------------------------------------------
All the constant variables needed for the Assembler
 ------------------------------------------------------------------------------------
 */

#ifndef CONSTS_H_
#define CONSTS_H_
#include <limits.h>

/* Length of line in code */
#define MAX_LINE_LENGTH 200

/* Base used in output files */
#define TARGET_BASE 8
#define BASE_TEN 10

/* Output files separator */
#define COLUMN_SEPARATOR '\t'
#define HEADER_SEPARATOR ' '
/* Address constants */
#define ADDRESS_START 100
#define NO_ADDRESS 0

/* Label definitions */
#define LABEL_END_TOKEN ':'
#define LABEL_MAX_LENGTH 31

#define END_OF_LINE '\n'
#define END_OF_STRING '\0'

/* A comment line */
#define COMMENT ';'

/* Data definition handling */
#define MINUS_SIGN '-'
#define PLUS_SIGN '+'
#define NUMBER_TOKEN_SEPERATOR ','
#define QUOTATION '\"'
#define STRING_DATA_END 0

/* Memory word bits */
#define MEMORY_WORD 15
#define DATA_WORD_BITS_LENGTH MEMORY_WORD
#define OPERATION_OP_CODE_BITS_LENGTH 4
#define OPERATION_ADDRESS_METHOD_BITS_LENGTH 4
#define OPERATION_ERA_BITS_LENGTH 3
#define OPERAND_REGISTER_BITS_LENGTH 3
#define OPERAND_NON_REGISTER_BITS_LENGTH 12
#define REST_BITS_LENGTH 17
#define TARGET_MEMORY_ADDRESS_WORD_LENGTH 4
#define TARGET_OPERATION_WORD_LENGTH 5
#define MAX_MEM 4096 

/* Operation definitions */
#define MOV_OPERATION "mov"
#define CMP_OPERATION "cmp"
#define ADD_OPERATION "add"
#define SUB_OPERATION "sub"
#define LEA_OPERATION "lea"
#define CLR_OPERATION "clr"
#define NOT_OPERATION "not"
#define INC_OPERATION "inc"
#define DEC_OPERATION "dec"
#define JMP_OPERATION "jmp"
#define BNE_OPERATION "bne"
#define RED_OPERATION "red"
#define PRN_OPERATION "prn"
#define JSR_OPERATION "jsr"
#define RTS_OPERATION "rts"
#define STOP_OPERATION "stop"
#define OPERATION_LINE_MAX_LENGTH 80
#define OPERAND_SEPERATOR ','
#define IMMEDIATE_TOKEN '#'
#define REGISTER_FIRST_TOKEN 'r'
#define REGISTERS_COUNT 8
#define OPERAION_MIN_WORD_SIZE 1
#define INVALID_OPEARTION_COUNTER -1

/* Instruction */
#define ENTRY_OPERATION ".entry"
#define STRING_OPERATION ".string"
#define DATA_OPERATION ".data"
#define EXTERN_OPERATION ".extern"

/* File usage */
#define READ_ONLY_MODE "r"
#define WRITE_MODE "w"

/* Input code file extension */
#define FILE_EXT ".as"

/* Output file extensions */
#define EXTERN_FILE_EXT ".ext"
#define CODE_FILE_EXT ".ob"
#define ENTRY_FILE_EXT ".ent"

#endif /* CONSTS_H_ */
