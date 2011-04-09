#include "PickHandler.h"

#include "eva/osg/route/RouteGraphVisualizer.h"
#include "eva/route/evaRouteNode.h"
#include "eva/route/evaRouteGraph.h"

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
        case(osgGA::GUIEventAdapter::KEYDOWN):
        {
        	switch(ea.getKey())
        	{
        	case '1':
        		if(mRouteNodesGraph->getNodeMask() == 0)
        			mRouteNodesGraph->setNodeMask(1);
        		else
        			mRouteNodesGraph->setNodeMask(0);
        		break;
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
				osg::Group *newNodeDrawn = eva::RouteGraphVisualizer::drawRoutePath(pathResults,1.0);
				mClicked->addChild(newNodeDrawn);

				mRoot->addChild(mClicked);
    		}
        }
    }
}
