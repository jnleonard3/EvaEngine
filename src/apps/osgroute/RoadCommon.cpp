#include "RoadCommon.h"

#include "eva/route/evaRouteNode.h"
#include "eva/route/evaRouteGraph.h"
#include "eva/math/evaVector3D.h"
#include "eva/math/evaMathCommon.h"
#include "eva/geometry/basic/3d/evaLine3D.h"
#include "eva/geometry/intersection/Intersection3D.h"

void RoadCommon::addRoad(eva::Point3Dd start, eva::Point3Dd end, std::vector<Road> &roads, eva::RouteGraph &graph)
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
		if(eva::Intersection3D::Intersection<e_double64>(newRoadLine,roadLine,intersection))
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
				if(eva::Intersection3D::Intersects<e_double64>(newRoadLine,roadSegmentLine))
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
