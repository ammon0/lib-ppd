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


#include <ppd/routine.hpp>

#include <util/data.h>
#include <util/msg.h>


PPD_Routine::PPD_Routine(const char * full_name){
	std::string temp;
	
	blocks = DS_new_list(sizeof(Block));
	
//	temp = full_name;
//	temp += "_params";
//	formal_params.set_name(temp.c_str());
//	
//	temp = full_name;
//	temp += "_autos";
//	auto_storage.set_name(temp.c_str());
	
}
PPD_Routine::~PPD_Routine(void){
	blk_pt blk;
	
	while((blk = first())) delete blk;
	DS_delete(blocks);
}

/******************************* ACCESSOR *********************************/

bool   PPD_Routine::isempty(void)const{ return         DS_isempty(blocks); }
blk_pt PPD_Routine::first  (void)const{ return (blk_pt)DS_first  (blocks); }
blk_pt PPD_Routine::next   (void)const{ return (blk_pt)DS_next   (blocks); }

//const char * PPD_Routine::print(void) const{
//	std::string str;
//	
//	str = "PPD_Routine: ";
//	str += get_name();
//	
//	str += "\n\tParameters:";
//	str += formal_params.print();
//	
//	str += "\n\tStack Variables:";
//	str += auto_storage.print();
//	
//	return str.c_str();
//}

/******************************* MUTATORS *********************************/

inst_pt PPD_Routine::add_inst (inst_pt instruction){
	blk_pt  last_blk;
	inst_pt inst;
	
	msg_print(NULL, V_TRACE, "Procedure::add(): start");
	
	// if the instruction is a label it is a leader
	if(instruction->op == i_lbl && !isempty()) DS_nq(blocks, new Block);
	
	// add the instruction
	last_blk = (blk_pt)DS_last(blocks);
	inst = last_blk->enqueue(instruction);
	
	// if the instruction is a branch the next one is a leader
	if(
		instruction->op==i_jmp ||
		instruction->op==i_jt  ||
		instruction->op==i_jf  )
			DS_nq(blocks, new Block);
	
	
	msg_print(NULL, V_TRACE, "Procedure::add(): stop");
	return inst;
}


