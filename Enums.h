/*
 ------------------------------------------------------------------------------------
 The Enums file contains all the enums 
 ------------------------------------------------------------------------------------
 */
#ifndef ENUMS_H_
#define ENUMS_H_

/* Addressing methods */
typedef enum {
	INVALID_ADDRESS_METHOD = -1,
	IMMEDIATE = 1,
	DIRECT = 2,
	INDIRECT_REGISTER = 4,
	DIRECT_REGISTER = 8
} ADDRESS_METHOD;

/* ARE options */
typedef enum {
	ABSOLUTE = 4,
	RELOCATABLE = 2,
	EXTERNAL = 1
} ARE;

/* Number of operands */
typedef enum {
	NO_OPERANDS = 0,
	ONE_OPERAND = 1,
	TWO_OPERANDS = 2
} OPERANDS_COUNT;



#endif /* ENUMS_H_ */
