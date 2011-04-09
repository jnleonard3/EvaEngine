#include "OsgHelper.h"

#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/StateSet>

osg::Group* OsgHelper::drawSquare(eva::Square2Dd rect, e_double64 z)
{
	osg::Group* root = new osg::Group();

	osg::Geode* baseGeode = new osg::Geode();
	osg::Geometry* baseGeometry = new osg::Geometry();
	baseGeode->addDrawable(baseGeometry);

	osg::Vec3Array* baseVertices = new osg::Vec3Array;
	baseVertices->push_back(osg::Vec3(rect.from().x(),rect.to().y(),z)); // front left
	baseVertices->push_back(osg::Vec3(rect.to().x(),rect.to().y(),z)); // front right
	baseVertices->push_back(osg::Vec3(rect.to().x(),rect.from().y(),z)); // back right
	baseVertices->push_back(osg::Vec3(rect.from().x(),rect.from().y(),z)); // back left
	baseGeometry->setVertexArray(baseVertices);

	osg::DrawElementsUInt* line =  new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0);
	line->push_back(0);
	line->push_back(1);
	baseGeometry->addPrimitiveSet(line);
	line =  new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0);
	line->push_back(0);
	line->push_back(3);
	baseGeometry->addPrimitiveSet(line);
	line =  new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0);
	line->push_back(1);
	line->push_back(2);
	baseGeometry->addPrimitiveSet(line);
	line =  new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0);
	line->push_back(3);
	line->push_back(2);
	baseGeometry->addPrimitiveSet(line);

	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0f));
	baseGeometry->setColorArray(colors);
	baseGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	root->addChild(baseGeode);
	return root;
}

osg::Group* OsgHelper::drawBase(eva::Point3Dd from, eva::Point3Dd to, e_double64 z, eva::Point3Dd color)
{
	osg::Group* root = new osg::Group();

	osg::Geode* baseGeode = new osg::Geode();
	osg::Geometry* baseGeometry = new osg::Geometry();
	baseGeode->addDrawable(baseGeometry);

	osg::DrawElementsUInt* baseElements =  new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	baseElements->push_back(3);
	baseElements->push_back(2);
	baseElements->push_back(1);
	baseElements->push_back(0);

	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(color.x(), color.y(), color.z(), 1.0f));

	osg::Vec3Array* baseVertices = new osg::Vec3Array;
	baseVertices->push_back(osg::Vec3(from.x(),to.y(),z)); // front left
	baseVertices->push_back(osg::Vec3(to.x(),to.y(),z)); // front right
	baseVertices->push_back(osg::Vec3(to.x(),from.y(),z)); // back right
	baseVertices->push_back(osg::Vec3(from.x(),from.y(),z)); // back left

	baseGeometry->setVertexArray(baseVertices);
	baseGeometry->addPrimitiveSet(baseElements);
	baseGeometry->setColorArray(colors);
	baseGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	root->addChild(baseGeode);
	return root;
}

osg::Group* OsgHelper::drawQuadtree(std::vector<QuadAppearance> &quadtree)
{
	osg::Group* root = new osg::Group();

	for(e_uint32 i = 0; i < quadtree.size(); ++i)
		root->addChild(drawSquare(quadtree[i].mQuad,5.0+quadtree[i].mLevel*0.25));

	return root;
}
