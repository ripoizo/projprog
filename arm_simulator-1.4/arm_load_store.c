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
#include "arm_load_store.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_core.h"
#include "util.h"
#include "debug.h"
#include "registers.h"
#include <stdio.h>

uint32_t offset(arm_core p, uint32_t ins) ;
uint32_t offset_h(arm_core p, uint32_t ins) ;

int str(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit);
int strb(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit);
int ldr(arm_core p, uint8_t rd, uint8_t rn, uint32_t offset, int p_bit, int u_bit, int w_bit);
int ldrb(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit);
int strh(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit);
int ldrh(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit);

uint32_t offset(arm_core p, uint32_t ins) {
	if(!get_bit(ins, 25)) {
		return get_bits(ins, 11, 0) ;
	} else {
		uint32_t rm = arm_read_register(p, get_bits(ins, 3, 0)) ;
		uint8_t shift_imm = get_bits(ins, 11, 7) ;
		uint8_t shift = get_bits(ins, 6, 5) ;
		if(get_bits(ins, 11, 4) == 0){
			return rm ;
		} else {
			switch(shift) {
				case LSL :
					return rm << shift_imm ;
					break ;
				case LSR :
					if(shift_imm == 0) {
						return 0 ;
					} else {
						return rm >> shift_imm ;
					}
					break ;
				case ASR : 
					if(shift_imm == 0) {
						if(get_bit(rm, 31)){
							return -1 ;
						} else {
							return 0 ;
						}
					} else {
						return asr(rm, shift_imm) ;
					}
					break ;
				default : // ROR ou RRX
					if(shift_imm == 0) { // RRX
						int c_flag = get_bit(arm_read_cpsr(p), C) ;
						return (c_flag << 31 | rm >> 1) ;
					} else { // ROR
						return ror(rm, shift_imm) ;
					}
					break ;
			}
		}
	}
}

uint32_t offset_h(arm_core p, uint32_t ins) {
	if(get_bit(ins, 22)) {
		return (get_bits(ins, 11, 8) << 4 | get_bits(ins, 3, 0)) ;
	} else {
		return get_bits(ins, 3, 0) ;
	}
}

int arm_load_store(arm_core p, uint32_t ins) {
    
    // récupération des bits P,U,W
    int p_bit = get_bit(ins, 24) ;
    int u_bit = get_bit(ins, 23) ;
    int w_bit = get_bit(ins, 21) ;
    
    // recuperation des registres
    uint8_t rn = get_bits(ins, 19, 16) ;
    uint8_t rd = get_bits(ins, 15, 12) ;
    
    if(get_bit(ins,26)){
    	uint32_t off = offset(p, ins) ;
	switch(get_bit(ins, 22) << 1 | get_bit(ins, 20)){
		case 00 :
			return str(p, rd, rn, off, p_bit, u_bit, w_bit);
			break ;
		case 01 :
			return ldr(p, rd, rn, off, p_bit, u_bit, w_bit) ;
			break ;
		case 10 :
			return strb(p, rd, rn, off, p_bit, u_bit, w_bit) ;
			break;
		default : 
			return ldrb(p, rd, rn, off, p_bit, u_bit, w_bit) ;
			break ;
	}
    } else {
    	uint32_t off = offset_h(p, ins) ;
     	if(get_bit(ins, 20)) {
		return ldrh(p, rd, rn, off, p_bit, u_bit, w_bit) ;
	} else {
		return strh(p, rd, rn, off, p_bit, u_bit, w_bit) ;
	}
     }
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}

int str(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit) {
	uint32_t rd_value = arm_read_register(p, rd) ;
	uint32_t rn_value = arm_read_register(p, rn) ;
	uint32_t add ;
	int writing_res ;
	
	if(u_bit) {
		add = rn_value + off ;
		
	} else {
		add = rn_value - off ;
	}
	
	if(p_bit) {
		writing_res = arm_write_word(p, add, rd_value) ;
		if(w_bit) {
			arm_write_register(p, rn, add) ;
		}
	} else { 
		writing_res = arm_write_word(p, rn_value, rd_value) ;
		arm_write_register(p, rn, add) ;
	}
	
	if(writing_res) {
		return DATA_ABORT ;
	}
	return 0 ;		
}

int strb(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit) {
	uint32_t rd_value = arm_read_register(p, rd) ;
	uint32_t rn_value = arm_read_register(p, rn) ;
	uint32_t add ;
	int writing_res ;
	
	if(u_bit) {
		add = rn_value + off ;
		
	} else {
		add = rn_value - off ;
	}
	
	if(p_bit) {
		writing_res = arm_write_byte(p, add, rd_value) ;
		if(w_bit) {
			arm_write_register(p, rn, add) ;
		}
	} else { 
		writing_res = arm_write_byte(p, rn_value, rd_value) ;
		arm_write_register(p, rn, add) ;
	}
	
	if(writing_res) {
		return DATA_ABORT ;
	}
	return 0 ;		
}

int strh(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit) {
	uint32_t rd_value = arm_read_register(p, rd) ;
	uint32_t rn_value = arm_read_register(p, rn) ;
	uint32_t add ;
	int writing_res ;
	
	if(u_bit) {
		add = rn_value + off ;
		
	} else {
		add = rn_value - off ;
	}
	
	if(p_bit) {
		writing_res = arm_write_half(p, add, rd_value) ;
		if(w_bit) {
			arm_write_register(p, rn, add) ;
		}
	} else { 
		writing_res = arm_write_half(p, rn_value, rd_value) ;
		arm_write_register(p, rn, add) ;
	}
	
	if(writing_res) {
		return DATA_ABORT ;
	}
	return 0 ;		
}

int ldr(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit) {
	uint32_t rd_value ;
	uint32_t rn_value = arm_read_register(p, rn) ;
	uint32_t add ;
	int reading_res ;
	
	if(u_bit) {
		add = rn_value + off ;
		
	} else {
		add = rn_value - off ;
	}
	
	if(p_bit) {
		reading_res = arm_read_word(p, add, &rd_value) ;
		if(w_bit) {
			arm_write_register(p, rn, add) ;
		}
	} else { 
		reading_res = arm_read_word(p, rn_value, &rd_value) ;
		arm_write_register(p, rn, add) ;
	}
	
	if(reading_res) {
		return DATA_ABORT ;
	}
	return 0 ;		
}

int ldrb(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit){
	uint8_t rd_value ;
	uint8_t rn_value = arm_read_register(p, rn) ;
	uint32_t add ;
	int reading_res ;
	
	if(u_bit) {
		add = rn_value + off ;
		
	} else {
		add = rn_value - off ;
	}
	
	if(p_bit) {
		reading_res = arm_read_byte(p, add, &rd_value) ;
		if(w_bit) {
			arm_write_register(p, rn, add) ;
		}
	} else { 
		reading_res = arm_read_byte(p, rn_value, &rd_value) ;
		arm_write_register(p, rn, add) ;
	}
	
	if(reading_res) {
		return DATA_ABORT ;
	}
	return 0 ;		
}

int ldrh(arm_core p, uint8_t rd, uint8_t rn, uint32_t off, int p_bit, int u_bit, int w_bit) { 
	uint16_t rd_value ;
	uint16_t rn_value = arm_read_register(p, rn) ;
	uint32_t add ;
	int reading_res ;
	
	if(u_bit) {
		add = rn_value + off ;
		
	} else {
		add = rn_value - off ;
	}
	
	if(p_bit) {
		reading_res = arm_read_half(p, add, &rd_value) ;
		if(w_bit) {
			arm_write_register(p, rn, add) ;
		}
	} else { 
		reading_res = arm_read_half(p, rn_value, &rd_value) ;
		arm_write_register(p, rn, add) ;
	}
	
	if(reading_res) {
		return DATA_ABORT ;
	}
	return 0 ;		
}

