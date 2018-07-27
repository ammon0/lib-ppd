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
#include "objects.h"
#include <ppd/debug.h>
#include <ppd/syms.h>
#include <string.h>
#include <stdlib.h>


/******************************************************************************/
//                            MODULE VARIABLES
/******************************************************************************/


token_t token;

DS syms;

uint8_t *byteBuffer;
size_t  bufferSize, ip;

#define BUF_SZ 1024

struct current_class{
	
}


/******************************************************************************/
//                             INLINE FUNCTIONS
/******************************************************************************/


static inline void getNext(void){
	token = yylex();
}

/************************ ERROR REPORTING & RECOVERY **************************/


static inline void parse_error(const char * message){
	error("%u: %s.\n", yylineno, message);
}

static inline void parse_warn(const char * message){
	warn("%u: %s.\n", yylineno, message);
}

static inline void expected(const char* thing){
	error("Expected '%s', found %s: '%s'",
		thing,
		token_dex[token],
		yytext
	);
}

/********************************* GETTERS ************************************/

static inline RETURN match_string(const char * string){
	if ( strcmp(string, yytext) ){
		expected(string);
		return r_failure;
	}
	getNext();
	return r_success;
}

static inline void match_token(token_t t){
	if(token == t) getNext();
	else expected(token_dex[t]);
}

/******************************* BYTECODE BUFFER ******************************/

static RETURN initBuffer(void){
	byteBuffer = malloc(BUF_SZ);
	if(!byteBuffer){
		error("initBuffer(): out of memory.\n");
		return r_failure;
	}
	bufferSize = BUF_SZ;
	ip = 0;
	return r_success;
}

static inline RETURN growBuffer(void){
	bufferSize <<=1;
	byteBuffer = realloc(byteBuffer, bufferSize);
	if(!byteBuffer){
		error("growBuffer(): out of memory.\n");
		return r_failure;
	}
	return r_success;
}

static void addByte(uint8_t b){
	byteBuffer[ip] = b;
	ip++;
	
	if(ip == bufferSize) growBuffer();
}


/******************************************************************************/
//                            INSTRUCTION MACROS
/******************************************************************************/


static inline void simpleInstruction(void){ addByte(token); getNext(); }

static void jumpInstruction(void){
	simpleInstruction();
	match_token(T_NAME);
}

static void frameInstruction(void){
	simpleInstruction();
	match_token(T_INT);
}

static void sendInstruction(void){
	simpleInstruction();
	match_token(T_INT);
	match_token(T_STR);
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
		// message instructions
		case T_SEND:
		case T_SENDS: sendInstruction(); break;
		
		// arithmetic message instructions
		case T_ADD:
		case T_SUB:
		case T_MUL:
		case T_DIV:
		case T_MOD:
		case T_LSH:
		case T_RSH:
		case T_SHA:
		case T_ROL:
		case T_ROR:
		case T_AND:
		case T_OR :
		case T_NOT:
		case T_LT :
		case T_GT :
		case T_LTE:
		case T_GTE:
		case T_EQ :
		case T_NEQ:
		
		// special message instructions
		case T_TYPE:
		case T_COPY:
		case T_NEW :
		case T_NEWC:
		case T_GET :
		case T_SET : simpleInstruction(); break;
		
		// stack instructions
		//case T_PUSH :
		case T_PUSHA:
		case T_PUSHC:
		case T_PUSHR:
		case T_PUSHT:
		case T_PUSHX:
		case T_POP  :
		case T_STT  :
		
		//case T_DUP  : simpleInstruction(); break;
		
		// jump instructions
		case T_JMP:
		case T_JT :
		case T_JF : jumpInstruction(); break;
		
		// return instructions
		case T_RETS:
		//case T_RETR: simpleInstruction(); break;
		//case T_RETC: frameInstruction(); break;
		
		// errors
		case T_INT :
		case T_STR :
		case T_NAME:
		case T_COLO:
		case T_OBRC:
		case T_CBRC:
		case T_OTYPE:
		case T_CTYPE:
		case T_OPAR:
		case T_CPAR:
		case T_IS:   // declares initial value
		case T_METH: // indicates begining of methods section
		case T_TEMP:
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
	trace("staticType(): start\n");
	
	match_token(T_OPAR);
	
	if(token == T_NAME){
		getNext();
		if(token == T_OPAR) staticType();
	}
	
	match_token(T_CPAR);
	
	trace("staticType(): stop\n");
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
	trace("Fields(): start\n");
	
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
	
	trace("Fields(): stop\n");
}

static void Methods(void){
	trace("Methods(): start\n");
	
	while(token != T_CBRC){
		if(token == T_NL){ // empty declaration
			getNext();
			continue;
		}
		
		trace("%d: method\n", yylineno);
		
		if(token == T_OPAR) staticType();
		
		match_token(T_NAME);
		argList();
		
		initBuffer(); // initialize the bytecode buffer
		
		match_token(T_OBRC);
		instructions();
		match_token(T_CBRC);
		
	}
	
	trace("Methods(): stop\n");
}


void objectParser(DS symbol_table){
	
	debug("objectParser(): start\n");
	
	syms = symbol_table;
	
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
	
	debug("objectParser(): stop\n");
}



