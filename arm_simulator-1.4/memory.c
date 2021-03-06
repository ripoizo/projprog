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
#include <stdlib.h>
#include <stdio.h>
#include "memory.h"
#include "util.h"

struct memory_data {
	uint8_t *classeur;
	size_t size ;
	uint8_t big_endian;
};

memory memory_create(size_t size, int is_big_endian) {
	memory m;
	m=malloc(sizeof(struct memory_data));
	m->classeur=malloc(sizeof(uint8_t)*size*4);
	m->big_endian=is_big_endian;
	m->size=size;
	return m;
}

size_t memory_get_size(memory mem) {
    return mem->size;
}

void memory_destroy(memory mem) {
	
	free( mem->classeur);
	free(mem);
}

int memory_read_byte(memory mem, uint32_t address, uint8_t *value) {
	if(address<0 || address/4>=mem->size){
		return 1;
	}
	*value=mem->classeur[address];	
	return 0;
}

int memory_read_half(memory mem, uint32_t address, uint16_t *value) {
	if(address<0 || address/4>=mem->size || address%2!=0){
		return 1;
	}
	uint8_t a, b;
	*value=0;
	if( mem->big_endian ){
		a= mem->classeur[address]  ;
		b= mem->classeur[address+1];
		*value= *value | a<<8;
		*value= *value | b;
	}else{
		a= mem->classeur[address]  ;
		b= mem->classeur[address+1];
		*value= *value | b<<8;
		*value= *value | a;
	}
	
	return 0;
}

int memory_read_word(memory mem, uint32_t address, uint32_t *value) {
	if(address<0 || address/4>=mem->size || address%4!=0){
		return 1;
	}
	uint8_t a,b,c,d;
	a= mem->classeur[address]  ;
	b= mem->classeur[address+1];
	c= mem->classeur[address+2]  ;
	d= mem->classeur[address+3];
	*value=0;
	if( mem->big_endian ){
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
	if(address<0 || address/4>=mem->size){
		return 1;
	}
	if( mem->big_endian ){
		mem->classeur[address]=set_bits( mem->classeur[address], 7,0, value);
	}else{
		mem->classeur[address]=set_bits( mem->classeur[address], 7, 0, value);
	}
	return 0;
}

int memory_write_half(memory mem, uint32_t address, uint16_t value) {
	if(address<0 || address/4>=mem->size || address%2!=0){
		return 1;
	}
	uint8_t a, b;
	if( mem->big_endian ){
		a=get_bits(value, 7,0);
		b=get_bits(value, 15,8);
		mem->classeur[address]=0;
		mem->classeur[address]=b;
		mem->classeur[address+1]=a;
		
	}else{
		a=get_bits(value, 7,0);
		b=get_bits(value, 15,8);
		mem->classeur[address]=0;
		mem->classeur[address]=a;
		mem->classeur[address+1]=b;
		
	}
	return 0;
}


int memory_write_word(memory mem, uint32_t address, uint32_t value) {
	if(address<0 || address/4>=mem->size || address%4!=0){
		return 1;
	}
	uint8_t a, b, c, d;
	if( mem->big_endian ){
		a=get_bits(value , 7, 0) ;
		b=get_bits(value, 15, 8);
		c=get_bits(value, 23, 16);
		d=get_bits(value, 31, 24);
		mem->classeur[address]=0;
		mem->classeur[address]=d;
		mem->classeur[address+1]=c;
		mem->classeur[address+2]=b;
		mem->classeur[address+3]=a;
	}else{
		a=get_bits(value ,7,0) ;
		b=get_bits(value, 15, 8);
		c=get_bits(value, 23, 16);
		d=get_bits(value, 31, 24);
		mem->classeur[address]=0;
		mem->classeur[address]=a;
		mem->classeur[address+1]=b;
		mem->classeur[address+2]=c;
		mem->classeur[address+3]=d;
	}
	return 0;
}













