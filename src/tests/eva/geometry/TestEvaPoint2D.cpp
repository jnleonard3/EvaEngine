#include "TestEvaPoint2D.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestEvaPoint2D);

void TestEvaPoint2D::setUp ()
{
    a = new eva::Point2Di(1, 2);
    b = new eva::Point2Di(2, 3);
    c = new eva::Point2Df(1.0f, 2.0f);
    d = new eva::Point2Df(2.0f, 3.0f);
    e = new eva::Point2Dd(1.0, 2.0);
    f = new eva::Point2Dd(2.0, 3.0);
}

void TestEvaPoint2D::tearDown()
{
    delete a; delete b;
    delete c; delete d;
    delete e; delete f;
}

void TestEvaPoint2D::distanceTest()
{
	CPPUNIT_ASSERT_EQUAL(a->distance(*b), 1);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(c->distance(*d), 1.414213562, 0.000001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(e->distance(*f), 1.414213562, 0.000001);
}
