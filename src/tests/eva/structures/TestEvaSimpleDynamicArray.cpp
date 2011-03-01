#include "TestEvaSimpleDynamicArray.h"
#include "eva/structures/evaSimpleDynamicArray.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestEvaSimpleDynamicArray);

void TestEvaSimpleDynamicArray::constructorTest()
{
	eva::SimpleDynamicArray<int> array;
	CPPUNIT_ASSERT_EQUAL(array.getSize(), (e_uint32)0);
	CPPUNIT_ASSERT_EQUAL(array.getArray(), (int*)0);
	eva::SimpleDynamicArray<int> arrayTwo(300);
	CPPUNIT_ASSERT_EQUAL(arrayTwo.getSize(), (e_uint32)300);
	CPPUNIT_ASSERT(arrayTwo.getArray());
	int *dataThree = new int[300];
	eva::SimpleDynamicArray<int> arrayThree(dataThree, (e_uint32)300);
	CPPUNIT_ASSERT_EQUAL(arrayThree.getSize(), (e_uint32)300);
	CPPUNIT_ASSERT(arrayThree.getArray());

}

void TestEvaSimpleDynamicArray::resizeTest()
{
	eva::SimpleDynamicArray<int> array;
	CPPUNIT_ASSERT_EQUAL(array.getSize(),(e_uint32)0);
	CPPUNIT_ASSERT_EQUAL(array.getArray(), (int*)0);
	array.resize(100);
	CPPUNIT_ASSERT_EQUAL(array.getSize(),(e_uint32)100);
	CPPUNIT_ASSERT(array.getArray());
	array.doubleSize();
	CPPUNIT_ASSERT_EQUAL(array.getSize(),(e_uint32)200);
	CPPUNIT_ASSERT(array.getArray());
	array.halfSize();
	CPPUNIT_ASSERT_EQUAL(array.getSize(),(e_uint32)100);
	CPPUNIT_ASSERT(array.getArray());
	array.resize(10);
	CPPUNIT_ASSERT_EQUAL(array.getSize(),(e_uint32)10);
	CPPUNIT_ASSERT(array.getArray());
}

void TestEvaSimpleDynamicArray::releaseTest()
{
	eva::SimpleDynamicArray<int> array(10);
	int* data = array.release();
	CPPUNIT_ASSERT_EQUAL(array.getSize(),(e_uint32)0);
	CPPUNIT_ASSERT_EQUAL(array.getArray(), (int*)0);
	CPPUNIT_ASSERT(data);
	delete data;
}
