#ifndef PICKHANDLER_H_
#define PICKHANDLER_H_

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

#include "eva/math/evaPoint.h"

namespace eva
{
	class RouteGraph;
}

class IntelligentAgent;

class PickHandler : public osgGA::GUIEventHandler
{
	public:
		PickHandler(osg::Group *root, eva::RouteGraph &graph, osg::Group *routeNodesGraph, IntelligentAgent& agent)
		:mGraph(graph),mRoot(root),mClicked(0),mRouteNodesGraph(routeNodesGraph),isActive(false),mFirstPoint(),mSecondPoint(),mAgent(agent){};
		virtual ~PickHandler(){};

		bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
		virtual void pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea);
	private:
		eva::RouteGraph &mGraph;
		osg::Group *mRoot,*mClicked, *mRouteNodesGraph;
		bool isActive;
		eva::Point3Dd mFirstPoint, mSecondPoint;
		IntelligentAgent& mAgent;
};

#endif
