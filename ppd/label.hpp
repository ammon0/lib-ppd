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


#ifndef _LABEL_HPP
#define _LABEL_HPP


#include "ppd/definitions.hpp"

#include <util/types.h>
#include <util/msg.h>
#include <string>



typedef class PPD_Label  * lbl_pt;


/******************************************************************************/
//                              MEMORY LABELS
/******************************************************************************/


/* A label is a name that represents a memory location or register. All program instructions act on labels only. The amount of space to reserve at each labeled location is determined by querying the label's definition pointer.
*/

typedef enum{
	am_none         , ///< This is just to catch errors; it is not used.
	am_static_pub   , ///< label
	am_static_priv  , ///< label
	am_static_extern, ///< label
	am_stack_fparam , ///< BP+(2*stack_width)+label
	am_stack_aparam , ///< SP+label
	am_stack_auto   , ///< BP-label
	am_temp         , ///< register or spill
	am_constant     , ///< ??? includes offsets
	am_NUM            ///< This is the count of storage classes
} access_mode;



/** Representation of a memory label
*/
class PPD_Label{
	std::string name;
	access_mode mode=am_none;
	def_pt      def=NULL;
	
public:
	bool live=false; ///< Used internally by the dead-code optimizer.
	
protected:
	constexpr static const char * str_sclass[am_NUM]= {
		"NONE  ",
		"pub   ",
		"prv   ",
		"extern",
		"fparam",
		"aparam",
		"auto  ",
		"TEMP  ",
		"const "
	}; ///< to facilitate the print funtions
	
public:
	/****************************** CONSTRUCTOR *******************************/
	
	PPD_Label(const char * full_name){ name = full_name; }
	
	/******************************* ACCESSOR *********************************/
	
	const char * get_name(void)const{ return name.c_str(); }
	access_mode get_mode(void)const{ return mode  ; }
	def_pt      get_def (void)const{ return def   ; }
	size_t      get_size(void)const{ return def->get_size(); }
	umax        get_value(void)const{
		if(mode != am_constant) throw;
		return static_cast<PPD_Integer*>(def)->get_value();
	}
	
	
	const char * print_declaration(void) const{
		static std::string buffer; // wish I had closures instead
		
		switch(mode){
		case am_static_pub:
			buffer = ".global ";
			buffer += name;
			return buffer.c_str();
		case am_static_extern:
			buffer = ".extern ";
			buffer += name;
			return buffer.c_str();
		case am_stack_fparam:
		case am_stack_aparam:
		case am_constant:
		
		case am_static_priv:
		case am_none:
		case am_temp:
		case am_NUM:
		default:
			msg_print(NULL, V_ERROR,
				"internal: PPD_Label::print_declaration(): invalid mode.\n");
			return NULL;
		}
	}
	
	const char * print_definition(void) const{
		static std::string buffer; // wish I had closures instead
		
		switch(mode){
		case am_static_pub:
		case am_static_priv:
		case am_static_extern:
			buffer = name;
			buffer += ": ";
			if(def) buffer += def->print();
			return buffer.c_str();
		
		case am_stack_fparam:
		case am_stack_aparam:
		
		case am_constant:
		case am_none:
		case am_temp:
		case am_NUM:
		default:
			msg_print(NULL, V_ERROR,
				"internal: PPD_Label::print_declaration(): invalid mode.\n");
			return NULL;
		}
	}
	
	/******************************* MUTATORS *********************************/
	
	void set_mode(access_mode m) { mode = m; }
	void set_def(def_pt definition) {
		if(mode == am_none || mode == am_temp){
			msg_print(NULL, V_ERROR,
				"internal: PPD_Label::set_def(): cannot set definition on am_temp or am_none\n");
			throw;
		}
		def = definition;
	}
	
};


#endif // _LABEL_HPP


