/*******************************************************************************
 *
 *	Lib-ppd : Portable Program Data Library
 *
 *	Copyright (c) 2017 Ammon Dodson
 *	You should have received a copy of the license terms with this software. If
 *	not, please visit the project homepage at:
 *	https://github.com/ammon0/lib-ppd
 *
 ******************************************************************************/

#include <stdio.h>

#include <ppd/sym.hpp>

const char * PPD_Integer::print(void)const{
	static char string[30] = "bytes 0 +00000000000000000000";
	
	switch(width){
	case w_byte : string[6] = '1'; break;
	case w_byte2: string[6] = '2'; break;
	case w_byte4: string[6] = '4'; break;
	case w_byte8: string[6] = '8'; break;
	case w_max  : string[6] = 'M'; break;
	case w_word : string[6] = 'W'; break;
	case w_ptr  : string[6] = 'P'; break;
	
	// error condition
	case w_NUM:
	case w_none:string[6] = 'X'; break;
	}
	
	return string;
}


