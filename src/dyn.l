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
#include <stdio.h>
#include <util/msg.h>


/// Report errors in the scanner
static inline void scan_error(const char * message){
	msg_print(NULL, V_ERROR, "scanner: %s, on line %d\n", message, yylineno);
}


%}


/* prevents flex from generating the default rule
 do not expect to call yywrap at the end of the file
 prevents the unput and input functions from being built
 */
%option nodefault noyywrap nounput noinput

/* keep track of the line number in yylineno
 turn on warnings
 have a start condition stack
 */
%option yylineno warn 

%x STRING
/*%s METHODS */

DIGIT  [0-9]
LETTER [A-Za-z_]
WHTSPC [ \t\v]
NL     (\r|\r\n|\n)
OP     [!@$%^&*/\-+=<>|~?]

HEX    0[xX]({DIGIT}|[a-fA-F])+

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


<INITIAL><<EOF>> {
	// if this file was an include, will return to
	// previous point. pops and delete's buffer. see <INCLUDE>
	yypop_buffer_state();
	// if we are at the end of the top-level buffer then we are done.
	if ( !YY_CURRENT_BUFFER ) yyterminate();
}


<STRING><<EOF>> {
	msg_print(NULL, V_ERROR, 
		"End of file encountered before closing quotation\n"
	);
}

 /******************************* COMMENTS ************************************/

#.*$ {} // line comment

 /******************************* DECLARATIONS ********************************/

:    return T_COLO;
	/*\<   return T_OTYPE;
	\>   return T_CTYPE; */
\{   return T_OBRC;
\}   return T_CBRC;
\(   return T_OPAR;
\)   return T_CPAR;


IS      return T_IS;
METHODS {
	//BEGIN(METHODS);
	return T_METH;
}

 /******************************* INSTRUCTIONS ********************************/

 /* arithmetic instructions */
ADD return T_ADD;
SUB return T_SUB;
MUL return T_MUL;
DIV return T_DIV;
MOD return T_MOD;
SL  return T_LSH;
SR  return T_RSH;
SRS return T_SHA;
ROL return T_ROL;
ROR return T_ROR;

AND return T_AND;
OR  return T_OR;
NOT return T_NOT;

LT  return T_LT;
GT  return T_GT;
LTE return T_LTE;
GTE return T_GTE;
EQ  return T_EQ;
NEQ return T_NEQ;

 /* special instructions */
CLASS return T_CLSS;
COPY  return T_CPY;


PUSH  return T_PUSH;
PUSHR return T_PUSHR;
PUSHT return T_PUSHT;

POP  return T_POP;
SEND return T_SEND;

RETS return T_RETS;
RETR return T_RETR;
RETT return T_RETT;
RETF return T_RETF;
RETN return T_RETN;

 /************************** PRIMARY DATA TYPES *******************************/

{NAME} return T_NAME;
({OP})+ return T_NAME; // definable operations

{HEX}      return T_INT;

({DIGIT})+ return T_INT;

\"\" return T_STR; // the empty string FIXME

\"                   BEGIN(STRING);
<STRING>(\\\"|[^"])* return T_STR; // escaped quotes and non-quotes
<STRING>\"           BEGIN(INITIAL);


 /* This needs to be the last thing */
<*>.|\n msg_print(NULL, V_ERROR, "unknown token: %s\n", yytext);

%%




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


