#include "RouteGraphVisualizer.h"

#include "eva/Typedefs.h"
#include "eva/route/evaRouteNode.h"
#include "eva/route/evaRouteGraphEdge.h"
#include "eva/route/evaRouteGraph.h"
#include "eva/structures/evaBlockArray.h"

#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include <osg/Array>

namespace eva
{
	osg::Box* RouteGraphVisualizer::UNIT_CUBE = 0;
	osg::Box* RouteGraphVisualizer::UNCONNECTED_UNIT_CUBE = 0;
	osg::ShapeDrawable* RouteGraphVisualizer::UNIT_CUBE_DRAWABLE = 0;
	osg::ShapeDrawable* RouteGraphVisualizer::UNCONNECTED_UNIT_CUBE_DRAWABLE = 0;
	osg::Geode* RouteGraphVisualizer::UNIT_CUBE_GEODE = 0;
	osg::Geode* RouteGraphVisualizer::UNCONNECTED_UNIT_CUBE_GEODE = 0;

	osg::DrawElementsUInt* RouteGraphVisualizer::OSG_LINE = 0;
	osg::Vec4Array* RouteGraphVisualizer::OSG_RED_LINE_COLORS = 0;
	osg::Vec4Array* RouteGraphVisualizer::OSG_BLUE_LINE_COLORS = 0;

	osg::Group* RouteGraphVisualizer::drawRouteGraph(const RouteGraph& graph)
	{
		initialize();
		osg::Group* root = new osg::Group();

		const BlockArray<RouteNode>& nodes = graph.getNodes();
		for(e_uint32 i = 0; i < nodes.getSize(); ++i)
		{
			Point3Dd pt = nodes.at(i).getPoint();
			osg::PositionAttitudeTransform* CubeXForm = new osg::PositionAttitudeTransform();
			CubeXForm->setPosition(osg::Vec3(pt.x(),pt.y(),pt.z()));
			if(nodes.at(i).isUnconnected())
				CubeXForm->addChild(UNCONNECTED_UNIT_CUBE_GEODE);
			else
				CubeXForm->addChild(UNIT_CUBE_GEODE);
			root->addChild(CubeXForm);
		}

		const BlockArray<RouteGraphEdge>& edges = graph.getEdges();
		for(e_uint32 i = 0; i < edges.getSize(); ++i)
		{
			const RouteGraphEdge &edge = edges.at(i);
			if(edge.getType() != ROUTEEDGE_INVALID && edge.getNodeTo() != 0 && edge.getNodeFrom() != 0)
			{
				Point3Dd from = edge.getNodeFrom()->getPoint(), to = edge.getNodeTo()->getPoint();
				osg::Geode* lineGeode = new osg::Geode();
				osg::Geometry* lineGeometry = new osg::Geometry();
				lineGeode->addDrawable(lineGeometry);
				osg::Vec3Array* lineVertices = new osg::Vec3Array;
				lineVertices->push_back(osg::Vec3(from.x(),from.y(),from.z()+0.1)); // front left
				lineVertices->push_back(osg::Vec3(to.x(),to.y(),to.z()+0.1)); // front right
				lineGeometry->setVertexArray(lineVertices);
			    lineGeometry->addPrimitiveSet(OSG_LINE);
			    lineGeometry->setColorArray(OSG_RED_LINE_COLORS);
			    lineGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
			    lineGeode->addDrawable(lineGeometry);
			    root->addChild(lineGeode);
			}
		}
		return root;
	}

	osg::Group* RouteGraphVisualizer::drawRouteNode(const RouteNode& node, e_double64 zoffset)
	{
		initialize();
		osg::Group* root = new osg::Group();

		Point3Dd pt = node.getPoint();
		osg::PositionAttitudeTransform* CubeXForm = new osg::PositionAttitudeTransform();
		CubeXForm->setPosition(osg::Vec3(pt.x(),pt.y(),pt.z()+zoffset));
		CubeXForm->addChild(UNIT_CUBE_GEODE);
		root->addChild(CubeXForm);

		for(e_uint32 i = 0; i < node.getNumEdgesFrom(); ++i)
		{
			const RouteGraphEdge &edge = *node.getFromEdge(i);
			if(node.getFromEdge(i) != 0 && edge.getType() != ROUTEEDGE_INVALID && edge.getNodeTo() != 0 && edge.getNodeFrom() != 0)
			{
				Point3Dd from = edge.getNodeFrom()->getPoint(), to = edge.getNodeTo()->getPoint();
				osg::Geode* lineGeode = new osg::Geode();
				osg::Geometry* lineGeometry = new osg::Geometry();
				lineGeode->addDrawable(lineGeometry);
				osg::Vec3Array* lineVertices = new osg::Vec3Array;
				lineVertices->push_back(osg::Vec3(from.x(),from.y(),from.z()+zoffset)); // front left
				lineVertices->push_back(osg::Vec3(to.x(),to.y(),to.z())); // front right
				lineGeometry->setVertexArray(lineVertices);
			    lineGeometry->addPrimitiveSet(OSG_LINE);
			    lineGeometry->setColorArray(OSG_BLUE_LINE_COLORS);
			    lineGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
			    lineGeode->addDrawable(lineGeometry);
			    root->addChild(lineGeode);
			}
		}

		for(e_uint32 i = 0; i < node.getNumEdgesTo(); ++i)
		{
			const RouteGraphEdge &edge = *node.getToEdge(i);
			if(node.getToEdge(i) != 0 && edge.getType() != ROUTEEDGE_INVALID && edge.getNodeTo() != 0 && edge.getNodeFrom() != 0)
			{
				Point3Dd from = edge.getNodeFrom()->getPoint(), to = edge.getNodeTo()->getPoint();
				osg::Geode* lineGeode = new osg::Geode();
				osg::Geometry* lineGeometry = new osg::Geometry();
				lineGeode->addDrawable(lineGeometry);
				osg::Vec3Array* lineVertices = new osg::Vec3Array;
				lineVertices->push_back(osg::Vec3(from.x(),from.y(),from.z())); // front left
				lineVertices->push_back(osg::Vec3(to.x(),to.y(),to.z()+zoffset)); // front right
				lineGeometry->setVertexArray(lineVertices);
			    lineGeometry->addPrimitiveSet(OSG_LINE);
			    lineGeometry->setColorArray(OSG_BLUE_LINE_COLORS);
			    lineGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
			    lineGeode->addDrawable(lineGeometry);
			    root->addChild(lineGeode);
			}
		}

		return root;
	}

	osg::Group* RouteGraphVisualizer::drawRoutePath(const PathNode *path, e_double64 zoffset)
	{
		initialize();
		osg::Group* root = new osg::Group();

		const PathNode *currentNode = path;
		while(currentNode != 0)
		{
			Point3Dd pt = currentNode->mPoint;
			osg::PositionAttitudeTransform* CubeXForm = new osg::PositionAttitudeTransform();
			CubeXForm->setPosition(osg::Vec3(pt.x(),pt.y(),pt.z()+zoffset));
			CubeXForm->addChild(UNIT_CUBE_GEODE);
			root->addChild(CubeXForm);

			const PathNode *nextNode = 0;
			PathEdge *currentEdge = currentNode->mNext;
			if(currentEdge)
			{
				nextNode = currentEdge->mNext;
				if(nextNode)
				{
					Point3Dd to = nextNode->mPoint;
					osg::Geode* lineGeode = new osg::Geode();
					osg::Geometry* lineGeometry = new osg::Geometry();
					lineGeode->addDrawable(lineGeometry);
					osg::Vec3Array* lineVertices = new osg::Vec3Array;
					lineVertices->push_back(osg::Vec3(pt.x(),pt.y(),pt.z()+zoffset)); // front left
					lineVertices->push_back(osg::Vec3(to.x(),to.y(),to.z()+zoffset)); // front right
					lineGeometry->setVertexArray(lineVertices);
					lineGeometry->addPrimitiveSet(OSG_LINE);
					lineGeometry->setColorArray(OSG_BLUE_LINE_COLORS);
					lineGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
					lineGeode->addDrawable(lineGeometry);
					root->addChild(lineGeode);
				}
			}
			currentNode = nextNode;
		}
		return root;
	}

	void RouteGraphVisualizer::initialize()
	{
		if(1)
		{
			UNIT_CUBE = new osg::Box( osg::Vec3(0,0,0), 0.5f);
			UNIT_CUBE_DRAWABLE = new osg::ShapeDrawable(UNIT_CUBE);
			UNIT_CUBE_GEODE = new osg::Geode();
			UNIT_CUBE_GEODE->addDrawable(UNIT_CUBE_DRAWABLE);

			UNCONNECTED_UNIT_CUBE = new osg::Box( osg::Vec3(0,0,10), 0.5f);
			UNCONNECTED_UNIT_CUBE_DRAWABLE = new osg::ShapeDrawable(UNCONNECTED_UNIT_CUBE);
			UNCONNECTED_UNIT_CUBE_DRAWABLE->setColor(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
			UNCONNECTED_UNIT_CUBE_GEODE = new osg::Geode();
			UNCONNECTED_UNIT_CUBE_GEODE->addDrawable(UNCONNECTED_UNIT_CUBE_DRAWABLE);

			OSG_LINE =  new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0);
			OSG_LINE->push_back(0);
			OSG_LINE->push_back(1);

			OSG_RED_LINE_COLORS = new osg::Vec4Array;
			OSG_RED_LINE_COLORS->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
			OSG_RED_LINE_COLORS->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) );

			OSG_BLUE_LINE_COLORS = new osg::Vec4Array;
			OSG_BLUE_LINE_COLORS->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
			OSG_BLUE_LINE_COLORS->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f) );
		}
	}

}
