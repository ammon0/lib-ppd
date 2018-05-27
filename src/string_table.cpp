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


#include <ppd/string_table.hpp>
#include <util/msg.h>
#include <stdlib.h>
#include <string.h>

#define START_SZ 1024 ///< Starting size for the dynamic name array

String_Table::String_Table(void){
	array = (char*)malloc(sizeof(char) * START_SZ);
	if(!array) {
		msg_print(NULL, V_ERROR, "Out of Memory");
		throw STRING_EXCEPTION_MEM;
	}
	
	next = 1;
	size = START_SZ;
	array[0]      = 0;
	array[size-1] = 0;
}

String_Table::~String_Table(void){
	free(array);
}

str_dx String_Table::add(const char * name){
	str_dx name_sz, temporary;
	
	name_sz = (str_dx)strlen(name)+1; // +1 for the null
	
	// resize the array if necessary
	while(size - next < name_sz){
		if (size > UINT_MAX>>1){ // TODO: check for one-off error
			msg_print(NULL, V_ERROR, "Name_array maxed out");
			throw STRING_EXCEPTION_MAX;
		}
		
		array = (char*) realloc(array, (size *= 2));
		if(!array){
			msg_print(NULL, V_ERROR, "Out of Memory");
			throw STRING_EXCEPTION_MEM;
		}
		array[size-1]=0; // ensure the string table ends with NULL
	}
	
	// Add name
	strncpy(array+next, name, name_sz);
	temporary = next;
	next += name_sz;
	return temporary;
}

const char * String_Table::get(const str_dx dx) const{
	return array+dx;
}

size_t String_Table::getSize(void) const{
	return next;
}


