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


#include <ppd/object_mem.h>
//#include <ppd/objects.h>
#include <ppd/debug.h>
#include <stdlib.h>


/******************************************************************************/
//                                 TYPES
/******************************************************************************/


/****************************** OBJECT POINTERS *******************************/

typedef enum PrimativeType{
	pc_r=0 ,
	pc_umax,
	pc_uint,
	pc_u64 ,
	pc_u32 ,
	pc_u16 ,
	pc_u8  ,
	pc_smax,
	pc_sint,
	pc_s64 ,
	pc_s32 ,
	pc_s16 ,
	pc_s8  ,
	pc_f64 ,
	pc_f32 ,
	pc_NUM // 15
} PrimativeType;

#define VAL_BIT_W (sizeof(umax)*8 - 4) // 4 bits for type
#define VAL_MASK  ((((umax)1)<<VAL_BIT_W)-1)

/********************************** OBJECTS ***********************************/

#include "objects.h"

/******************************** OBJECT TABLE ********************************/

typedef struct Object_table_entry{
	Object *location;
	umax    refs; // reference count
	bool    free; // whether this entry is free for use
} Object_table_entry;

/****************************** ESSENTIAL CLASSES *****************************/

typedef enum e_class{
	e_Umax,
	e_Uint,
	e_U64,
	e_U32,
	e_U16,
	e_U8,
	e_Smax,
	e_Sint,
	e_S64,
	e_S32,
	e_S16,
	e_S8,
	e_F64,
	e_F32,
	e_Class,
	e_Object,
	e_Context,
	e_CompiledMethod,
	e_String,
	e_CNT
} e_class;


/******************************************************************************/
//                                CONSTANTS
/******************************************************************************/


#define OBJ_TAB_ENTRIES ((umax)1<<16)
//#define HEADER_SIZE     (sizeof(Object))


/******************************************************************************/
//                                GLOBALS
/******************************************************************************/


static Object_table_entry   object_table[OBJ_TAB_ENTRIES];
static Object_table_entry * freeEntries;


/******************************************************************************/
//                                 HELPERS
/******************************************************************************/


/*************************** OBJECT POINTER ACCESS ****************************/

static inline PrimativeType
pointerType(Object_pt pointer){ return pointer>>VAL_BIT_W; }
static inline umax
pointerVal (Object_pt pointer){ return pointer & VAL_MASK; }

static inline Object_pt pointerSetType(Object_pt object, PrimativeType type){
	return pointerVal(object) | (((Object_pt)type)<<VAL_BIT_W);
}

static inline Object_pt pointerSetVal(Object_pt pointer, umax value){
	if(value > VAL_MASK){
		msg_print(NULL, V_ERROR, "pointerSetVal(): value %x too large.\n", value);
		return (Object_pt)NULL;
	}
	
	return value | (pointer & ~VAL_MASK);
}

/**************************** OBJECT TABLE ACCESS *****************************/

static inline Object_table_entry * tableEntry(Object_pt pointer){
	if(isRef(pointer)) return (Object_table_entry*)pointer;
	else{
		msg_print(NULL, V_ERROR,
			"tableEntry(): pointer %x is not a reference\n", pointer
		);
		return NULL; // fail hard
	} 
}

static inline Object * objectOf(Object_pt object){
	return tableEntry(object)->location;
}

/******************************* THE FREE LIST ********************************/

static inline void addToFreeList(Object_table_entry * entry){
	
	// clear entry
	entry->free     = true;
	entry->refs     = 0;
	
	// add to list
	entry->location = (Object*)freeEntries;
	freeEntries     = entry;
}

static inline Object_table_entry * getFromFreeList(Object * object){
	Object_table_entry * entry;
	
	if(freeEntries == NULL) return NULL;
	
	// get an entry from the list
	entry       = freeEntries;
	freeEntries = (Object_table_entry*) freeEntries->location;
	
	// initialize entry
	entry->free     = false;
	entry->refs     = 1;
	entry->location = object;
	return entry;
}

/************************** ALLOCATE AND DEALLOCATE ***************************/

static inline void deallocate(Object_pt pointer){
	free(objectOf(pointer));
	addToFreeList(tableEntry(pointer));
}

static inline Object_pt allocate(umax size){
	Object    *object;
	Object_table_entry * entry;
	
	object = (Object*)malloc(size + sizeof(Object));
	if(!object){
		msg_print(NULL, V_ERROR, "allocate(): out of memory.\n");
		return NIL;
	}
	
	entry = getFromFreeList(object);
	if(entry == NULL){
		msg_print(NULL, V_ERROR, "allocate(): object table full.\n");
		free(object);
		return NIL;
	}
	
	object->size = size;
	
	return (Object_pt)entry;
}


/******************************************************************************/
//                            PUBLIC INTERFACE
/******************************************************************************/


Object_pt insertObject(Object * object){
	Object_table_entry * entry;
	
	entry = getFromFreeList(object);
	if(entry == NULL){
		error("insertObject(): object table full or uninitialized.\n");
		return NIL;
	}
	
	return (Object_pt)entry;
}


/******************************************************************************/
//                            PUBLIC INTERFACE
/******************************************************************************/


void initObjectMem(){
	freeEntries = NULL;
	for(uint i = OBJ_TAB_ENTRIES-1; i>0; i--)
		addToFreeList(&object_table[i]);
}

/***************************** OBJECT PROPERTIES ******************************/

bool isRef(Object_pt pointer){
	return (pointerType(pointer)==pc_r);
}

Object_pt typeOf(Object_pt pointer){
	switch(pointerType(pointer)){
	case pc_r   : return objectOf(pointer)->type;
	
	case pc_umax: return pointerSetVal(0,(umax)&object_table[e_Umax]);
	case pc_uint: return pointerSetVal(0,(umax)&object_table[e_Uint]);
	case pc_u64 : return pointerSetVal(0,(umax)&object_table[e_U64 ]);
	case pc_u32 : return pointerSetVal(0,(umax)&object_table[e_U32 ]);
	case pc_u16 : return pointerSetVal(0,(umax)&object_table[e_U16 ]);
	case pc_u8  : return pointerSetVal(0,(umax)&object_table[e_U8  ]);
	
	case pc_smax: return pointerSetVal(0,(umax)&object_table[e_Smax]);
	case pc_sint: return pointerSetVal(0,(umax)&object_table[e_Sint]);
	case pc_s64 : return pointerSetVal(0,(umax)&object_table[e_S64 ]);
	case pc_s32 : return pointerSetVal(0,(umax)&object_table[e_S32 ]);
	case pc_s16 : return pointerSetVal(0,(umax)&object_table[e_S16 ]);
	case pc_s8  : return pointerSetVal(0,(umax)&object_table[e_S8  ]);
	
	case pc_f64 : return pointerSetVal(0,(umax)&object_table[e_F64]);
	case pc_f32 : return pointerSetVal(0,(umax)&object_table[e_F32]);
	
	case pc_NUM:
	default    :
		msg_print(NULL, V_ERROR, "typeOf(): unknown type.\n");
		return NIL;
	}
}

umax sizeOf(Object_pt object){
	if(isRef(object))
		return objectOf(object)->size;
	else return sizeof(Object_pt);
}

//umax fieldCount(Object_pt pointer){
//	if(isRef(pointer))
//		return objectOf(pointer)->size / objectOf(pointer)->width;
//	else return 0;
//}


/************************** INSTANTIATE NEW OBJECTS ***************************/

Object_pt newObject(Object_pt type, umax bytes){
	Object_pt pointer;
	
	pointer                  = allocate(bytes);
	objectOf(pointer)->type  = type;
	return pointer;
}

void incRefsTo(Object_pt pointer){
	if(isRef(pointer)) tableEntry(pointer)->refs++;
	else msg_print(NULL, V_ERROR, "incRefsTo(): %x not a ref.\n", pointer);
}

void decRefsTo(Object_pt pointer){
	if(isRef(pointer)){
		if(tableEntry(pointer)->refs <= 1) deallocate(pointer);
		else tableEntry(pointer)->refs--;
	}
	else msg_print(NULL, V_ERROR, "incRefsTo(): %x not a ref.\n", pointer);
}

/******************************** FIELD ACCESS ********************************/


static inline bool outOfBounds(Object_pt target, umax index, umax radix){
	return( index*radix > objectOf(target)->size );
}

static inline bool check(Object_pt target, umax index, umax radix){
	if(!isRef(target)){
		msg_print(NULL, V_ERROR,
			"check(): %x is not a reference.\n", target);
		return true;
	}
	
	if(outOfBounds(target,index,radix)){
		msg_print(NULL, V_ERROR,
			"check(): %x is out of bounds.\n", index);
		return true;
	}
	return false;
}

Object_pt fetchPointer(Object_pt target, umax index){
	Object_pt *data_pt;
	
	if(check(target, index, sizeof(Object_pt))) return NIL; // fail
	
	data_pt = (Object_pt*)objectOf(target)->data;
	return data_pt[index];
}

void storePointer(Object_pt target, umax index, Object_pt value){
	Object_pt *data_pt;
	
	if(check(target, index, sizeof(Object_pt))) return; // fail
	
	data_pt = (Object_pt*)objectOf(target)->data;
	
	if( isRef(data_pt[index]) ) decRefsTo(data_pt[index]);
	
	data_pt[index] = value;
	incRefsTo(value);
}

umax fetchUmax(Object_pt target, umax index){
	umax *data_pt;
	
	if(check(target, index, sizeof(umax))) return NIL;
	
	data_pt = (umax*)objectOf(target)->data;
	return data_pt[index];
}

void storeUmax(Object_pt target, umax index, umax value){
	Object_pt *data_pt;
	
	if(check(target, index, sizeof(umax))) return; // fail
	
	data_pt = (umax*)objectOf(target)->data;
	
	if( isRef(data_pt[index]) ) decRefsTo(data_pt[index]);
	
	data_pt[index] = value;
	incRefsTo(value);
}


/****************************** PRIMITIVE TYPES *******************************/

uint32_t U32ValOf (Object_pt object){ return (uint32_t)pointerVal(object); }
uint16_t U16ValOf (Object_pt object){ return (uint16_t)pointerVal(object); }
uint8_t  U8ValOf  (Object_pt object){ return  (uint8_t)pointerVal(object); }
uint     UintValOf(Object_pt object){ return     (uint)pointerVal(object); }
umax     UmaxValOf(Object_pt object){ return     (umax)pointerVal(object); }
int32_t  S32ValOf (Object_pt object){ return  (int32_t)pointerVal(object); }
int16_t  S16ValOf (Object_pt object){ return  (int16_t)pointerVal(object); }
int8_t   S8ValOf  (Object_pt object){ return   (int8_t)pointerVal(object); }
int      SintValOf(Object_pt object){ return      (int)pointerVal(object); }
imax     SmaxValOf(Object_pt object){ return     (imax)pointerVal(object); }
float    F32ValOf (Object_pt object){
	umax value;
	
	value = pointerVal(object);
	return    (float) value;
}


Object_pt UintObjOf (uint num){
	return pointerSetType(pointerSetVal(0,num),pc_uint);
}
Object_pt UmaxObjOf (umax num){
	return pointerSetType(pointerSetVal(0,num),pc_umax);
}
Object_pt U32ObjOf (uint32_t num){
	return pointerSetType(pointerSetVal(0,num),pc_u32);
}
Object_pt U16ObjOf (uint16_t num){
	return pointerSetType(pointerSetVal(0,num),pc_u16);
}
Object_pt U8ObjOf (uint8_t num){
	return pointerSetType(pointerSetVal(0,num),pc_u8);
}

Object_pt SintObjOf (int num){
	return pointerSetType(pointerSetVal(0,(umax)num),pc_sint);
}
Object_pt SmaxObjOf (imax num){
	return pointerSetType(pointerSetVal(0,(umax)num),pc_smax);
}
Object_pt S32ObjOf (int32_t num){
	return pointerSetType(pointerSetVal(0,(umax)num),pc_s32);
}
Object_pt S16ObjOf (int16_t num){
	return pointerSetType(pointerSetVal(0,(umax)num),pc_s16);
}
Object_pt S8ObjOf (int8_t num){
	return pointerSetType(pointerSetVal(0,(umax)num),pc_s8);
}

Object_pt F32ObjOf (float    num){
	return pointerSetType(pointerSetVal(0,(umax)num),pc_f32);
}


