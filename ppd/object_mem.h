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


#ifndef _OBJECT_MEM_H
#define _OBJECT_MEM_H

#include <util/types.h>

#ifdef __cplusplus
	extern "C" {
#endif

typedef umax Object_pt;

#define NIL ((Object_pt)0);


#define SZ_PT (sizeof(Object_pt))


void initObjectMem(void);

/***************************** OBJECT PROPERTIES ******************************/

bool isRef(Object_pt object);

Object_pt typeOf(Object_pt object);
/// Return the byte size of an object
umax sizeOf  (Object_pt object);
/// Return the number of fields in an object
umax fieldCount(Object_pt object);

/************************** INSTANTIATE NEW OBJECTS ***************************/

Object_pt newObject(Object_pt clss, umax bytes);
Object_pt copyObjecet(Object_pt object);

void incRefsTo(Object_pt object);
void decRefsTo(Object_pt object);

/******************************** FIELD ACCESS ********************************/

// add more of these as necessary
Object_pt fetchPointer(Object_pt target, umax index);
void      storePointer(Object_pt target, umax index, Object_pt value);

umax    fetchUmax(Object_pt target, umax index);
void    storeUmax(Object_pt target, umax index, umax value);

uint8_t fetchU8(Object_pt target, umax index);
void    storeU8(Object_pt target, umax index, uint8_t value);

/********************************* ENUMERATION ********************************/

Object_pt firstInst(Object_pt class_object);
Object_pt nextInst (Object_pt object);

/****************************** PRIMITIVE TYPES *******************************/

uint32_t U32ValOf (Object_pt object);
uint16_t U16ValOf (Object_pt object);
uint8_t  U8ValOf  (Object_pt object);
uint     UintValOf(Object_pt object);
umax     UmaxValOf(Object_pt object);
int32_t  S32ValOf (Object_pt object);
int16_t  S16ValOf (Object_pt object);
int8_t   S8ValOf  (Object_pt object);
int      SintValOf(Object_pt object);
imax     SmaxValOf(Object_pt object);
float    F32ValOf (Object_pt object);


Object_pt U32ObjOf (uint32_t num);
Object_pt U16ObjOf (uint16_t num);
Object_pt U8ObjOf  (uint8_t  num);
Object_pt UintObjOf(uint     num);
Object_pt UmaxObjOf(umax     num);
Object_pt S32ObjOf (int32_t  num);
Object_pt S16ObjOf (int16_t  num);
Object_pt S8ObjOf  (int8_t   num);
Object_pt SintObjOf(int      num);
Object_pt SmaxObjOf(imax     num);
Object_pt F32ObjOf (float    num);


#ifdef __cplusplus
	}
#endif

#endif // _OBJECT_MEM_H


