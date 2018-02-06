/*******************************************************************************
 *
 *	Lib-PPD : Portable Program Data
 *
 *	Copyright (c) 2017 Ammon Dodson
 *	You should have received a copy of the license terms with this software. If
 *	not, please visit the project homepage at:
 *	https://github.com/ammon0/MPL
 *
 ******************************************************************************/


#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP


struct{
	size_t size=0; // this size is dependent on the compilation target.
	
	
} PPDDefinition;


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

struct PPDDefInteger: public PPDDefinition{
	width_t width;
	bool    signd;
};

struct PPDefArray: public PPDDefinition{
	
}


typedef PPDDefinition * PPDDef_pt;

#endif // DEFINITIONS_HPP


