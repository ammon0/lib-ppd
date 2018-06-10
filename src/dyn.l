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
 */
%option yylineno warn

%x STRING

DIGIT  [0-9]
HEX    0[xX]({DIGIT}|[a-fA-F])+
LETTER [A-Za-z_]
NAME   ({DIGIT}|{LETTER})*
WHTSPC [ \t\v]
NL     (\r|\r\n|\n)
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

({WHTSPC})*#+.*{NEWL} { return T_NL; } // line comment

 /*({WHTSPC})*#+.*{NEWL} {} // test*/

 /******************************* OPERATORS ***********************************/

\]   return T_CBRK;
\[   return T_OBRK;
\(   return T_OPAR;
\)   return T_CPAR;

:    return T_COLO;
\<   return T_OTYPE;
\>   return T_CTYPE;
\{   return T_OBRC;
\}   return T_CBRC;
METHODS { return T_METH; }


RETURN  { return T_RET; }

 /************************** PRIMARY DATA TYPES *******************************/

{NAME} return T_NAME;

{HEX}|({DIGIT})+ return T_INT;

\"\" return T_STR; // the empty string

\"                   BEGIN(STRING);
<STRING>(\\\"|[^"])* return T_STR; // escaped quotes and non-quotes
<STRING>\"           BEGIN(INITIAL);


 /* This needs to be the last thing */
<*>.|\n msg_print(NULL, V_ERROR, "unknown token: %s\n", yytext);

%%


