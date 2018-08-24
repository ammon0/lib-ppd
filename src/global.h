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

/**	@file global.h
 *	
 *	A place to declare all of the library globals
 */


/******************************************************************************/
//                             TYPE DEFINITIONS
/******************************************************************************/


#include <ppd/types.h>
#include <util/msg.h>


/******************************************************************************/
//                                 GLOBALS
/******************************************************************************/


#ifdef _GLOBAL_C
	#define EXTERN
#else
	#define EXTERN extern
#endif


EXTERN log_descriptor logfile;


#undef EXTERN


