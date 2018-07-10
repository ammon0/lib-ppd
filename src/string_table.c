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


#include <ppd/string_table.h>
#include <util/msg.h>
#include <stdlib.h>
#include <string.h>

#define START_SZ 1024 ///< Starting size for the dynamic name array

struct s_table{
	char * array;
	str_dx size;
	str_dx next;
};


/******************************************************************************/
//                             PUBLIC FUNCTIONS
/******************************************************************************/


str_dx stable_add(stable_pt table, const char * string){
	str_dx name_sz, temporary;
	
	// initialize the table if necessary
	if(!table){
		table = malloc(sizeof(struct s_table));
		if(!table) {
			msg_print(NULL, V_ERROR, "Out of Memory");
			exit(EXIT_FAILURE);
		}
		
		table->array = malloc(sizeof(char   ) * START_SZ);
		if(!table->array) {
			msg_print(NULL, V_ERROR, "Out of Memory");
			exit(EXIT_FAILURE);
		}
		
		table->array[0] = '\0'; // null string
		table->next = 1;
		table->size = START_SZ;
		
		table->array[table->size-1] = 0;
	}
	
	
	name_sz = strlen(string)+1; // +1 for the null
	
	// resize the array if necessary
	while(table->size - table->next < name_sz){
		if (table->size > UINT_MAX>>1){ // TODO: check for one-off error
			msg_print(NULL, V_ERROR, "Name_array maxed out");
			return 0;
		}
		
		table->array = (char*) realloc(table->array, (table->size <<= 1));
		if(!table->array){
			msg_print(NULL, V_ERROR, "Out of Memory");
			exit(EXIT_FAILURE);
		}
		table->array[table->size-1]=0; // ensure the string table ends with NULL
	}
	
	// Add name
	strncpy(table->array+table->next, string, name_sz);
	temporary = table->next;
	table->next += name_sz;
	return temporary;
	
}

const char * stable_get(stable_pt table, const str_dx index ){
	return table->array+index;
}

size_t       stable_size(stable_pt table){
	return table->next;
}


