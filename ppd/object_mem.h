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


#ifndef OBJECT_MEM_H
#define OBJECT_MEM_H

#include <util/types.h>

typedef struct Object_pt Object_pt;
typedef size_t           Object_idx;


void initObjectMem(void);

/// Return the class of an object
Object_pt ClassOf(Object_pt object);
/// Return the byte size of an object
size_t SizeOf  (Object_pt object);
/// Return the number of fields in an object
Object_idx   FieldsOf(Object_pt object);


// add more of these as necessary
Object_pt FetchPt(Object_pt target, Object_idx index);
void      StorePt(Object_pt target, Object_idx index, Object_pt value);

uint8_t FetchByte(Object_pt target, Object_idx index);
void    StoreByte(Object_pt target, Object_idx index, uint8_t value);

void IncRefsTo(Object_pt object);
void DecRefsTo(Object_pt object);



Object_pt NewObject(Object_pt clss, Object_idx pointers);

Object_pt FirstInst(Object_pt class_object);
Object_pt NextInst (Object_pt object);

//void SwapPointers(Object_pt first, Object_pt second);


uint64_t U64ValOf (Object_pt object);
uint32_t U32ValOf (Object_pt object);
uint16_t U16ValOf (Object_pt object);
uint8_t  U8ValOf  (Object_pt object);
uint     UintValOf(Object_pt object);
umax     UmaxValOf(Object_pt object);
int64_t  S64ValOf (Object_pt object);
int32_t  S32ValOf (Object_pt object);
int16_t  S16ValOf (Object_pt object);
int8_t   S8ValOf  (Object_pt object);
int      SintValOf(Object_pt object);
imax     SmaxValOf(Object_pt object);
double   F64ValOf (Object_pt object);
float    F32ValOf (Object_pt object);


Object_pt U64ObjOf (uint64_t num);
Object_pt U32ObjOf (uint32_t num);
Object_pt U16ObjOf (uint16_t num);
Object_pt U8ObjOf  (uint8_t  num);
Object_pt UintObjOf(uint     num);
Object_pt UmaxObjOf(umax     num);
Object_pt S64ObjOf (int64_t  num);
Object_pt S32ObjOf (int32_t  num);
Object_pt S16ObjOf (int16_t  num);
Object_pt S8ObjOf  (int8_t   num);
Object_pt SintObjOf(int      num);
Object_pt SmaxObjOf(imax     num);
Object_pt F64ObjOf (double   num);
Object_pt F32ObjOf (float    num);


#endif // OBJECT_MEM_H


