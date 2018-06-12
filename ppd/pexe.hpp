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


#ifndef PEXE_HPP
#define PEXE_HPP


/******************************************************************************/
//                   FUNCTIONS FOR MANIPULATING PEXE FILES
/******************************************************************************/


/**	Load a pexe file into object memory
*/
void LoadPEXE(FILE * pexe_file);

/**	Generate a PEXE file from the object memory
*/
return_t GenPEXE(FILE * pexe_file);


#endif // PEXE_HPP


