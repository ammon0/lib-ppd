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


#include <ppd/object_mem.h>
#include <util/msg.h>

int main (void){
	
	Object_pt objects[20];
	
	msg_set_verbosity(V_TRACE);
	
	msg_print(NULL,V_INFO, "memtest: start\n");
	
	msg_print(NULL, V_INFO, "sizeof(umax)       == %d\n", sizeof(umax)      );
	msg_print(NULL, V_INFO, "sizeof(Object_idx) == %d\n", sizeof(Object_idx));
	msg_print(NULL, V_INFO, "sizeof(Object_pt)  == %d\n", sizeof(Object_pt) );
	msg_print(NULL, V_INFO, "sizeof(void*)      == %d\n", sizeof(void*)     );
	
	
	initObjectMem();
	
	
	for(int i=0; i<20; i++){
		objects[i] = SintObjOf(i);
	}
	
	
	for(int i=0; i<20; i++){
		if(i != SintValOf(objects[i]))
			msg_print(NULL, V_ERROR, "memtest: Sint value mismatch.\n");
	}
	
	
	msg_print(NULL,V_INFO, "memtest: stop\n");
	
}


