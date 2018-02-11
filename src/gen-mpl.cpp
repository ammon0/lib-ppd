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


#include <ppd/mpl.hpp>
#include <util/msg.h>


FILE * out;

static inline int comment(const char * s){
	return fprintf(out, "\n;%s\n\n", s);
}





return_t PrintMPL(FILE * outfile, PPD_ModRef memory){
	
	
	msg_print(NULL, V_TRACE, "PrintMPL(): start\n");
	
	if(!outfile){
		msg_print(NULL, V_ERROR, "mpl(): out_fd is NULL\n");
		return failure;
	}
	
	
	out = outfile;
	if(comment("This is an MPL source file") < 0){
		msg_print(NULL, V_ERROR, "mpl(): out_fd is not writable.\n");
		return failure;
	}
	
	
	/*********** DECLARE STRUCTURES ***********/
	
	comment("== STRUCTURE OFFSETS ==");
	
	/*********** DECLARE VISIBILITY ***********/
	
	comment("== VISIBILITY ==");
	
	/************* READ ONLY DATA *************/
	
	comment("== STATIC READ ONLY DATA ==");
	
	/************* DECLARE STRINGS ************/
	
	comment("== READ ONLY STRINGS ==");
	
	/************ VARIABLE DATA ************/
	
	comment("== STATIC VARIABLE DATA ==");
	
	/************** PROGRAM CODE **************/
	
	comment("== PROGRAM CODE ==");
	
	
	
	comment("== END ==");
	msg_print(NULL, V_TRACE, "PrintMPL(): stop\n");
	
	return success;
}


