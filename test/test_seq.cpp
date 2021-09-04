/*
 * test_seq.cpp
 *
 *  Created on: Sep 1, 2021
 *      Author: jfouret
 */
#define BOOST_TEST_DYN_LINK

#include <test_seq.hpp>

using namespace cppbio;

std::string i1			="AAAATTTCCG";
std::string rev_i1		="GCCTTTAAAA";
std::string comp_i1		="TTTTAAAGGC";
std::string revcomp_i1	="CGGAAATTTT";

TestFixture1::TestFixture1(){
    this->s1 = i1;

};

BOOST_FIXTURE_TEST_SUITE(Test_seq, TestFixture1);

BOOST_AUTO_TEST_CASE( context )
{
	BOOST_TEST_MESSAGE( "i1         = " << i1 );
	BOOST_TEST_MESSAGE( "rev_i1     = " << rev_i1 );
	BOOST_TEST_MESSAGE( "comp_i1    = " << comp_i1 );
	BOOST_TEST_MESSAGE( "revcomp_i1 = " << revcomp_i1 );
};

BOOST_AUTO_TEST_CASE( get_string )
{
	TestFixture1 f;
	BOOST_CHECK(i1==f.s1.get_string());
};

BOOST_AUTO_TEST_CASE( reverse )
{
	TestFixture1 f;
	f.s1.reverse();
	BOOST_CHECK(rev_i1==f.s1.get_string());
	f.s1.reverse();
	BOOST_CHECK(i1==f.s1.get_string());
};

BOOST_AUTO_TEST_CASE( complement )
{
	TestFixture1 f;
	f.s1.complement();
	BOOST_CHECK(comp_i1==f.s1.get_string());
	f.s1.complement();
	BOOST_CHECK(i1==f.s1.get_string());
};

BOOST_AUTO_TEST_CASE( reverse_complement )
{
	TestFixture1 f;
	f.s1.reverse_complement();
	BOOST_CHECK(revcomp_i1==f.s1.get_string());
	f.s1.reverse_complement();
	BOOST_CHECK(i1==f.s1.get_string());
};

BOOST_AUTO_TEST_SUITE_END();
