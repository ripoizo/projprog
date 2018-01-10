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
    uint32_t Rn, Rd, shifter_operand, cpsr, spsr, shifter_carry_out;
    uint32_t carry;
    carry=0;
    int typeflags;
    printf("shift\n");
    Rn = arm_read_register(p,get_bits(ins, 19, 16));
    Rd = get_bits(ins, 16, 12);
    shifter_operand = find_shifter_operand(p,ins,&shifter_carry_out);
    cpsr = arm_read_cpsr(p);
    spsr = arm_read_spsr(p);

    switch(get_bits(ins,24,21)){
        case 0b0000:
            typeflags = 0;
            ins_AND(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b0001:
            typeflags = 0;
            ins_EOR(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b0010:
            typeflags = 1;
            ins_SUB(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b0011:
            typeflags = 1;
            ins_RSB(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b0100:
            typeflags = 1;
            ins_ADD(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b0101:
            return ins_ADC(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b0110:
            return ins_SBC(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b0111:
            return ins_RSC(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b1000:
            return ins_TST(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b1001:
            return ins_TEQ(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b1010:
            return ins_CMP(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b1011:
            return ins_CMN(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b1100:
            typeflags = 0;
            ins_ORR(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b1101:
        	typeflags = 0;
            ins_MOV(p,ins, Rd, shifter_operand,&carry);
        break;
        case 0b1110:
            return ins_BIC(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b1111:
        	typeflags = 0;
            ins_MVN(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
    }
    if(get_bit(ins, 20) && Rd==0b1111){
    	if(arm_current_mode_has_spsr(p)){
    		cpsr = spsr;
    	}
    	else{
    		return UNPREDICTABLE;
    	}
    }
    if(get_bit(ins, 20)){                              
        put_flag (p,cpsr,Rd,carry,typeflags);
        arm_write_cpsr(p, cpsr);  
    }
    return 0;
}


int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
    uint32_t Rn, Rd, shifter_operand, cpsr, spsr, shifter_carry_out;
    uint32_t carry;
    carry=0;
    int typeflags;
    printf("MSR\n");
    Rn = arm_read_register(p,get_bits(ins, 19, 16));
    Rd = get_bits(ins, 16, 12);
    shifter_operand = find_shifter_operand(p,ins,&shifter_carry_out);
    cpsr = arm_read_cpsr(p);
    spsr = arm_read_spsr(p);

    switch(get_bits(ins,24,21)){
        case 0b0000:
            typeflags = 0;
            ins_AND(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b0001:
            typeflags = 0;
            ins_EOR(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b0010:
            typeflags = 1;
            ins_SUB(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b0011:
            typeflags = 1;
            ins_RSB(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b0100:
            typeflags = 1;
            ins_ADD(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b0101:
            return ins_ADC(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b0110:
            return ins_SBC(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b0111:
            return ins_RSC(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b1000:
            return ins_TST(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b1001:
            return ins_TEQ(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b1010:
            return ins_CMP(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b1011:
            return ins_CMN(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b1100:
            typeflags = 0;
            ins_ORR(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b1101:
        	typeflags = 0;
            ins_MOV(p,ins, Rd, shifter_operand,&carry);
        break;
        case 0b1110:
            return ins_BIC(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
        case 0b1111:
        	typeflags = 0;
            ins_MVN(p,ins,Rn, Rd, shifter_operand,&carry);
        break;
    }

    if(get_bit(ins, 20) && Rd==0b1111){
    	if(arm_current_mode_has_spsr(p)){
    		cpsr = spsr;
    	}
    	else{
    		return UNPREDICTABLE;
    	}
    }
    
    if(get_bit(ins, 20)){                              
        put_flag (p,cpsr,Rd,carry,typeflags);
        arm_write_cpsr(p, cpsr);  
    }
    return 0;    
}

/* Décodage pour le SHIFT*/
/*n=negatif /z=zéro /c=retenue /v=depassement de capacité*/
void put_flag(arm_core p,uint32_t cpsr,uint32_t Rd,uint32_t carry,int typeflags){
    if (typeflags == 1){
        if(arm_read_register(p,Rd)==0){cpsr = set_bit(cpsr, Z);}
        if(Rd<0){cpsr = set_bit(cpsr, N);}
        //if(get_bit(*carry,32)==1){cpsr = set_bit(cpsr, C);}
       // if(*carry > 0xFFFFFFFF){cpsr = set_bit(cpsr, V);}
    }
    else {
        if(arm_read_register(p,Rd)==0){cpsr = set_bit(cpsr, Z);}
        if(Rd<0){cpsr = set_bit(cpsr, N);}
       // if(get_bit(*carry,32)==1){cpsr = set_bit(cpsr, C);}       
    }

}

void ins_AND(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint32_t *carry){
    arm_write_register(p, Rd, shifter_operand & Rn);
    *carry = shifter_operand & Rn;
}

void ins_EOR(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint32_t *carry){
    arm_write_register(p, Rd, shifter_operand ^ Rn);
    *carry = shifter_operand ^ Rn;
}

void ins_SUB(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint32_t *carry){
    arm_write_register(p, Rd, shifter_operand - Rn);
    *carry = shifter_operand - Rn;
}

void ins_RSB(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint32_t *carry){
    arm_write_register(p, Rd, Rn - shifter_operand);
    *carry = shifter_operand - Rn;
}

void ins_ADD(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint32_t *carry){
    arm_write_register(p, Rd, shifter_operand + Rn);
    *carry = shifter_operand + Rn;
}

int ins_ADC(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint32_t *carry){
	return UNDEFINED_INSTRUCTION;
}

int ins_SBC(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint32_t *carry){
	return UNDEFINED_INSTRUCTION;
}

int ins_RSC(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint32_t *carry){
	return UNDEFINED_INSTRUCTION;
}

int ins_TST(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint32_t *carry){
	return UNDEFINED_INSTRUCTION;
}

int ins_TEQ(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint32_t *carry){
	return UNDEFINED_INSTRUCTION;
}

int ins_CMP(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint32_t *carry){
	return UNDEFINED_INSTRUCTION;
}

int ins_CMN(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint32_t *carry){
	return UNDEFINED_INSTRUCTION;
}

void ins_ORR(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint32_t *carry){
    arm_write_register(p, Rd, shifter_operand | Rn);
    *carry = shifter_operand | Rn;
}

void ins_MOV(arm_core p, uint32_t ins, uint32_t Rd, uint32_t shifter_operand, uint32_t *carry){
    arm_write_register(p, Rd, shifter_operand);
    *carry = shifter_operand;
}

int ins_BIC(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint32_t *carry){
	return UNDEFINED_INSTRUCTION;
}

void ins_MVN(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint32_t *carry){
	arm_write_register(p, Rd, ~shifter_operand);
    *carry = ~shifter_operand;	
}

uint32_t find_shifter_operand(arm_core p, uint32_t ins, uint32_t *shifter_carry_out){
    printf("enter in find shiter op\n");
	uint32_t shifter_operand, I, Rm, Rs, immed8, rotateimm, shift, bit4, shiftimm, Rs70, Rm31;
	I = get_bit(ins, 25);
	immed8 = get_bits(ins, 7, 0);
	rotateimm = get_bits(ins, 11, 8);
	shiftimm = get_bits(ins, 11, 7);
	shift = get_bits(ins, 6, 5);
	bit4 = get_bit(ins, 4);
	Rm = arm_read_register(p, get_bits(ins, 3, 0));
	Rs = arm_read_register(p, get_bits(ins, 11, 8));
	Rs70 = get_bits(Rs, 7, 0);
	Rm31 = get_bit(Rm, 31);
	if(I){ //32 bit immediate
        printf("\n immediate \n");
		shifter_operand = ror(immed8 ,rotateimm*2);
	}
	else{
        printf("\n register\n");
		if(bit4){ //immediate shifts
            printf("\n if immediate shifts\n");
			switch(shift){
				//LSL
				case 0b00:
					if(shiftimm == 0){
						shifter_operand = Rm;
						*shifter_carry_out = get_bit(ins, C);
					}
					else{
						shifter_operand = lsl(Rm, shiftimm);
						*shifter_carry_out = get_bit(arm_read_usr_register(p,Rm), (32 - shiftimm));
					}		
				break;
				//LSR
				case 0b01:
					if(shiftimm == 0){
						shifter_operand = 0;
						*shifter_carry_out = Rm31;
					}
					else{
						shifter_operand = lsr(Rm, shiftimm);
						*shifter_carry_out = get_bit(arm_read_usr_register(p,Rm), (shiftimm - 1));
					}
				break;
				//ASR
				case 0b10:
					if(shiftimm == 0){
						if(Rm31 == 0){
							shifter_operand = 0;
							*shifter_carry_out = Rm31;
						}
						else{
							shifter_operand = 0xFFFFFFFF;
							*shifter_carry_out = Rm31;
						}
					}
					else{
						shifter_operand = asr(Rm, shiftimm);
						*shifter_carry_out = get_bit(arm_read_usr_register(p,Rm), (shiftimm - 1));
					}
					
				break;
				//ROR
				case 0b11:
					if(shiftimm == 0){

					}
					else{
						shifter_operand = ror(Rm, shiftimm);
						*shifter_carry_out = get_bit(arm_read_usr_register(p,Rm), (shiftimm - 1));
					}
				break;
			}
		}
		else{ //register shifts 
            printf("\n else register shifts\n");
			switch(shift){
				//LSL
				case 0b00:
					if(Rs70 == 0){
						shifter_operand = Rm;
						*shifter_carry_out = get_bit(ins, C);
					}
					else{
						if(Rs70 < 32){
							shifter_operand = lsl(Rm,Rs70);
							*shifter_carry_out = get_bit(arm_read_usr_register(p,Rm), (32 - Rs70));
						}
						else{
							if(Rs70 == 32){
								shifter_operand = 0;
								*shifter_carry_out = get_bit(Rm, 0);
							}
							else{
								shifter_operand = 0;
								*shifter_carry_out = 0;
							}
						}
					}
				break;
				//LSR
				case 0b01:
					if(Rs70 == 0){
						shifter_operand = Rm;
						*shifter_carry_out = get_bit(ins, C);
					}
					else{
						if(Rs70 < 32){
							shifter_operand = lsr(Rm, Rs70);
							*shifter_carry_out = get_bit(arm_read_usr_register(p,Rm), (Rs70 - 1));
						}
						else{
							if(Rs70 == 32){
								shifter_operand = 0;
								*shifter_carry_out = Rm31;
							}
							else{
								shifter_operand = 0;
								*shifter_carry_out = 0;
							}
						}
					}
				break;
				//ASR
				case 0b10:
					if(Rs70 == 0){
						shifter_operand = Rm;
						*shifter_carry_out = get_bit(ins, C);
					}
					else{
						if(Rs70 < 32){
							shifter_operand = asr(Rm, Rs70);
							*shifter_carry_out = get_bit(arm_read_usr_register(p,Rm), (Rs70 - 1));
						}
						else{
							if(Rs70 == 32){
								shifter_operand = 0;
								*shifter_carry_out = Rm31;
							}
							else{
								shifter_operand = 0xFFFFFFFF;
								*shifter_carry_out = Rm31;
							}
						}
					}
				break;
				//ROR
				case 0b11:
					if(Rs70 == 0){
						shifter_operand = Rm;
						*shifter_carry_out = get_bit(ins, C);
					}
					else{
						if(get_bits(Rs, 4, 0) == 0){
							shifter_operand = Rm;
							*shifter_carry_out = Rm31;
						}
						else{
							shifter_operand = ror(Rm, get_bits(Rs, 4, 0));
							*shifter_carry_out = get_bit(arm_read_usr_register(p,Rm), (get_bits(Rs, 4, 0) - 1));
						}
					}
				break;
			}
		}
	}

	return shifter_operand;
}