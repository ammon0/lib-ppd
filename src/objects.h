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


/*

This file defines interfaces to the object memory that should not be exposed outside the library

*/

#ifndef _OBJECTS_H
#define _OBJECTS_H

#include <ppd/types.h>


#ifdef __cplusplus
	extern "C" {
#endif



// the C definition of an object
typedef struct Object{
	Object_pt type  ; // the class of the object
	umax      size  ; // bytes of data in object
	uint8_t   data[]; // the object's data
} Object;

const umax classFieldCnt = 7;
const umax classBytes    = classFieldCnt * sizeof(Object_pt);


// insert an arbitrary object into the object memory
Object_pt insert_object(Object * object);

#ifdef __cplusplus
	}
#endif

#endif // _OBJECTS_H

