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


#include <ppd/routine.hpp>
#include <string>


/**	Portable representation of a complete object file.
*/
class PPD_Module{
	DS lbl_dex;	///< index of all program labels
	DS defs;	///< container for definitions
	std::string file_name = "";
	
public:
	/****************************** CONSTRUCTOR *******************************/
	
	PPD_Module();
	~PPD_Module();
	
	/******************************* ACCESSOR *********************************/
	
	lbl_pt find (const char * name) const; ///< find a label by name
	lbl_pt first(      void       ) const; ///< Returns the first label
	lbl_pt next (      void       ) const; ///< Returns the next label
	
	/******************************* MUTATORS *********************************/
	
	lbl_pt remove(const char   * name  ); ///< Remove an label by its name
	lbl_pt add   (      lbl_pt   object); ///< add a new label
	
	def_pt AddDef(def_pt definition); ///< add a new definition
};


#endif // _PPD_HPP


