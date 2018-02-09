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

#ifndef _LEX_H
#define _LEX_H

#ifdef __cplusplus
	extern "C" {
#endif

/******************************************************************************/
//                            TYPE DEFINITIONS
/******************************************************************************/


#include <util/types.h>
#include <util/msg.h>
#include <stdio.h>


/******************************************************************************/
//                                 TOKENS
/******************************************************************************/


typedef enum{
	T_EOF,
	T_NL,
	T_NUM,
	T_STR,
	T_CHAR,
	T_NAME
} token_t;


/******************************************************************************/
//                          SCANNER MODULE VARIABLES
/******************************************************************************/


// Global variables provided by the scanner
#ifndef _MPL_L
	extern int    yylineno;
	extern char * yytext;
	extern int    yyleng; ///< the length of yytext
	extern FILE * yyin;
#endif


/******************************************************************************/
//                                PROTOTYPES
/******************************************************************************/


/// Report errors in the scanner
static inline void scan_error(const char * message){
	msg_print(NULL, V_ERROR, "scanner: %s, on line %d\n", message, yylineno);
}

#define YY_DECL token_t yylex(void)
token_t yylex(void);

#ifdef __cplusplus
	}
#endif

#endif // _LEX_H


