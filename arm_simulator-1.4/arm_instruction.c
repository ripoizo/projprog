/*
Armator - simulateur de jeu d'instruction ARMv5T � but p�dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G�n�rale GNU publi�e par la Free Software
Foundation (version 2 ou bien toute autre version ult�rieure choisie par vous).

Ce programme est distribu� car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp�cifique. Reportez-vous � la
Licence Publique G�n�rale GNU pour plus de d�tails.

Vous devez avoir re�u une copie de la Licence Publique G�n�rale GNU en m�me
temps que ce programme ; si ce n'est pas le cas, �crivez � la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
�tats-Unis.

Contact: Guillaume.Huard@imag.fr
	 B�timent IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'H�res
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
    int cond;
    cpsr = arm_read_cpsr(p);
    cond = arm_fetch(p, &value);

    switch(get_bits(cond,31,28)){
    	case 0000:
    			if(!get_bit(cpsr,Z)){
    				return 0;
    			}
    	break;
    	case 0001:
    	    	if(get_bit(cpsr,Z)){
    				return 0;
    			}
    	break;
    	case 0010:
    			if(!get_bit(cpsr,C)){
    				return 0;
    			}
    	break;
    	case 0011:
    			if(get_bit(cpsr,C)){
    				return 0;
    			}
    	break;
    	case 0100:
    			if(!get_bit(cpsr,N)){
    				return 0;
    			}
    	break;
    	case 0101:
    			if(get_bit(cpsr,N)){
    				return 0;
    			}
    	break;
    	case 0110:
    			if(!get_bit(cpsr,V)){
    				return 0;
    			} 
    	break;
    	case 0111:
    			if(get_bit(cpsr,V)){
    				return 0;
    			}
    	break;
    	case 1000:
    			if(!get_bit(cpsr,C) || get_bit(cpsr,Z)){
    				return 0;
    			}
    	break;
    	case 1001:
    			if(get_bit(cpsr,C) || !get_bit(cpsr,Z)){
    				return 0;
    			}
    	break;
    	case 1010:
    			if(get_bit(cpsr,N) != get_bit(cpsr,V)){
    				return 0;
    			}
    	break;
    	case 1011:
    			if(get_bit(cpsr,N) == get_bit(cpsr,V)){
    				return 0;
    			}
    	break;
    	case 1100:
    			if(get_bit(cpsr,Z) || (get_bit(cpsr,N) != get_bit(cpsr,V))){
    				return 0;
    			}
    	break;
    	case 1101:
    			if(!get_bit(cpsr,Z) || (get_bit(cpsr,N) == get_bit(cpsr,V))){
    				return 0;
    			}
    	break;
    }

    switch(get_bits(cpsr,27,25)){ //d�codage de l'instruction
    	case 000:
    		return arm_data_processing_shift(p,cond);
    	break;
    	case 001:
    		return arm_data_processing_immediate_msr(p,cond);
    	break;
    	case 010:
    		return arm_load_store(p,cond); //voir prof
    	break;
    	case 011:
    		return arm_load_store(p,cond); //voir prof
    	break;
    	case 100:
    		return arm_load_store_multiple(p,cond);
    	break;
    	case 101:
    		return arm_branch(p,cond);
    	break;
    	case 110:
    		return arm_coprocessor_load_store(p,cond);
    	break;
    	case 111:
    		return arm_coprocessor_others_swi(p,cond);
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