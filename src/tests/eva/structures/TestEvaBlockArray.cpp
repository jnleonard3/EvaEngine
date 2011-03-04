#include "TestEvaBlockArray.h"
#include "eva/structures/evaBlockArray.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestEvaBlockArray);

#include <iostream>

void TestEvaBlockArray::setUp()
{
	a = new eva::BlockArray<e_uint32>();
	b = new eva::BlockArray<e_uint32>(1000, 256);
}

void TestEvaBlockArray::tearDown()
{
	delete a;
	delete b;
}

void TestEvaBlockArray::constructorTest()
{
	CPPUNIT_ASSERT_EQUAL((e_uint32)256, a->getBlockSize());
	CPPUNIT_ASSERT_EQUAL((e_uint32)64, a->getElementsPerBlock());
	CPPUNIT_ASSERT_EQUAL((e_uint32)0, a->getSize());
	CPPUNIT_ASSERT_EQUAL((e_uint32)0, a->getBlockCount());
	CPPUNIT_ASSERT_EQUAL((e_uint32)0, a->getMemoryUsage());

	CPPUNIT_ASSERT_EQUAL((e_uint32)256, b->getBlockSize());
	CPPUNIT_ASSERT_EQUAL((e_uint32)64, b->getElementsPerBlock());
	CPPUNIT_ASSERT_EQUAL((e_uint32)1000, b->getSize());
	CPPUNIT_ASSERT_EQUAL((e_uint32)16, b->getBlockCount());
	CPPUNIT_ASSERT_EQUAL((e_uint32)4096, b->getMemoryUsage());
	for(e_uint32 i = 0; i < b->getSize(); ++i)
		b->at(i) = i;
	for(e_uint32 i = 0; i < b->getSize(); ++i)
		CPPUNIT_ASSERT_EQUAL(i, b->at(i));
}

void TestEvaBlockArray::resizeTest()
{
	a->resize(1000);
	CPPUNIT_ASSERT_EQUAL((e_uint32)1000, a->getSize());
	CPPUNIT_ASSERT_EQUAL((e_uint32)16, a->getBlockCount());
	CPPUNIT_ASSERT_EQUAL((e_uint32)4096, a->getMemoryUsage());
	for(e_uint32 i = 0; i < a->getSize(); ++i)
		a->at(i) = i;
	for(e_uint32 i = 0; i < a->getSize(); ++i)
		CPPUNIT_ASSERT_EQUAL(i, a->at(i));

	b->resize(500);
	CPPUNIT_ASSERT_EQUAL((e_uint32)500, b->getSize());
	CPPUNIT_ASSERT_EQUAL((e_uint32)8, b->getBlockCount());
	CPPUNIT_ASSERT_EQUAL((e_uint32)4096, b->getMemoryUsage());
	for(e_uint32 i = 0; i < b->getSize(); ++i)
		b->at(i) = i;
	for(e_uint32 i = 0; i < b->getSize(); ++i)
		CPPUNIT_ASSERT_EQUAL(i, b->at(i));

}

void TestEvaBlockArray::swapBlockTest()
{
	for(e_uint32 i = 0; i < b->getSize(); ++i)
		b->at(i) = i;
	e_uint32 first = b->getBlockIndex(64), second = b->getBlockIndex(128);
	CPPUNIT_ASSERT_EQUAL((e_uint32)1,first);
	CPPUNIT_ASSERT_EQUAL((e_uint32)2,second);
	b->swapBlocks(first,second);
	CPPUNIT_ASSERT_EQUAL((e_uint32)128,(e_uint32)b->at(64));
	CPPUNIT_ASSERT_EQUAL((e_uint32)64,(e_uint32)b->at(128));
}

void TestEvaBlockArray::moveBlockTest()
{
	for(e_uint32 i = 0; i < b->getSize(); ++i)
		b->at(i) = i;
	e_uint32 from = b->getBlockIndex(64), to = b->getBlockIndex(0);
	CPPUNIT_ASSERT_EQUAL((e_uint32)1,from);
	CPPUNIT_ASSERT_EQUAL((e_uint32)0,to);
	b->moveBlock(from,to);
	CPPUNIT_ASSERT_EQUAL((e_uint32)0,(e_uint32)b->at(64));
	CPPUNIT_ASSERT_EQUAL((e_uint32)64,(e_uint32)b->at(0));
}

void TestEvaBlockArray::deleteBlockTest()
{
	for(e_uint32 i = 0; i < b->getSize(); ++i)
		b->at(i) = i;
	e_uint32 del = b->getBlockIndex(0);
	CPPUNIT_ASSERT_EQUAL((e_uint32)0,del);
	b->deleteBlock(del);
	CPPUNIT_ASSERT_EQUAL((e_uint32)15, b->getBlockCount());
	CPPUNIT_ASSERT_EQUAL((e_uint32)128,(e_uint32)b->at(64));
	CPPUNIT_ASSERT_EQUAL((e_uint32)64,(e_uint32)b->at(0));
}
