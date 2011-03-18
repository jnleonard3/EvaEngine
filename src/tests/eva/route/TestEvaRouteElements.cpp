#include "TestEvaRouteElements.h"

#include "eva/route/evaRouteGraphEdge.h"
#include "eva/route/evaRouteNode.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestRouteElements);

void TestRouteElements::setUp()
{
	eva::Point3Df a(4.0f,5.0f, 1.0f), b(3.0f,1.0f, 2.0f), c(8.0f,4.0f,7.0f), d(1.0f,2.0f,3.0f);
	nodeA = new eva::RouteNode(a);
	nodeB = new eva::RouteNode(b);
	nodeC = new eva::RouteNode(c);
	nodeD = new eva::RouteNode(d);
	edgeA = new eva::RouteGraphEdge(eva::ROUTEEDGE_DIRECT,nodeA,nodeB);
	edgeB = new eva::RouteGraphEdge(eva::ROUTEEDGE_DIRECT,nodeB,nodeC);
	edgeC = new eva::RouteGraphEdge(eva::ROUTEEDGE_DIRECT,nodeC,nodeD);
	edgeD = new eva::RouteGraphEdge(eva::ROUTEEDGE_DIRECT,nodeD,nodeA);
	edgeE = new eva::RouteGraphEdge(eva::ROUTEEDGE_SEMIDIRECT,nodeA,nodeC);
	edgeF = new eva::RouteGraphEdge(eva::ROUTEEDGE_SEMIDIRECT,nodeB,nodeD);

	edgeArrayA = new eva::RouteGraphEdge*[3];
	edgeArrayA[0] = edgeA;
	edgeArrayA[1] = edgeD;
	edgeArrayA[2] = edgeE;
	edgeArrayB = new eva::RouteGraphEdge*[3];
	edgeArrayB[0] = edgeF;
	edgeArrayB[1] = edgeB;
	edgeArrayB[2] = edgeA;
}

void TestRouteElements::tearDown()
{
	delete nodeA;
	delete nodeB;
	delete nodeC;
	delete nodeD;
	delete edgeA;
	delete edgeB;
	delete edgeC;
	delete edgeD;
	delete edgeE;
	delete edgeF;
	delete edgeArrayA;
	delete edgeArrayB;
}

void TestRouteElements::edgeAccessorsTest()
{
	CPPUNIT_ASSERT_EQUAL(nodeA, edgeA->getNodeTo());
	CPPUNIT_ASSERT_EQUAL(nodeB, edgeB->getNodeTo());
	CPPUNIT_ASSERT_EQUAL(nodeC, edgeC->getNodeTo());
	CPPUNIT_ASSERT_EQUAL(nodeD, edgeD->getNodeTo());

	CPPUNIT_ASSERT_EQUAL(nodeB, edgeA->getNodeFrom());
	CPPUNIT_ASSERT_EQUAL(nodeC, edgeB->getNodeFrom());
	CPPUNIT_ASSERT_EQUAL(nodeD, edgeC->getNodeFrom());
	CPPUNIT_ASSERT_EQUAL(nodeA, edgeD->getNodeFrom());

	CPPUNIT_ASSERT_EQUAL((e_uchar8)0, edgeA->getNumEdgesConnected());
	CPPUNIT_ASSERT_EQUAL((eva::RouteGraphEdge*)0, edgeA->getConnectedEdge(0));

	eva::RouteGraphEdge a(eva::ROUTEEDGE_INDIRECT,nodeA,nodeB,3,edgeArrayA);
	CPPUNIT_ASSERT_EQUAL(nodeA, a.getNodeTo());
	CPPUNIT_ASSERT_EQUAL(nodeB, a.getNodeFrom());
	CPPUNIT_ASSERT_EQUAL((e_uchar8)3, a.getNumEdgesConnected());
	CPPUNIT_ASSERT_EQUAL(edgeA, a.getConnectedEdge(0));
	CPPUNIT_ASSERT_EQUAL(edgeD, a.getConnectedEdge(1));
	CPPUNIT_ASSERT_EQUAL(edgeE, a.getConnectedEdge(2));
	CPPUNIT_ASSERT_EQUAL((eva::RouteGraphEdge*)0, a.getConnectedEdge(3));
}

void TestRouteElements::nodeAccessorsTest()
{
	CPPUNIT_ASSERT_EQUAL((e_float32)4.0f, nodeA->getPoint().x());
	CPPUNIT_ASSERT_EQUAL((e_float32)5.0f, nodeA->getPoint().y());
	CPPUNIT_ASSERT_EQUAL((e_float32)1.0f, nodeA->getPoint().z());

	CPPUNIT_ASSERT_EQUAL((e_uchar8)0, nodeA->getNumEdgesFrom());
	CPPUNIT_ASSERT_EQUAL((e_uchar8)0, nodeA->getNumEdgesTo());

	CPPUNIT_ASSERT_EQUAL((eva::RouteGraphEdge*)0, nodeA->getFromEdge(0));
	CPPUNIT_ASSERT_EQUAL((eva::RouteGraphEdge*)0, nodeA->getToEdge(0));

	eva::RouteNode a(eva::Point3Df(1.0f,2.0f,3.0f),3,edgeArrayA,edgeArrayB);
	CPPUNIT_ASSERT_EQUAL((e_float32)1.0f, a.getPoint().x());
	CPPUNIT_ASSERT_EQUAL((e_float32)2.0f, a.getPoint().y());
	CPPUNIT_ASSERT_EQUAL((e_float32)3.0f, a.getPoint().z());

	CPPUNIT_ASSERT_EQUAL((e_uchar8)3, a.getNumEdgesFrom());
	CPPUNIT_ASSERT_EQUAL((e_uchar8)3, a.getNumEdgesTo());

	CPPUNIT_ASSERT_EQUAL(edgeA, a.getFromEdge(0));
	CPPUNIT_ASSERT_EQUAL(edgeD, a.getFromEdge(1));
	CPPUNIT_ASSERT_EQUAL(edgeE, a.getFromEdge(2));

	CPPUNIT_ASSERT_EQUAL(edgeF, a.getToEdge(0));
	CPPUNIT_ASSERT_EQUAL(edgeB, a.getToEdge(1));
	CPPUNIT_ASSERT_EQUAL(edgeA, a.getToEdge(2));
}

void TestRouteElements::edgeSettersTest()
{

}

void TestRouteElements::nodeSettersTest()
{

}

void TestRouteElements::clearTest()
{

}
