/*
 * seq.cpp
 *
 *  Created on: Aug 31, 2021
 *      Author: jfouret
 */

#include <seq.hpp>

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

static void unexpected_seq_char(char c){
	switch (c){
		case '\n': case '\0':
			break;
		case 'D':case 'Y':
			std::cerr << "Unexpected char ('" << c << "') when encoding a biological sequence (D does not match any amino acid)"; exit(1);
			break;
		default:
			std::cerr << "Unexpected char ('" << c << "') when encoding a biological sequence"; exit(1);
			break;
	};
};

// Definition of objects declared in header

seq::seq(){
	this->e_type=enc_UNDEFINED;
	this->m_type=mol_UNDEFINED;
	this->n_data=0;
	this->n_bytes=0;
	this->is_rev=false;
	this->data.reset(new char[0]);
};

seq::seq(std::string s,encode_type e_type,mol_type m_type){
	this->e_type=e_type;
	this->m_type=m_type;
	std::cout << "initialize seq";
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
		for (ulong i=0;i<this->n_bytes;i++){
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
	this->e_type=e_type;
	this->m_type=m_type;
	//std::cout << "initialize seq";
	this->set_encode_parameters(s);
	this->encode(s);
	this->is_rev=false;
};

void seq::set_encode_parameters(std::string s){
	ulong length=0 ;

	for (char const &c: s) {

		switch(this->e_type){
			case (enc_UNDEFINED):
				switch(toupper(c)){
					case 'A':case 'T':case 'C':case 'G':
						this->e_type=NUC_2BITS;
						this->m_type=DNA;
						break;
					case 'U':
						this->e_type=NUC_2BITS;
						this->m_type=RNA;
					break;
					case 'N':case '-':
						this->e_type=NUC_3BITS;
						this->m_type=DNA;
						break;
					case 'R':case 'K':case 'B':case 'D':case 'S':case 'W':case 'H':case 'V':case 'M':case 'Y':
						this->e_type=NUC_4BITS;
						this->m_type=DNA;
						break;
					case 'E':case 'F':case 'I':case 'L':case 'P':case 'Q':case 'X':case 'Z':
						this->e_type=PRO_5BITS;
						this->m_type=PROTEIN;
						break;
					default:
						unexpected_seq_char(toupper(c));
				};
				break;
			case (NUC_2BITS):
				switch(toupper(c)){
					case 'A':case 'T':case 'C':case 'G':
						break;
					case 'U':
						if(this->m_type==DNA){this->m_type=RNA;};
						break;
					case 'N':case '-':
						this->e_type=NUC_3BITS;
						break;
					case 'R':case 'K':case 'B':case 'D':case 'S':case 'W':case 'H':case 'V':case 'M':case 'Y':
						this->e_type=NUC_4BITS;
						break;
					case 'E':case 'F':case 'I':case 'L':case 'P':case 'Q':case 'X':case 'Z':case '*':case '!':
						this->e_type=PRO_5BITS;
						this->m_type=PROTEIN;
						break;
					default:
						unexpected_seq_char(toupper(c));
				};
				break;
			case (NUC_3BITS):
				switch(toupper(c)){
					case 'A':case 'T':case 'C':case 'G':
						break;
					case 'U':
						if(this->m_type==DNA){this->m_type=RNA;};
						break;
					case 'N':case '-':
						break;
					case 'R':case 'K':case 'B':case 'D':case 'S':case 'W':case 'H':case 'V':case 'M':case 'Y':
						this->e_type=NUC_4BITS;
						break;
					case 'E':case 'F':case 'I':case 'L':case 'P':case 'Q':case 'X':case 'Z':case '*':case '!':
						this->e_type=PRO_5BITS;
						this->m_type=PROTEIN;
						break;
					default:
						unexpected_seq_char(toupper(c));
				};
				break;
			case (NUC_4BITS):
				switch(toupper(c)){
					case 'A':case 'T':case 'C':case 'G':
						break;
					case 'U':
						if(this->m_type==DNA){this->m_type=RNA;};
						break;
					case 'N':case '-':
						break;
					case 'R':case 'K':case 'B':case 'D':case 'S':case 'W':case 'H':case 'V':case 'M':case 'Y':
						break;
					case 'E':case 'F':case 'I':case 'L':case 'P':case 'Q':case 'X':case 'Z':case '*':case '!':
						this->e_type=PRO_5BITS;
						this->m_type=PROTEIN;
						break;
					default:
						unexpected_seq_char(toupper(c));
				};
				break;
			case (PRO_5BITS):
				switch(toupper(c)){
					case 'A':case 'T':case 'C':case 'G':
						break;
					case 'U':
						break;
					case 'N':case '-':
						break;
					case 'R':case 'K':case 'B':case 'S':case 'W':case 'H':case 'V':case 'M':case 'Y':
						break;
					case 'E':case 'F':case 'I':case 'L':case 'P':case 'Q':case 'X':case 'Z':case '*':case '!':
						break;
					default:
						unexpected_seq_char(toupper(c));
				};
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
};

void seq::encode(std::string s){
	for (ulong i=0;i<this->n_data;i++){
		switch(this->e_type){
		case NUC_2BITS:
			this->data[i/4]=this->data[i/4] << 2;
			//std::cout << toupper(s[i]) << "\n";
			//std::cout << (std::bitset<8>) data[i/4] << "\n";
			switch(toupper(s[i])){
			case 'A': this->data[i/4] |= 0b00; break; // b2_A is 00
			case 'C': this->data[i/4] |= 0b01; break; // b2_C is 01
			case 'G': this->data[i/4] |= 0b10; break; // b2_G is 10
			case 'T': this->data[i/4] |= 0b11; break; // b2_t is 11
			default:break;
			};
			break;
		//case NUC_3BITS:
			//char b1;
			//char b2=0;
			// get byte id
			//this->data[i/4]=this->data[i/4] << 2;
			//break;
		default:break;
		};
	};

	// shift the final bits if required

	char nshift;

	switch(this->e_type){
		case NUC_2BITS:
			nshift=(CHAR_BIT*this->n_bytes)-(2*this->n_data);
			if (nshift){ this->data[this->n_bytes-1]=data[this->n_bytes-1] << nshift;};
			break;
		default:break;
	};
};

char seq::decode(ulong j){
	char r;
	char byte;
	char nshift;
	ulong i;
	if (this->is_rev){
		i=this->n_data-j-1;
	}else{
		i=j;
	};
	//std::cerr << "DECODE i=" << i << "\n";
	switch(this->e_type){
	case NUC_2BITS:
		nshift=6-2*(i%4);
		byte=this->data[i/4]>>nshift;
		switch(0b00000011 & byte){
		case 0b00: r='A'; break;
		case 0b01: r='C'; break;
		case 0b10: r='G'; break;
		case 0b11: r='T'; break;
		default:break;
		};
		break;
	default:break;
	};
	return(r);
}

std::string seq::decode(){
	std::string r = "";

	for(ulong i=0; i < this->n_data ;i++){
		//std::cerr << "DECODE i=" << i << "\n";
		r.push_back(this->decode(i));
	};

	return(r);
};






