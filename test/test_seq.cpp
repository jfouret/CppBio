/*
 * test_seq.cpp
 *
 *  Created on: Sep 1, 2021
 *      Author: jfouret
 */

#include <test_seq.hpp>

std::string i1			="AAAATTTCCG";
std::string view_i1		="AAAATTTCCG";
std::string rev_i1		="GCCTTTAAAA";
std::string comp_i1		="TTTTAAAGGC";
std::string revcomp_i1	="CGGAAATTTT";

using namespace CppUnit;

void TESTseq::get_string(void)
{
    CPPUNIT_ASSERT(view_i1==s1.get_string());
};

void TESTseq::reverse(void)
{
	s1.reverse();
	CPPUNIT_ASSERT(rev_i1==s1.get_string());
	s1.reverse();
    CPPUNIT_ASSERT(view_i1==s1.get_string());
};

void TESTseq::complement(void)
{
	s1.complement();
	CPPUNIT_ASSERT(comp_i1==s1.get_string());
	s1.complement();
    CPPUNIT_ASSERT(view_i1==s1.get_string());
};

void TESTseq::reverse_complement(void)
{
	s1.reverse_complement();
	CPPUNIT_ASSERT(revcomp_i1==s1.get_string());
	s1.reverse_complement();
    CPPUNIT_ASSERT(view_i1==s1.get_string());
};

void TESTseq::setUp(void)
{
    s1 = i1;
};

void TESTseq::tearDown(void){
};

//-----------------------------------------------------------------------------

CPPUNIT_TEST_SUITE_REGISTRATION( TESTseq );

int main(int argc, char* argv[])
{
    // informs test-listener about testresults
    CPPUNIT_NS::TestResult testresult;

    // register listener for collecting the test-results
    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);

    // register listener for per-test progress output
    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener (&progress);

    // insert test-suite at test-runner by registry
    CPPUNIT_NS::TestRunner testrunner;
    testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest ());
    testrunner.run(testresult);

    // output results in compiler-format
    CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write ();

    // Output XML for Jenkins CPPunit plugin
    // std::ofstream xmlFileOut("seq.xml");
    // XmlOutputter xmlOut(&collectedresults, xmlFileOut);
    // xmlOut.write();

    // return 0 if tests were successful
    return collectedresults.wasSuccessful() ? 0 : 1;
}

