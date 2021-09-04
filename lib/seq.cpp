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
	this->nbits=0;
	this->is_rev=false;
	this->is_comp=false;
	this->set_miscomplemented_encoding();
	this->data.reset(new char[0]);
	// note that decode is not defined
};

seq::seq(std::string & s,encode_type in_e_type,mol_type in_m_type){
	this->e_type=in_e_type;
	this->m_type=in_m_type;
	//std::cout << "initialize seq";
	this->set_encode_parameters(s);
	this->encode(s);
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
			this->data[i] = ~ this->data[i] ;
		}
	}else{
		std::cerr << "Protein sequence cannot be complemented\n";
		exit(1);
	};
	this->is_comp=!this->is_comp;
	this->set_miscomplemented_encoding();
};

void seq::reverse_complement(){
	this->reverse();
	this->complement();
};

void seq::operator = (std::string & s){
	this->e_type=enc_UNDEFINED;
	this->m_type=mol_UNDEFINED;
	this->set_encode_parameters(s);
	this->encode(s);
};

// START Data position iterator managing the reverse state

uint32_t seq::get_begin_data_pos(){
	if (this->is_rev){
		return this->n_data-1;
	}else{
		return 0;
	}
};

void seq::increment_begin_data_pos(uint32_t& i){
	if (this->is_rev){
		i--;
	}else{
		i++;
	}
};

bool seq::is_data_pos_valid(uint32_t& i){
	if ( i<this->n_data ){ // equivalent also to a "human" i<0 since (0-1) will become the max value for an unsigned integer
		return true;
	}else{
		return false;
	}
};

// END Data position iterator managing the reverse state

void seq::set_encode_parameters(std::string& s){
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

	this->nbits=encode_type2nbits[this->e_type] ;

	this->n_bytes = (length*this->nbits) / CHAR_BIT ;
	this->n_data = length;
	if ((length*this->nbits) % CHAR_BIT){ this->n_bytes++; } ;
	this->data.reset(new char[this->n_bytes]);
	this->is_rev=false;
	this->is_comp=false;
	switch(this->e_type){
	case NUC_2BITS:
		this->decode_e_type = [=](uint32_t in_k){return this->decode_NUC_2BITS(in_k);};
		this->encode_e_type = [=](char& in_c, char& in_b){this->encode_NUC_2BITS(in_c,in_b);};
	break;
	case NUC_4BITS:
			this->decode_e_type = [=](uint32_t in_k){return this->decode_NUC_4BITS(in_k);};
			this->encode_e_type = [=](char& in_c, char& in_b){this->encode_NUC_4BITS(in_c,in_b);};
		break;
	default: break;
	};
	this->set_miscomplemented_encoding();

};

void seq::set_miscomplemented_encoding(){
	if (this->is_comp){
		this->miscomplemented_encoding_ini_n='-';
		this->miscomplemented_encoding_ini_gap='N';
		this->miscomplemented_encoding_ini_s='W';
		this->miscomplemented_encoding_ini_w='S';
	}else{
		this->miscomplemented_encoding_ini_n='N';
		this->miscomplemented_encoding_ini_gap='-';
		this->miscomplemented_encoding_ini_s='S';
		this->miscomplemented_encoding_ini_w='W';
	}
};

void seq::encode_NUC_2BITS(char& c, char& b){
	b = b << 2;
	switch(toupper(c)){
	case 'A': b |= 0b00; break;
	case 'C': b |= 0b01; break;
	case 'G': b |= 0b10; break;
	case 'T': b |= 0b11; break;
	default:break;
	};
}

void seq::encode_NUC_4BITS(char& c, char& b){
	b = b << 4;
	switch(toupper(c)){
	case 'A': b |= 0b0000; break;
	case 'C': b |= 0b0001; break;
	case 'R': b |= 0b0010; break;
	case 'K': b |= 0b0011; break;
	case 'B': b |= 0b0100; break;
	case 'D': b |= 0b0101; break;
	case 'S': b |= 0b0110; break;
	case 'N': b |= 0b0111; break;
	case '-': b |= 0b1000; break;
	case 'W': b |= 0b1001; break;
	case 'H': b |= 0b1010; break;
	case 'V': b |= 0b1011; break;
	case 'M': b |= 0b1100; break;
	case 'Y': b |= 0b1101; break;
	case 'G': b |= 0b1110; break;
	case 'T': b |= 0b1111; break;
	default:break;
	};
}

void seq::encode(std::string& s){
	for (uint32_t i=0;i<this->n_data;i++){

		this->encode_e_type(s[i],this->data[(this->nbits * i) / CHAR_BIT]);
	};
	// shift the final bits if required
	char nshift=(CHAR_BIT * this->n_bytes)-(this->nbits * this->n_data);
		if (nshift){ this->data[this->n_bytes-1]=data[this->n_bytes-1] << nshift;};
};

char seq::decode_NUC_2BITS(uint32_t& i){
	char r;
	char nshift=6 - 2 * (i%4);
	char byte= this->data[i/4] >> nshift;
	switch(byte & 0b00000011){
	case 0b00: r='A'; break;
	case 0b01: r='C'; break;
	case 0b10: r='G'; break;
	case 0b11: r='T'; break;
	default:break;
	};
	return(r);
}

char seq::decode_NUC_4BITS(uint32_t& i){
	char r;

	char nshift=4 * (1 - i%2);
	char byte=this->data[i/2]>>nshift;

	switch(0b00001111 & byte){
	case 0b0000: r='A'; break;
	case 0b0001: r='C'; break;
	case 0b0010: r='R'; break;
	case 0b0011: r='K'; break;
	case 0b0100: r='B'; break;
	case 0b0101: r='D'; break;
	case 0b0110: r=this->miscomplemented_encoding_ini_s; break;
	case 0b0111: r=this->miscomplemented_encoding_ini_n; break;
	case 0b1000: r=this->miscomplemented_encoding_ini_gap; break;
	case 0b1001: r=this->miscomplemented_encoding_ini_w; break;
	case 0b1010: r='H'; break;
	case 0b1011: r='V'; break;
	case 0b1100: r='M'; break;
	case 0b1101: r='Y'; break;
	case 0b1110: r='G'; break;
	case 0b1111: r='T'; break;
	default:break;
	};
	return(r);
}

std::string seq::decode(){
	std::string r = "";
	for(uint32_t i=this->get_begin_data_pos(); this->is_data_pos_valid(i) ; this->increment_begin_data_pos(i) ){
		r.push_back(this->decode_e_type(i));
	};

	return(r);
};

#undef PARSE_STR_ENCODING

