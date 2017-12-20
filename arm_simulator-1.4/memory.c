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
#include <stdlib.h>
#include <stdio.h>
#include "memory.h"
#include "util.h"

struct memory_data {
	uint8_t *classeur;
	uint16_t size ;
	uint8_t big_endian;
};

memory memory_create(size_t size, int is_big_endian) {
	memory m;
	m=malloc(sizeof(memory));
	(*m).classeur=malloc(sizeof(uint8_t)*size*4);
	(*m).big_endian=is_big_endian;
	(*m).size=size;
	return m;
}

size_t memory_get_size(memory mem) {
    return mem->size;
}

void memory_destroy(memory mem) {
	
	free( (*mem).classeur);
	free(mem);
}

int memory_read_byte(memory mem, uint32_t address, uint8_t *value) {
	*value=get_bits( (*mem).classeur[address], 7, 0);	
	return 0;
}

int memory_read_half(memory mem, uint32_t address, uint16_t *value) {
	uint8_t a, b;
	*value=0;
	if( (*mem).big_endian ){
		a=get_bits( (*mem).classeur[address], 7, 0);
		b=get_bits( (*mem).classeur[address+1], 7,0);
		*value= *value | a<<8;
		*value= *value | b;
	}else{
		a=get_bits( (*mem).classeur[address], 7, 0);
		b=get_bits( (*mem).classeur[address+1], 7,0);
		*value= *value | b<<8;
		*value= *value | a;
	}
	
	return 0;
}

int memory_read_word(memory mem, uint32_t address, uint32_t *value) {
	uint8_t a,b,c,d;
	a=get_bits( (*mem).classeur[address], 7, 0);
	b=get_bits( (*mem).classeur[address+1], 7,0);
	c=get_bits( (*mem).classeur[address+2], 7, 0);
	d=get_bits( (*mem).classeur[address+3], 7,0);
	*value=0;
	if( (*mem).big_endian ){
		*value=*value|a<<24;
		*value=*value|b<<16;
		*value=*value|c<<8;
		*value=*value|d;
	}else{
		*value=*value|d<<24;
		*value=*value|c<<16;
		*value=*value|b<<8;
		*value=*value|a;
	}
	return 0;
}

int memory_write_byte(memory mem, uint32_t address, uint8_t value) {
	if( (*mem).big_endian ){
		(*mem).classeur[address]=set_bits( (*mem).classeur[address], 7,0, value);
	}else{
		(*mem).classeur[address]=set_bits( (*mem).classeur[address], 7, 0, value);
	}
	return 0;
}

int memory_write_half(memory mem, uint32_t address, uint16_t value) {
	uint8_t a, b;
	if( (*mem).big_endian ){
		a=get_bits(value, 7,0);
		b=get_bits(value, 15,8);
		(*mem).classeur[address]=0;
		(*mem).classeur[address]=b;
		(*mem).classeur[address+1]=a;
		
	}else{
		a=get_bits(value, 7,0);
		b=get_bits(value, 15,8);
		(*mem).classeur[address]=0;
		(*mem).classeur[address]=a;
		(*mem).classeur[address+1]=b;
		
	}
	return 0;
}


int memory_write_word(memory mem, uint32_t address, uint32_t value) {
	uint8_t a, b, c, d;
	if( (*mem).big_endian ){
		a=get_bits(value, 7, 0);
		b=get_bits(value, 15, 8);
		c=get_bits(value, 23, 16);
		d=get_bits(value, 31, 24);
		(*mem).classeur[address]=0;
		(*mem).classeur[address]=d;
		(*mem).classeur[address+1]=c;
		(*mem).classeur[address+2]=b;
		(*mem).classeur[address+3]=a;
	}else{
		a=get_bits(value, 7, 0);
		b=get_bits(value, 15, 8);
		c=get_bits(value, 23, 16);
		d=get_bits(value, 31, 24);
		(*mem).classeur[address]=0;
		(*mem).classeur[address]=a;
		(*mem).classeur[address+1]=b;
		(*mem).classeur[address+2]=c;
		(*mem).classeur[address+3]=d;
	}
	return 0;
}












