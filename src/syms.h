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
#include <ppd/string_table.hpp>


//#ifdef __cplusplus
//	extern "C" {
//#endif

typedef struct{
	Object_pt location;
	umax      index;
	str_dx    name;
	bool      resolved;
} symbol_record;

/// Initialize the symbol table
void sym_init(void);

/// close the symboltable
void sym_close(void);

/**	Indicates to the loader that the given field (object/index) needs to fixedup to point to the object bound to name.
 */
void sym_import(const char * name, Object_pt object, umax index);

/**	Add the location of a known symbol to the table
 */
void sym_export(const char * name, Object_pt object);

/**	Search the symbol table for a needed object
 */
Object_pt sym_search(const char *name);

/**	Removes and returns the first unresolved symbol record in the table
 */
symbol_record sym_fixup(void);


//#ifdef __cplusplus
//	}
//#endif

#endif // _SYMS_H


