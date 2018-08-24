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

#include <ppd/object_mem.h>
#include <util/msg.h>

#include <stdlib.h>


int main(int argc, char** argv){
	DS symbols;
	
	msg_set_verbosity(V_TRACE);
	msg_print(NULL, V_DEBUG, "main():start\n");
	
	if(argc != 2){
		msg_print(NULL, V_ERROR, "testScanner takes one argument\n");
		exit(EXIT_FAILURE);
	}
	
	initObjectMem();
	symbols = sym_init();
	
	//msg_print(NULL, V_INFO, "number of bytecodes: %d.\n", NUM_TOKENS);
	
	yyin = fopen(argv[1], "r");
	
	dynParser(symbols);
	
	sym_close(symbols);
	
	msg_print(NULL, V_DEBUG, "main():stop\n");
	return EXIT_SUCCESS;
}


