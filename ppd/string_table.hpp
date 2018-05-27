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


#ifndef _STRING_TABLE_HPP
#define _STRING_TABLE_HPP


#include <util/types.h>

typedef size_t str_dx; ///< indexes into the string_array
#define NULL_STRING ((str_dx) 0) ///< A null name


// Exceptions
#define STRING_EXCEPTION_MEM 0 ///< Out of memory exception
#define STRING_EXCEPTION_MAX 1 ///< Overflow Exception


class String_Table{
	char * array;
	str_dx size;
	str_dx next;
	
public:
	 String_Table(void);
	~String_Table(void);
	
	      str_dx add    (const char * name)      ;
	const char * get    (const str_dx dx  ) const;
	size_t       getSize(void             ) const;
};


#endif // _STRING_TABLE_HPP


