/*
Armator - simulateur de jeu d'instruction ARMv5T à but pédagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Générale GNU publiée par la Free Software
Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).
Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
Licence Publique Générale GNU pour plus de détails.
Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même
temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
États-Unis.
Contact: Guillaume.Huard@imag.fr
	 Bâtiment IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'Hères
*/
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"
#include "registers.h"
#include <debug.h>
#include <stdlib.h>
/*pc = r15*/

uint32_t shiftage(uint32_t val){
	uint32_t mask=0x00FFFFFF;
	if(get_bit(val,23)){
		val=val|(~mask);
	}else{
		val=val&mask;
	}
	val=val<<2;
	return val;
}

int arm_branch(arm_core p, uint32_t ins) {
	uint32_t address=0;

	/*si BL*/
	if(get_bit(ins, 24)){
		/*On sauvegarde PC dans r14*/
		arm_write_register( p, 14,  arm_read_register(p,15) );
	}
	
	address= set_bits(address,23,0, get_bits(ins, 23, 0) );
	/*On tente de sign extending*/
	address=shiftage(address);
	/*On met l'addresse dans PC (r15)*/
	arm_write_register(p, 15,  address);
	
	return 0;		
}



int arm_coprocessor_others_swi(arm_core p, uint32_t ins) {
    if (get_bit(ins, 24)) {
        /* Here we implement the end of the simulation as swi 0x123456 */
        if ((ins & 0xFFFFFF) == 0x123456)
            exit(0);
        return SOFTWARE_INTERRUPT;
    } 
    return UNDEFINED_INSTRUCTION;
}

int arm_miscellaneous(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}















