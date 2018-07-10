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


#include "syms.h"

#include <util/msg.h>

#include <string.h>
#include <stdlib.h>



static inline const void * key(const void * r){
	return &((symbol_record*)r)->name;
}

static inline imax cmp_str(const void * left, const void * right){
	return strcmp( (char*)left, (char*)right );
}


static inline char * cpy_str(const char * string){
	char * temp;
	
	temp = malloc(strlen(string));
	if(!temp){
		msg_print(NULL, V_ERROR, "cpy_str(): out of memory\n");
		exit(EXIT_FAILURE);
	}
	
	strncpy(temp, string, strlen(string));
	return temp;
}



DS sym_init(void){
	//st = string_table;
	
	return DS_new_bst(
		sizeof(symbol_record),
		true,
		&key,
		&cmp_str
	);
}


void sym_close(DS table){
	DS_delete(table);
}


void sym_import(DS table, const char * name, Object_pt object, umax index){
	symbol_record template;
	
	template.location = object;
	template.index    = index;
	template.name     = cpy_str(name);
	template.resolved = false;
	
	DS_insert(table, &template);
}


void sym_export(DS table, const char * name, Object_pt object){
	symbol_record template;
	
	template.location = object;
	template.index    = 0;
	template.name     = cpy_str(name);
	template.resolved = true;
	
	DS_insert(table, &template);
}


Object_pt sym_search(DS table, const char *name){
	symbol_record * result;
	
	result = DS_find(table, name);
	if(!result || !result->resolved) return NIL;
	
	return result->location;
}


const symbol_record * sym_fixup(DS table){
	symbol_record * result;
	
	result = DS_first(table);
	
	while (result && result->resolved) DS_next(table);
	
	return DS_remove(table);
}


