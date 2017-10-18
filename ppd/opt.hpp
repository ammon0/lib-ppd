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

/**	@file opt.hpp
 *	
 *	Code optimization routines
 */


#ifndef _OPT_HPP
#define _OPT_HPP


#include <ppd/ppd.hpp>

/** Removes dead instructions and operands.
 */
void opt_dead(PPD * prog_data);

/** Propagates constants.
 */
void opt_const(PPD * prog_data);

/**	Unroll inner loops
 *	If the loop contains branches and more than 16 iterations don't unroll.
 */
void opt_loop(PPD * prog_data);

/**	Inline small functions & recursive tail call optimization
 *	use inlining if it will reduce code size or if the function is small and
 *	often used
 */
void opt_inline(PPD * prog_data);

/**	## Optimization Strategies
 *	### Eliminate Branches
 *	* Rearrange and fuse basic blocks to be contiguous
 *	* do not put more that 3 branches in 16 bytes of code
 *
 *	* make the fallthrough code the more likely jump
 *
 *	### Keep all memory objects and accesses aligned
 *
 *
 */


#endif // _OPT_HPP


