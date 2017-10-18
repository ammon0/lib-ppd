/*******************************************************************************
 *
 *	MPL : Minimum Portable Language
 *
 *	Copyright (c) 2017 Ammon Dodson
 *	You should have received a copy of the license terms with this software. If
 *	not, please visit the project home page at:
 *	https://github.com/ammon0/MPL
 *
 ******************************************************************************/


#include "x86.hpp"

/******************************************************************************/
//                            DECLARE STATIC DATA
/******************************************************************************/


static void static_array(Array * array){
	
	// if the array is not initialized just do this
	if(array->string_lit.empty()){
		put_str(FORM_3,
			"resb",
			str_num(array->get_size()),
			"Uninitialized array"
		);
		return;
	}
	
	// so it is initialized. sanity check
	if(array->string_lit.size() > array->get_size()){
		msg_print(NULL, V_ERROR,
			"static_array(): initialization too large");
		throw;
	}
	
	put_str("\tdb\t");
	put_str("%s\n", array->string_lit.c_str());
	
	// declare an iterator
//	std::vector<uint8_t>::iterator byte;
//	
//	byte=array->value.begin();
//	if(*byte<0x80 && *byte>=0x20) put_str("'%c'", *byte);
//	else put_str("%s", str_num(*byte));
//	
//	while(++byte < array->value.end()){
//		if(*byte<0x80 && *byte>=0x20) put_str(",'%c'", *byte);
//		else put_str(",%s", str_num(*byte));
//	}
	
}

static void static_prime(Primative * prime){
	switch (prime->get_size()){
	case BYTE : put_str("\tdb\t%s\n", str_num(prime->get_value())); break;
	case WORD : put_str("\tdw\t%s\n", str_num(prime->get_value())); break;
	case DWORD: put_str("\tdd\t%s\n", str_num(prime->get_value())); break;
	case QWORD: put_str("\tdq\t%s\n", str_num(prime->get_value())); break;
	default:
		msg_print(NULL, V_ERROR,
			"static_prime(): got a bad size from size_of()");
		throw;
	}
}

static void static_structure(Structure * structure){
	put_str("\tresb\t%s\n", str_num(structure->get_size()) );
}

/// Generate a static data object
static void static_data(lbl_pt data){
	// set padding if needed
	if(mode == xm_long && data->get_size() > QWORD){
		put_str("\talign %s\n", str_num(QWORD));
	}
	else if(mode == xm_protected && data->get_size() > DWORD){
		put_str("\talign %s\n", str_num(DWORD));
	}
	// if we are here the field cannot be bigger than the mode alignment
	else put_str("\talign %s\n", str_num(data->get_size()));
	
	// place the label
	put_lbl(data);

	switch(data->get_def()->get_type()){
	// data
	case st_prime:
		static_prime(dynamic_cast<Primative*>(data->get_def()));
		break;
	case st_array:
		static_array(dynamic_cast<Array*>(data->get_def()));
		break;
	case st_struct:
		static_structure(dynamic_cast<Structure*>(data->get_def()));
		break;

	// not data
	default        :
		msg_print(NULL, V_ERROR, "Internal static_data(): not data");
		throw;
	}
}


/******************************************************************************/
//                          CALCULATE SIZES & OFFSETS
/******************************************************************************/


// forward declaration here
static void set_size(sym_pt);

/// calculate the size of a prime
static void set_prime_size(Primative * prime){
	switch(prime->get_width()){
	case w_byte : prime->set_size(BYTE); break;
	case w_byte2: prime->set_size(WORD); break;
	case w_byte4: prime->set_size(DWORD); break;
	
	case w_byte8:
		if(mode == xm_long) prime->set_size(QWORD);
		else{
			msg_print(NULL, V_ERROR,
				"set_prime_size(): cannot use qwords in protected mode");
			throw;
		}
		break;
	
	case w_word :
	case w_ptr  :
	case w_max  :
		if(mode == xm_long) prime->set_size(QWORD);
		else prime->set_size(DWORD);
		break;
	
	case w_none:
	case w_NUM :
	default    :
		msg_print(NULL, V_ERROR, "set_prime_size(): received invalid width");
		throw;
	}
}

/// calculate the size of an array
static void set_array_size(Array * array){
	if(!array->get_child()){
		msg_print(NULL, V_ERROR,
			"set_array_size(): found array %s with no child",
			array->get_name()
		);
		throw;
	}
	
	// recursively set member sizes
	if(!array->get_size()) set_size(array->get_child());
	
	//FIXME: need to define a constant label equal to the step size.
}

/** calculate the size of structure and its member offsets
 *	This function also prints a structure declaration to the out_fd
 */
void set_struct_size(Structure * structure){
	size_t bytes=0, pad;
	lbl_pt field;
	
	// make sure all field sizes are available first
	field = structure->first();
	do{
		if(!field->get_size()){
			if(field->get_type() == st_struct)
				set_struct_size(dynamic_cast<Structure*>(field));
			else set_size(field);
		}
	}while(( field = structure->next() ));
	
	
	// calculate offsets and padding
	put_str("\nstruc %s\n", structure->get_name());
	
	// get the first field
	field = structure->first();
	
	// print the first field
	put_str("\t%s\t: resb %s\n",
		field->get_name(),
		str_num(field->get_size())
	);
	
	// store any necessary alignment info
	bytes += field->get_size();
	
	// get the next field
	while(( field = structure->next() )){
		// decide whether any padding is needed
		if(mode == xm_long && field->get_size() > QWORD){
			msg_print(NULL, V_WARN, "Padding before field %s in structure %s",
				field->get_name(),
				structure->get_name()
			);
			put_str("\talign %s\n", str_num(QWORD));
		}
		else if(mode == xm_protected && field->get_size() > DWORD){
			msg_print(NULL, V_WARN, "Padding before field %s in structure %s",
				field->get_name(),
				structure->get_name()
			);
			put_str("\talign %s\n", str_num(DWORD));
		}
		// if we are here the field cannot be bigger than the mode alignment
		else if(( pad=bytes%field->get_size() )){
			msg_print(NULL, V_WARN, "Padding before field %s in structure %s",
				field->get_name(),
				structure->get_name()
			);
			put_str("\talign %s\n", str_num(field->get_size()));
		}
		
		// print the field
		put_str("\t%s\t: resb %s\n",
			field->get_name(),
			str_num(field->get_size())
		);
		// store any necessary alignment info
		bytes += field->get_size();
	}
	
	put_str("endstrc\n");
	
	structure->set_size(bytes);
	
	put_str(
		"%%if (%s != %s_size)\n\
		\t%%error \"Internal, struct size mismatch\"\n\
		%%endif\n\n",
		str_num(structure->get_size()),
		structure->get_name()
	
	);
}

static void set_size(sym_pt sym){
	switch( sym->get_type() ){
	case st_prime : set_prime_size ( dynamic_cast<Primative*>(sym) ); break;
	case st_array : set_array_size ( dynamic_cast<Array*    >(sym) ); break;
	case st_struct: set_struct_size( dynamic_cast<Structure*>(sym) ); break;
	
	// ignore
	case st_routine:
	case st_label  : break;
	
	// error
	default     :
		msg_print(NULL, V_ERROR, "set_size(): found an invalid obj_t");
		throw;
	}
}




void x86_declarations(void){
	sym_pt sym;
	lbl_pt lbl;
	
	if(!( sym=program_data->symbols.first() )){
		msg_print(NULL, V_ERROR, "x86(): Program is empty");
		return;
	}
	
	// FIXME: declare constants with `equ`
	
	/***** SET SIZES AND STRUCTURE OFFSETS *****/
	
	put_str("\n; Declaring record offsets\n");
	
	do{
		set_size(sym);
	}while(( sym=program_data->symbols.next() ));
	
	/*********** DECLARE VISIBILITY ***********/
	
	put_str("\n; Declaring Visibility\n");
	
	lbl=dynamic_cast<lbl_pt>(program_data->symbols.first());
	
	do{
		if(lbl)
			switch(lbl->get_mode()){
			case am_static_pub:
				put_str("global %s\n", lbl->get_name());
				break;
			case am_static_extern:
				put_str("extern %s\n", lbl->get_name());
				break;
		
			// Do nothing
			case am_static_priv:
			case am_stack_fparam:
			case am_stack_aparam:
			case am_stack_auto  :
			case am_temp   :
			case am_constant  : break;
		
			// error
			case am_none:
			case am_NUM :
			default     :
				msg_print(NULL, V_ERROR,
					"x86(): got an object with an invalid storage class");
				throw;
			}
	}while(( lbl=dynamic_cast<lbl_pt>(program_data->symbols.next()) ));
	
	/************** STRING TABLE **************/
	
	/*
	An ELF executable can have any number of string tables, but a dynamically linked executable needs to have at least one. The format of a string table is very simple: It's simply a continuous stream of characters, with NUL bytes separating one string from the next. A particular string within the table is then identified by its offset within the table. The first byte of a string table must always be a NUL byte; thus an offset of zero always means the empty string.

That sounds simple enough. Let's put together a string table right now. We'll put two strings in it — the name of the function we want to link to, and the name of the library to find it in:

  strtab:
                db      0
  libc_name     equ     $ - strtab
                db      'libc.so.6', 0
  exit_name     equ     $ - strtab
                db      '_exit', 0
	*/
	
	/************ STATIC VARIABLES ************/
	
	lbl=dynamic_cast<lbl_pt>(program_data->symbols.first());
	
	put_str("\nsection .data\t; Declaring static data\n");
	
	if (mode == xm_long) put_str("align 8");
	else put_str("align 4");
	put_str(" ; this probably does nothing\n");
	
	do{
		if(lbl){
			if(lbl->get_def()->get_type() == st_routine) break;
			
			switch(lbl->get_mode()){
			case am_static_priv:
			case am_static_pub :
				static_data(lbl);
				break;
	
			// ignore
			case am_stack_aparam:
			case am_stack_fparam:
			case am_stack_auto:
			case am_temp  :
			case am_static_extern:
			case am_constant: break;
	
			//error
			case am_none:
			case am_NUM:
			default: throw;
			}
		}
	}while(( lbl=dynamic_cast<lbl_pt>(program_data->symbols.next()) ));
}


