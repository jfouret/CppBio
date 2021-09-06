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

std::string i2			="C-KBDRSTANWHMYGV";
std::string rev_i2		="VGYMHWNATSRDBK-C";
std::string comp_i2		="G-MVHYSATNWDKRCB";
std::string revcomp_i2	="BCRKDWNTASYHVM-G";

std::string i3			="AAAANTTT-CCG";
std::string rev_i3		="GCC-TTTNAAAA";
std::string comp_i3		="TTTTNAAA-GGC";
std::string revcomp_i3	="CGG-AAANTTTT";

TestFixture1::TestFixture1(){
	spdlog::set_level(spdlog::level::trace); // Set global log level to debug
	spdlog::set_pattern("[%H:%M:%S %z] [%L] %v");
    this->s1 = i1;
    this->s2.reset(new seq<uint16_t>(i2));
    this->s3 = i3;
};

BOOST_FIXTURE_TEST_SUITE(Test_seq, TestFixture1);

BOOST_AUTO_TEST_CASE( context )
{
	BOOST_TEST_MESSAGE( "i1         = " << i1 );
	BOOST_TEST_MESSAGE( "rev_i1     = " << rev_i1 );
	BOOST_TEST_MESSAGE( "comp_i1    = " << comp_i1 );
	BOOST_TEST_MESSAGE( "revcomp_i1 = " << revcomp_i1 );

	BOOST_TEST_MESSAGE( "i2         = " << i2 );
	BOOST_TEST_MESSAGE( "rev_i2     = " << rev_i2 );
	BOOST_TEST_MESSAGE( "comp_i2    = " << comp_i2 );
	BOOST_TEST_MESSAGE( "revcomp_i2 = " << revcomp_i2 );

	BOOST_TEST_MESSAGE( "i3         = " << i3 );
	BOOST_TEST_MESSAGE( "rev_i3     = " << rev_i3 );
	BOOST_TEST_MESSAGE( "comp_i3    = " << comp_i3 );
	BOOST_TEST_MESSAGE( "revcomp_i3 = " << revcomp_i3 );
};

BOOST_AUTO_TEST_CASE( get_string )
{
	TestFixture1 f;
	BOOST_CHECK(i1==f.s1.get_string());
	BOOST_CHECK(i2==f.s2->get_string());
	BOOST_CHECK(i3==f.s3.get_string());
};

BOOST_AUTO_TEST_CASE( reverse )
{
	TestFixture1 f;
	f.s1.reverse();
	BOOST_CHECK(rev_i1==f.s1.get_string());
	f.s1.reverse();
	BOOST_CHECK(i1==f.s1.get_string());

	f.s2->reverse();
	BOOST_CHECK(rev_i2==f.s2->get_string());
	f.s2->reverse();
	BOOST_CHECK(i2==f.s2->get_string());

	f.s3.reverse();
	BOOST_CHECK(rev_i3==f.s3.get_string());
	f.s3.reverse();
	BOOST_CHECK(i3==f.s3.get_string());
};

BOOST_AUTO_TEST_CASE( complement )
{
	TestFixture1 f;
	f.s1.complement();
	BOOST_CHECK(comp_i1==f.s1.get_string());
	f.s1.complement();
	BOOST_CHECK(i1==f.s1.get_string());

	f.s2->complement();
	BOOST_CHECK(comp_i2==f.s2->get_string());
	f.s2->complement();
	BOOST_CHECK(i2==f.s2->get_string());

	f.s3.complement();
	BOOST_CHECK(comp_i3==f.s3.get_string());
	f.s3.complement();
	BOOST_CHECK(i3==f.s3.get_string());

};

BOOST_AUTO_TEST_CASE( reverse_complement )
{
	TestFixture1 f;
	f.s1.reverse_complement();
	BOOST_CHECK(revcomp_i1==f.s1.get_string());
	f.s1.reverse_complement();
	BOOST_CHECK(i1==f.s1.get_string());
	f.s2->reverse_complement();
	BOOST_CHECK(revcomp_i2==f.s2->get_string());
	f.s2->reverse_complement();
	BOOST_CHECK(i2==f.s2->get_string());

	f.s3.reverse_complement();
	BOOST_CHECK(revcomp_i3==f.s3.get_string());
	f.s3.reverse_complement();
	BOOST_CHECK(i3==f.s3.get_string());

};

BOOST_AUTO_TEST_SUITE_END();
