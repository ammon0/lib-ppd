


#include "dyn.h"

#include <util/msg.h>

#include <stdlib.h>


int main(int argc, char** argv){
	token_t token;
	
	
	msg_set_verbosity(V_TRACE);
	msg_print(NULL, V_TRACE, "main():start\n");
	
	argc = argc;
	
	yyin = fopen(argv[1], "r");
	
	
	do {
		token = yylex();
		
		
		printf("%s\t: at %u is '%s'\n",
			token_dex[token],
			yylineno,
			yytext
		);
		
		
	} while(token != T_EOF);
	
	
	msg_print(NULL, V_TRACE, "main():stop\n");
	return EXIT_SUCCESS;
}


