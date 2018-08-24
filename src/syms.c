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


#include <ppd/syms.h>
#include <ppd/debug.h>

#include <util/string.h>
#include <stdlib.h>
#include <assert.h>



static inline const void * key(const void * r){
	return &((symbol_record*)r)->name;
}

static inline imax cmp_str(const void * left, const void * right){
	return strcmp( (char*)left, (char*)right );
}


static inline char * cpy_str(const char * string){
	char * temp;
	size_t size;
	
	size = strlen(string)+1; // +1 for the null
	
	temp = malloc(size);
	assert(temp);
/*	if(!temp){*/
/*		msg_print(NULL, V_ERROR, "cpy_str(): out of memory\n");*/
/*		exit(EXIT_FAILURE);*/
/*	}*/
	
	assert(strlcpy(temp, string, size) < size);
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
	
	msg_trace(NULL, "sym_import(): start\n");
	
	template.location = object;
	template.index    = index;
	template.name     = cpy_str(name);
	template.resolved = false;
	
/*	#ifdef DEBUG*/
/*		msg_print(NULL, V_DEBUG,*/
/*			"sym_import(): print symbol_record before insertion\n");*/
/*		sym_print(&template);*/
/*	#endif*/
	
	DS_insert(table, &template);
	
	#ifdef DEBUG
		msg_print(NULL, V_DEBUG,
			"sym_import(): print symbol_record after insertion\n");
		sym_print(DS_current(table));
	#endif
	
	msg_trace(NULL, "sym_import(): stop\n");
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
	const symbol_record *second;
	
	msg_trace(NULL, "sym_fixup(): start\n");
	
	result = DS_first(table);
	
	#ifdef DEBUG
		msg_print(NULL, V_DEBUG, "sym_fixup(): printing result of DS_first()\n");
		if(result) sym_print(result);
	#endif // DEBUG
	
	while (result && result->resolved){
		msg_trace(NULL, "sym_fixup(): calling DS_next()\n");
		result = DS_next(table);
		
		#ifdef DEBUG
			msg_print(NULL, V_DEBUG,
				"sym_fixup(): printing result of DS_next()\n");
			if(result) sym_print(result);
		#endif // DEBUG
	}
	
	#ifdef DEBUG
		msg_print(NULL, V_DEBUG, "sym_fixup(): printing result before return\n");
		if(result) sym_print(result);
	#endif // DEBUG
	
	second = DS_current(table);
	
	#ifdef DEBUG
		if(second != result)
			msg_print(NULL, V_ERROR, "sym_fixup(): return from DS_current() '%p' does\
 not equal previous position '%p'\n", second, result);
	#endif
	
	second = DS_remove(table);
	
	#ifdef DEBUG
		if(second != result)
			msg_print(NULL, V_ERROR, "sym_fixup(): return from DS_remove() '%p' does\
 not equal current position '%p'\n", second, result);
	#endif
	
	msg_trace(NULL, "sym_fixup(): stop\n");
	return second;
}


void sym_print(const symbol_record *rec){
	msg_print(NULL, V_DEBUG, "\n\t\tRecord name  : '%s'\n\t\tRecord status: %s\n\t\tObject_pt    : %p\n\t\tIndex        : %u\n",
		rec->name,
		rec->resolved? "resolved": "unresolved",
		rec->location,
		rec->index
	);
}


