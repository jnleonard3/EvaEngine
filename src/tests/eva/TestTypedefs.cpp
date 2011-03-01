#include "TestTypedefs.h"
#include "eva/Typedefs.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestEvaTypedefs);

void TestEvaTypedefs::sizeTest()
{
	CPPUNIT_ASSERT_EQUAL((int)sizeof(e_bool8), 1);
	CPPUNIT_ASSERT_EQUAL((int)sizeof(e_uchar8), 1);
	CPPUNIT_ASSERT_EQUAL((int)sizeof(e_char8), 1);
	CPPUNIT_ASSERT_EQUAL((int)sizeof(e_uint16), 2);
	CPPUNIT_ASSERT_EQUAL((int)sizeof(e_int16), 2);
	CPPUNIT_ASSERT_EQUAL((int)sizeof(e_uint32), 4);
	CPPUNIT_ASSERT_EQUAL((int)sizeof(e_int32), 4);
	CPPUNIT_ASSERT_EQUAL((int)sizeof(e_float32), 4);
	CPPUNIT_ASSERT_EQUAL((int)sizeof(e_double64), 8);
}
