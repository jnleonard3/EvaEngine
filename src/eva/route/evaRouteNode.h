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

#ifndef EVA_ROUTENODE_H_
#define EVA_ROUTENODE_H_

#include "eva/Typedefs.h"
#include "eva/geometry/basic/3d/evaPoint3D.h"
#include "eva/geometry/basic/3d/evaCuboid.h"

namespace eva
{
	class RouteGraphEdge;

	class RouteNode
	{
		public:
			RouteNode():mNumEdgesFrom(0),mNumEdgesTo(0){};
			RouteNode(Point3Dd pt):mPoint(pt),mNumEdgesFrom(0),mNumEdgesTo(0){};
			RouteNode(Point3Dd pt, e_uchar8 numfromto, RouteGraphEdge **from, RouteGraphEdge **to):mPoint(pt),mNumEdgesFrom(numfromto),mNumEdgesTo(numfromto),mEdgesFrom(from),mEdgesTo(to){};
			RouteNode(Point3Dd pt, e_uchar8 numfrom, RouteGraphEdge **from, e_uchar8 numto, RouteGraphEdge **to):mPoint(pt),mNumEdgesFrom(numfrom),mNumEdgesTo(numto),mEdgesFrom(from),mEdgesTo(to){};
			virtual ~RouteNode(){};

			Point3Dd getPoint() const {return mPoint;};

			e_uchar8 getNumEdgesFrom() const { return mNumEdgesFrom; };
			e_uchar8 getNumEdgesTo() const { return mNumEdgesTo; };

			RouteGraphEdge* getFromEdge(e_uchar8 index) const;
			RouteGraphEdge* getToEdge(e_uchar8 index) const;

			bool setFromEdge(e_uchar8 index, RouteGraphEdge *fromEdge);
			bool setToEdge(e_uchar8 index, RouteGraphEdge *toEdge);

			void setEdgesFrom(e_uchar8 count, RouteGraphEdge **from);
			void setEdgesTo(e_uchar8 count, RouteGraphEdge **to);

			const Point3Dd getClosestEdgePointTo(const Point3Dd &point, bool *isHeadingTo) const;

			const Cuboidd getBoundingBox() const;

			virtual bool isTemporary() const { return false; };
			bool isUnconnected() const;
			void clear();

		private:
			Point3Dd mPoint;
			e_uchar8 mNumEdgesFrom, mNumEdgesTo;
			RouteGraphEdge **mEdgesFrom, **mEdgesTo;
	};
}

#endif
