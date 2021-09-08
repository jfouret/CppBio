/*
 * seq.cpp
 *
 *  Created on: Aug 31, 2021
 *      Author: jfouret
 */

#include <seq.hpp>

// constant of byte-typed binary code often used
const std::byte b00 {0b00};
const std::byte b01 {0b01};
const std::byte b10 {0b10};
const std::byte b11 {0b11};

const std::byte b000 {0b000};
const std::byte b001 {0b001};
const std::byte b010 {0b010};
const std::byte b011 {0b011};
const std::byte b100 {0b100};
const std::byte b101 {0b101};
const std::byte b110 {0b110};
const std::byte b111 {0b111};

const std::byte b0000 {0b0000};
const std::byte b0001 {0b0001};
const std::byte b0010 {0b0010};
const std::byte b0011 {0b0011};
const std::byte b0100 {0b0100};
const std::byte b0101 {0b0101};
const std::byte b0110 {0b0110};
const std::byte b0111 {0b0111};
const std::byte b1000 {0b1000};
const std::byte b1001 {0b1001};
const std::byte b1010 {0b1010};
const std::byte b1011 {0b1011};
const std::byte b1100 {0b1100};
const std::byte b1101 {0b1101};
const std::byte b1110 {0b1110};
const std::byte b1111 {0b1111};

const std::byte b11111111 {0b11111111};
const std::byte b00000001 {0b00000001};
const std::byte b00000011 {0b00000011};
const std::byte b00000111 {0b00000111};
const std::byte b00001111 {0b00001111};

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

// CONSTRUCTORS
template<IsAnyOf T_uint>
seq<T_uint>::seq(){
	assert(typeid(T_uint)==typeid(uint8_t) || typeid(T_uint)==typeid(uint16_t) || typeid(T_uint)==typeid(uint32_t) || typeid(T_uint)==typeid(uint64_t));
	SPDLOG_DEBUG("seq::seq");
	this->e_type=enc_UNDEFINED;
	this->m_type=mol_UNDEFINED;
	this->n_data=0;
	this->n_bytes=0;
	this->nbits=0;
	this->is_rev=false;
	this->is_comp=false;
	this->set_miscomplemented_encoding();
	this->data.reset(new std::byte[0]);
	// note that decode is not defined
};
template<IsAnyOf T_uint>
seq<T_uint>::seq(std::string & s,encode_type in_e_type,mol_type in_m_type){
	SPDLOG_DEBUG("seq::seq with string");
	this->e_type=in_e_type;
	this->m_type=in_m_type;
	this->set_encode_parameters(s);
	this->encode_byte_array(s);
};


template<IsAnyOf T_uint>
seq<T_uint>::~seq(){
    SPDLOG_DEBUG("seq::~seq DESTRUCTOR CALLED");
}

// GETTERS
template<IsAnyOf T_uint>
std::string seq<T_uint>::get_string(){
	SPDLOG_DEBUG("seq::get_string");
	return(this->decode());
};

// MODIFIERS
template<IsAnyOf T_uint>
void seq<T_uint>::reverse(){
	SPDLOG_DEBUG("seq::reverse");
	this->is_rev=!this->is_rev;
};
template<IsAnyOf T_uint>
void seq<T_uint>::complement(){
	SPDLOG_DEBUG("seq::complement");
	if (this->m_type!= PROTEIN){
		for (T_uint i=0;i<this->n_bytes;i++){
			this->data[i] = ~ this->data[i] ;
		}
	}else{
		std::cerr << "Protein sequence cannot be complemented\n";
		exit(1);
	};
	this->is_comp=!this->is_comp;
	SPDLOG_DEBUG("seq::complement this->is_comp is now " + this->is_comp) ;
	if (this->e_type==NUC_4BITS) this->set_miscomplemented_encoding();
};
template<IsAnyOf T_uint>
void seq<T_uint>::reverse_complement(){
	SPDLOG_DEBUG("seq::reverse_complement");
	this->reverse();
	this->complement();
};

// OPERATORS
template<IsAnyOf T_uint>
void seq<T_uint>::operator = (std::string & s){
	SPDLOG_DEBUG("seq::operator =");
	this->e_type=enc_UNDEFINED;
	this->m_type=mol_UNDEFINED;
	this->set_encode_parameters(s);
	this->encode_byte_array(s);
};

// INTERNAL FUNCTIONS
template<IsAnyOf T_uint>
void seq<T_uint>::set_encode_parameters(std::string& s){
	assert(typeid(T_uint)==typeid(uint8_t) || typeid(T_uint)==typeid(uint16_t) || typeid(T_uint)==typeid(uint32_t) || typeid(T_uint)==typeid(uint64_t));
	SPDLOG_DEBUG("seq::set_encode_parameters");
	T_uint length=0;
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
	this->data.reset(new std::byte[this->n_bytes]);
	this->is_rev=false;
	this->is_comp=false;
	switch(this->e_type){
	case NUC_2BITS:
		this->decode_e_type = [this](std::byte in_k,miscomplemented_encoding mis_enc){return this->decode_NUC_2BITS(in_k);};
		this->right_append_bytecode_after_left_shift = [this](std::byte in_c, std::byte& in_b,uint8_t& in_k,encoding_astrideness in_astride){this->right_append_bytecode_after_left_shift_NUC_2BITS(in_c,in_b);};
		this->get_byte = [this](T_uint in_k){return this->get_byte_NUC_2BITS(in_k);};
		this->encode = [this](char in_k){return this->encode_NUC_2BITS(in_k);};
	break;
	case NUC_3BITS:
		this->decode_e_type = [this](std::byte in_k,miscomplemented_encoding mis_enc){return this->decode_NUC_3BITS(in_k);};
		this->right_append_bytecode_after_left_shift = [this](std::byte in_c, std::byte& in_b,uint8_t& in_k,encoding_astrideness in_astride){this->right_append_bytecode_after_left_shift_NUC_3BITS(in_c,in_b,in_k,in_astride);};
		this->get_byte = [this](T_uint in_k){return this->get_byte_NUC_3BITS(in_k);};
		this->encode = [this](char in_k){return this->encode_NUC_3BITS(in_k);};
	break;
	case NUC_4BITS:
		this->decode_e_type = [this](std::byte in_k,miscomplemented_encoding mis_enc){return this->decode_NUC_4BITS(in_k,mis_enc);};
		this->right_append_bytecode_after_left_shift = [this](std::byte in_c, std::byte& in_b,uint8_t& in_k,encoding_astrideness in_astride){this->right_append_bytecode_after_left_shift_NUC_4BITS(in_c,in_b);};
		this->get_byte = [this](T_uint in_k){return this->get_byte_NUC_4BITS(in_k);};
		this->encode = [this](char in_k){return this->encode_NUC_4BITS(in_k);};
	break;
	default: break;
	};
	this->set_miscomplemented_encoding();
};

template<IsAnyOf T_uint>
void seq<T_uint>::set_miscomplemented_encoding(){
	SPDLOG_DEBUG("seq::set_miscomplemented_encoding");
	if (this->is_comp){
		this->comp_dep_mis_enc.ini_n='-';
		this->comp_dep_mis_enc.ini_gap='N';
		this->comp_dep_mis_enc.ini_s='W';
		this->comp_dep_mis_enc.ini_w='S';
	}else{
		this->comp_dep_mis_enc.ini_n='N';
		this->comp_dep_mis_enc.ini_gap='-';
		this->comp_dep_mis_enc.ini_s='S';
		this->comp_dep_mis_enc.ini_w='W';
	}
};

// DATA POSITION ITERATION FUNCTIONS
template<IsAnyOf T_uint>
T_uint seq<T_uint>::get_begin_data_pos(){
	SPDLOG_DEBUG("seq::get_begin_data_pos");
	if (this->is_rev){
		return this->n_data-1;
	}else{
		return 0;
	}
};
template<IsAnyOf T_uint>
void seq<T_uint>::increment_begin_data_pos(T_uint& i){
	SPDLOG_DEBUG("seq::increment_begin_data_pos");
	if (this->is_rev){
		i--;
	}else{
		i++;
	}
};
template<IsAnyOf T_uint>
bool seq<T_uint>::is_data_pos_valid(T_uint& i){
	SPDLOG_DEBUG("seq::is_data_pos_valid");
	if ( i<this->n_data ){ // equivalent also to a "human" i<0 since (0-1) will become the max value for an unsigned integer
		return true;
	}else{
		return false;
	}
};

// ENCODING FUNCTIONS
template<IsAnyOf T_uint>
std::byte seq<T_uint>::encode_NUC_2BITS(char& c){
	switch(toupper(c)){
	case 'A': return b00; break;
	case 'C': return b01; break;
	case 'G': return b10; break;
	case 'T': return b11; break;
	default:std::string msg="Input char cannot be encoded as 2-bits DNA (ATCG only):";msg.push_back(c); throw std::invalid_argument(msg); break;
	};
};
template<IsAnyOf T_uint>
std::byte seq<T_uint>::encode_NUC_3BITS(char& c){
	switch(toupper(c)){
	case 'A': return(b000); break;
	case 'C': return(b001); break;
	case 'N': return(b010); break;
	case '-': return(b011); break;
	case 'G': return(b110); break;
	case 'T': return(b111); break;
	default:std::string msg="Input char cannot be encoded as 2-bits DNA (ATCG-N only):";msg.push_back(c); throw std::invalid_argument(msg); break;
	};
};
template<IsAnyOf T_uint>
std::byte seq<T_uint>::encode_NUC_4BITS(char& c){
	switch(toupper(c)){
		case 'A': return b0000; break;
		case 'C': return b0001; break;
		case 'R': return b0010; break;
		case 'K': return b0011; break;
		case 'B': return b0100; break;
		case 'D': return b0101; break;
		case 'S': return b0110; break;
		case 'N': return b0111; break;
		case '-': return b1000; break;
		case 'W': return b1001; break;
		case 'H': return b1010; break;
		case 'V': return b1011; break;
		case 'M': return b1100; break;
		case 'Y': return b1101; break;
		case 'G': return b1110; break;
		case 'T': return b1111; break;
		default:std::string msg="Input char cannot be encoded as 2-bits DNA (IUPAC letter and '-' for gap only):";msg.push_back(c); throw std::invalid_argument(msg); break;
		};
};
template<IsAnyOf T_uint>
void seq<T_uint>::right_append_bytecode_after_left_shift_NUC_2BITS(std::byte c, std::byte& b){
	SPDLOG_DEBUG("seq::encode_NUC_2BITS");
	b = b << 2;
	b |= c;
}
template<IsAnyOf T_uint>
void seq<T_uint>::right_append_bytecode_after_left_shift_NUC_3BITS(std::byte c, std::byte & b,uint8_t & nbits_in_byte,encoding_astrideness astrideness){
	SPDLOG_DEBUG("seq::encode_NUC_3BITS");
	uint8_t nbits=3;
	uint8_t shift=std::min(nbits_in_byte,nbits);
	b = b << shift;
	shift=nbits-shift;

	switch (astrideness){
	case unique_byte:
		b |= c;
	break;
	case astride_first_byte:
		b |= (c >> shift);
	break;
	case astride_second_byte:
		b |= (c & (b111 >> shift));
	break;
	};
}
template<IsAnyOf T_uint>
void seq<T_uint>::right_append_bytecode_after_left_shift_NUC_4BITS(std::byte c, std::byte& b){
	SPDLOG_DEBUG("seq::encode_NUC_4BITS");
	b = b << 4;
	b |= c;
}
template<IsAnyOf T_uint>
void seq<T_uint>::encode_byte_array(std::string& s){
	SPDLOG_DEBUG("seq::encode");
	uint8_t nbits_in_bytes_after_first_bit;
	switch(this->e_type){
	case NUC_2BITS: case NUC_4BITS:
		for (T_uint i=0;i<this->n_data;i++){
			this->right_append_bytecode_after_left_shift(encode(s[i]),this->data[(this->nbits * i) / CHAR_BIT],this->nbits,unique_byte);
		};
		break;
	case NUC_3BITS: case PRO_5BITS:
		for (T_uint i=0;i<this->n_data;i++){
			nbits_in_bytes_after_first_bit=CHAR_BIT - i*this->nbits % CHAR_BIT;

			if (nbits_in_bytes_after_first_bit<this->nbits){

				this->right_append_bytecode_after_left_shift(encode(s[i]),this->data[(this->nbits * i) / CHAR_BIT],nbits_in_bytes_after_first_bit,astride_first_byte);

				uint8_t nbits_in_the_next_byte=this->nbits-nbits_in_bytes_after_first_bit;

				this->right_append_bytecode_after_left_shift(encode(s[i]),this->data[1 + (this->nbits * i) / CHAR_BIT],nbits_in_the_next_byte,astride_second_byte);
			}else{
				this->right_append_bytecode_after_left_shift(encode(s[i]),this->data[(this->nbits * i) / CHAR_BIT],nbits_in_bytes_after_first_bit,unique_byte);
			}
		};
		break;
	case enc_UNDEFINED: break;
	};

	// shift the final bits if required
	char nshift=(CHAR_BIT * this->n_bytes)-(this->nbits * this->n_data);
		if (nshift){ this->data[this->n_bytes-1]=data[this->n_bytes-1] << nshift;};
};

// GET BYTES FUNCTIONS
template<IsAnyOf T_uint>
std::byte seq<T_uint>::get_byte_NUC_2BITS(T_uint& i){
	SPDLOG_DEBUG("seq::get_byte_NUC_2BITS");
	char nshift=6 - 2 * (i%4);
	std::byte byte= this->data[i/4] >> nshift;
	return(byte & b00000011);
};
template<IsAnyOf T_uint>
std::byte seq<T_uint>::get_byte_NUC_3BITS(T_uint& i){
	uint8_t nbits_in_bytes_after_first_bit=CHAR_BIT - (i)*this->nbits % CHAR_BIT; //
	SPDLOG_TRACE("seq::decode_NUC_3BITS::nbits_in_bytes_after_first_bit={}",nbits_in_bytes_after_first_bit);
	std::byte mask;
	std::byte byte;
	//char byte_log;
	uint8_t shift;

	if (nbits_in_bytes_after_first_bit<this->nbits){
		SPDLOG_TRACE("seq::decode_NUC_3BITS::info is on 2 bytes");

		uint8_t nbits_in_the_next_byte=this->nbits-nbits_in_bytes_after_first_bit;
		mask=b00000111 >>nbits_in_the_next_byte;

		SPDLOG_TRACE("seq::decode_NUC_3BITS::nbits_in_the_next_byte={}",nbits_in_the_next_byte);
		SPDLOG_TRACE("seq::decode_NUC_3BITS::mask={}",std::bitset<8>{(long long unsigned int)mask}.to_string());

		SPDLOG_TRACE("seq::decode_NUC_3BITS::1st  byte={}",std::bitset<8>{(long long unsigned int)(this->data[(this->nbits * i) / CHAR_BIT])}.to_string());

		SPDLOG_TRACE("seq::decode_NUC_3BITS::2nd  byte={}",std::bitset<8>{(long long unsigned int)(this->data[1+(this->nbits * i) / CHAR_BIT])}.to_string());

		byte= mask & (this->data[(this->nbits * i) / CHAR_BIT]);

		SPDLOG_TRACE("seq::decode_NUC_3BITS::1st  byte masked={}",std::bitset<8>{(long long unsigned int)byte}.to_string());

		byte=byte<<nbits_in_the_next_byte;

		SPDLOG_TRACE("seq::decode_NUC_3BITS::1st  byte masked shifted={}",std::bitset<8>{(long long unsigned int)byte}.to_string());

		shift=CHAR_BIT-nbits_in_the_next_byte;

		SPDLOG_TRACE("seq::decode_NUC_3BITS::2nd byte shift={}",shift);

		SPDLOG_TRACE("seq::decode_NUC_3BITS::2nd  byte shifted={}",std::bitset<8>{(long long unsigned int)((this->data[1 + (this->nbits * i) / CHAR_BIT] >> shift))}.to_string());

		byte|= (this->data[1 + (this->nbits * i) / CHAR_BIT] >> shift) ;

		SPDLOG_TRACE("seq::decode_NUC_3BITS::final byte={}",std::bitset<8>{(long long unsigned int)byte}.to_string());
	}else{
		SPDLOG_TRACE("seq::decode_NUC_3BITS::info is on  byte");
		SPDLOG_TRACE("seq::decode_NUC_3BITS::byte={}",std::bitset<8>{(long long unsigned int)(data[(this->nbits * i) / CHAR_BIT])}.to_string());
		mask=b00000111;
		shift=nbits_in_bytes_after_first_bit-this->nbits;
		byte=b00000111 & (this->data[(this->nbits * i) / CHAR_BIT]>>shift);
	}
	return(byte);
};
template<IsAnyOf T_uint>
std::byte seq<T_uint>::get_byte_NUC_4BITS(T_uint& i){
	SPDLOG_DEBUG("seq::get_byte_NUC_4BITS");
	uint8_t nshift=4 * (1 - i%2);
	std::byte byte=this->data[i/2]>>nshift;
	return(b00001111 & byte);
};

// DECODING FUNCTIONS
template<IsAnyOf T_uint>
char seq<T_uint>::decode_NUC_2BITS(std::byte b){
	char r{'_'};
	SPDLOG_DEBUG("seq::decode_NUC_2BITS");
	switch(b){
	case b00: r='A'; break;
	case b01: r='C'; break;
	case b10: r='G'; break;
	case b11: r='T'; break;
	default:break;
	};
	return(r);
}
template<IsAnyOf T_uint>
char seq<T_uint>::decode_NUC_3BITS(std::byte b){
	SPDLOG_DEBUG("seq::decode_NUC_3BITS");
	char r {'_'};
	switch(b){
	case b000: r='A'; break;
	case b001: r='C'; break;
	case b010: case b101: r='N'; break;
	case b011: case b100: r='-'; break;
	case b110: r='G'; break;
	case b111: r='T'; break;
	default:break;
	};
	return(r);
}

template<IsAnyOf T_uint>
char seq<T_uint>::decode_NUC_4BITS(std::byte b,miscomplemented_encoding mis_enc){
	SPDLOG_DEBUG("seq::decode_NUC_4BITS");
	char r{'_'};
	switch(b){
	case b0000: r='A'; break;
	case b0001: r='C'; break;
	case b0010: r='R'; break;
	case b0011: r='K'; break;
	case b0100: r='B'; break;
	case b0101: r='D'; break;
	case b0110: r=mis_enc.ini_s; break;
	case b0111: r=mis_enc.ini_n; break;
	case b1000: r=mis_enc.ini_gap; break;
	case b1001: r=mis_enc.ini_w; break;
	case b1010: r='H'; break;
	case b1011: r='V'; break;
	case b1100: r='M'; break;
	case b1101: r='Y'; break;
	case b1110: r='G'; break;
	case b1111: r='T'; break;
	default:break;
	};
	return(r);
}
template<IsAnyOf T_uint>
std::string seq<T_uint>::decode(){
	SPDLOG_DEBUG("seq::decode");
	std::string r = "";
	SPDLOG_TRACE("seq::decode::start iteration");
	for(T_uint i=this->get_begin_data_pos(); this->is_data_pos_valid(i) ; this->increment_begin_data_pos(i) ){
		SPDLOG_TRACE("seq::decode::i={}",i);
		SPDLOG_TRACE("seq::decode:: before push_back r= "+r);
		r.push_back(this->decode_e_type(this->get_byte(i),this->comp_dep_mis_enc));
	};
	SPDLOG_TRACE("seq::decode::iteration eneded");
	SPDLOG_TRACE("seq::decode::r= "+r);
	return(r);
};

#undef PARSE_STR_ENCODING

