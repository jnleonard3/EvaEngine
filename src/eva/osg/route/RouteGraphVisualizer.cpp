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

namespace eva
{
	osg::Group* RouteGraphVisualizer::drawRouteGraph(const RouteGraph& graph)
	{
		osg::Group* root = new osg::Group();

		osg::Box* unitCube = new osg::Box( osg::Vec3(0,0,0), 0.5f);
		osg::ShapeDrawable* unitCubeDrawable = new osg::ShapeDrawable(unitCube);
		osg::Geode* unitCubeGeode = new osg::Geode();
		unitCubeGeode->addDrawable(unitCubeDrawable);

		osg::Box* unitCubeUnconnected = new osg::Box( osg::Vec3(0,0,10), 0.5f);
		osg::ShapeDrawable* unitCubeUnconnectedDrawable = new osg::ShapeDrawable(unitCubeUnconnected);
		unitCubeUnconnectedDrawable->setColor(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
		osg::Geode* unitCubeUnconnectedGeode = new osg::Geode();
		unitCubeUnconnectedGeode->addDrawable(unitCubeUnconnectedDrawable);

		const BlockArray<RouteNode>& nodes = graph.getNodes();
		for(e_uint32 i = 0; i < nodes.getSize(); ++i)
		{
			Point3Dd pt = nodes.at(i).getPoint();
			osg::PositionAttitudeTransform* CubeXForm = new osg::PositionAttitudeTransform();
			CubeXForm->setPosition(osg::Vec3(pt.x(),pt.y(),pt.z()));
			if(nodes.at(i).isUnconnected())
				CubeXForm->addChild(unitCubeUnconnectedGeode);
			else
				CubeXForm->addChild(unitCubeGeode);
			root->addChild(CubeXForm);
		}

	    osg::DrawElementsUInt* line =  new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0);
	    line->push_back(0);
	    line->push_back(1);

	    osg::Vec4Array* colors = new osg::Vec4Array;
	    colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );
	    colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) );

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
				lineVertices->push_back(osg::Vec3(from.x(),from.y(),from.z())); // front left
				lineVertices->push_back(osg::Vec3(to.x(),to.y(),to.z())); // front right
				lineGeometry->setVertexArray(lineVertices);
			    lineGeometry->addPrimitiveSet(line);
			    lineGeometry->setColorArray(colors);
			    lineGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
			    lineGeode->addDrawable(lineGeometry);
			    root->addChild(lineGeode);
			}
		}

		return root;
	}
}
