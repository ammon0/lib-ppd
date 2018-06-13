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
	
	msg_set_verbosity(V_TRACE);
	msg_print(NULL, V_DEBUG, "main():start\n");
	
	argc = argc; // prevent complaints
	
	//msg_print(NULL, V_INFO, "number of bytecodes: %d.\n", NUM_TOKENS);
	
	yyin = fopen(argv[1], "r");
	
	objectParser();
	
	msg_print(NULL, V_DEBUG, "main():stop\n");
	return EXIT_SUCCESS;
}

