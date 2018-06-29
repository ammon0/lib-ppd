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
#include <stdio.h>


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
	T_COLO,
	T_OBRC,
	T_CBRC,
	T_OTYPE,
	T_CTYPE,
	T_OPAR,
	T_CPAR,
	T_IS,   // declares initial value
	T_METH, // indicates begining of methods section
	
	// send instructions
	T_SEND,  // send constant selector with j arguments
	T_SENDS, // send constant selector to superClass with j arguments
	
	// arithmetic messages
	T_ADD,
	T_SUB,
	T_MUL,
	T_DIV,
	T_MOD,
	T_LSH,
	T_RSH,
	T_SHA,
	T_ROL,
	T_ROR,
	
	T_AND,
	T_OR,
	T_NOT,
	T_LT,
	T_GT,
	T_LTE,
	T_GTE,
	T_EQ,
	T_NEQ,
	
	// special messages
	T_TYPE, // 0, replace object on stack top with its type
	T_COPY, // 0, replace object on stack top with a shallow copy
	T_NEW , // 0, replace type object on stack top with an instance
	T_NEWC, // 1, re
	T_GET , // 1, 
	T_SET , // 2, 
	
	// stack bytecodes
//	T_PUSHF, // push a receiver field onto the stack
//	T_PUSHT, // push a temp onto the stack
//	T_PUSHC, // push a constant onto the stack
	T_PUSH,  
	T_PUSHR, // push the receiver onto the stack
	T_PUSHX, // push the active context
	
//	T_POPT,  // pop a temp off the stack
//	T_POPF,  // pop a receiver field off the stack
	T_POP,   // pop stack top
	T_DUP,   // duplicate the stack top
	
	// jump bytecodes
	T_JMP, // unconditional relative jump
	T_JT,  // relative jump on true
	T_JF,  // relative jump on false
	
	// return bytecodes
	T_RETS, // return the stack top
	T_RETR, // return the receiver
	T_RETC, // return a constant
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
		// Primary
		"T_INT  ",
		"T_STR  ",
		"T_NAME ",
		"T_COLO ",
		"T_OBRC ",
		"T_CBRC ",
		"T_OTYPE",
		"T_CTYPE",
		"T_OPAR ",
		"T_CPAR ",
		"T_IS   ",
		"T_METH ",
		"T_SEND ",
		"T_SENDS",
		"T_ADD  ",
		"T_SUB  ",
		"T_MUL  ",
		"T_DIV  ",
		"T_MOD  ",
		"T_SL   ",
		"T_SR   ",
		"T_SRS  ",
		"T_ROL  ",
		"T_ROR  ",
		
		"T_AND  ",
		"T_OR   ",
		"T_NOT  ",
		"T_LT   ",
		"T_GT   ",
		"T_LTE  ",
		"T_GTE  ",
		"T_EQ   ",
		"T_NEQ  ",
		
		"T_CLASS",
		"T_COPY ",
		
		"PUSH ",
		"PUSH RECVR",
		"PUSH TEMP",
		"POP  ",
		
		"JMP  ",
		"JMP ifT",
		"JMP ifF",
		
		"RET STACK",
		"RET RECVR",
		"RET TRUE ",
		"RET FALSE",
		"RET NULL "
	}
#endif // _GLOBALS_C
;


#undef EXTERN


/******************************************************************************/
//                          SCANNER MODULE VARIABLES
/******************************************************************************/


// Global variables provided by the scanner
#ifndef _DYN_L
	extern int    yylineno;
	extern char * yytext;
	extern int    yyleng; ///< the length of yytext
	extern FILE * yyin;
	
	void yyrestart( FILE *new_file );
#endif


/******************************************************************************/
//                                PROTOTYPES
/******************************************************************************/


#define YY_DECL token_t yylex(void)
token_t yylex(void);

void objectParser(void);

#ifdef __cplusplus
	}
#endif

#endif // _DYN_H


