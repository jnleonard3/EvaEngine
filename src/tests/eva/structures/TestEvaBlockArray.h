#ifndef TEST_EVA_BLOCKARRAY_H_
#define TEST_EVA_BLOCKARRAY_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "eva/Typedefs.h"

namespace eva
{
	template <class T>
	class BlockArray;
}

class TestEvaBlockArray : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE(TestEvaBlockArray);
    CPPUNIT_TEST(constructorTest);
    CPPUNIT_TEST(resizeTest);
    CPPUNIT_TEST(getIndexOfAddressTest);
    CPPUNIT_TEST(swapBlockTest);
    CPPUNIT_TEST(moveBlockTest);
    CPPUNIT_TEST(deleteBlockTest);
    CPPUNIT_TEST_SUITE_END();

	public:
    	void setUp();
    	void tearDown();

	protected:
    	void constructorTest();
    	void resizeTest();
    	void getIndexOfAddressTest();
    	void swapBlockTest();
    	void moveBlockTest();
    	void deleteBlockTest();

	private:
    	eva::BlockArray<e_uint32> *a, *b;
};

#endif
