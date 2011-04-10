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

		static osg::Group* drawFilledSquare(eva::Square2Dd rect, e_double64 z, eva::Point3Dd color);

		static osg::Group* drawBase(eva::Point3Dd from, eva::Point3Dd to, e_double64 z, eva::Point3Dd color);

		static osg::Group* drawQuadtree(std::vector<QuadAppearance> &quadtree);

	private:
		OsgHelper(){};
};

#endif /* OSGHELPER_H_ */
