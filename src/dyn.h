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
	
	// directives
	T_METH, // indicates begining of methods section
	
	// Bytecodes
	
	
	T_ASS,
	T_OP,
	
	T_OPAR,
	T_CPAR,
	T_OBRK,
	T_CBRK,
	
	T_DOT,
	T_COMA,
	
	T_QUES,
	
	
	
	
	// instructions
	
	T_RET,
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
		"EOF", "NL",
		// Primary
		"Integer", "String", "Name",
		":", "{", "}", "<", ">",
		"(", ")", "[", "]", 
		".", ",", ":", "?",
		"METHOD", "RETURN"
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


