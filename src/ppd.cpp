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

#include <ppd/ppd.hpp>
#include <util/data.h>
#include <util/msg.h>

#include <util/string.h>

static inline const void * lbl_name(const void * obj){
	return (*(lbl_pt*)obj)->get_name();
}
static inline imax lbl_cmp(const void * left, const void * right){
	return strcmp((char*) left, (char*) right);
}


/****************************** CONSTRUCTOR *******************************/

PPD_Module::PPD_Module(void){
	
	lbl_dex = DS_new_bst(
		sizeof(lbl_pt),
		false,
		&lbl_name,
		&lbl_cmp
	);
	
	defs = DS_new_list(sizeof(def_pt));
}

PPD_Module::~PPD_Module(void){
	DS_delete(lbl_dex);
	DS_delete(defs);
}

/******************************* ACCESSOR *********************************/

lbl_pt PPD_Module::find(const char * name) const{
	return *(lbl_pt*)DS_find(lbl_dex, name);
}
lbl_pt PPD_Module::first(void)const{
	return *(lbl_pt*)DS_first(lbl_dex);
}
lbl_pt PPD_Module::next (void)const{
	return *(lbl_pt*)DS_next(lbl_dex);
}

/******************************* MUTATORS *********************************/

lbl_pt PPD_Module::remove(const char * name){
	if(DS_find(lbl_dex, name)) return *(lbl_pt*)DS_remove(lbl_dex);
	else{
		msg_print(NULL, V_ERROR, "Internal PPD::remove(): no such object");
		throw;
	}
}

lbl_pt PPD_Module::add(lbl_pt object){
//	if(!object->named()){
//		msg_print(NULL, V_ERROR, "Internal PPD::add(): object has no name");
//		throw;
//	}
	
	return *(lbl_pt*)DS_insert(lbl_dex, object);
}


