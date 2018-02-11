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


#include <ppd/mpl.hpp>
#include "lex.h"
#include <setjmp.h>


/******************************************************************************/
//                              MODULE VARIABLES
/******************************************************************************/


static token_t    token ;
static PPD_ModRef module;
static jmp_buf    new_statement;


/******************************************************************************/
//                              MODULE FUNCTIONS
/******************************************************************************/


static inline void next_token(void);

/************************ ERROR REPORTING & RECOVERY **************************/

static inline void parse_error(const char * message){
	msg_print(NULL, V_ERROR, "%u: %s.\n", yylineno, message);
	
	while(token != T_EOF && token != T_NL)
		next_token();
	msg_print(NULL, V_NOTE, "continuing...\n");

	longjmp(new_statement, true);
}

static inline void expected(const char* thing){
	char err_array[100]; // FIXME: this risks stack smashing
	
	sprintf(err_array, "Expected '%s', found '%s'",
		thing,
		yytext
	);
	parse_error(err_array);
}

/********************************** GETTERS ***********************************/

static inline void next_token(void){
	token = yylex();
}

static inline void match_token(token_t t){
	if(token == t) next_token();
	else expected(token_dex[t]);
}

/*********************************** PARSER ***********************************/


static void Statement(void){
	
	msg_print(NULL, V_TRACE, "Statement(): start");
	
	// labeled statement
	if(token == T_LBL){}
	
	switch (token){
	default:
		expected("a statement.");
	}
	
	match_token(T_NL);
	
	msg_print(NULL, V_TRACE, "Statement(): stop");
}


/******************************************************************************/
//                              PUBLIC FUNCTION
/******************************************************************************/


PPD_ModRef ReadMPL (FILE * infile){
	bool errors=false;
	
	msg_print(NULL, V_TRACE, "ReadMPL: start\n");
	
	module = new PPD_Module;
	yyin = infile;
	
	// initialize scanner
	next_token();
	
	// save stack state for error continuation
	errors = setjmp(new_statement);
	
	do{
		Statement();
	} while (token != T_EOF);
	
	
	if(errors){
		msg_print(NULL, V_ERROR, "parse completed with errors.\n");
		module->dump_stderr();
		delete module;
		msg_print(NULL, V_TRACE, "ReadMPL: stop failure\n");
		return NULL;
	}
	
	msg_print(NULL, V_INFO, "file parsed successfully.\n");
	msg_print(NULL, V_TRACE, "ReadMPL: stop successfully\n");
	return module;
}


