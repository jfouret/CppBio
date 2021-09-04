/*
 * seq.cpp
 *
 *  Created on: Aug 31, 2021
 *      Author: jfouret
 */

#include <seq.hpp>

// MACRO to be used within seq::set_encode_parameters only

#define PARSE_STR_ENCODING(XA,XB,XC,XD,XE,XF) {\
switch(toupper(c)){\
	case 'A':case 'T':case 'C':case 'G':\
		XA break;\
	case 'U':\
		XB break;\
	case 'N':case '-':\
		XC break;\
	case 'R':case 'K':case 'B':case 'S':case 'W':case 'H':case 'V':case 'M':case 'Y':\
		XD break;\
	case 'D':\
		XE break;\
	case 'E':case 'F':case 'I':case 'L':case 'P':case 'Q':case 'X':case 'Z':case '*':case '!':\
		XF break;\
	default:\
		throw std::invalid_argument( "Unexpected char when parsing the biological sequence" );\
}}

// Make it so that the char is given where the error is thrown.

using namespace cppbio;

// Declaration and Definition of objects used only within this scope

static std::map<encode_type,char> encode_type2nbits {
	{NUC_2BITS,2},
	{NUC_3BITS,3},
	{NUC_4BITS,4},
	{PRO_5BITS,5}
 };

static std::map<encode_type,mol_type> encode_type2mol_type {
	{NUC_2BITS,DNA},
	{NUC_3BITS,DNA},
	{NUC_4BITS,DNA},
	{PRO_5BITS,PROTEIN}
 };

// Definition of objects declared in header

seq::seq(){
	this->e_type=enc_UNDEFINED;
	this->m_type=mol_UNDEFINED;
	this->n_data=0;
	this->n_bytes=0;
	this->is_rev=false;
	this->data.reset(new char[0]);
	// note that decode is not defined
};

seq::seq(std::string s,encode_type in_e_type,mol_type in_m_type){
	this->e_type=in_e_type;
	this->m_type=in_m_type;
	//std::cout << "initialize seq";
	this->set_encode_parameters(s);
	this->encode(s);
	this->is_rev=false;
};

std::string seq::get_string(){
	return(this->decode());
};

void seq::reverse(){
	this->is_rev=!this->is_rev;
};

void seq::complement(){
	if (this->m_type!= PROTEIN){
		for (uint32_t i=0;i<this->n_bytes;i++){
			//std::cerr << "complement" << std::endl;
			this->data[i] = ~ this->data[i] ;
		}
	}else{
		std::cerr << "Protein sequence cannot be complemented\n";
		exit(1);
	};

};

void seq::reverse_complement(){
	this->reverse();
	this->complement();
};

void seq::operator = (std::string & s){
	this->e_type=enc_UNDEFINED;
	this->m_type=mol_UNDEFINED;
	//std::cout << "initialize seq";
	this->set_encode_parameters(s);
	this->encode(s);
	this->is_rev=false;
};

void seq::set_encode_parameters(std::string s){
	uint32_t length=0 ;
	for (char const &c: s) {
		switch(this->e_type){
			case (enc_UNDEFINED):
				PARSE_STR_ENCODING(
						this->e_type=NUC_2BITS;this->m_type=DNA;,
						this->e_type=NUC_2BITS;this->m_type=RNA;,
						this->e_type=NUC_3BITS;this->m_type=DNA;,
						this->e_type=NUC_4BITS;this->m_type=DNA;,
						this->e_type=NUC_4BITS;this->m_type=DNA;,
						this->e_type=PRO_5BITS;this->m_type=PROTEIN;);
				break;
			case (NUC_2BITS):
				PARSE_STR_ENCODING(
						,
						if (this->m_type==DNA){this->m_type=RNA;};,
						this->e_type=NUC_3BITS;,
						this->e_type=NUC_4BITS;,
						this->e_type=NUC_4BITS;,
						this->e_type=PRO_5BITS;this->m_type=PROTEIN;);
				break;
			case (NUC_3BITS):
				PARSE_STR_ENCODING(
						,
						if (this->m_type==DNA){this->m_type=RNA;};,
						,
						this->e_type=NUC_4BITS;,
						this->e_type=NUC_4BITS;,
						this->e_type=PRO_5BITS;this->m_type=PROTEIN;);
				break;
			case (NUC_4BITS):
				PARSE_STR_ENCODING(
						,
						if (this->m_type==DNA){this->m_type=RNA;};,
						,
						,
						,
						this->e_type=PRO_5BITS;this->m_type=PROTEIN;);
				break;
			case (PRO_5BITS):
				PARSE_STR_ENCODING(
						,
						,
						,
						,
						throw std::invalid_argument( "Unexpected char 'D' when parsing the protein sequence" );,
						);
				break;
		};
		length++;
	}

	//std::cerr << "PARAMs SET LENGTH=" << length << "\n";

	char nbits=encode_type2nbits[this->e_type] ;
	this->n_bytes = (length*nbits) / CHAR_BIT ;
	this->n_data = length;
	if ((length*nbits) % CHAR_BIT){ this->n_bytes++; } ;
	this->data.reset(new char[this->n_bytes]);

	switch(this->e_type){
	case NUC_2BITS:
		this->decode_e_type = [=](uint32_t in_k){return this->decode_NUC_2BITS(in_k);};
		this->encode_e_type = [=](char& in_c, char& in_b){this->encode_NUC_2BITS(in_c,in_b);};
	break;
	default: break;
	};

};

void seq::encode_NUC_2BITS(char& c, char& b){
	b = b << 2;
	switch(toupper(c)){
	case 'A': b |= 0b00; break; // b2_A is 00
	case 'C': b |= 0b01; break; // b2_C is 01
	case 'G': b |= 0b10; break; // b2_G is 10
	case 'T': b |= 0b11; break; // b2_t is 11
	default:break;
	};
}

void seq::encode(std::string s){
	for (uint32_t i=0;i<this->n_data;i++){
		this->encode_NUC_2BITS(s[i],this->data[i/4]);
	};
	// shift the final bits if required
	char nshift=(CHAR_BIT*this->n_bytes)-(encode_type2nbits[this->e_type]*this->n_data);
		if (nshift){ this->data[this->n_bytes-1]=data[this->n_bytes-1] << nshift;};
};

char seq::decode_NUC_2BITS(uint32_t j){
	char r;
	char byte;
	char nshift;
	uint32_t i;
	// to do replace this by an iterator
	if (this->is_rev){
		i=this->n_data-j-1;
	}else{
		i=j;
	};
	//std::cerr << "DECODE i=" << i << "\n";
	nshift=6-2*(i%4);
	byte=this->data[i/4]>>nshift;
	switch(0b00000011 & byte){
	case 0b00: r='A'; break;
	case 0b01: r='C'; break;
	case 0b10: r='G'; break;
	case 0b11: r='T'; break;
	default:break;
	};
	return(r);
}

std::string seq::decode(){
	std::string r = "";
	for(uint32_t i=0; i < this->n_data ;i++){
		//std::cerr << "DECODE i=" << i << "\n";
		r.push_back(this->decode_e_type(i));
	};

	return(r);
};

#undef PARSE_STR_ENCODING

