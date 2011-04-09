#ifndef OSGHELPER_H_
#define OSGHELPER_H_

#include "FixedQuadtree.h"

#include "eva/Typedefs.h"
#include "eva/geometry/evaPoint3D.h"
#include "eva/geometry/evaRectangle2D.h"

namespace osg
{
	class Group;
}

class OsgHelper
{
	public:
		virtual ~OsgHelper(){};

		static osg::Group* drawSquare(eva::Square2Dd rect, e_double64 z);

		static osg::Group* drawBase(eva::Point3Dd from, eva::Point3Dd to, e_double64 z, eva::Point3Dd color);

		static osg::Group* drawQuadtree(std::vector<QuadAppearance> &quadtree);

	private:
		OsgHelper(){};
};

#endif /* OSGHELPER_H_ */
