/*******************************************************************************
 *
 *	Lib-ppd : Portable Program Data Library
 *
 *	Copyright (c) 2017 Ammon Dodson
 *	You should have received a copy of the license terms with this software. If
 *	not, please visit the project homepage at:
 *	https://github.com/ammon0/lib-ppd
 *
 ******************************************************************************/

#include <ppd/gen.hpp>

#include <util/msg.h>


FILE * out;

static inline int comment(const char * s){
	return fprintf(out, "\n;%s\n\n", s);
}


static void Gen_static(lbl_pt lbl){
	
}

static void Gen_routine(lbl_pt name, Routine * code){
	
}



void mpl(FILE * out_fd, PPD * prog){
	int test_int;
	lbl_pt lbl;
	
	msg_print(NULL, V_INFO, "mpl(): start\n");
	
	if(!out_fd){
		msg_print(NULL, V_ERROR, "mpl(): out_fd is NULL\n");
		throw;
	}
	
	out = out_fd;
	test_int = comment("This is an MPL source file");
	if(test_int < 0){
		msg_print(NULL, V_ERROR, "mpl(): out_fd is not writable.\n");
		throw;
	}
	
	
	
	/*********** DECLARE STRUCTURES ***********/
	
	
	/*********** DECLARE VISIBILITY ***********/
	
	
	/************* DECLARE STRINGS ************/
	
	comment("== Static Strings ==");
	
	lbl=dynamic_cast<lbl_pt>(prog->symbols.first());
	
	do{
		
	}while(( lbl=dynamic_cast<lbl_pt>(prog->symbols.next()) ));
	
	/************ STATIC VARIABLES ************/
	
	comment("== Static Variables ==");
	
	lbl=dynamic_cast<lbl_pt>(prog->symbols.first());
	
	do{
		if(lbl){
			if(lbl->get_def()->get_type() == st_routine) break;
			
			switch(lbl->get_mode()){
			case am_static_priv:
			case am_static_pub :
				Gen_static(lbl);
				break;
	
			// ignore
			case am_stack_aparam:
			case am_stack_fparam:
			case am_stack_auto:
			case am_temp  :
			case am_static_extern:
			case am_constant: break;
	
			//error
			case am_none:
			case am_NUM:
			default: throw;
			}
		}
	}while(( lbl=dynamic_cast<lbl_pt>(prog->symbols.next()) ));
	
	/************** PROGRAM CODE **************/
	
	comment("== Program Code ==");
	
	lbl=dynamic_cast<lbl_pt>(prog->symbols.first());
	
	do{
		if(lbl && lbl->get_def()->get_type() == st_routine)
			Gen_routine(
				lbl,
				dynamic_cast<Routine*>(lbl->get_def())
			);
	}while(( lbl=dynamic_cast<lbl_pt>(prog->symbols.next()) ));
	
	
	comment("== End ==");
	msg_print(NULL, V_INFO, "mpl(): stop\n");
}


