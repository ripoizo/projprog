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
#include "arm_instruction.h"
#include "arm_exception.h"
#include "arm_data_processing.h"
#include "arm_load_store.h"
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"

/* Decoding functions for actual operation in load/store */
/* to be implemented */



static int arm_execute_instruction(arm_core p) {
    uint32_t value;
    uint32_t cpsr;
    cpsr = arm_read_cpsr(p);
    arm_fetch(p, &value);

    switch(get_bits(value,31,28)){
    	case 0b0000:
    			if(!get_bit(cpsr,Z)){
    				return 0;
    			}
    	break;
    	case 0b0001:
    	    	if(get_bit(cpsr,Z)){
    				return 0;
    			}
    	break;
    	case 0b0010:
    			if(!get_bit(cpsr,C)){
    				return 0;
    			}
    	break;
    	case 0b0011:
    			if(get_bit(cpsr,C)){
    				return 0;
    			}
    	break;
    	case 0b0100:
    			if(!get_bit(cpsr,N)){
    				return 0;
    			}
    	break;
    	case 0b0101:
    			if(get_bit(cpsr,N)){
    				return 0;
    			}
    	break;
    	case 0b0110:
    			if(!get_bit(cpsr,V)){
    				return 0;
    			} 
    	break;
    	case 0b0111:
    			if(get_bit(cpsr,V)){
    				return 0;
    			}
    	break;
    	case 0b1000:
    			if(!get_bit(cpsr,C) || get_bit(cpsr,Z)){
    				return 0;
    			}
    	break;
    	case 0b1001:
    			if(get_bit(cpsr,C) || !get_bit(cpsr,Z)){
    				return 0;
    			}
    	break;
    	case 0b1010:
    			if(get_bit(cpsr,N) != get_bit(cpsr,V)){
    				return 0;
    			}
    	break;
    	case 0b1011:
    			if(get_bit(cpsr,N) == get_bit(cpsr,V)){
    				return 0;
    			}
    	break;
    	case 0b1100:
    			if(get_bit(cpsr,Z) || (get_bit(cpsr,N) != get_bit(cpsr,V))){
    				return 0;
    			}
    	break;
    	case 0b1101:
    			if(!get_bit(cpsr,Z) || (get_bit(cpsr,N) == get_bit(cpsr,V))){
    				return 0;
    			}
    	break;
    }

    switch(get_bits(cpsr,27,25)){ //décodage de l'instruction
    	case 0b000:
    		return arm_data_processing_shift(p,value);
    	break;
    	case 0b001:
    		return arm_data_processing_immediate_msr(p,value);
    	break;
    	case 0b010:
    		return arm_load_store(p,value); //voir prof
    	break;
    	case 0b011:
    		return arm_load_store(p,value); //voir prof
    	break;
    	case 0b100:
    		return arm_load_store_multiple(p,value);
    	break;
    	case 0b101:
    		return arm_branch(p,value);
    	break;
    	case 0b110:
    		return arm_coprocessor_load_store(p,value);
    	break;
    	case 0b111:
    		return arm_coprocessor_others_swi(p,value);
    	break;
    }
    return 0;
}

int arm_step(arm_core p) {
    int result;

    result = arm_execute_instruction(p);
    if (result)
        arm_exception(p, result);
    return result;
}