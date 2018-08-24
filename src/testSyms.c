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


#include <ppd/syms.h>
#include <ppd/debug.h>

/*	There is a lot of testing I can't do without having some essential classes in the memory
 */

int main (void){
	DS symbol_table;
	const symbol_record * rec;
	
	
	msg_set_verbosity(V_TRACE);
	
	msg_trace(NULL, "main(): start\n");
	
	msg_trace(NULL, "main(): initialize the symbol table\n");
	symbol_table = sym_init();
	
	msg_trace(NULL, "main(): inserting 'TestSymbol'\n");
	sym_import(symbol_table, "TestSymbol", NIL, 110);
	
	msg_trace(NULL, "main(): calling fixup\n");
	rec = sym_fixup(symbol_table);
	
	msg_trace(NULL, "main(): result of sym_fixup()\n");
	sym_print(rec);
	
	
	msg_trace(NULL, "main(): freeing name string\n");
	free(rec->name);
	
	msg_trace(NULL, "main(): close the symbol table\n");
	sym_close(symbol_table);
	
	msg_trace(NULL, "main(): stop\n");
}


