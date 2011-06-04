#ifndef TEST_EVA_POINT2D_H_
#define TEST_EVA_POINT2D_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "eva/math/evaPoint.h"

class TestEvaPoint2D : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE(TestEvaPoint2D);
    CPPUNIT_TEST(distanceTest);
    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

    protected:
        void distanceTest();

    private:
        eva::Point2Di *a, *b;
        eva::Point2Df *c, *d;
        eva::Point2Dd *e, *f;
};

#endif
