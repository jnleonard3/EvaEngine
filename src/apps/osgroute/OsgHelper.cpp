/*
 *  Copyright (c) 2011, EvaEngine Project
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions are met:
 *		* Redistributions of source code must retain the above copyright
 *		  notice, this list of conditions and the following disclaimer.
 *		* Redistributions in binary form must reproduce the above copyright
 *		  notice, this list of conditions and the following disclaimer in the
 *		  documentation and/or other materials provided with the distribution.
 *
 *	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *	DISCLAIMED. IN NO EVENT SHALL THE EVAENGINE PROEJCT BE LIABLE FOR ANY
 *	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "OsgHelper.h"

osg::Group* OsgHelper::drawSquare(const eva::Squared rect, e_double64 z)
{
	osg::Group* root = new osg::Group();

	osg::Geode* baseGeode = new osg::Geode();
	osg::Geometry* baseGeometry = new osg::Geometry();
	baseGeode->addDrawable(baseGeometry);

	osg::Vec3Array* baseVertices = new osg::Vec3Array;
	for(e_uchar8 i = 0; i < 4; ++i)
		baseVertices->push_back(OsgHelper::eva2DPointToOsgVec(rect.getVerticies()[i],z));
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

osg::Group* OsgHelper::drawFilledSquare(const eva::Squared rect, e_double64 z, eva::Point3Dd color)
{
	osg::Group* root = new osg::Group();

	osg::Geode* baseGeode = new osg::Geode();
	osg::Geometry* baseGeometry = new osg::Geometry();
	baseGeode->addDrawable(baseGeometry);

	osg::Vec3Array* baseVertices = new osg::Vec3Array;
	for(e_uchar8 i = 0; i < 4; ++i)
		baseVertices->push_back(OsgHelper::eva2DPointToOsgVec(rect.getVerticies()[i],z));
	baseGeometry->setVertexArray(baseVertices);

	osg::DrawElementsUInt* quad =  new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	quad->push_back(0);
	quad->push_back(1);
	quad->push_back(2);
	quad->push_back(3);
	baseGeometry->addPrimitiveSet(quad);

	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(color.x(), color.y(), color.z(), 0.5f));
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
		if(quadtree[i].mHasData)
			root->addChild(drawFilledSquare(quadtree[i].mQuad,5.0+quadtree[i].mLevel*0.25,eva::Point3Dd(1.0,0.0,0.0)));
		else
			root->addChild(drawSquare(quadtree[i].mQuad,5.0+quadtree[i].mLevel*0.25));

	return root;
}

osg::Vec3 OsgHelper::eva2DPointToOsgVec(const eva::Point2Dd& pt, e_double64 z)
{
	return osg::Vec3(pt.x(),pt.y(),z);
}
