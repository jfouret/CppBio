/*
 * test_seq.cpp
 *
 *  Created on: Sep 1, 2021
 *      Author: jfouret
 */
#define BOOST_TEST_DYN_LINK

#include <test_seq.hpp>
#include <tuple>

using namespace cppbio;

std::string inputs[N_INPUTS]={
    "AAAATTTCCG",
    "C-KBDRSTANWHMYGV",
    "AAAANTTT-CCG"
};

std::string revs[N_INPUTS]={
    "GCCTTTAAAA",
    "VGYMHWNATSRDBK-C",
    "GCC-TTTNAAAA"
};

std::string comps[N_INPUTS]={
    "TTTTAAAGGC",
    "G-MVHYSATNWDKRCB",
    "TTTTNAAA-GGC"
};

std::string revcomps[N_INPUTS]={
    "CGGAAATTTT",
    "BCRKDWNTASYHVM-G",
    "CGG-AAANTTTT"
};

TestFixture1::TestFixture1(){
	spdlog::set_level(spdlog::level::trace); // Set global log level to debug
	spdlog::set_pattern("[%H:%M:%S %z] [%L] %v");

	for (unsigned int i=0;i<N_INPUTS;i++){
        std::string in_s=inputs[i];
        std::apply(
            [&in_s](auto&&... x) {
               ((x=in_s),...);
            }, seqs[i]
        );
	};
};

BOOST_FIXTURE_TEST_SUITE(Test_seq, TestFixture1);

BOOST_AUTO_TEST_CASE( context )
{
    for (unsigned int i=0;i<N_INPUTS;i++){

    BOOST_TEST_MESSAGE( "TEST SEQ "<<i);
    BOOST_TEST_MESSAGE( "input      = " << inputs[i] );
    BOOST_TEST_MESSAGE( "reverse    = " << revs[i] );
    BOOST_TEST_MESSAGE( "complement = " << comps[i] );
    BOOST_TEST_MESSAGE( "revcomp    = " << revcomps[i] );

    };
};

BOOST_AUTO_TEST_CASE( get_string )
{
    TestFixture1 f;

    for (unsigned int i=0;i<N_INPUTS;i++){
    BOOST_CHECK(std::get<0>(seqs[i]).get_string()==inputs[i]);
    BOOST_CHECK(std::get<2>(seqs[i]).get_string()==inputs[i]);
    BOOST_CHECK(std::get<2>(seqs[i]).get_string()==inputs[i]);
    BOOST_CHECK(std::get<3>(seqs[i]).get_string()==inputs[i]);
	};
};


BOOST_AUTO_TEST_CASE( reverse )
{
    TestFixture1 f;

    for (unsigned int i=0;i<N_INPUTS;i++){
        std::apply(
            [](auto&&... x) {
               ((x.reverse()),...);
            }, seqs[i]
        );
	};

        for (unsigned int i=0;i<N_INPUTS;i++){
    BOOST_CHECK(std::get<0>(seqs[i]).get_string()==revs[i]);
    BOOST_CHECK(std::get<2>(seqs[i]).get_string()==revs[i]);
    BOOST_CHECK(std::get<2>(seqs[i]).get_string()==revs[i]);
    BOOST_CHECK(std::get<3>(seqs[i]).get_string()==revs[i]);
	};

    for (unsigned int i=0;i<N_INPUTS;i++){
        std::apply(
            [](auto&&... x) {
               ((x.reverse()),...);
            }, seqs[i]
        );
	};

    for (unsigned int i=0;i<N_INPUTS;i++){
    BOOST_CHECK(std::get<0>(seqs[i]).get_string()==inputs[i]);
    BOOST_CHECK(std::get<2>(seqs[i]).get_string()==inputs[i]);
    BOOST_CHECK(std::get<2>(seqs[i]).get_string()==inputs[i]);
    BOOST_CHECK(std::get<3>(seqs[i]).get_string()==inputs[i]);
	};
};


BOOST_AUTO_TEST_CASE( complement )
{
    TestFixture1 f;

    for (unsigned int i=0;i<N_INPUTS;i++){
        std::apply(
            [](auto&&... x) {
               ((x.complement()),...);
            }, seqs[i]
        );
	};

        for (unsigned int i=0;i<N_INPUTS;i++){
    BOOST_CHECK(std::get<0>(seqs[i]).get_string()==comps[i]);
    BOOST_CHECK(std::get<2>(seqs[i]).get_string()==comps[i]);
    BOOST_CHECK(std::get<2>(seqs[i]).get_string()==comps[i]);
    BOOST_CHECK(std::get<3>(seqs[i]).get_string()==comps[i]);
	};

    for (unsigned int i=0;i<N_INPUTS;i++){
        std::apply(
            [](auto&&... x) {
               ((x.complement()),...);
            }, seqs[i]
        );
	};

    for (unsigned int i=0;i<N_INPUTS;i++){
    BOOST_CHECK(std::get<0>(seqs[i]).get_string()==inputs[i]);
    BOOST_CHECK(std::get<2>(seqs[i]).get_string()==inputs[i]);
    BOOST_CHECK(std::get<2>(seqs[i]).get_string()==inputs[i]);
    BOOST_CHECK(std::get<3>(seqs[i]).get_string()==inputs[i]);
	};
};


BOOST_AUTO_TEST_CASE( reverse_complement )
{
    TestFixture1 f;

    for (unsigned int i=0;i<N_INPUTS;i++){
        std::apply(
            [](auto&&... x) {
               ((x.reverse_complement()),...);
            }, seqs[i]
        );
	};

        for (unsigned int i=0;i<N_INPUTS;i++){
    BOOST_CHECK(std::get<0>(seqs[i]).get_string()==revcomps[i]);
    BOOST_CHECK(std::get<2>(seqs[i]).get_string()==revcomps[i]);
    BOOST_CHECK(std::get<2>(seqs[i]).get_string()==revcomps[i]);
    BOOST_CHECK(std::get<3>(seqs[i]).get_string()==revcomps[i]);
	};

    for (unsigned int i=0;i<N_INPUTS;i++){
        std::apply(
            [](auto&&... x) {
               ((x.reverse_complement()),...);
            }, seqs[i]
        );
	};

    for (unsigned int i=0;i<N_INPUTS;i++){
    BOOST_CHECK(std::get<0>(seqs[i]).get_string()==inputs[i]);
    BOOST_CHECK(std::get<2>(seqs[i]).get_string()==inputs[i]);
    BOOST_CHECK(std::get<2>(seqs[i]).get_string()==inputs[i]);
    BOOST_CHECK(std::get<3>(seqs[i]).get_string()==inputs[i]);
	};
};

BOOST_AUTO_TEST_SUITE_END();
