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

#include <stdlib.h>


int main(int argc, char** argv){
	token_t token;
	
	
	msg_set_verbosity(V_TRACE);
	msg_print(NULL, V_DEBUG, "main():start\n");
	
	if(argc != 2){
		msg_print(NULL, V_ERROR, "testScanner takes one argument\n");
		exit(EXIT_FAILURE);
	}
	
	msg_print(NULL, V_INFO, "number of bytecodes: %d.\n", NUM_TOKENS);
	
	yyin = fopen(argv[1], "r");
	
	
	do {
		token = yylex();
		
		msg_print(NULL, V_TRACE, "%3u: %s from '%s'\n",
			yylineno,
			token_dex[token],
			(token!=T_NL)? yytext:""
		);
		
		
	} while(token != T_EOF);
	
	
	msg_print(NULL, V_DEBUG, "main():stop\n");
	return EXIT_SUCCESS;
}


