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

#include "object_mem.h"

/** Contains definitions of data structures needed by the vm
 */


/******************************************************************************/
//                                 METHOD
/******************************************************************************/


const umax headerIdx  = 0;
const umax constStart = 1;


/******************************************************************************/
//                                 CONTEXT
/******************************************************************************/


// MethodContext fields
const umax senderIdx      = 0;
const umax ipIdx          = 1;
const umax spIdx          = 2;
const umax methodIdx      = 3;
const umax receiverIdx    = 5;
const umax tempFrameStart = 6;


// BlockContext fields
const umax callerIdx   = 0;
const umax argCountIdx = 3;
const umax ipInitIdx   = 4;
const umax homeIdx     = 5;


/******************************************************************************/
//                                  CLASS
/******************************************************************************/


const umax superClassIdx        = 0;
const umax messageDictionaryIdx = 1;
const umax instanceSpecIdx      = 2;

/******************************************************************************/
//                            METHOD DICTIONARY
/******************************************************************************/

const umax methodArrayIdx = 1;
const umax selectorStart  = 2;


#endif // _VM_H


