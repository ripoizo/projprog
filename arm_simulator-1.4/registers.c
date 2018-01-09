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
#include "registers.h"
#include "arm_constants.h"
#include "util.h"
#include <stdlib.h>

struct registers_data {
	uint32_t cpsr; //registre 16
	uint32_t spsr; //registre 17
	uint32_t *registre; //registres 0 à 15
	uint8_t mode;
};

registers registers_create() {
    registers r;
    r = malloc(sizeof(struct registers_data));
    (*r).registre=(uint32_t *) malloc(sizeof(uint32_t)*16);
    r->mode=USR;
    return r;

}

void registers_destroy(registers r) {
	free((*r).registre);
	free(r);
}

uint8_t get_mode(registers r) {
    return r->mode;
} 

int current_mode_has_spsr(registers r) {
	if(get_mode(r)==USR||get_mode(r)==SYS){
		return 1;
	}
    return 0;
}

int in_a_privileged_mode(registers r) {
	uint8_t mode;
	mode = get_mode(r);
	if(mode==USR){
		return 1;
	}
    return 0;
}

uint32_t read_register(registers r, uint8_t reg) {

    return (*r).registre[reg];
}

uint32_t read_usr_register(registers r, uint8_t reg) {
    return (*r).registre[reg];
}

uint32_t read_cpsr(registers r) {
    return r->cpsr;
}

uint32_t read_spsr(registers r) {
    return r->spsr;
}

void write_register(registers r, uint8_t reg, uint32_t value) {
  
    (*r).registre[reg]=value;
}

void write_usr_register(registers r, uint8_t reg, uint32_t value) {
	(*r).registre[reg]=value;
}

void write_cpsr(registers r, uint32_t value) {
	r->cpsr=value;
}

void write_spsr(registers r, uint32_t value) {
	r->spsr=value;
}
