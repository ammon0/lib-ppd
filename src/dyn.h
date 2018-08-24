/*******************************************************************************
 *
 *	Lib-ppd : Portable Program Data Library
 *
 *	Copyright (c) 2018 Ammon Dodson
 *	You should have received a copy of the license terms with this software. If
 *	not, please visit the project homepage at:
 *	https://github.com/ammon0/lib-ppd
 *
 ******************************************************************************/

/** This file contains the interface between the flex generated component and
the rest of the scanner.
*/

#ifndef _DYN_H
#define _DYN_H

#ifdef __cplusplus
	extern "C" {
#endif

/******************************************************************************/
//                            TYPE DEFINITIONS
/******************************************************************************/


#include <util/types.h>
#include <util/io.h>

#include <ppd/syms.h>

//#include <stdio.h>


/******************************************************************************/
//                                 TOKENS
/******************************************************************************/


typedef enum{
	T_EOF , // yyterminate returns a 0 automatically
	T_NL  ,
	
	// Contstants
	T_INT ,
	T_STR ,
	T_NAME,
	
	// Declarations
	//T_COLO,
	T_OBRC,
	T_CBRC,
	T_OTYPE,
	T_CTYPE,
	T_OPAR,
	T_CPAR,
	T_IS,   // declares initial value
	T_METH, // indicates begining of methods section
	T_PRIM, // declare a primative response
	T_TEMP, // declare number of temps, start bytecodes section
	
	// Bytecodes
	T_SEND,  // send constant selector with j arguments
	T_SENDS, // send constant selector to superClass with j arguments
	
	T_PUSHA, // push a method argument
	T_PUSHC, // push a constant
	//T_PUSHF, // push a receiver field
	T_PUSHT, // push a temp variable
	T_PUSHR, // push the receiver onto the stack
	T_PUSHX, // push the active context
	
	T_POP, // pop stack top
	
	//T_STF, // store in a receiver field
	T_STT, // store in a temp
	
	T_JMP, // unconditional relative jump
	T_JT,  // relative jump on true
	T_JF,  // relative jump on false
	
	T_RETS, // return the stack top
	
	
	// primative operations
	T_ADD, // 1
	T_SUB, // 1
	T_MUL, // 1
	T_DIV, // 1
	//T_MOD, // 1
	T_LSH, // 1
	T_RSH, // 1
	T_SHA, // 1
	T_ROL, // 1
	T_ROR, // 1
	
	T_AND, // 1
	T_OR , // 1
	T_XOR, // 1
	T_NOT, // 1
	T_LT , // 1, return boolean
	T_GT , // 1, return boolean
	T_LTE, // 1, return boolean
	T_GTE, // 1, return boolean
	T_EQ , // 1, return boolean
	T_NEQ, // 1, return boolean
	
	T_TYPE, // 0, return receiver type
	T_SIZE, // 0, return size in bytes
	T_COPY, // 0, return receiver copy
	T_NEW , // 0, return receiver instance
	T_GET , // 1, return receiver field
	T_SET , // 2, set receiver field
	
	NUM_TOKENS
} token_t;


#ifdef _DYN_L
	#define EXTERN
#else
	#define EXTERN extern
#endif

EXTERN const char * token_dex[NUM_TOKENS]
#ifdef _DYN_L
	= {
		// Formatting
		"T_EOF  ",
		"T_NL   ",
		// Contstants
		"T_INT  ",
		"T_STR  ",
		"T_NAME ",
		// Declarations
		//"T_COLO ",
		"T_OBRC ",
		"T_CBRC ",
		"T_OTYPE",
		"T_CTYPE",
		"T_OPAR ",
		"T_CPAR ",
		"T_IS   ",
		"T_METH ",
		"T_PRIM ",
		"T_TEMP ",
		// Bytecodes
		"T_SEND ",
		"T_SENDS",
		
		"T_PUSHA",
		"T_PUSHC",
		"T_PUSHT",
		"T_PUSHR",
		"T_PUSHX",
		"T_POP  ",
		
		"T_STT  ",
		
		"T_JMP  ",
		"T_JT   ",
		"T_JF   ",
		
		"T_RETS ",
		// Primative Operations
		
		"T_ADD  ",
		"T_SUB  ",
		"T_MUL  ",
		"T_DIV  ",
		//"T_MOD  ",
		"T_SL   ",
		"T_SR   ",
		"T_SRS  ",
		"T_ROL  ",
		"T_ROR  ",
		
		"T_AND  ",
		"T_OR   ",
		"T_XOR  ",
		"T_NOT  ",
		"T_LT   ",
		"T_GT   ",
		"T_LTE  ",
		"T_GTE  ",
		"T_EQ   ",
		"T_NEQ  ",
		
		"T_TYPE ",
		"T_SIZE ",
		"T_COPY ",
		"T_NEW  ",
		"T_NEWC ",
		"T_GET  ",
		"T_SET  "
	}
#endif // _GLOBALS_C
;


#undef EXTERN


/******************************************************************************/
//                          SCANNER MODULE VARIABLES
/******************************************************************************/


// Global variables provided by the scanner
#ifndef _DYN_L
	extern uint   yylineno;
	extern char * yytext;
	extern imax   yyval;  ///< numeric value
	extern size_t yyleng; ///< the length of yytext
	extern FILE * yyin;
	
	void yyrestart( FILE *new_file );
#endif


/******************************************************************************/
//                                PROTOTYPES
/******************************************************************************/


#define YY_DECL token_t yylex(void)
token_t yylex(void);

return_t dynParser(DS symbol_table);

#ifdef __cplusplus
	}
#endif

#endif // _DYN_H


