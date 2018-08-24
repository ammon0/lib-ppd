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


/*	A symbol table for linking
 *
 *	An unresolved table entry indicates a needed fixup at the given location. There
 *	can be multiple fixups for any given name. Once resolved there will be a single
 *	record for the name indicating where it is defined.
 */

#ifndef _SYMS_H
#define _SYMS_H

#include <ppd/types.h>
//#include <ppd/string_table.h>
#include <util/data.h>
#include <stdlib.h>

#ifdef __cplusplus
	extern "C" {
#endif

typedef struct{
	Object_pt location;
	umax      index;
	char *    name;
	bool      resolved;
} symbol_record;

/// Initialize a symbol table
DS sym_init(void);

/// close a symboltable
void sym_close(DS table);

/**	Indicates to the loader that the given field (object/index) needs to fixedup to point to the object bound to name.
 */
void sym_import(DS table, const char * name, Object_pt object, umax index);

/**	Add the location of a known symbol to the table
 */
void sym_export(DS table, const char * name, Object_pt object);

/**	Search the symbol table for a needed object
 */
Object_pt sym_search(DS table, const char *name);

/**	Removes and returns the first unresolved symbol record in the table
 */
const symbol_record * sym_fixup(DS table);

char * sym_next(DS table);

void sym_print(const symbol_record *rec);


#ifdef __cplusplus
	}
#endif

#endif // _SYMS_H


