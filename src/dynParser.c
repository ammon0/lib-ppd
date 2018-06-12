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


#include "dyn.h"
#include <util/msg.h>
#include <string.h>
#include <stdlib.h>


/******************************************************************************/
//                            MODULE VARIABLES
/******************************************************************************/


token_t token;
char err_array[100];


/******************************************************************************/
//                             INLINE FUNCTIONS
/******************************************************************************/


static inline void getNext(void){
	token = yylex();
}

/************************ ERROR REPORTING & RECOVERY **************************/

static inline void parse_crit(const char * message){
	msg_print(NULL, V_ERROR, "CRITICAL: %u: %s.\n",
		yylineno, message
	);
	exit(EXIT_FAILURE);
}

static inline void parse_error(const char * message){
	msg_print(NULL, V_ERROR, "%u: %s.\n", yylineno, message);
	
	while(token != T_EOF && token != T_NL) getNext();
	msg_print(NULL, V_ERROR, "continuing...\n");
}

static inline void expected(const char* thing){
	// FIXME: this is not a safe use of sprintf
	sprintf(err_array, "Expected '%s', found '%s'",
		thing,
		yytext
	);
	parse_error(err_array);
}

static inline void parse_warn(const char * message){
	msg_print(NULL, V_WARN, "%u: %s.\n", yylineno, message);
}

/********************************* GETTERS ************************************/

static inline bool match_string(const char * string){
	if ( strcmp(string, yytext) ){
		expected(string);
	}
	getNext();
	return true;
}

static inline void match_token(token_t t){
	if(token == t) getNext();
	else expected(token_dex[t]);
}



/******************************************************************************/
//                             PARSE FUNCTIONS
/******************************************************************************/


static void Fields(void){
	
}

static void Methods(void){
	
}


void objectParser(void){
	
	msg_print(NULL, V_DEBUG, "objectParser(): start\n");
	
	getNext(); // initialize the token
	
	while(token == T_NL) getNext(); // eat leading whitespace
	
	match_token(T_NAME);
	match_token(T_COLO);
	match_token(T_NAME);
	match_token(T_OBRC);
	match_token(T_NL);
	
	Fields();
	
	match_token(T_METH);
	
	Methods();
	
	match_token(T_CBRC);
	
	msg_print(NULL, V_DEBUG, "objectParser(): stop\n");
}



