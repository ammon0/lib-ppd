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

log_descriptor parseLog = NULL;


/******************************************************************************/
//                             INLINE FUNCTIONS
/******************************************************************************/


static inline void getNext(void){
	token = yylex();
}

/************************ ERROR REPORTING & RECOVERY **************************/

static inline void parse_crit(const char * message){
	msg_print(parseLog, V_ERROR, "CRITICAL: %u: %s.\n",
		yylineno, message
	);
	exit(EXIT_FAILURE);
}

static inline void parse_error(const char * message){
/*	msg_print(parseLog, V_ERROR, "%u: %s.\n", yylineno, message);*/
/*	*/
/*	//while(token != T_EOF && token != T_NL) getNext();*/
/*	msg_print(parseLog, V_ERROR, "exiting...\n");*/
/*	exit*/
	parse_crit(message);
}

static inline void expected(const char* thing){
	// FIXME: this is not a safe use of sprintf
	sprintf(err_array, "Expected '%s', found %s: '%s'",
		thing,
		token_dex[token],
		yytext
	);
	parse_error(err_array);
}

static inline void parse_warn(const char * message){
	msg_print(parseLog, V_WARN, "%u: %s.\n", yylineno, message);
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


static void instructions(void){
	while(token != T_CBRC){
		if(token == T_NL){ // empty instructions
			getNext();
			continue;
		}
		
		switch(token){
		// send instructions
		case T_SEND: getNext(); match_token(T_INT); break;
	
		// arithmetic instructions
		case T_ADD: getNext(); break;
		case T_SUB: getNext(); break;
		case T_MUL: getNext(); break;
		case T_DIV: getNext(); break;
		case T_MOD: getNext(); break;
		case T_LSH: getNext(); break;
		case T_RSH: getNext(); break;
		case T_SHA: getNext(); break;
		case T_ROL: getNext(); break;
		case T_ROR: getNext(); break;
		case T_AND: getNext(); break;
		case T_OR : getNext(); break;
		case T_NOT: getNext(); break;
		case T_LT : getNext(); break;
		case T_GT : getNext(); break;
		case T_LTE: getNext(); break;
		case T_GTE: getNext(); break;
		case T_EQ : getNext(); break;
		case T_NEQ: getNext(); break;
	
		// special instructions
		case T_CLSS: getNext(); break;
		case T_CPY : getNext(); break;
	
		// stack instructions
		case T_PUSH : getNext(); break;
		case T_PUSHR: getNext(); break;
		case T_PUSHT: getNext(); match_token(T_INT); break;
		case T_POP  : getNext(); break;
	
		// jump instructions
		case T_JMP: getNext(); match_token(T_NAME); break;
		case T_JT : getNext(); match_token(T_NAME); break;
		case T_JF : getNext(); match_token(T_NAME); break;
	
		// return instructions
		case T_RETS: getNext(); break;
		case T_RETR: getNext(); break;
		case T_RETT: getNext(); break;
		case T_RETF: getNext(); break;
		case T_RETN: getNext(); break;
		
		
		
		case T_INT : // errors
		case T_STR :
		case T_NAME:
		case T_OBRK:
		case T_CBRK:
		case T_DOT:
		case T_COMA:
		case T_COLO:
		case T_OBRC:
		case T_CBRC:
		case T_OTYPE:
		case T_CTYPE:
		case T_OPAR:
		case T_CPAR:
		case T_IS:   // declares initial value
		case T_METH: // indicates begining of methods section
		case T_EOF : // yyterminate returns a 0 automatically
		case T_NL  :
		case NUM_TOKENS:
		default: expected("instruction");
		}
	}
}


static void initList(void){
	if(token == T_OBRC){
		getNext();
		while(token!=T_CBRC) initList(); // list initializer
		match_token(T_CBRC);
	}
	else if(token == T_STR) getNext(); // string initializer
	else match_token(T_NAME); // name initializer
}

static void fieldInit(void){
	match_token(T_IS);
	initList();
}

static void staticType(void){
	msg_print(parseLog, V_TRACE, "staticType(): start\n");
	
	match_token(T_OPAR);
	
	if(token == T_NAME){
		getNext();
		if(token == T_OPAR) staticType();
	}
	
	match_token(T_CPAR);
	
	msg_print(parseLog, V_TRACE, "staticType(): stop\n");
}

static void argList(void){
	match_token(T_OPAR);
	
	while(token != T_CPAR){
		if(token==T_NL){
			getNext();
			continue;
		}
		if(token==T_OPAR) staticType();
		match_token(T_NAME);
	}
	
	match_token(T_CPAR);
}


static void Fields(void){
	msg_print(parseLog, V_TRACE, "Fields(): start\n");
	
	while(token != T_METH){
		if(token == T_NL){ // empty declaration
			getNext();
			continue;
		}
		
		if(token == T_OPAR) staticType();
	
		match_token(T_NAME); // field name
	
		if(token == T_IS) fieldInit();
		match_token(T_NL);
	}
	
	msg_print(parseLog, V_TRACE, "Fields(): stop\n");
}

static void Methods(void){
	msg_print(parseLog, V_TRACE, "Methods(): start\n");
	
	while(token != T_CBRC){
		if(token == T_NL){ // empty declaration
			getNext();
			continue;
		}
		
		msg_print(parseLog, V_TRACE, "%d: method\n", yylineno);
		
		if(token == T_OPAR) staticType();
		
		match_token(T_NAME);
		argList();
		
		match_token(T_OBRC);
		instructions();
		match_token(T_CBRC);
		
	}
	
	msg_print(parseLog, V_TRACE, "Methods(): stop\n");
}


void objectParser(void){
	
	msg_print(parseLog, V_DEBUG, "objectParser(): start\n");
	
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
	
	msg_print(parseLog, V_DEBUG, "objectParser(): stop\n");
}



