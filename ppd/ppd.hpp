/*******************************************************************************
 *
 *	MPL : Minimum Portable Language
 *
 *	Copyright (c) 2017 Ammon Dodson
 *	You should have received a copy of the license terms with this software. If
 *	not, please visit the project homepage at:
 *	https://github.com/ammon0/MPL
 *
 ******************************************************************************/

/**	@file ppd.hpp
 *	
 *	Bring all the Portable Program Data together.
 */


#ifndef _PPD_HPP
#define _PPD_HPP


#include <ppd/sym.hpp>


class PPDModule{
	
	
//	LLVMAddFunction (LLVMModuleRef M, const char *Name, LLVMTypeRef FunctionTy)
// 	Add a function to a module under a specified name. More...
// 
//LLVMValueRef 	LLVMGetNamedFunction (LLVMModuleRef M, const char *Name)
// 	Obtain a Function value from a Module by its name. More...
// 
//LLVMValueRef 	LLVMGetFirstFunction (LLVMModuleRef M)
// 	Obtain an iterator to the first Function in a Module. More...
// 
//LLVMValueRef 	LLVMGetLastFunction (LLVMModuleRef M)
// 	Obtain an iterator to the last Function in a Module. More...
// 
//LLVMValueRef 	LLVMGetNextFunction (LLVMValueRef Fn)
// 	Advance a Function iterator to the next Function. More...
// 
//LLVMValueRef 	LLVMGetPreviousFunction (LLVMValueRef Fn)
};


/******************************************************************************/
//                            THE SYMBOL INDEX
/******************************************************************************/


class Sym_index{
	DS index;
	
public:
	/****************************** CONSTRUCTOR *******************************/
	
	Sym_index(void);
	~Sym_index(void);
	
	/******************************* ACCESSOR *********************************/
	
	bool  isempty(void);
	
	sym_pt find (const char * name) const; ///< find an object by name
	sym_pt first(      void       ) const; ///< Returns the first object
	sym_pt next (      void       ) const; ///< Returns the next object
	
	/******************************* MUTATORS *********************************/
	
	sym_pt remove(const char   * name  ); ///< Remove an object by its name
	sym_pt add   (      sym_pt   object); ///< add a new object
	
};


/** This is a container for all the components of the portable program data.
*/
struct PPD{
	// Optimizations applied
	bool dead     =false; ///< indicates whether this PPD has passed opt_dead
	bool constants=false; ///< indicates whether this PPD has passed opt_const
	Sym_index symbols;
};


#endif // _PPD_HPP


