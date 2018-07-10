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


#ifndef _STRING_TABLE_H
#define _STRING_TABLE_H


#include <util/types.h>

typedef size_t str_dx; ///< indexes into the string_array
typedef struct s_table * stable_pt;
#define NULL_STRING ((str_dx) 0) ///< A null name


// Exceptions
#define STRING_EXCEPTION_MEM 0 ///< Out of memory exception
#define STRING_EXCEPTION_MAX 1 ///< Overflow Exception

/// pass NULL to initialize
str_dx       stable_add (stable_pt table, const char * string);
const char * stable_get (stable_pt table, const str_dx index );
size_t       stable_size(stable_pt table                     );


#endif // _STRING_TABLE_H


