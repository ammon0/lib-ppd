/*******************************************************************************
 *
 *	lib-ppd : Portable Program Data Library
 *
 *	Copyright (c) 2018 Ammon Dodson
 *	You should have received a copy of the license terms with this software. If
 *	not, please visit the project homepage at:
 *	https://github.com/ammon0/MPL
 *
 ******************************************************************************/


#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <util/types.h>
#include <string>

typedef struct _root     * DS;
typedef class  PPD_Label * lbl_pt;


/******************************************************************************/
//                          TYPE DEFINITIONS BASE CLASS
/******************************************************************************/


/** A base class for variable definitions
the code generator doesn't actually need to know anything about the structure of data. the only reason these exist is because the sizes are machine dependent. we use these to calculate the sizes of various offset symbols and that's it.
*/
class PPD_Definition{
	size_t size=0; // this size is dependent on the compilation target.
	
public:
	/****************************** CONSTRUCTOR *******************************/
	
	/******************************* ACCESSOR *********************************/
	
	/// returns the size in bytes of this defintion (if known)
	size_t get_size(void)const{ return size; }
	
	/******************************* MUTATORS *********************************/
	
	/// sets the size in bytes of this definition
	void set_size(size_t bytes){ size = bytes; }
};


typedef PPD_Definition * def_pt;


/******************************************************************************/
//                              INTEGER TYPE
/******************************************************************************/


/// data sizes for numerical operands
typedef enum width_t{
	w_none,
	w_byte,
	w_byte2,
	w_byte4,
	w_byte8,
	w_max,
	w_word,
	w_ptr,
	w_NUM
} width_t;

/// indicates if the integer is signed
typedef enum signedness_t{
	s_undef,
	s_signed,
	s_unsigned,
	s_NUM
} signedness_t;

/**	A PPD representation of an integer variable
 *	The way signedness is represented is machine dependent so the machine
 *	instructions for signed and unsigned arithmetic may be different.
*/
struct PPD_Integer: public PPD_Definition{
	width_t      width = w_none ;
	signedness_t signd = s_undef;
	umax         value          ;
	
public:
	/****************************** CONSTRUCTOR *******************************/
	
	/******************************* ACCESSOR *********************************/
	
	umax         get_value(void)const{ return value ; }
	/// get the integer width
	width_t      get_width(void)const{ return width ; }
	/// get the signedness of the integer
	signedness_t is_signed(void)const{ return signd ; }
	
	/******************************* MUTATORS *********************************/
	
	/// set the integer with a signed value
	void set_sval (imax i   ){ value = i; signd = s_signed  ; }
	/// set the integer with an unsigned value
	void set_uval (umax i   ){ value = i; signd = s_unsigned; }
	/// set the integer's width
	void set_width(width_t w){ width = w; }
};


/******************************************************************************/
//                                 ARRAY TYPE
/******************************************************************************/


/** A PPD representation of an array
*/
class PPD_Array: public PPD_Definition{
	def_pt chld;
	umax   cnt;
	
public:
	
	/****************************** CONSTRUCTOR *******************************/
	
	
	/******************************* ACCESSOR *********************************/
	
	/// returns a def_pt for the contents of each array cell
	def_pt get_child(void)const{ return chld   ; }
	const char * print(void) const{}
	
	/******************************* MUTATORS *********************************/
	
	void set_child(def_pt child){ chld = child; }
	void set_count(umax count){ cnt = count; }
};


/******************************************************************************/
//                            STRUCTURE TYPE
/******************************************************************************/


/**	A PPD representation of a structure
*/
class PPD_Structure: public PPD_Definition{
	DS fields;
	
public:
	
	
	/****************************** CONSTRUCTOR *******************************/
	
	PPD_Structure(void);
	~PPD_Structure(void);
	
	/******************************* ACCESSOR *********************************/
	
	bool isempty(void) const;
	umax count  (void) const;
	
	lbl_pt find (const char * name) const; ///< Find a label by name
	lbl_pt first(void             ) const; ///< get the first label
	lbl_pt next (void             ) const; ///< get the next label
	
	/******************************* MUTATORS *********************************/
	
	lbl_pt remove(const char * name  ); ///< Remove an object by its name
	lbl_pt add   (      lbl_pt object); ///< add a new object
	
};


#endif // DEFINITIONS_HPP



