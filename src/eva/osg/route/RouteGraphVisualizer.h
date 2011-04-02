#ifndef ROUTEGRAPHVISUALIZER_H_
#define ROUTEGRAPHVISUALIZER_H_

#include "eva/Typedefs.h"
#include <osg/Array>

#include <list>

namespace osg
{
	class Group;
	class Box;
	class ShapeDrawable;
	class Geode;
	class DrawElementsUInt;
}

namespace eva
{
	class RouteNode;
	class RouteGraph;
	class RoutePathElement;

	class RouteGraphVisualizer
	{
		public:
			virtual ~RouteGraphVisualizer(){};

			static osg::Group* drawRouteGraph(const RouteGraph &graph);

			static osg::Group* drawRouteNode(const RouteNode &node, e_double64 zoffset);

			static osg::Group* drawRoutePath(const std::list<RoutePathElement> &path, e_double64 zoffset);

		private:
			RouteGraphVisualizer(){};

			static void initialize();

			static osg::Box *UNIT_CUBE, *UNCONNECTED_UNIT_CUBE;
			static osg::ShapeDrawable *UNIT_CUBE_DRAWABLE, *UNCONNECTED_UNIT_CUBE_DRAWABLE;
			static osg::Geode *UNIT_CUBE_GEODE, *UNCONNECTED_UNIT_CUBE_GEODE;

			static osg::DrawElementsUInt *OSG_LINE;
			static osg::Vec4Array *OSG_RED_LINE_COLORS, *OSG_BLUE_LINE_COLORS;
	};

}

#endif /* ROUTEGRAPHVISUALIZER_H_ */
