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

/**	@file ppd.h
 *	
 *	Bring all the Portable Program Data together.
 */


#ifndef _PPD_H
#define _PPD_H


#include <util/types.h>
#include <stdio.h>


/**	Find the named object on disk and load it and its dependencies into the object
 *	memory.
 *	
 *	This function searches the current directory for the named class in either dyn
 *	text files or pexe object files. It then loads that object into the object
 *	memory and adds its imports and exports to the global symbol table. It then
 *	recurses on each imported object.
 */
Object_pt load(const char * object_name);


/**	Generate a dyn text file from a pexe object file.
 */
RETURN pexe2dyn(FILE * pexe, FILE * dyn);

/**	Generate a pexe object file from a dyn text file.
 */
RETURN dyn2pexe(FILE * dyn, FILE * pexe);


#endif // _PPD_H


