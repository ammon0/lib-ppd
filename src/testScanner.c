


#include "dyn.h"

#include <util/msg.h>

#include <stdlib.h>


int main(int argc, char** argv){
	token_t token;
	
	
	msg_set_verbosity(V_DEBUG);
	msg_print(NULL, V_DEBUG, "main():start\n");
	
	argc = argc;
	
	yyin = fopen(argv[1], "r");
	
	
	do {
		token = yylex();
		
		msg_print(NULL, V_TRACE, "%3u: %s is '%s'\n",
			yylineno,
			token_dex[token],
			(token!=T_NL)? yytext:""
		);
		
		
	} while(token != T_EOF);
	
	
	msg_print(NULL, V_DEBUG, "main():stop\n");
	return EXIT_SUCCESS;
}


