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


#include "syms.hpp"

#include <util/data.h>


DS table;


static void * key(symbol_record r){
	return r.name;
}

void sym_init(void){
	
}


void sym_close(void){
	
}


void sym_import(const char * name, Object_pt object, umax index){
	
}


void sym_export(const char * name, Object_pt object){
	
}


Object_pt sym_search(const char *name){
	
}

symbol_record sym_fixup(void){
	
}


