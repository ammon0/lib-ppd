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
#include <ppd/object_mem.h>
#include <ppd/vm.h>

#include <util/string.h>

#include <assert.h>
#include <stdlib.h>


/******************************************************************************/
//                                 TYPES
/******************************************************************************/


#define BUF_SZ 1024

typedef struct byteBuffer{
	size_t  size, ip;
	uint8_t bytes[];
} byteBuffer;

typedef struct methodData{
	//const char * method_name;
	umax tempCount, argCount, constCount, primativeIndex;
	byteBuffer * buf;
	DS constants;
} methodData;


/******************************************************************************/
//                            MODULE VARIABLES
/******************************************************************************/


token_t token;
DS syms, field_list;

uint errors =0;


/******************************************************************************/
//                             INLINE FUNCTIONS
/******************************************************************************/


static inline void getNext(void){
	token = yylex();
}

/************************ ERROR REPORTING & RECOVERY **************************/


static inline void parse_error(const char * message){
	error("%u: %s.\n", yylineno, message);
	errors++;
}

static inline void parse_warn(const char * message){
	warn("%u: %s.\n", yylineno, message);
}

static inline void expected(const char* thing){
	char buf[100];
	
	snprintf(buf, 100, "Expected '%s', found %s: '%s'",
		thing,
		token_dex[token],
		yytext
	);
	parse_error(buf);
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

static inline const char * get_name(void){
	char * name;
	size_t name_sz;
	
	
	if(token != T_NAME) {
		expected(token_dex[T_NAME]);
		return "";
	}
	
	name_sz = yyleng+1;
	
	name = (char*)malloc(name_sz); // +1 for '\0'
	assert(name);
	
	assert(strlcpy(name, yytext, name_sz) < name_sz);
	getNext();
	
	return name;
}

static inline Object_pt get_string_const(void){
	Object * o;
	Object_pt pt;
	
	if(token != T_STR) {
		expected(token_dex[T_STR]);
		return NIL;
	}
	
	o = (Object*)malloc(sizeof(Object)+yyleng+1); // +1 for '\0'
	assert(o);
	
	assert(strlcpy((char*)o->data, yytext, yyleng) < yyleng+1);
	getNext();
	
	
	o->size = yyleng+1;
	o->type = sym_search(syms, "String");
	pt = insertObject(o);
	
	if(o->type == NIL)
		sym_import(syms, "String", pt, 0);
	else
		incRefsTo(o->type);
	
	
	return pt;
}

static inline Object_pt get_integer_const(void){
	
}


/******************************* BYTECODE BUFFER ******************************/

static byteBuffer * initBuffer(void){
	byteBuffer * b;
	
	b = malloc(sizeof(byteBuffer) + BUF_SZ * sizeof(uint8_t));
	if(!b){
		error("initBuffer(): out of memory.\n");
		exit(EXIT_FAILURE);
	}
	
	b->size = BUF_SZ;
	b->ip   = 0;
	return b;
}

static inline void growBuffer(byteBuffer * b){
	b->size <<=1;
	b = realloc(b, b->size);
	if(!b){
		error("growBuffer(): out of memory.\n");
		exit(EXIT_FAILURE);
	}
}

static inline void addByte(byteBuffer *buffer, uint8_t byte){
	buffer->bytes[buffer->ip] = byte;
	buffer->ip++;
	
	if(buffer->ip == buffer->size) growBuffer(buffer);
}


/******************************************************************************/
//                            INSTRUCTION MACROS
/******************************************************************************/


static void integerByte(byteBuffer *buf, umax max){
	if(token != T_INT){
		expected("an integer");
		return;
	}
	
	if(yyval < 0){
		parse_error("sent arguments cannot be negative\n");
		errors++;
		return;
	}
	
	if((unsigned)yyval > max){
		parse_error("integer too great\n");
		error("found %d; maximum value is %u\n", yyval, max);
		errors++;
		return;
	}
	
	addByte(buf, (uint8_t)yyval);
	getNext();
}


static void constByte(byteBuffer *buf, DS constants){
	Object_pt constant;
	uint      constPos;
	
	constPos = DS_count(constants);
	if(constPos > constCountMask){
		parse_error("too many constants in method\n");
		errors++;
		return;
	}
	addByte(buf, (uint8_t)constPos);
	
	
	if     (token == T_STR) constant = get_string_const ();
	else if(token == T_INT) constant = get_integer_const();
	else{
		expected("a constant");
		errors++;
		return;
	}
	
	DS_nq(constants, &constant);
}


/******************************************************************************/
//                             PARSE FUNCTIONS
/******************************************************************************/


static void instructions(methodData mdata){
	trace("instructions(): start\n");
	
	while(token != T_CBRC && !errors){
		if(token == T_NL){ // empty instructions
			getNext();
			continue;
		}
		
		switch(token){
		case T_SEND:
			getNext();
			addByte(mdata.buf, b_send);
			integerByte(mdata.buf, argCountMask);
			constByte(mdata.buf, mdata.constants);
			match_token(T_NL);
			break;
		
		case T_SENDS:
			getNext();
			addByte(mdata.buf, b_send_super);
			integerByte(mdata.buf, argCountMask);
			constByte(mdata.buf, mdata.constants);
			match_token(T_NL);
			break;
		
		case T_PUSHA:
			getNext();
			addByte(mdata.buf, b_push_arg);
			integerByte(mdata.buf,mdata.argCount-1);
			match_token(T_NL);
			break;
		
		case T_PUSHC:
			getNext();
			addByte(mdata.buf, b_push_const);
			constByte(mdata.buf, mdata.constants);
			match_token(T_NL);
			break;
		
		case T_PUSHR:
			getNext();
			addByte(mdata.buf,b_push_rec);
			match_token(T_NL);
			break;
		
		case T_PUSHT:
			getNext();
			addByte(mdata.buf,b_push_temp);
			integerByte(mdata.buf, mdata.tempCount-1);
			match_token(T_NL);
			break;
		
		case T_PUSHX:
			getNext();
			addByte(mdata.buf,b_push_active);
			match_token(T_NL);
			break;
		
		case T_POP:
			getNext();
			addByte(mdata.buf,b_pop);
			match_token(T_NL);
			break;
		
		case T_STT:
			getNext();
			addByte(mdata.buf,b_store_temp);
			integerByte(mdata.buf,mdata.tempCount-1);
			match_token(T_NL);
			break;
		
		// return instructions
		case T_RETS:
			getNext();
			addByte(mdata.buf,b_return);
			match_token(T_NL);
			break;
		
		// jump instructions
		case T_JMP:
		case T_JT :
		case T_JF : //jumpInstruction(); break;
		
		
		
		
		// arithmetic message instructions
		case T_ADD:
		case T_SUB:
		case T_MUL:
		case T_DIV:
		case T_LSH:
		case T_RSH:
		case T_SHA:
		case T_ROL:
		case T_ROR:
		case T_AND:
		case T_OR :
		case T_XOR:
		case T_NOT:
		case T_LT :
		case T_GT :
		case T_LTE:
		case T_GTE:
		case T_EQ :
		case T_NEQ:
		
		// special message instructions
		case T_TYPE:
		case T_SIZE:
		case T_COPY:
		case T_NEW :
		case T_GET :
		case T_SET :
		
		
		
		// errors
		case T_INT :
		case T_STR :
		case T_NAME:
		case T_OBRC:
		case T_CBRC:
		case T_OTYPE:
		case T_CTYPE:
		case T_OPAR:
		case T_CPAR:
		case T_IS:   // declares initial value
		case T_METH: // indicates begining of methods section
		case T_PRIM:
		case T_TEMP:
		case T_EOF : // yyterminate returns a 0 automatically
		case T_NL  :
		case NUM_TOKENS:
		default: expected("instruction");
		}
	}
	
	trace("instructions(): stop\n");
}

static uint8_t getPrimative(void){
	switch(token){
		case T_ADD : getNext(); return p_add;
		case T_SUB : getNext(); return p_sub;
		case T_MUL : getNext(); return p_mul;
		case T_DIV : getNext(); return p_div;
	
		case T_LSH : getNext(); return p_lsh;
		case T_RSH : getNext(); return p_rsh;
		case T_SHA : getNext(); return p_sha;
		case T_ROL : getNext(); return p_rol;
		case T_ROR : getNext(); return p_ror;
	
		case T_AND : getNext(); return p_and;
		case T_OR  : getNext(); return p_or ;
		case T_XOR : getNext(); return p_xor;
		case T_NOT : getNext(); return p_not;
	
		case T_LT  : getNext(); return p_lt ;
		case T_GT  : getNext(); return p_gt;
		case T_LTE : getNext(); return p_lte;
		case T_GTE : getNext(); return p_gte;
		case T_EQ  : getNext(); return p_eq;
		case T_NEQ : getNext(); return p_neq;
	
		case T_TYPE: getNext(); return p_type;
		case T_SIZE: getNext(); return p_size;
		case T_NEW : getNext(); return p_new;
		case T_COPY: getNext(); return p_copy;
		case T_GET : getNext(); return p_get;
		case T_SET : getNext(); return p_set;
	
		// error
		case T_SEND:
		case T_SENDS:
		case T_PUSHA:
		case T_PUSHC:
		case T_PUSHR:
		case T_PUSHT:
		case T_PUSHX:
		case T_POP  :
		case T_STT  :
		case T_JMP:
		case T_JT :
		case T_JF :
		case T_RETS:
		case T_INT :
		case T_STR :
		case T_NAME:
		case T_OBRC:
		case T_CBRC:
		case T_OTYPE:
		case T_CTYPE:
		case T_OPAR:
		case T_CPAR:
		case T_IS:
		case T_METH:
		case T_PRIM:
		case T_TEMP:
		case T_EOF :
		case T_NL  :
		case NUM_TOKENS:
		default: expected("primative method");
	}
	errors++;
	getNext();
	return 0;
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


static uint argList(void){
	uint cnt=0;
	
	trace("argList(): start\n");
	match_token(T_OPAR);
	
	while(token != T_CPAR && !errors){
		if(token==T_NL){
			getNext();
			continue;
		}
		if(token==T_OPAR) staticType();
		match_token(T_NAME);
		cnt++;
	}
	
	match_token(T_CPAR);
	trace("argList(): stop\n");
	return cnt;
}



static void Method(void){
	const char * method_name;
	methodData mdata;
	Object_pt methodHeader=0;
	
	// Initialization
	mdata.constants = DS_new_list(sizeof(Object_pt));
	mdata.buf       = initBuffer();
	mdata.tempCount = 0;
	mdata.primativeIndex = p_none;
	
	
	// method return type
	if(token == T_OPAR) staticType();
	method_name = get_name();
	
	#ifdef DEBUG
		debug("%d: method named '%s'\n", yylineno, method_name);
	#endif
	
	// arguments
	mdata.argCount    = argList();
	match_token(T_OBRC);
	
	
	// primative declaration
	while(token == T_NL) getNext();
	if(token == T_PRIM){
		getNext();
		mdata.primativeIndex = getPrimative();
	}
	
	
	// temp declarations
	while(token == T_TEMP){
		getNext();
		if(token == T_OPAR) staticType();
		match_token(T_NAME);
		match_token(T_NL);
		mdata.tempCount++;
	}
	
	
	instructions(mdata);
	match_token(T_CBRC);
	
	mdata.constCount = DS_count(mdata.constants);
	
	// Build methodHeader
	if(mdata.tempCount > tempCountMask){
		parse_error("method has too many temps\n");
		errors++;
	}
	
	if(mdata.argCount > argCountMask){
		parse_error("method has too many args\n");
		errors++;
	}
	
	if(mdata.constCount > constCountMask){
		parse_error("method has too many constants\n");
		errors++;
	}
	
	#ifdef DEBUG
		// report findings
		debug("tempCount     : %u\n", mdata.tempCount);
		debug("argCount      : %u\n", mdata.argCount);
		debug("constCount    : %u\n", mdata.constCount);
		debug("primativeIndex: %u\n", mdata.primativeIndex);
	#endif
	
	methodHeader|=(mdata.     tempCount&     tempCountMask)<<     tempCountShift;
	methodHeader|=(mdata.      argCount&      argCountMask)<<      argCountShift;
	methodHeader|=(mdata.    constCount&    constCountMask)<<    constCountShift;
	methodHeader|=(mdata.primativeIndex&primativeIndexMask)<<primativeIndexShift;
	
}


static void Methods(void){
	trace("Methods(): start\n");
	
	while(token != T_CBRC && !errors){
		if(token == T_NL){ // empty declaration
			getNext();
			continue;
		}
		
		
		Method();
	}
	
	if(errors) debug("Methods(): errors detected\n");
	trace("Methods(): stop\n");
}


static Object * Fields(void){
	const char * field_name;
	umax field_cnt;
	Object * object;
	
	trace("Fields(): start\n");
	
	
	while(token != T_METH && !errors){
		if(token == T_NL){ // empty declaration
			getNext();
			continue;
		}
		
		if(token == T_OPAR) staticType();
		
		
		field_name = get_name(); // match_token(T_NAME); // field name
		
		#ifdef DEBUG
			debug("adding field '%s'\n", field_name);
		#endif
		
		DS_nq(field_list, field_name);
		
		if(token == T_IS) fieldInit();
		match_token(T_NL);
	}
	
	if(errors) debug("Fields(): errors detected\n");
	
	field_cnt = DS_count(field_list);
	
	#ifdef DEBUG
		debug("found %u fields\n", field_cnt);
	#endif
	
	// create object
	object = (Object*)malloc(sizeof(Object) + (sizeof(Object_pt) * field_cnt));
	
	trace("Fields(): stop\n");
	return object;
}


return_t dynParser(DS symbol_table){
	const char * object_name, *object_type;
	Object * object;
	
	
	trace("objectParser(): start\n");
	
	field_list = DS_new_list(sizeof(char*));
	syms = symbol_table;
	
	getNext(); // initialize the token
	
	
	while(token == T_NL) getNext(); // eat leading whitespace
	
	object_type = get_name(); // match_token(T_NAME); // type
	object_name = get_name(); // match_token(T_NAME); // object name
	match_token(T_OBRC);
	match_token(T_NL);
	
	#ifdef DEBUG
		debug("found object of class '%s' named '%s'\n",
			object_type,
			object_name
		);
	#endif
	
	object = Fields();
	
	if(errors) return r_failure;
	
	
	
	// insert object into the object memory
	// export the object
	// import the object type
	
	match_token(T_METH);
	
	Methods();
	
	match_token(T_CBRC);
	
	DS_delete(field_list);
	
	#ifdef DEBUG
		if(!errors) debug("no errors detected\n");
	#endif
	
	trace("objectParser(): stop\n");
	return r_success;
}


