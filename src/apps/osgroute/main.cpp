#include "eva/osg/route/RouteGraphVisualizer.h"
#include "eva/route/evaRouteNode.h"
#include "eva/route/evaRouteGraph.h"
#include "eva/math/evaVector3D.h"
#include "eva/math/evaMathCommon.h"
#include "eva/geometry/evaLine3D.h"

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

#include <iostream>
#include <list>
#include <vector>

const double CENTIMETER_ACCURACY = 0.01;
const double MILLIMETER_ACCURACY = 0.001;
const double ROAD_WIDTH = 3.0;

void printPoint(eva::Point3Dd point)
{
	std::cout << point.x() << "," << point.y() << "," << point.z() << "\n";
}

struct Road
{
	Road(std::list<eva::RouteNode*> list):nodes(list){};
	std::list<eva::RouteNode*> nodes;
};

struct RoadIntersection
{
	RoadIntersection(Road *road, eva::Point3Dd pt):intersectedWith(road),intersectionPoint(pt){};
	Road *intersectedWith;
	eva::Point3Dd intersectionPoint;
};

void addRoad(eva::Point3Dd start, eva::Point3Dd end, std::vector<Road> &roads, eva::RouteGraph &graph)
{
	eva::RouteNode &startNode = graph.createNode(start);
	eva::RouteNode &endNode = graph.createNode(end);
	eva::Line3Dd newRoadLine(startNode.getPoint(),endNode.getPoint());
	std::list<eva::RouteNode*> roadNodes;
	roadNodes.push_back(&startNode);

	std::vector<RoadIntersection> intersections;
	for(std::vector<Road>::iterator i = roads.begin(); i != roads.end(); ++i)
	{
		eva::Line3Dd roadLine(i->nodes.front()->getPoint(),i->nodes.back()->getPoint());
		eva::Point3Dd intersection;
		if(newRoadLine.intersects(roadLine,intersection))
			intersections.push_back(RoadIntersection(&*i,intersection));
	}

	for(e_uint32 i = 0; i < intersections.size(); ++i)
	{
		e_double64 minDist = 1000000.0;
		e_uint32 index = 0;
		for(e_uint32 j = i; j < intersections.size(); ++j)
		{
			e_double64 dist = intersections[j].intersectionPoint.distance(startNode.getPoint());
			if(dist < minDist)
			{
				index = j;
				minDist = dist;
			}
		}
		RoadIntersection temp = intersections[i];
		intersections[i] = intersections[index];
		intersections[index] = temp;
	}

	for(std::vector<RoadIntersection>::iterator i = intersections.begin(); i != intersections.end(); ++i)
	{
		eva::Point3Dd intersectionPoint = i->intersectionPoint;
		eva::RouteNode &intersectionNode = graph.createNode(intersectionPoint);
		for(std::list<eva::RouteNode*>::iterator j = i->intersectedWith->nodes.begin(); j != i->intersectedWith->nodes.end(); ++j)
		{
			std::list<eva::RouteNode*>::iterator next = j;
			++next;
			if(next != i->intersectedWith->nodes.end())
			{
				eva::Line3Dd roadSegmentLine((**j).getPoint(), (**next).getPoint());
				if(newRoadLine.intersects(roadSegmentLine))
				{
					graph.disconnectNodes((**j),(**next));
					graph.connectNodes((**j),intersectionNode);
					graph.connectNodes(intersectionNode,(**next));
					i->intersectedWith->nodes.insert(next,&intersectionNode);
					roadNodes.push_back(&intersectionNode);
					break;
				}
			}
			else
				roadNodes.push_back(&intersectionNode);
		}
	}
	roadNodes.push_back(&endNode);

	for(std::list<eva::RouteNode*>::iterator i = roadNodes.begin(); i != roadNodes.end(); ++i)
	{
		std::list<eva::RouteNode*>::iterator next = i;
		++next;
		if(next != roadNodes.end())
			graph.connectNodes(**i,**next);
	}

	roads.push_back(Road(roadNodes));
}

class PickHandler : public osgGA::GUIEventHandler
{
	public:
		PickHandler(osg::Group *root, eva::RouteGraph &graph)
		:mGraph(graph),mRoot(root),mClicked(0),firstNode(0),secondNode(0){};
		virtual ~PickHandler(){};

		bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
		virtual void pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea);
	private:
		eva::RouteGraph &mGraph;
		osg::Group *mRoot,*mClicked;
		eva::RouteNode *firstNode, *secondNode;
};

bool PickHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
{
    switch(ea.getEventType())
    {
        case(osgGA::GUIEventAdapter::PUSH):
        {
        	if(ea.getButton() == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON)
        	{
				osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
				if (view) pick(view,ea);
				return false;
        	}
        }
        default:
            return false;
    }
}

void PickHandler::pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea)
{
    osgUtil::LineSegmentIntersector::Intersections intersections;

    float x = ea.getX();
    float y = ea.getY();

    if (view->computeIntersections(x,y,intersections))
    {
    	if(intersections.begin() != intersections.end())
        {
    		osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();

    		mRoot->removeChild(mClicked);

    		firstNode = secondNode;
    		secondNode = mGraph.findClosest(hitr->getWorldIntersectPoint().x(),hitr->getWorldIntersectPoint().y(),hitr->getWorldIntersectPoint().z());
    		if(firstNode && secondNode)
    		{
    			std::list<eva::RoutePathElement> pathResults;
    			mGraph.findPath(*firstNode,*secondNode,pathResults);

				mClicked = new osg::Group;
				osg::Group *newNodeDrawn = eva::RouteGraphVisualizer::drawRoutePath(pathResults,5.0);
				mClicked->addChild(newNodeDrawn);

				mRoot->addChild(mClicked);
    		}
        }
    }
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

	osg::Vec2Array* texcoords = new osg::Vec2Array(5);
	(*texcoords)[0].set(0.0f,0.0f);
	(*texcoords)[1].set(0.0f,1.0f);
	(*texcoords)[2].set(1.0f,0.0f);
	(*texcoords)[3].set(1.0f,1.0f);

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
	bool flip = true, flipt = true;
	e_uint32 numOfRoads = 20;
	e_double64 spacing = 120.0;
	e_double64 max = spacing*numOfRoads+5.0;
	for(e_double64 x = 5.0; x <= max; x += spacing)
	{
		if(flipt)
			addRoad(eva::Point3Dd(x,0.0,0.0),eva::Point3Dd(x,max+5.0,0.0),listOfRoads,graph);
		else
			addRoad(eva::Point3Dd(x,max+5.0,0.0),eva::Point3Dd(x,0.0,0.0),listOfRoads,graph);

		if(flip)
			addRoad(eva::Point3Dd(max+5,x,0.0),eva::Point3Dd(0.0,x,0.0),listOfRoads,graph);
		else
			addRoad(eva::Point3Dd(0.0,x,0.0),eva::Point3Dd(max+5,x,0.0),listOfRoads,graph);

		flip = !flip;
		if(flip)
			flipt = !flipt;
	}

	//addRoad(eva::Point3Dd(2.0,4.0,0.0),eva::Point3Dd(89.0,72.0,0.0),listOfRoads,graph);

	osg::Group* root = new osg::Group();

	//osg::Group* graphRoot = eva::RouteGraphVisualizer::drawRouteGraph(graph);
	//root->addChild(graphRoot);

	osg::Group* roadsRoot = drawRoads(listOfRoads);
	root->addChild(roadsRoot);

	root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	osgViewer::Viewer viewer;
	viewer.addEventHandler(new PickHandler(root,graph));
	viewer.setSceneData(root);
	viewer.run();

	return 0;
}
