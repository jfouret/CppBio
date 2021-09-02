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

#define b2_A (00)   /**< A : 00 */
#define b2_C (01)   /**< T : 01 */
#define b2_G (02)   /**< C : 10 */
#define b2_T (03)   /**< G : 11 */

#define b3_A (00)		/**< A					: 000 */
#define b3_C (01)		/**< C					: 001 */
#define b3_N (02)		/**< N					: 010 */
#define b3_gap (03)		/**< gap				: 011 */
#define b3_cgap (04)	/**< complemented gap	: 100 */
#define b3_cN (05)		/**< complemented N		: 101 */
#define b3_G (06)		/**< G					: 110 */
#define b3_T (07)		/**< T					: 111 */

#define b4_A (000)				/**< A 		: 0000 */
#define b4_C (001)				/**< C 		: 0001 */
#define b4_R (002)				/**< R 		: 0010 */
#define b4_K (003)				/**< K 		: 0011 */
#define b4_B (004)				/**< B 		: 0100 */
#define b4_D (005)				/**< D 		: 0101 */
#define b4_S_or_W (006)			/**< S or W	: 0110 */
#define b4_N_or_gap (007)		/**< N or gap	: 0111 */
#define b4_gap_or_N (010)		/**< gap or N	: 1000 */
#define b4_W_or_S (011)			/**< W or S	: 1001 */
#define b4_H (012)				/**< H		: 1010 */
#define b4_V (013)				/**< V		: 1011 */
#define b4_M (014)				/**< M		: 1100 */
#define b4_Y (015)				/**< Y		: 1101 */
#define b4_G (016)				/**< G		: 1110 */
#define b4_T (017)				/**< T		: 1111 */

#define b5_A (000)				/**< A				: 00000 */
#define b5_B (001)				/**< B				: 00001 */
#define b5_C (002)				/**< C				: 00010 */
#define b5_E (003) 				/**< E				: 00011 */
#define b5_F (004)				/**< F				: 00100 */
#define b5_G (005)				/**< G				: 00101 */
#define b5_H (006)				/**< H				: 00110 */
#define b5_I (007)				/**< I				: 00111 */
#define b5_K (010)				/**< K				: 01000 */
#define b5_L (011)				/**< L				: 01001 */
#define b5_M (012)				/**< M				: 01010 */
#define b5_N (013)				/**< N				: 01011 */
#define b5_P (014)				/**< P				: 01100 */
#define b5_Q (015)				/**< Q				: 01101 */
#define b5_R (016)				/**< R				: 01110 */
#define b5_S (017)				/**< S				: 01111 */
#define b5_T (020)				/**< T				: 10000 */
#define b5_U (021)				/**< U				: 10001 */
#define b5_V (022)				/**< V				: 10010 */
#define b5_W (023)				/**< W				: 10011 */
#define b5_X (024)				/**< X				: 10100 */
#define b5_Y (025)				/**< Y				: 10101 */
#define b5_Z (026)				/**< Z				: 10110 */
#define b5_gap (027)   			/**< gap				: 10111 */
#define b5_frameshift_1bp (030)	/**< 1bp-frameshift	: 11000 */
#define b5_frameshift_2bp (031)	/**< 2bp-frameshift	: 11001 */
#define b5_stop (032)			/**< stop				: 11010 */
#define b5_undef1 (033)			/**< undefined 1		: 11011 */
#define b5_undef2 (034)			/**< undefined 2		: 11100 */
#define b5_undef3 (035)			/**< undefined 3		: 11101 */
#define b5_undef4 (036)			/**< undefined 4		: 11110 */
#define b5_end_of_seq (037)		/**< end of sequence	: 11111 */

/*!
 * @class seq
 * @brief A class representing biological sequence
 *
 *  This class implement efficient encoding of biological sequence and basic operations.
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

#endif /* SEQ_HPP_ */
