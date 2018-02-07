/*******************************************************************************
 *
 *	Lib-ppd : Portable Program Data Library
 *
 *	Copyright (c) 2017 Ammon Dodson
 *	You should have received a copy of the license terms with this software. If
 *	not, please visit the project homepage at:
 *	https://github.com/ammon0/lib-ppd
 *
 ******************************************************************************/


#include <ppd/definitions.hpp>
#include <util/data.h>
#include <util/msg.h>

//const char * PPD_Integer::print(void)const{
//	static char string[30] = "bytes 0 +00000000000000000000";
//	
//	switch(width){
//	case w_byte : string[6] = '1'; break;
//	case w_byte2: string[6] = '2'; break;
//	case w_byte4: string[6] = '4'; break;
//	case w_byte8: string[6] = '8'; break;
//	case w_max  : string[6] = 'M'; break;
//	case w_word : string[6] = 'W'; break;
//	case w_ptr  : string[6] = 'P'; break;
//	
//	// error condition
//	case w_NUM:
//	case w_none:string[6] = 'X'; break;
//	}
//	
//	return string;
//}


/******************************************************************************/
//                         STRUCTURE TYPE FUNCTIONS
/******************************************************************************/


/****************************** CONSTRUCTOR *******************************/

PPD_Structure::PPD_Structure(void){
	fields = DS_new_list(sizeof(lbl_pt));
}

PPD_Structure::~PPD_Structure(void){
	DS_delete(fields);
	// FIXME: itterate and delete
}

/******************************* ACCESSOR *********************************/

bool PPD_Structure::isempty(void) const{ return DS_isempty(fields); }
umax PPD_Structure::count  (void) const{ return DS_count  (fields); }

lbl_pt PPD_Structure::find (const char * name) const{
	
}

lbl_pt PPD_Structure::first(void) const{ return *(lbl_pt*)DS_first(fields); }
lbl_pt PPD_Structure::next (void) const{ return *(lbl_pt*)DS_next (fields); }

/******************************* MUTATORS *********************************/

lbl_pt PPD_Structure::remove(const char * name  ){
	if(find(name)) DS_remove(fields);
	else msg_print(NULL, V_ERROR, "PPD_Structure::remove: no such label");
}

lbl_pt PPD_Structure::add(lbl_pt object){
	DS_last(fields);
	return *(lbl_pt*)DS_insert(fields, &object);
}


