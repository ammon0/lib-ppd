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


#include <ppd/pexe.hpp>
#include <util/msg.h>
#include <elfio/elfio.hpp>

//using namespace ELFIO;


/**	Create a Portable Executable file from the object memory
	Takes the output file name as a parameter. stack or register based VM?
 */
return_t GenPEXE(const char * pexe_file){
	ELFIO::elfio writer;
	ELFIO::section *Strings, *Methods, *Objects, *Symbols;
	
	msg_print(NULL, V_TRACE, "GenPEXE(): start\n");
	
	// basic setup
	writer.create( ELFCLASS32, ELFDATA2LSB );
	writer.set_os_abi( ELFOSABI_LINUX );
	writer.set_type( ET_REL );
	writer.set_machine( EM_386 );
	
	// setup sections
	Strings = writer.sections.add("Strings"); // string table
	Strings->set_type( SHT_STRTAB );
	
	Methods = writer.sections.add("Methods");
	
	Objects = writer.sections.add("Objects");
	
	
	writer.save(pexe_file);
	
	msg_print(NULL, V_TRACE, "GenPEXE(): stop\n");
	return r_success;
}


