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


/** This is a container for all the components of the portable program data.
*/
struct PPD{
	// Optimizations applied
	bool dead     =false; ///< indicates whether this PPD has passed opt_dead
	bool constants=false; ///< indicates whether this PPD has passed opt_const
	Sym_index symbols;
	
public:
	return_t Make_MPL(FILE * outfile) const;
};


#endif // _PPD_HPP


