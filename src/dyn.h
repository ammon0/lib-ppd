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
	
	
	
	// Bytecodes
	T_OBRK,
	T_CBRK,
	
	T_DOT,
	T_COMA,
	
	// send instructions
	T_SEND,
	
	// arithmetic instructions
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
	
	// special instructions
	T_CLSS,
	T_CPY,
	
	// stack instructions
	T_PUSH,
	T_PUSHR,
	T_PUSHT,
	T_POP,
	
	// jump instructions
	T_JMP,
	T_JT,
	T_JF,
	
	// return instructions
	T_RETS,
	T_RETR,
	T_RETT,
	T_RETF,
	T_RETN,
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
		"EOF  ",
		"NL   ",
		// Primary
		"INT  ",
		"STR  ",
		"NAME ",
		":    ",
		"{    ",
		"}    ",
		"<    ",
		">    ",
		"(    ",
		")    ",
		"IS   ",
		"METHOD",
		
		"[    ",
		"]    ",
		".    ",
		",    ",
		
		"SEND ",
		"ADD  ",
		"SUB  ",
		"MUL  ",
		"DIV  ",
		"MOD  ",
		"SL   ",
		"SR   ",
		"SRS  ",
		"ROL  ",
		"ROR  ",
		
		"AND  ",
		"OR   ",
		"NOT  ",
		"LT   ",
		"GT   ",
		"LTE  ",
		"GTE  ",
		"EQ   ",
		"NEQ  ",
		
		"CLASS",
		"COPY ",
		
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

#ifdef __cplusplus
	}
#endif

#endif // _DYN_H


