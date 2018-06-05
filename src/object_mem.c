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
#include <util/msg.h>
#include <stdlib.h>


/******************************************************************************/
//                                 TYPES
/******************************************************************************/









// Can I do this with bitfields?



//typedef union{
//	struct Object_table_entry *pv_r;
////	uint64_t                   pv_u64;
//	uint32_t                   pv_u32;
//	uint16_t                   pv_u16;
//	uint8_t                    pv_u8;
//	uint                       pv_uint;
//	umax                       pv_umax;
////	int64_t                    pv_s64;
//	int32_t                    pv_s32;
//	int16_t                    pv_s16;
//	int8_t                     pv_s8;
//	int                        pv_sint;
//	imax                       pv_smax;
//	float                      pv_f32;
////	double                     pv_f64;
//} PrimativeValue;

typedef enum{
	pc_r=0,
//	pc_u64,
	pc_u32,
	pc_u16,
	pc_u8,
	pc_uint,
	pc_umax,
//	pc_s64,
	pc_s32,
	pc_s16,
	pc_s8,
	pc_sint,
	pc_smax,
	pc_f32,
//	pc_f64
} PrimativeType;

#define VAL_BIT_W ((sizeof(umax)-1)*8)



static inline PrimativeType
pointerType(Object_pt pointer){ return pointer>>VAL_BIT_W; }
static inline umax
pointerVal (Object_pt pointer){ return pointer&(~(((umax)0xff)<<VAL_BIT_W)); }

static inline Object_pt pointerSetType(Object_pt object, PrimativeType type){
	return pointerVal(object) | ((umax)type)<<VAL_BIT_W;
}

static inline Object_pt pointerSetVal(Object_pt object, umax val){
	if(val >= ((umax)1<<VAL_BIT_W)){
		msg_print(NULL, V_ERROR, "pointerSetVal(): val too large.\n");
		return null;
	}
	
	return pointerType(object) | val;
}

//typedef struct Object_pt{
//	umax c:8;
//	umax v:POINTER_SHIFT;
//}Object_pt;

//typedef struct Object_pt{
//	PrimativeClass c:8;
//	umax           v:((sizeof(umax)-1)*8);
//} Object_pt;


typedef struct Object{
	umax      count   ; // field count
	umax      size    ; // field size in bytes
	Object_pt type    ; // the class of the object
	Object_pt fields[]; // the object's data
} Object;

//typedef struct Object_byte{
//	Object_idx size; // field count
//	Object_pt  clss;
//	uint8_t    fields[];
//} Object_byte;

typedef struct Object_table_entry{
	Object *location;
	umax    count; // reference count
	bool    free;  // whether this entry is free for use
} Object_table_entry;

//typedef umax Object_table_idx;


/******************************************************************************/
//                                CONSTANTS
/******************************************************************************/


#define OBJ_TAB_ENTRIES ((umax)1<<16)
#define HEADER_SIZE     ((umax)sizeof(Object))


/******************************************************************************/
//                                GLOBALS
/******************************************************************************/


static Object_table_entry object_table[OBJ_TAB_ENTRIES];

static Object_table_entry * freeEntries;

typedef enum{
	e_Umax,
	e_Uint,
	e_U32,
	e_U16,
	e_U8,
	e_Smax,
	e_Sint,
	e_S32,
	e_S16,
	e_S8,
	e_F32,
	e_Class,
	e_Object,
	e_Context,
	e_CompiledMethod,
	e_StaticString,
	e_CNT
} e_class;

//static Object_pt Essential_classes[e_CNT];



// the rest will have to be initialized when loading a memory image
//static Object_pt U64  = { .v.pv_r=0, .c=pc_r };
//static Object_pt U32  = { .v.pv_r=0, .c=pc_r };
//static Object_pt U16  = { .v.pv_r=0, .c=pc_r };
//static Object_pt U8   = { .v.pv_r=0, .c=pc_r };
//static Object_pt Uint = { .v.pv_r=0, .c=pc_r };
//static Object_pt Umax = { .v.pv_r=0, .c=pc_r };
//static Object_pt S64  = { .v.pv_r=0, .c=pc_r };
//static Object_pt S32  = { .v.pv_r=0, .c=pc_r };
//static Object_pt S16  = { .v.pv_r=0, .c=pc_r };
//static Object_pt S8   = { .v.pv_r=0, .c=pc_r };
//static Object_pt Sint = { .v.pv_r=0, .c=pc_r };
//static Object_pt Smax = { .v.pv_r=0, .c=pc_r };
//static Object_pt F64  = { .v.pv_r=0, .c=pc_r };
//static Object_pt F32  = { .v.pv_r=0, .c=pc_r };


//static Object_pt U32  = { .v=0, .c=pc_r };
//static Object_pt U16  = { .v=0, .c=pc_r };
//static Object_pt U8   = { .v=0, .c=pc_r };
//static Object_pt Uint = { .v=0, .c=pc_r };
//static Object_pt Umax = { .v=0, .c=pc_r };

//static Object_pt S32  = { .v=0, .c=pc_r };
//static Object_pt S16  = { .v=0, .c=pc_r };
//static Object_pt S8   = { .v=0, .c=pc_r };
//static Object_pt Sint = { .v=0, .c=pc_r };
//static Object_pt Smax = { .v=0, .c=pc_r };

//static Object_pt F32  = { .v=0, .c=pc_r };


/******************************************************************************/
//                                 HELPERS
/******************************************************************************/


/**************************** OBJECT TABLE ACCESS *****************************/

static inline bool isRef(Object_pt object){
	return (pointerType(object)==pc_r);
}

static inline void mustBeRef(Object_pt object){
	if(!isRef(object)) msg_print(NULL, V_ERROR, "mustBeRef(): not a ref.\n");
}

static inline Object_table_entry * tableEntry(Object_pt object){
	umax value;
	
	mustBeRef(object);
	value = pointerVal(object);
	
	return (Object_table_entry*)value;
}

static inline Object * objectOf(Object_pt object){
	return tableEntry(object)->location;
}

/******************************* THE FREE LIST ********************************/

static inline void addToFreeList(Object_pt object){
	tableEntry(object)->free     = true;
	tableEntry(object)->count    = 0;
	tableEntry(object)->location = (Object*)freeEntries;
	freeEntries                  = tableEntry(object);
}

static inline Object_pt removeFromFreeList(void){
	Object_pt objectPointer;
	
	if(freeEntries == NULL) return null;
	
	objectPointer = pointerSetType(0, pc_r);
	objectPointer = pointerSetVal(objectPointer, (umax)freeEntries);
	//objectPointer.v   = (umax)freeEntries;
	freeEntries->free = false;
	freeEntries       = (Object_table_entry*) freeEntries->location;
	return objectPointer;
}

/************************** ALLOCATE AND DEALLOCATE ***************************/

static inline void deallocate(Object_pt object){
	free(objectOf(object));
	addToFreeList(object);
}

static inline Object_pt allocate(size_t size){
	Object    *object;
	Object_pt pointer;
	
	object = (Object*)malloc(size);
	if(!object){
		msg_print(NULL, V_ERROR, "allocate(): out of memory.\n");
		return null;
	}
	
	pointer = removeFromFreeList();
	if(pointer == null){
		msg_print(NULL, V_ERROR, "allocate(): object table full.\n");
		free(object);
		return null;
	}
	
	tableEntry(pointer)->location = object;
	return pointer;
}


/******************************************************************************/
//                            PUBLIC INTERFACE
/******************************************************************************/


void initObjectMem(){
	for(uint i = OBJ_TAB_ENTRIES-1; i>0; i--){
		object_table[i].count = 0;
		object_table[i].free = true;
		object_table[i].location = (Object*)&object_table[i+1];
	}
	
	object_table[OBJ_TAB_ENTRIES-1].location = NULL;
	freeEntries = &object_table[1];
}


Object_pt TypeOf(Object_pt pointer){
	switch(pointerType(pointer)){
	case pc_r   : return objectOf(pointer)->type;
	
	case pc_u32 : return pointerSetVal(0,(umax)&object_table[e_U32]);
	case pc_u16 : return pointerSetVal(0,(umax)&object_table[e_U16]);
	case pc_u8  : return pointerSetVal(0,(umax)&object_table[e_U8]);
	case pc_uint: return pointerSetVal(0,(umax)&object_table[e_Uint]);
	case pc_umax: return pointerSetVal(0,(umax)&object_table[e_Umax]);
	
	case pc_s32 : return pointerSetVal(0,(umax)&object_table[e_S32]);
	case pc_s16 : return pointerSetVal(0,(umax)&object_table[e_S16]);
	case pc_s8  : return pointerSetVal(0,(umax)&object_table[e_S8]);
	case pc_sint: return pointerSetVal(0,(umax)&object_table[e_Sint]);
	case pc_smax: return pointerSetVal(0,(umax)&object_table[e_Smax]);
	
	case pc_f32 : return pointerSetVal(0,(umax)&object_table[e_F32]);
	default:
		msg_print(NULL, V_ERROR, "ClassOf(): unknown class.\n");
		return null;
	}
}

Object_idx fieldCount(Object_pt object){
	return objectOf(object)->count;
}

size_t SizeOf(Object_pt object){
	if(isRef(object))
		return fieldCount(object) * objectOf(object)->size + HEADER_SIZE;
	else return sizeof(Object_pt);
}


Object_pt FetchPointer(Object_pt pointer, Object_idx index){
	Object_pt * fields;
	
	if(index >= objectOf(pointer)->count){
		msg_print(NULL, V_ERROR, "FetchField(): out of bounds.\n");
		return null;
	}
	
	fields = (Object_pt*)&(objectOf(pointer)->fields);
	
	return fields[index];
}

void StorePointer(Object_pt pointer, Object_idx index, Object_pt value){
	mustBeRef(pointer);
	
	if(isRef(objectOf(pointer)->fields[index]))
		DecRefsTo(objectOf(pointer)->fields[index]);
	
	objectOf(pointer)->fields[index]=value;
	IncRefsTo(value);
}

void IncRefsTo(Object_pt object){
	tableEntry(object)->count++;
	//object.v.pv_r->count++;
}

void DecRefsTo(Object_pt object){
	mustBeRef(object);
	if(tableEntry(object)->count <= 1) deallocate(object);
	else tableEntry(object)->count--;
}



Object_pt NewObject(Object_pt type, umax fieldCount, size_t fieldSize){
	size_t    size;
	Object_pt pointer;
	
	size = fieldCount * fieldSize + HEADER_SIZE;
	pointer = allocate(size);
	
	objectOf(pointer)->type = type;
	objectOf(pointer)->count = size;
	
	tableEntry(pointer)->count =1;
	return pointer;
}

/****************************** PRIMITIVE TYPES *******************************/

//uint64_t U64ValOf (Object_pt object){ return object.v.pv_u64 ; }
//uint32_t U32ValOf (Object_pt object){ return object.v.pv_u32 ; }
//uint16_t U16ValOf (Object_pt object){ return object.v.pv_u16 ; }
//uint8_t  U8ValOf  (Object_pt object){ return object.v.pv_u8  ; }
//uint     UintValOf(Object_pt object){ return object.v.pv_uint; }
//umax     UmaxValOf(Object_pt object){ return object.v.pv_umax; }
//int64_t  S64ValOf (Object_pt object){ return object.v.pv_s64 ; }
//int32_t  S32ValOf (Object_pt object){ return object.v.pv_s32 ; }
//int16_t  S16ValOf (Object_pt object){ return object.v.pv_s16 ; }
//int8_t   S8ValOf  (Object_pt object){ return object.v.pv_s8  ; }
//int      SintValOf(Object_pt object){ return object.v.pv_sint; }
//imax     SmaxValOf(Object_pt object){ return object.v.pv_smax; }
//double   F64ValOf (Object_pt object){ return object.v.pv_f64 ; }
//float    F32ValOf (Object_pt object){ return object.v.pv_f32 ; }


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

//Object_pt U32ObjOf (uint32_t num){
//	Object_pt object = { .c=pc_u32, .v.pv_u32 = num};
//	return object;
//}
//Object_pt U16ObjOf (uint16_t num){
//	Object_pt object = { .c=pc_u16, .v.pv_u16 = num};
//	return object;
//}
//Object_pt U8ObjOf  (uint8_t  num){
//	Object_pt object = { .c=pc_u8, .v.pv_u8 = num};
//	return object;
//}
//Object_pt UintObjOf(uint     num){
//	Object_pt object = { .c=pc_uint, .v.pv_uint = num};
//	return object;
//}
//Object_pt UmaxObjOf(umax     num){
//	Object_pt object = { .c=pc_umax, .v.pv_umax = num};
//	return object;
//}
////Object_pt S64ObjOf (int64_t num){
////	Object_pt object = { .c=pc_s64, .v.pv_s64 = num};
////	return object;
////}
//Object_pt S32ObjOf (int32_t num){
//	Object_pt object = { .c=pc_s32, .v.pv_s32 = num};
//	return object;
//}
//Object_pt S16ObjOf (int16_t num){
//	Object_pt object = { .c=pc_s16, .v.pv_s16 = num};
//	return object;
//}
//Object_pt S8ObjOf  (int8_t  num){
//	Object_pt object = { .c=pc_s8, .v.pv_s8 = num};
//	return object;
//}
//Object_pt SintObjOf(int     num){
//	Object_pt object = { .c=pc_sint, .v.pv_sint = num};
//	return object;
//}
//Object_pt SmaxObjOf(imax     num){
//	Object_pt object = { .c=pc_smax, .v.pv_smax = num};
//	return object;
//}
////Object_pt F64ObjOf (double   num){
////	Object_pt object = { .c=pc_f64, .v.pv_f64 = num};
////	return object;
////}
//Object_pt F32ObjOf (float    num){
//	Object_pt object = { .c=pc_f32, .v.pv_f32 = num};
//	return object;
//}



