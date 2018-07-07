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


#ifndef _VM_H
#define _VM_H


/** Contains indexes and accessors of dynamic objects needed by the vm
 */

#include <ppd/object_mem.h>


/******************************************************************************/
//                                 METHOD
/******************************************************************************/


const umax headerIdx  = 0;
const umax constStart = 1;

/*
typedef struct CompiledMethodHeader{
	type
	padding
	
	umax largeContext  : 1;
	umax primativeSelf : 1;
	umax primativeField: 1;
	
	umax primativeIndex: 8;
	umax constants     : 6; // or field to be returned
	umax temps         : 5;
	umax args          : 5;
} CompiledMethodHeader;

*/

typedef enum byteCode{
/*	bytecode     ,  pattern args description */
	b_nop        , ///< B   X	do nothing
	
	/* Stack Bytecodes */
	b_push_field , ///< BB  X	push a receiver field
	b_push_temp  , ///< BB  X	push a temp
	b_push_const , ///< BB  X	push a constant
	b_push_rec   , ///< B   X	push the receiver
	b_push_active, ///< B   X	push the active context
	
	b_pop        , ///< B   X	pop
	
	b_store_field, ///< BB  X	store the stack top in a receiver field
	b_store_temp , ///< BB  X	store the stack top in a temp variable
	
	/* Return Bytecodes */
	b_return     , ///< B   X	stack top is return value
	
	/* Jump Bytecodes */
	b_jump       , ///< BB  X	unconditional jump
	b_jump_true  , ///< BB  X	pop and jump if stack top is true
	b_jump_false , ///< BB  X	pop and jump if stack top is false
	
	/* Send ByteCodes */
	b_send       , ///< BBB *	send const selector with args
	b_send_super , ///< BBB *	send const selector to superClass with args
	
	b_send_add   , ///< B   1	
	b_send_sub   , ///< B   1	
	b_send_mul   , ///< B   1	
	b_send_div   , ///< B   1	
	b_send_mod   , ///< B   1	
	b_send_shift , ///< B   1	
	b_send_rotate, ///< B   1	
	b_send_and   , ///< B   1	
	b_send_or    , ///< B   1	
	b_send_lt    , ///< B   1	
	b_send_gt    , ///< B   1	
	b_send_lte   , ///< B   1	
	b_send_gte   , ///< B   1	
	b_send_eq    , ///< B   1	
	b_send_neq   , ///< B   1	
	
	b_send_type  , ///< B   0	
	b_send_copy  , ///< B   0	
	b_send_new   , ///< B   0	
	b_send_newc  , ///< B   1	
	b_send_get   , ///< B   1	
	b_send_set   , ///< B   1	
	b_send_same  , ///< B   0	whether two pointers on stack top are the same
	b_NUM
} ByteCode;


/******************************************************************************/
//                                 CONTEXT
/******************************************************************************/


// MethodContext fields
const umax senderIdx   = 0;
const umax ipIdx       = 1;
const umax spIdx       = 2;
const umax methodIdx   = 3;
const umax receiverIdx = 5;
const umax tempStart   = 6;


// BlockContext fields
const umax callerIdx   = 0;
const umax argCountIdx = 3;
const umax ipInitIdx   = 4;
const umax homeIdx     = 5;


/******************************************************************************/
//                                  CLASS
/******************************************************************************/


/*
typedef struct instanceSpec{
	Object_pt type           : 4;
	Object_pt padding        : *;
	Object_pt indexable      : 1; // indexable fields present
	Object_pt fieldWidth     : 2; // 2^fieldWidth
	Object_pt fixedFieldCount: 13;
} instanceSpec;
 */

const uint8_t fixedFieldCountShift = 0;
const uint8_t fieldWidthShift      = 13;
const uint8_t indexableShift       = 15;
const umax    fixedFieldCountMask  = 0X1FFF;
const umax    fieldWidthMask       = 0X3;
const umax    indexableMask        = 0X1;

const umax superClassIdx        = 0;
const umax messageDictionaryIdx = 1;
const umax instanceSpecIdx      = 2;

static inline Object_pt instanceSpec(Object_pt typePointer){
	return fetchPointer(typePointer, instanceSpecIdx);
}

static inline umax fieldWidth(Object_pt instanceSpec){
	return (1<<((instanceSpec>>fieldWidthShift)&fieldWidthMask));
}


/******************************************************************************/
//                            METHOD DICTIONARY
/******************************************************************************/


const umax methodArrayIdx = 1;
const umax selectorStart  = 2;


#endif // _VM_H


