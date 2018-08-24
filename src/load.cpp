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


#include <ppd/object_mem.h>
#include <ppd/debug.h>
#include <ppd/syms.h>


#include <util/string.h>
#include <util/io.h>

#include <assert.h>



#define BUF_EXTRA 10
#define FILE_NAME_START 256


static void process_pexe(FILE * infile){
	
}

static void process_dyn(FILE * infile){
	
}


static Object_pt load(const char * top_level_object){
	char * file_name, *object_name;
	size_t object_name_sz, file_name_sz;
	FILE * file;
	DS symbol_table;
	
	trace("load(): start\n");
	
	
	file_name_sz = FILE_NAME_START;
	file_name    = (char*)malloc(file_name_sz);
	assert(file_name);
	
	object_name = (char*)top_level_object;
	
	symbol_table = sym_init();
	
	do{
		object_name_sz = strlen(object_name);
		
		if(object_name_sz + BUF_EXTRA > file_name_sz){
			file_name_sz = object_name_sz + BUF_EXTRA;
			free(file_name);
			file_name = (char*)malloc(file_name_sz);
			assert(file_name);
		}
		
		
		// check for a pexe file
		assert(strlcpy(file_name, object_name, file_name_sz) < file_name_sz);
		assert(strlcat(file_name, ".pexe", file_name_sz) < file_name_sz);
		
		file = fopen(file_name, "r");
		
		// process pexe file
		if(file) {
			process_pexe(file);
			continue;
		}
		
		// check for dyn file
		assert(strlcpy(file_name, object_name, file_name_sz) < file_name_sz);
		assert(strlcat(file_name, ".dyn", file_name_sz) < file_name_sz);
		
		file = fopen(file_name, "r");
		
		// process dyn file
		if(file){
			process_dyn(file);
			continue;
		}
		
		// file not found
		error("PPD: could not find a file for object '%s'\n", object_name);
		return NIL;
		
		
	} while(( object_name=sym_next(symbol_table) ));
	
	
	
	trace("load(): stop\n");
	return NIL;
}


