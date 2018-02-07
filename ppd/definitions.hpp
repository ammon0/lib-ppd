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
	
	size_t get_size(void)const{ return size; }
	
	/******************************* MUTATORS *********************************/
	
	void set_size(size_t bytes){ size = bytes; }
};


typedef PPD_Definition * def_pt;


/******************************************************************************/
//                             DATA TYPES
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

typedef enum signedness_t{
	s_undef,
	s_signed,
	s_unsigned,
	s_NUM
} signedness_t;

/* The way signedness is represented is machine dependent so the machine instructions for signed and unsigned arithmetic may be different.
*/


/**	A PPD representation of an integer variable
*/
struct PPD_Integer: public PPD_Definition{
	width_t      width = w_none ;
	signedness_t signd = s_undef;
	umax         value          ;
	
public:
	/****************************** CONSTRUCTOR *******************************/
	
	/******************************* ACCESSOR *********************************/
	
	umax         get_value(void)const{ return value ; }
	width_t      get_width(void)const{ return width ; }
	signedness_t is_signed(void)const{ return signd ; }
	//sym_t        get_type (void)const{ return st_int; }
	
	const char * print(void) const;
	
	/******************************* MUTATORS *********************************/
	
	
	
};


/** A PPD representation of an array
*/
class PPD_Array: public PPD_Definition{
	def_pt child;
	umax   count;
	
public:
	
	/****************************** CONSTRUCTOR *******************************/
	
	PPD_Array();
	
	/******************************* ACCESSOR *********************************/
	
	def_pt       get_child(void)const{ return child   ; }
	//sym_t        get_type (void)const{ return st_array; }
	const char * print(void) const{}
	
	/******************************* MUTATORS *********************************/
	
	
};


/**	A PPD representation of a structure
*/
class PPD_Structure: public PPD_Definition{
	DS fields;
	
public:
	
	
	/****************************** CONSTRUCTOR *******************************/
	
	PPD_Structure(void);
	~PPD_Structure(void);
	
	
	/******************************* ACCESSOR *********************************/
	
	bool isempty(void);
	umax count  (void);
	
	lbl_pt find   (const char * name) const;
	lbl_pt first  (void             ) const;
	lbl_pt next   (void             ) const;
	
	const char * print(void) const;
	
	//sym_t        get_type(void)const{ return st_struct; }
//	const char * print(void) const{
//		std::string str;
//		lbl_pt field;
//		
//		str = "Struct: ";
//		str += get_name();
//		str += "\n";
//		
//		if(( field = first() )){
//			do{
//				str += "\t";
//				str += field->print();
//				str += "\n";
//			}while(( field = next() ));
//		}
//		
//		str += "\n";
//		
//		return str.c_str();
//	}
	
	/******************************* MUTATORS *********************************/
	
	lbl_pt remove(const char * name  ); ///< Remove an object by its name
	lbl_pt add   (      lbl_pt object); ///< add a new object
	
};


#endif // DEFINITIONS_HPP



