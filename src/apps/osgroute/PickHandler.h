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

namespace eva
{
	class RouteGraph;
	class RouteNode;
}

class PickHandler : public osgGA::GUIEventHandler
{
	public:
		PickHandler(osg::Group *root, eva::RouteGraph &graph, osg::Group *routeNodesGraph)
		:mGraph(graph),mRoot(root),mClicked(0),mRouteNodesGraph(routeNodesGraph),firstNode(0),secondNode(0){};
		virtual ~PickHandler(){};

		bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
		virtual void pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea);
	private:
		eva::RouteGraph &mGraph;
		osg::Group *mRoot,*mClicked, *mRouteNodesGraph;
		eva::RouteNode *firstNode, *secondNode;
};

#endif /* PICKHANDLER_H_ */
