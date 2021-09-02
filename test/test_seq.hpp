/*
 * test_seq.hpp
 *
 *  Created on: Sep 1, 2021
 *      Author: jfouret
 */

#ifndef TEST_SEQ_HPP_
#define TEST_SEQ_HPP_

#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include "seq.hpp"

using namespace CppUnit;

class TESTseq : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TESTseq);
    CPPUNIT_TEST(get_string);
    CPPUNIT_TEST(reverse);
    CPPUNIT_TEST(complement);
    CPPUNIT_TEST(reverse_complement);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void);
    void tearDown(void);

protected:
    void get_string(void);
    void reverse();
    void complement();
    void reverse_complement();

private:
    seq s1;
};

#endif /* TEST_SEQ_HPP_ */
