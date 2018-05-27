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
	T_EOF,
	T_NL,
	T_INT,
	T_STR,
	T_CHAR,
	T_NAME,
	
	
	
	// directives
	T_METH, // indicates begining of methods section
	
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
#ifdef _GLOBAL_C
	= {
		// Formatting
		"End of File", "New Line",
		// Primary
		"Integer", "String", "Character", "Name"
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


