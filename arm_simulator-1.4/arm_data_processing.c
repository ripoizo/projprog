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
#include "arm_data_processing.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_branch_other.h"
#include "util.h"
#include "debug.h"

int arm_data_processing_shift(arm_core p, uint32_t ins) {

    uint32_t Rn, Rd, shifter_operand, cpsr, spsr;
    uint64_t * carry=0;
    int typeflags;
    Rn = arm_read_register(p,get_bits(ins, 19, 16));
    Rd = get_bits(ins, 16, 12);
    shifter_operand = arm_read_register(p,get_bits(ins, 3, 0));
    cpsr = arm_read_cpsr(p);
    spsr = arm_read_spsr(p);

    switch(get_bits(ins,24,21)){
        case 0000:
            typeflags = 0;
            ins_AND(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 0001:
            typeflags = 0;
            ins_EOR(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 0010:
            typeflags = 1;
            ins_SUB(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 0011:
            typeflags = 1;
            ins_RSB(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 0100:
            typeflags = 1;
            ins_ADD(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 0101:
            return ins_ADC(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 0110:
            return ins_SBC(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 0111:
            return ins_RSC(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 1000:
            return ins_TST(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 1001:
            return ins_TEQ(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 1010:
            return ins_CMP(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 1011:
            return ins_CMN(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 1100:
            typeflags = 0;
            ins_ORR(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 1101:
        	typeflags = 0;
            ins_MOV(p,ins, Rd, shifter_operand,carry);
        break;
        case 1110:
            return ins_BIC(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 1111:
        	typeflags = 0;
            ins_MVN(p,ins,Rn, Rd, shifter_operand,carry);
        break;
    }
    if(get_bit(ins, 20) && Rd==1111){
    	if(arm_current_mode_has_spsr(p)){
    		cpsr = spsr;
    	}
    	else{
    		return UNPREDICTABLE;
    	}
    }
    if(get_bit(ins, 20)){                              
        put_flag (cpsr,Rd,carry,typeflags);
        arm_write_cpsr(p, cpsr);  
    }
    return 0;
}


int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
    uint32_t Rn, Rd, shifter_operand, cpsr, spsr;
    uint64_t * carry=0;
    int typeflags;
    Rn = arm_read_register(p,get_bits(ins, 19, 16));
    Rd = get_bits(ins, 16, 12);
    shifter_operand = get_bits(ins,7,0);
    shifter_operand = ror(shifter_operand,get_bits(ins,11,8)*2);
    cpsr = arm_read_cpsr(p);
    spsr = arm_read_spsr(p);

    switch(get_bits(ins,24,21)){
        case 0000:
            typeflags = 0;
            ins_AND(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 0001:
            typeflags = 0;
            ins_EOR(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 0010:
            typeflags = 1;
            ins_SUB(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 0011:
            typeflags = 1;
            ins_RSB(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 0100:
            typeflags = 1;
            ins_ADD(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 0101:
            return ins_ADC(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 0110:
            return ins_SBC(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 0111:
            return ins_RSC(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 1000:
            return ins_TST(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 1001:
            return ins_TEQ(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 1010:
            return ins_CMP(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 1011:
            return ins_CMN(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 1100:
            typeflags = 0;
            ins_ORR(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 1101:
        	typeflags = 0;
            ins_MOV(p,ins, Rd, shifter_operand,carry);
        break;
        case 1110:
            return ins_BIC(p,ins,Rn, Rd, shifter_operand,carry);
        break;
        case 1111:
        	typeflags = 0;
            ins_MVN(p,ins,Rn, Rd, shifter_operand,carry);
        break;
    }

    if(get_bit(ins, 20) && Rd==1111){
    	if(arm_current_mode_has_spsr(p)){
    		cpsr = spsr;
    	}
    	else{
    		return UNPREDICTABLE;
    	}
    }
    
    if(get_bit(ins, 20)){                              
        put_flag (cpsr,Rd,carry,typeflags);
        arm_write_cpsr(p, cpsr);  
    }
    return 0;    
}

/* Décodage pour le SHIFT*/
/*n=negatif /z=zéro /c=retenue /v=depassement de capacité*/
void put_flag(uint32_t cpsr,uint32_t Rd,uint64_t *carry,int typeflags){
    if (typeflags == 1){
        if(Rd==0){cpsr = set_bit(cpsr, Z);}
        if(Rd<0){cpsr = set_bit(cpsr, N);}
        if(get_bit(*carry,32)==1){cpsr = set_bit(cpsr, C);}
        if(*carry > 0xFFFFFFFF){cpsr = set_bit(cpsr, V);}
    }
    else {
        if(Rd==0){cpsr = set_bit(cpsr, Z);}
        if(Rd<0){cpsr = set_bit(cpsr, N);}
        if(get_bit(*carry,32)==1){cpsr = set_bit(cpsr, C);}       
    }

}

void ins_AND(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry){
    arm_write_register(p, Rd, shifter_operand & Rn);
    *carry=shifter_operand & Rn;
}

void ins_EOR(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry){
    arm_write_register(p, Rd, shifter_operand ^ Rn);
    *carry=shifter_operand ^ Rn;
}

void ins_SUB(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry){
    arm_write_register(p, Rd, shifter_operand - Rn);
    *carry = shifter_operand - Rn;
}

void ins_RSB(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry){
    arm_write_register(p, Rd, Rn - shifter_operand);
    *carry = shifter_operand - Rn;
}

void ins_ADD(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry){
    arm_write_register(p, Rd, shifter_operand + Rn);
    *carry = shifter_operand + Rn;
}

int ins_ADC(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry){
	return UNDEFINED_INSTRUCTION;
}

int ins_SBC(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry){
	return UNDEFINED_INSTRUCTION;
}

int ins_RSC(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry){
	return UNDEFINED_INSTRUCTION;
}

int ins_TST(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry){
	return UNDEFINED_INSTRUCTION;
}

int ins_TEQ(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry){
	return UNDEFINED_INSTRUCTION;
}

int ins_CMP(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry){
	return UNDEFINED_INSTRUCTION;
}

int ins_CMN(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry){
	return UNDEFINED_INSTRUCTION;
}

void ins_ORR(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry){
    arm_write_register(p, Rd, shifter_operand | Rn);
    *carry = shifter_operand | Rn;
}

void ins_MOV(arm_core p, uint32_t ins, uint32_t Rd, uint32_t shifter_operand, uint64_t *carry){
	arm_write_register(p, Rd, shifter_operand);
    *carry = shifter_operand;
}

int ins_BIC(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry){
	return UNDEFINED_INSTRUCTION;
}

void ins_MVN(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry){
	arm_write_register(p, Rd, ~shifter_operand);
    *carry = ~shifter_operand;	
}