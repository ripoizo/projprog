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
#ifndef __ARM_DATA_PROCESSING_H__
#define __ARM_DATA_PROCESSING_H__
#include <stdint.h>
#include "arm_core.h"

int arm_data_processing_shift(arm_core p, uint32_t ins);
int arm_data_processing_immediate_msr(arm_core p, uint32_t ins);

void ins_AND(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry);
void ins_EOR(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry);
void ins_SUB(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry);
void ins_RSB(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry);
void ins_ADD(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry);
int ins_ADC(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry);
int ins_SBC(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry);
int ins_RSC(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry);
int ins_TST(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry);
int ins_TEQ(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry);
int ins_CMP(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry);
int ins_CMN(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry);
void ins_ORR(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry);
void ins_MOV(arm_core p, uint32_t ins,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry);
int ins_BIC(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry);
void ins_MVN(arm_core p, uint32_t ins,uint32_t Rn,uint32_t Rd,uint32_t shifter_operand,uint64_t *carry);

void put_flag(uint32_t cpsr,uint32_t Rd,uint64_t *carry, int typeflags);
uint32_t find_shifter_operand(arm_core p, uint32_t ins, uint32_t *shifter_carry_out);

#endif
