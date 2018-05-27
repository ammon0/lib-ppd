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



/*	I could remove 64-bit values from the set of primatives and use the upper
	bits for identification.
 */

typedef union{
	struct Object_table_entry *pv_r;
	uint64_t                   pv_u64;
	uint32_t                   pv_u32;
	uint16_t                   pv_u16;
	uint8_t                    pv_u8;
	uint                       pv_uint;
	umax                       pv_umax;
	int64_t                    pv_s64;
	int32_t                    pv_s32;
	int16_t                    pv_s16;
	int8_t                     pv_s8;
	int                        pv_sint;
	imax                       pv_smax;
	float                      pv_f32;
	double                     pv_f64;
} PrimativeValue;

typedef enum{
	pc_r,
	pc_u64,
	pc_u32,
	pc_u16,
	pc_u8,
	pc_uint,
	pc_umax,
	pc_s64,
	pc_s32,
	pc_s16,
	pc_s8,
	pc_sint,
	pc_smax,
	pc_f32,
	pc_f64
} PrimativeClass;

typedef struct Object_pt{
	PrimativeValue v;
	PrimativeClass c;
} Object_pt;

typedef struct Object{
	Object_idx size; // field count
	Object_pt  clss;
	Object_pt  fields[];
} Object;

typedef struct Object_byte{
	Object_idx size; // field count
	Object_pt  clss;
	uint8_t    fields[];
} Object_byte;

typedef struct Object_table_entry{
	void * location;
	uint count; // reference count
	bool free; // whether this entry is free for use
} Object_table_entry;


/******************************************************************************/
//                                CONSTANTS
/******************************************************************************/


#define OBJ_TAB_ENTRIES ((uint)1<<16)
#define HEADER_SIZE     ((Object_idx)sizeof(Object))


/******************************************************************************/
//                                GLOBALS
/******************************************************************************/


static Object_table_entry object_table[OBJ_TAB_ENTRIES];

typedef enum{
	e_nil,
	e_Class,
	e_Object,
	e_Context,
	e_CompiledMethod,
	e_StaticString,
	e_CNT
} e_class;

static Object_pt Essential_classes[e_CNT] = {{ .v.pv_r=0, .c=pc_r }};

static Object_pt nil  = { .v.pv_r=0, .c=pc_r };

// the rest will have to be initialized when loading a memory image
static Object_pt U64  = { .v.pv_r=0, .c=pc_r };
static Object_pt U32  = { .v.pv_r=0, .c=pc_r };
static Object_pt U16  = { .v.pv_r=0, .c=pc_r };
static Object_pt U8   = { .v.pv_r=0, .c=pc_r };
static Object_pt Uint = { .v.pv_r=0, .c=pc_r };
static Object_pt Umax = { .v.pv_r=0, .c=pc_r };
static Object_pt S64  = { .v.pv_r=0, .c=pc_r };
static Object_pt S32  = { .v.pv_r=0, .c=pc_r };
static Object_pt S16  = { .v.pv_r=0, .c=pc_r };
static Object_pt S8   = { .v.pv_r=0, .c=pc_r };
static Object_pt Sint = { .v.pv_r=0, .c=pc_r };
static Object_pt Smax = { .v.pv_r=0, .c=pc_r };
static Object_pt F64  = { .v.pv_r=0, .c=pc_r };
static Object_pt F32  = { .v.pv_r=0, .c=pc_r };

static Object_table_entry *freePointerList;


/******************************************************************************/
//                           OBJECT TABLE ACCESS
/******************************************************************************/


static inline Object * getObject(Object_pt object){
	return (Object*) object.v.pv_r->location;
}


/******************************************************************************/
//                                 HELPERS
/******************************************************************************/


static inline bool isRef(Object_pt object){ return (object.c==pc_r); }

static inline void mustBeRef(Object_pt object){
	if(!isRef(object)) msg_print(NULL, V_ERROR, "mustBeRef(): not a ref.\n");
}

static inline void addToFreeList(Object_pt object){
	mustBeRef(object);
	object.v.pv_r->location = freePointerList;
	object.v.pv_r->free     = true;
	freePointerList         = object.v.pv_r;
}

static inline Object_pt removeFromFreeList(void){
	Object_pt objectPointer;
	
	if(freePointerList == NULL) return nil;
	
	objectPointer.c       = pc_r;
	objectPointer.v.pv_r  = freePointerList;
	freePointerList->free = false;
	freePointerList       = (Object_table_entry*) freePointerList->location;
	return objectPointer;
}

static inline void deallocate(Object_pt object){
	free(getObject(object));
	addToFreeList(object);
}

static inline Object_pt allocate(size_t size){
	void     *object;
	Object_pt pointer;
	
	object = malloc(size);
	if(!object){
		msg_print(NULL, V_ERROR, "allocate(): out of memory.\n");
		return nil;
	}
	
	pointer = removeFromFreeList();
	if(pointer.v.pv_r == NULL){
		msg_print(NULL, V_ERROR, "allocate(): object table full.\n");
		free(object);
		return nil;
	}
	
	pointer.v.pv_r->location = object;
	return pointer;
}


/******************************************************************************/
//                            PUBLIC INTERFACE
/******************************************************************************/


void initObjectMem(){
	for(uint i = OBJ_TAB_ENTRIES-1; i>0; i--){
		object_table[i].count = 0;
		object_table[i].free = false;
		object_table[i].location = &object_table[i+1];
	}
	
	object_table[OBJ_TAB_ENTRIES-1].location = NULL;
	freePointerList = &object_table[1];
}


Object_pt ClassOf(Object_pt object){
	switch(object.c){
	case pc_r   : return getObject(object)->clss;
	case pc_u64 : return U64;
	case pc_u32 : return U32;
	case pc_u16 : return U16;
	case pc_u8  : return U8;
	case pc_uint: return Uint;
	case pc_umax: return Umax;
	case pc_s64 : return S64;
	case pc_s32 : return S32;
	case pc_s16 : return S16;
	case pc_s8  : return S8;
	case pc_sint: return Sint;
	case pc_smax: return Smax;
	case pc_f32 : return F32;
	case pc_f64 : return F64;
	default:
		msg_print(NULL, V_ERROR, "ClassOf(): unknown class.\n");
		return nil;
	}
}

size_t SizeOf(Object_pt object){
	if(isRef(object))
		return getObject(object)->size * sizeof(Object_pt) + HEADER_SIZE;
	else return sizeof(Object_pt);
}

Object_idx FieldsOf(Object_pt object){
	mustBeRef(object);
	return getObject(object)->size + HEADER_SIZE;
}


Object_pt FetchPt(Object_pt object, Object_idx index){
	mustBeRef(object);
	// TODO: bounds check
	return getObject(object)->fields[index];
}

void StorePt(Object_pt object, Object_idx index, Object_pt value){
	mustBeRef(object);
	
	if(isRef(getObject(object)->fields[index]))
		DecRefsTo(getObject(object)->fields[index]);
	
	getObject(object)->fields[index]=value;
	value.v.pv_r->count++;
}

void IncRefsTo(Object_pt object){
	mustBeRef(object);
	object.v.pv_r->count++;
}

void DecRefsTo(Object_pt object){
	mustBeRef(object);
	if(object.v.pv_r->count <= 1) deallocate(object);
	else object.v.pv_r->count--;
}



Object_pt NewObject(Object_pt clss, Object_idx pointers){
	size_t    size;
	Object_pt object;
	
	size = pointers * sizeof(Object_pt) + HEADER_SIZE;
	object = allocate(size);
	
	getObject(object)->clss = clss;
	getObject(object)->size = size;
	
	object.v.pv_r->count    = 1;
	return object;
}

/****************************** PRIMITIVE TYPES *******************************/

uint64_t U64ValOf (Object_pt object){ return object.v.pv_u64 ; }
uint32_t U32ValOf (Object_pt object){ return object.v.pv_u32 ; }
uint16_t U16ValOf (Object_pt object){ return object.v.pv_u16 ; }
uint8_t  U8ValOf  (Object_pt object){ return object.v.pv_u8  ; }
uint     UintValOf(Object_pt object){ return object.v.pv_uint; }
umax     UmaxValOf(Object_pt object){ return object.v.pv_umax; }
int64_t  S64ValOf (Object_pt object){ return object.v.pv_s64 ; }
int32_t  S32ValOf (Object_pt object){ return object.v.pv_s32 ; }
int16_t  S16ValOf (Object_pt object){ return object.v.pv_s16 ; }
int8_t   S8ValOf  (Object_pt object){ return object.v.pv_s8  ; }
int      SintValOf(Object_pt object){ return object.v.pv_sint; }
imax     SmaxValOf(Object_pt object){ return object.v.pv_smax; }
double   F64ValOf (Object_pt object){ return object.v.pv_f64 ; }
float    F32ValOf (Object_pt object){ return object.v.pv_f32 ; }

Object_pt U64ObjOf (uint64_t num){
	Object_pt object = { .c=pc_u64, .v.pv_u64 = num};
	return object;
}
Object_pt U32ObjOf (uint32_t num){
	Object_pt object = { .c=pc_u32, .v.pv_u32 = num};
	return object;
}
Object_pt U16ObjOf (uint16_t num){
	Object_pt object = { .c=pc_u16, .v.pv_u16 = num};
	return object;
}
Object_pt U8ObjOf  (uint8_t  num){
	Object_pt object = { .c=pc_u8, .v.pv_u8 = num};
	return object;
}
Object_pt UintObjOf(uint     num){
	Object_pt object = { .c=pc_uint, .v.pv_uint = num};
	return object;
}
Object_pt UmaxObjOf(umax     num){
	Object_pt object = { .c=pc_umax, .v.pv_umax = num};
	return object;
}
Object_pt S64ObjOf (int64_t num){
	Object_pt object = { .c=pc_s64, .v.pv_s64 = num};
	return object;
}
Object_pt S32ObjOf (int32_t num){
	Object_pt object = { .c=pc_s32, .v.pv_s32 = num};
	return object;
}
Object_pt S16ObjOf (int16_t num){
	Object_pt object = { .c=pc_s16, .v.pv_s16 = num};
	return object;
}
Object_pt S8ObjOf  (int8_t  num){
	Object_pt object = { .c=pc_s8, .v.pv_s8 = num};
	return object;
}
Object_pt SintObjOf(int     num){
	Object_pt object = { .c=pc_sint, .v.pv_sint = num};
	return object;
}
Object_pt SmaxObjOf(imax     num){
	Object_pt object = { .c=pc_smax, .v.pv_smax = num};
	return object;
}
Object_pt F64ObjOf (double   num){
	Object_pt object = { .c=pc_f64, .v.pv_f64 = num};
	return object;
}
Object_pt F32ObjOf (float    num){
	Object_pt object = { .c=pc_f32, .v.pv_f32 = num};
	return object;
}



