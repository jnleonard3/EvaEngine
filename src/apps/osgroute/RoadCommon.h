#ifndef ROADCOMMON_H_
#define ROADCOMMON_H_

#include "eva/math/evaPoint.h"

#include <vector>
#include <list>

namespace eva
{
	class RouteNode;
	class RouteGraph;
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

class RoadCommon
{
	public:
		static void addRoad(eva::Point3Dd start, eva::Point3Dd end, std::vector<Road> &roads, eva::RouteGraph &graph);
	private:
		RoadCommon(){};
};

#endif
