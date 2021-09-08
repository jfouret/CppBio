/*
 * test_seq.hpp
 *
 *  Created on: Sep 1, 2021
 *      Author: jfouret
 */
#ifndef TEST_SEQ_HPP_
#define TEST_SEQ_HPP_

#include <seq.hpp>
#define BOOST_TEST_MODULE "C++ Unit Tests for cppbio::seq"
#include <boost/test/unit_test.hpp>

#define N_INPUTS 3

using namespace cppbio;

struct TestFixture1
{
	TestFixture1();
	// ~TestFixture1(); not needed

	std::tuple<seq<uint8_t>,seq<uint16_t>,seq<uint32_t>,seq<uint64_t>> seqs[N_INPUTS];

	std::tuple<std::shared_ptr<seq<uint8_t>>,std::shared_ptr<seq<uint16_t>>,std::shared_ptr<seq<uint32_t>>,std::shared_ptr<seq<uint64_t>>> ptr_seqs[N_INPUTS];
};

#endif /* TEST_SEQ_HPP_ */
