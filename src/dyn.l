%{

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

#define _DYN_L
#include "dyn.h"
#include <util/io.h>
#include <util/types.h>
#include <ppd/debug.h>


/// Report errors in the scanner
//static inline void scan_error(const char * message){
//	error("scanner: %s, on line %d\n", message, yylineno);
//}

imax yyval;

static void read_hex(void);
static void read_dec(void);

%}


/* prevents flex from generating the default rule
 do not expect to call yywrap at the end of the file
 prevents the unput and input functions from being built
 */
%option nodefault noyywrap nounput noinput

/* keep track of the line number in yylineno
 turn on warnings
 */
%option yylineno warn

%x STRING


DIGIT  [0-9]
LETTER [A-Za-z_]
HEX    [a-fA-F0-9]
WHTSPC [ \t\v]
NL     (\r|\r\n|\n)
OP     [!@$%^&*/\-+=<>|~?]


NAME   {LETTER}({DIGIT}|{LETTER})*

ESCNL  ({WHTSPC})*\\{NL}({WHTSPC})*

NEWL   ({WHTSPC})*({NL})


/******************************************************************************/
/*                                  RULES*/
/******************************************************************************/
%%


^({NEWL})    {              } // ignore empty lines
{NEWL}       { return T_NL; } // report the rest

{ESCNL}     {} // ignore escaped newlines
({WHTSPC})  {} // ignore all other whitespace


<INITIAL><<EOF>> yyterminate();


<STRING><<EOF>> {
	error("End of file encountered before closing quotation\n");
}

 /******************************* COMMENTS ************************************/

#.*$ {} // line comment

 /******************************* DECLARATIONS ********************************/

 /*:    return T_COLO;*/
	/*\<   return T_OTYPE;
	\>   return T_CTYPE; */
\{   return T_OBRC;
\}   return T_CBRC;
\(   return T_OPAR;
\)   return T_CPAR;


IS        return T_IS;
METHODS   return T_METH;
PRIMATIVE return T_PRIM;

TEMP return T_TEMP;

 /********************************* BYTECODES *********************************/

 /* send instructions */
SEND  return T_SEND;
SENDS return T_SENDS;

 /* stack instructions */
PUSHA return T_PUSHA; // push an argument
PUSHC return T_PUSHC; // push a constant onto the stack
PUSHT return T_PUSHT; // push a temp onto the stack
PUSHR return T_PUSHR; // push the receiver onto the stack
PUSHX return T_PUSHX; // push the active context

STORET return T_STT; // store stack top in temporary variable

POP   return T_POP;   // pop stack top

 /* jump bytecodes */
JMP return T_JMP; // unconditional relative jump
JT  return T_JT;  // relative jump on true
JF  return T_JF;  // relative jump on false

 /* return bytecodes */
RETS return T_RETS; // return the stack top
 /*
RETR return T_RETR; // return the receiver
RETC return T_RETC; // return a constant */

 /***************************** PRIMATIVE METHODS *****************************/

 /* arithmetic instructions */
ADD return T_ADD;
SUB return T_SUB;
MUL return T_MUL;
DIV return T_DIV;

SL  return T_LSH;
SR  return T_RSH;
SRS return T_SHA;
ROL return T_ROL;
ROR return T_ROR;

AND return T_AND;
OR  return T_OR;
XOR return T_XOR;
NOT return T_NOT;

LT  return T_LT;
GT  return T_GT;
LTE return T_LTE;
GTE return T_GTE;
EQ  return T_EQ;
NEQ return T_NEQ;

 /* special instructions */
TYPE return T_TYPE;
SIZE return T_SIZE;
COPY return T_COPY;
NEW  return T_NEW;
GET  return T_GET;
SET  return T_SET;

 /************************** PRIMARY DATA TYPES *******************************/

{NAME}  return T_NAME;
({OP})+ return T_NAME; // definable operations


-?0[xX]({HEX})+ read_hex(); return T_INT;
-?({DIGIT})+    read_dec(); return T_INT;

\"\" return T_STR; // the empty string FIXME

\"                   BEGIN(STRING);
<STRING>(\\\"|[^"])* return T_STR; // escaped quotes and non-quotes
<STRING>\"           BEGIN(INITIAL);


 /* This needs to be the last thing */
<*>.|\n error("unknown token: '%s'\n", yytext);

%%


static void read_hex(){
	int sign = 1;
	
	if(yytext[0] == '-') sign = -1;
	
	
}

static void read_dec(){
	int sign = 1;
	
	
}

//static umax read_num(const char * str) {
//	umax num=0;
//	const uint hexmax=16;
//	const uint decmax=19;
//	
//	/*umax can hold up to UMAX_MAX
//	2^64 = 10^x -> log_10 2^64 = x -> x=19.2659...
//	2^64 = 16^y -> log_16 2^64 = y -> y=16
//	*/
//	
//	// TODO: allow spaces in numbers
//	
//	// hex number
//	if(str[1] == 'x' || str[1] == 'X'){ 
//		if(yyleng > hexmax +2){ // overflow
//			msg_print(NULL, V_ERROR, "Number is too large");
//			return UMAX_MAX;
//		}
//		
//		for(uint i=2; i< yyleng; i++){
//			if (str[i]>= '0' && str[i]<='9')
//				num = (num << 4) | (str[i] - '0');
//			else if(str[i]>= 'A' && str[i]<='F')
//				num = (num << 4) | (str[i] - 'A' + 10);
//			else if(str[i]>= 'a' && str[i]<='f')
//				num = (num << 4) | (str[i] - 'a' + 10);
//			else msg_print(NULL, V_ERROR, "Internal: read_num(): hex");
//		}
//	}
//	
//	// decimal number
//	else {
//		if(yyleng > decmax){ // overflow
//			msg_print(NULL, V_ERROR, "Number is too large");
//			return UMAX_MAX;
//		}
//		
//		for(uint i=0; i< yyleng; i++){
//			num = (num * 10) + (str[i] - '0');
//		}
//	}
//	
//	return num;
//}


