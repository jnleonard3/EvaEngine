#ifndef TEST_EVA_SIMPLEDYNAMICARRAY_H_
#define TEST_EVA_SIMPLEDYNAMICARRAY_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "eva/structures/evaSimpleDynamicArray.h"

class TestEvaSimpleDynamicArray : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE(TestEvaSimpleDynamicArray);
    CPPUNIT_TEST(constructorTest);
    CPPUNIT_TEST(resizeTest);
    CPPUNIT_TEST(releaseTest);
    CPPUNIT_TEST_SUITE_END();

    protected:
        void constructorTest();
        void resizeTest();
        void releaseTest();
};

#endif
