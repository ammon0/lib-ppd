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


#ifndef MPL_HPP
#define MPL_HPP

#include <ppd/ppd.hpp>
#include <util/types.h>
#include <stdio.h>


/**	Read an MPL text file and produce a PPD module
 *	Returns NULL if there are errors in the MPL file
*/
PPD_ModRef ReadMPL (FILE * infile);

/**	Print an MPL file from the PPD data present in memory
*/
return_t PrintMPL(FILE * outfile, PPD_ModRef memory);


#endif // MPL_HPP


