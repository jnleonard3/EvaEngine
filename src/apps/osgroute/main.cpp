/*
 *  Copyright (c) 2011, EvaEngine Project
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions are met:
 *		* Redistributions of source code must retain the above copyright
 *		  notice, this list of conditions and the following disclaimer.
 *		* Redistributions in binary form must reproduce the above copyright
 *		  notice, this list of conditions and the following disclaimer in the
 *		  documentation and/or other materials provided with the distribution.
 *
 *	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *	DISCLAIMED. IN NO EVENT SHALL THE EVAENGINE PROEJCT BE LIABLE FOR ANY
 *	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "RoadCommon.h"
#include "PickHandler.h"
#include "OsgHelper.h"
#include "FixedQuadtree.h"
#include "IntelligentAgentManager.h"

#include "eva/osg/route/RouteGraphVisualizer.h"
#include "eva/route/evaRouteNode.h"
#include "eva/route/evaRouteGraph.h"
#include "eva/math/evaVector.h"
#include "eva/math/evaMathCommon.h"
#include "eva/geometry/basic/3d/evaLine3D.h"
#include "eva/math/evaPoint.h"
#include "eva/geometry/basic/2d/evaRectangle.h"
#include "eva/geometry/basic/2d/evaSquare.h"

#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/StateSet>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/PositionAttitudeTransform>
#include <osgGA/TrackballManipulator>
#include <osgGA/UFOManipulator>

#include <iostream>
#include <list>
#include <vector>

const double CENTIMETER_ACCURACY = 0.01;
const double MILLIMETER_ACCURACY = 0.001;
const double ROAD_WIDTH = 5.0;
const double TIME_CONSTANT = 0.05;

void printPoint(eva::Point3Dd point)
{
	std::cout << point.x() << "," << point.y() << "," << point.z() << "\n";
}

osg::Group* drawRoads(std::vector<Road>& roads)
{
	osg::Group* root = new osg::Group();

	osg::Texture2D* asphaltTexture = new osg::Texture2D;
	asphaltTexture->setDataVariance(osg::Object::DYNAMIC);
	osg::Image* asphaltFace = osgDB::readImageFile("data/asphalt.png");
	if(!asphaltFace)
		return 0;
	asphaltTexture->setImage(asphaltFace);

	osg::DrawElementsUInt* roadElements =  new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	roadElements->push_back(3);
	roadElements->push_back(2);
	roadElements->push_back(1);
	roadElements->push_back(0);

	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(0.7f, 0.7f, 0.7f, 1.0f));
	colors->push_back(osg::Vec4(0.7f, 0.7f, 0.7f, 1.0f));
	colors->push_back(osg::Vec4(0.7f, 0.7f, 0.7f, 1.0f));
	colors->push_back(osg::Vec4(0.7f, 0.7f, 0.7f, 1.0f));

	osg::StateSet* state = new osg::StateSet();
	state->setTextureAttributeAndModes(0,asphaltTexture,osg::StateAttribute::ON);

	eva::Vector3Dd downVector(0.0,0.0,-1.0);

	for(e_uint32 i = 0; i < roads.size(); ++i)
	{
		for(std::list<eva::RouteNode*>::iterator j = roads[i].nodes.begin(); j != roads[i].nodes.end(); ++j)
		{
			std::list<eva::RouteNode*>::iterator next = j;
			++next;
			if(next != roads[i].nodes.end())
			{
				osg::Geode* roadGeode = new osg::Geode();
				osg::Geometry* roadGeometry = new osg::Geometry();
				roadGeode->addDrawable(roadGeometry);

				eva::Point3Dd start = (*j)->getPoint(), end = (*next)->getPoint();
				eva::Vector3Dd vec(end,start);
				vec.normalize();
				eva::Vector3Dd perp = vec.crossProduct(downVector);
				perp.normalize();
				perp.scale(ROAD_WIDTH/2.0);

				osg::Vec3Array* roadVertices = new osg::Vec3Array;
				roadVertices->push_back(osg::Vec3(start.x()-perp.i(), start.y()-perp.j(), start.z()-perp.k())); // front left
				roadVertices->push_back(osg::Vec3(start.x()+perp.i(), start.y()+perp.j(), start.z()+perp.k())); // front right
				roadVertices->push_back(osg::Vec3(end.x()+perp.i(), end.y()+perp.j(), end.z()+perp.k())); // back right
				roadVertices->push_back(osg::Vec3(end.x()-perp.i(), end.y()-perp.j(), end.z()-perp.k())); // back left

				osg::Vec2Array* texcoords = new osg::Vec2Array(4);
				(*texcoords)[0].set(0.0f,0.0f);
				(*texcoords)[1].set(0.0f,1.0f);
				(*texcoords)[2].set(1.0f,0.0f);
				(*texcoords)[3].set(1.0f,1.0f);

				roadGeometry->setVertexArray(roadVertices);
				roadGeometry->addPrimitiveSet(roadElements);
				roadGeometry->setColorArray(colors);
				roadGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
				roadGeometry->setTexCoordArray(0,texcoords);

				roadGeode->setStateSet(state);
				root->addChild(roadGeode);
			}
		}
	}
	return root;
}

int main(int argc, char** argv)
{
	eva::RouteGraph graph(8,1);
	std::vector<Road> listOfRoads;
	bool flipOne = true, flipTwo = false, flipThree = true;;
	e_uint32 numOfRoads = 20;
	e_double64 extra = 5.0 + ROAD_WIDTH;
	e_double64 spacing = 50.0;
	e_double64 max = spacing*numOfRoads+extra;
	for(e_double64 x = extra; x <= max; x += spacing)
	{
		if(flipTwo)
		{
			RoadCommon::addRoad(eva::Point3Dd(x,0.0,0.0),eva::Point3Dd(x,max+extra,0.0),listOfRoads,graph);
			if(flipThree)
				RoadCommon::addRoad(eva::Point3Dd(x-ROAD_WIDTH,max+extra,0.0),eva::Point3Dd(x-ROAD_WIDTH,0.0,0.0),listOfRoads,graph);
		}
		else
		{
			RoadCommon::addRoad(eva::Point3Dd(x,max+extra,0.0),eva::Point3Dd(x,0.0,0.0),listOfRoads,graph);
			if(flipThree)
				RoadCommon::addRoad(eva::Point3Dd(x-ROAD_WIDTH,0.0,0.0),eva::Point3Dd(x-ROAD_WIDTH,max+extra,0.0),listOfRoads,graph);
		}

		if(flipOne)
		{
			RoadCommon::addRoad(eva::Point3Dd(max+extra,x,0.0),eva::Point3Dd(0.0,x,0.0),listOfRoads,graph);
			if(flipThree)
				RoadCommon::addRoad(eva::Point3Dd(0.0,x+ROAD_WIDTH,0.0),eva::Point3Dd(max+extra,x+ROAD_WIDTH,0.0),listOfRoads,graph);
		}
		else
		{
			RoadCommon::addRoad(eva::Point3Dd(0.0,x,0.0),eva::Point3Dd(max+extra,x,0.0),listOfRoads,graph);
			if(flipThree)
				RoadCommon::addRoad(eva::Point3Dd(max+extra,x+ROAD_WIDTH,0.0),eva::Point3Dd(0.0,x+ROAD_WIDTH,0.0),listOfRoads,graph);
		}

		flipThree = flipOne & flipTwo;
		flipOne = !flipOne;
		if(flipOne)
			flipTwo = !flipTwo;
	}

	osg::Group* root = new osg::Group();
	eva::Rectangled base(eva::Point2Dd(-extra,-extra),eva::Point2Dd(max+(extra*2.0),max+(extra*2.0)));
	osg::Group* grassBase = OsgHelper::drawBase(eva::Point3Dd(-extra,-extra,0.0),eva::Point3Dd(max+(extra*2.0),max+(extra*2.0),0.0),-0.1,eva::Point3Dd(39.0/255.0,115.0/255.0,31.0/255.0));
	root->addChild(grassBase);

	osg::Group* graphRoot = eva::RouteGraphVisualizer::drawRouteGraph(graph);
	graphRoot->setNodeMask(0);
	root->addChild(graphRoot);

	osg::Group* roadsRoot = drawRoads(listOfRoads);
	root->addChild(roadsRoot);

	FixedQuadtree<int> quadtree(eva::Squared(base.constRef().getCenter(),(max+(extra*2.0)+extra)/2.0),10);

	for(std::vector<Road>::iterator j = listOfRoads.begin(); j != listOfRoads.end(); ++j)
		for(std::list<eva::RouteNode*>::iterator i = j->nodes.begin(); i != j->nodes.end(); ++i)
			quadtree.insert(0,eva::Point2Dd((*i)->getPoint().x(),(*i)->getPoint().y()));

	int* data = new int(8);
	//quadtree.insert(data,eva::Point2Dd(50.0,50.0));

	quadtree.insert(data,eva::Rectangled(50.0,50.0,100.0,100.0));
	quadtree.move(data,eva::Rectangled(50.0,50.0,100.0,100.0),eva::Rectangled(500.0,500.0,550.0,550.0));
	quadtree.move(data,eva::Rectangled(500.0,500.0,550.0,550.0),eva::Rectangled(50.0,50.0,100.0,100.0));

	//quadtree.insert(data,eva::Rectangle2Dd(500.0,500.0,550.0,550.0));
	//root->addChild(OsgHelper::drawFilledSquare(eva::Squared(eva::Point2Dd(525.0,525.0),25.0),0.2,eva::Point3Dd(0.0,1.0,0.0)));

	std::vector<QuadAppearance> quads = quadtree.getAppearance();

	osg::Group* quadsRoot = OsgHelper::drawQuadtree(quads);
	root->addChild(quadsRoot);
	quadsRoot->setNodeMask(0);

	BasicQuadtreeLineIntersectVisitor<int> visitor;
	quadtree.lineOfSightQuery< BasicQuadtreeLineIntersectVisitor<int> >(eva::Line2Dd(50.0,50.0,550.0,550),visitor);

	IntelligentAgentManager intelligentManager(graph, eva::Squared(base.constRef().getCenter(),(max+(extra*2.0)+extra)/2.0));
	IntelligentAgent& agent = intelligentManager.addNewAgent(10.0,15.0,0.0);

	OsgHelper::IntelligentAgentRender imRender(intelligentManager);
	root->addChild(imRender.mRenderRoot);

	root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	osgViewer::Viewer viewer;
	viewer.addEventHandler(new PickHandler(root,graph,graphRoot,agent));
	viewer.setSceneData(root);
	viewer.setCameraManipulator(new osgGA::TrackballManipulator);
	viewer.realize();

	while(!viewer.done())
	{
		intelligentManager.updatePositions(TIME_CONSTANT);

		imRender.redraw();
		viewer.frame();

		intelligentManager.everybodyAct();

		usleep(10000);
	}

	return 0;
}
