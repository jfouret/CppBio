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

using namespace cppbio;

struct TestFixture1
{
	TestFixture1();
	// ~TestFixture1(); not needed
	seq s1;
	std::shared_ptr<seq> s2;
};

#endif /* TEST_SEQ_HPP_ */
