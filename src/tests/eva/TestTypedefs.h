#ifndef TEST_EVA_TYPEDEFS_H_
#define TEST_EVA_TYPEDEFS_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class TestEvaTypedefs : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE(TestEvaTypedefs);
    CPPUNIT_TEST(sizeTest);
    CPPUNIT_TEST_SUITE_END();

	public:
		void setUp(){};
		void tearDown(){};

    protected:
        void sizeTest();

};

#endif
