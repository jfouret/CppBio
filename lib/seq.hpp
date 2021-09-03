/*!
 * @file seq.hpp
 * @brief Dealing efficiently with biological sequence in C++
 * @author Julien FOURET
 * @version 0.1
 */

#ifndef SEQ_HPP_
#define SEQ_HPP_

#include <climits> // for CHAR_BIT
#include <iostream>
#include <string>
#include <bitset>
#include <memory>
#include <map>
#include <iterator>

namespace cppbio {

	/**
	 * @brief Definition of type `encode_type`
	 *
	 * Enumeration of encoding type
	 *
	 */

	typedef enum {
		enc_UNDEFINED, /**<  Undefined encoding */
		NUC_2BITS, /**<  Nucleotide encoding on 2bits */
		NUC_3BITS, /**<  Nucleotide encoding on 3bits */
		NUC_4BITS, /**<  Nucleotide encoding on 4bits */
		PRO_5BITS /**<  Protein encoding on 5bits */
	} encode_type ;

	/**
	 * @brief Definition of type `mol_type`
	 *
	 * Enumeration of molecule type
	 *
	 */

	typedef enum {
		mol_UNDEFINED, /**<  Undefined molecule */
		DNA, /**<  DNA molecule */
		RNA, /**<  RNA molecule */
		PROTEIN /**<  PROTEIN molecule */
	} mol_type ;

	/**
	 * @brief Definition of type `ulong`
	 *
	 * alias for unsigned long
	 *
	 */

	typedef unsigned long ulong;

	/*!
	 * @class seq
	 * @brief A class representing biological sequence
	 *
	 *  This class implement efficient encoding of biological sequence and basic operations.
	 *
	 * 	2-bits nucleotide encoding
	 *
	 *  A : 00
	 *  T : 01
	 *  C : 10
	 *  G : 11
	 *
	 *
	 *	3-bits nucleotide encode
	 *
	 *  A                : 000
	 *  C                : 001
	 *  N                : 010
	 *  gap              : 011
	 *  complemented gap : 100
	 *  complemented N   : 101
	 *  G                : 110
	 *  T                : 111
	 *
	 *
	 *	4-bits nucleotide encoding
	 *
	 *	A       : 0000
	 *	C       : 0001
	 *	R       : 0010
	 *	K       : 0011
	 *	B       : 0100
	 *	D       : 0101
	 *	S or W  : 0110
	 *	N or gap: 0111
	 *	gap or N: 1000
	 *	W or S  : 1001
	 *	H       : 1010
	 *	V       : 1011
	 *	M       : 1100
	 *	Y       : 1101
	 *	G       : 1110
	 *	T       : 1111
	 *
	 *
	 *  5bit amino-acid encoding
	 *
	 *	A               : 00000
	 *	B               : 00001
	 *	C               : 00010
	 *	E               : 00011
	 *	F               : 00100
	 *	G               : 00101
	 *	H               : 00110
	 *	I               : 00111
	 *	K               : 01000
	 *	L               : 01001
	 *	M               : 01010
	 *	N               : 01011
	 *	P               : 01100
	 *	Q               : 01101
	 *	R               : 01110
	 *	S               : 01111
	 *	T               : 10000
	 *	U               : 10001
	 *	V               : 10010
	 *	W               : 10011
	 *	X               : 10100
	 *	Y               : 10101
	 *	Z               : 10110
	 *	gap             : 10111
	 *	1bp-frameshift	: 11000
	 *	2bp-frameshift	: 11001
	 *	stop            : 11010
	 *	undefined 1     : 11011
	 *	undefined 2     : 11100
	 *	undefined 3     : 11101
	 *	undefined 4     : 11110
	 *	end of sequence : 11111
	 *
	 *
	 *
	 *
	 *
	 */

	class seq{
		public:
			/*!
			 *  @brief Default constructor
			 *
			 *  Default constructor without args.
			 *
			 */
			seq();
			/*!
			 *  @brief String constructor
			 *
			 *  String constructor without std::string as mandatory argument.
			 *
			 *  @param s : A string to encode within seq object
			 *  @param e_type : Force a type of encoding
			 *  @param m_type : Force a type of molecule
			 */
			seq(std::string s,encode_type e_type=enc_UNDEFINED,mol_type m_type=mol_UNDEFINED);
			/*!
			 *  @brief Operator = string
			 *
			 *  Define the assignment of an object seq with a std::string: The string is encoded within the seq object.
			 *  Any other data seq object held before is erased.
			 *
			 *  @param s : A string to encode within seq object
			 */
			void operator = (std::string & s);
			/*!
			 *  @brief Complement the seq
			 *
			 *  Implement the complement operation for DNA or RNA.
			 *
			 */
			void complement();
			/*!
			 *  @brief reverse the seq
			 *
			 *  Implement the reverse operation
			 *
			 */
			void reverse();
			/*!
			 *  @brief reverse the seq
			 *
			 *  Implement the reverse complement operation
			 *
			 */
			void reverse_complement();
			/*!
			 *  @brief Get the seq as a string.
			 *
			 *  Decode the seq object as a std::string
			 *
			 *	@return The encoded sequence as a std::string
			 *
			 */
			std::string get_string();
		private:
			std::shared_ptr<char[]> data;  /**<  Smart pointer to the byte array of encoded data */
			bool is_rev; /**<  whether the seq should be read in reverse or not */
			ulong n_bytes; /**<  Number of bytes to encode the seq */
			ulong n_data; /**<  Number of element (base or amino-acid) in the seq */
			encode_type e_type; /**<  encoding type */
			mol_type m_type; /**<  molecule type */
			void set_encode_parameters(std::string s);
			void encode(std::string s);
			char decode(ulong i);
			std::string decode();
	};
}
#endif /* SEQ_HPP_ */
