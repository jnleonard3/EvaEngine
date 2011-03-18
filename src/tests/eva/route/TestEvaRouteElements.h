#ifndef TEST_EVA_ROUTEGRAPHEDGE_H_
#define TEST_EVA_ROUTEGRAPHEDGE_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace eva
{
	class RouteGraphEdge;
	class RouteNode;
}

class TestRouteElements : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE(TestRouteElements);
    CPPUNIT_TEST(edgeAccessorsTest);
    CPPUNIT_TEST(nodeAccessorsTest);
    CPPUNIT_TEST(edgeSettersTest);
    CPPUNIT_TEST(nodeSettersTest);
    CPPUNIT_TEST(clearTest);
    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

    protected:
        void edgeAccessorsTest();
        void nodeAccessorsTest();
        void edgeSettersTest();
        void nodeSettersTest();
        void clearTest();

    private:
        eva::RouteGraphEdge *edgeA, *edgeB, *edgeC, *edgeD, *edgeE, *edgeF;
        eva::RouteNode *nodeA, *nodeB, *nodeC, *nodeD;
        eva::RouteGraphEdge **edgeArrayA, **edgeArrayB;
};

#endif
