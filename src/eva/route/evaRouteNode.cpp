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

#include "evaRouteNode.h"
#include "evaRouteGraphEdge.h"

namespace eva
{
	RouteGraphEdge* RouteNode::getFromEdge(e_uchar8 index) const
	{
		if(index < mNumEdgesFrom)
			return mEdgesFrom[index];
		return 0;
	}

	RouteGraphEdge* RouteNode::getToEdge(e_uchar8 index) const
	{
		if(index < mNumEdgesTo)
			return mEdgesTo[index];
		return 0;
	}

	bool RouteNode::setFromEdge(e_uchar8 index, RouteGraphEdge *fromEdge)
	{
		if(index < mNumEdgesFrom)
		{
			mEdgesFrom[index] = fromEdge;
			return true;
		}
		return false;
	}

	bool RouteNode::setToEdge(e_uchar8 index, RouteGraphEdge *toEdge)
	{
		if(index < mNumEdgesTo)
		{
			mEdgesTo[index] = toEdge;
			return true;
		}
		return false;
	}

	void RouteNode::setEdgesFrom(e_uchar8 count, RouteGraphEdge **from)
	{
		mNumEdgesFrom = count;
		mEdgesFrom = from;
	}

	void RouteNode::setEdgesTo(e_uchar8 count, RouteGraphEdge **to)
	{
		mNumEdgesTo = count;
		mEdgesTo = to;
	}

	const Point3Dd RouteNode::getClosestEdgePointTo(const Point3Dd &point, bool *isHeadingTo) const
	{
		Point3Dd pt = this->getPoint();
		e_double64 dist = 1000000.0;
		for(e_uchar8 i = 0; i < this->getNumEdgesFrom(); ++i)
		{
			const RouteGraphEdge* edge = this->getFromEdge(i);
			if(edge != 0 && edge->getType() != ROUTEEDGE_INVALID)
			{
				Point3Dd inter = edge->toLine().projectOnto(point);
				e_double64 nowdist = inter.distance(point);
				if(nowdist < dist)
				{
					dist = nowdist;
					pt = inter;
					Point3Dd a = edge->getNodeTo()->getPoint(), b = edge->getNodeFrom()->getPoint();
					if(isHeadingTo)
						(*isHeadingTo) = false;
				}

			}
		}
		for(e_uchar8 i = 0; i < this->getNumEdgesTo(); ++i)
		{
			const RouteGraphEdge* edge = this->getToEdge(i);
			if(edge != 0 && edge->getType() != ROUTEEDGE_INVALID)
			{
				Point3Dd inter = edge->toLine().projectOnto(point);
				e_double64 nowdist = inter.distance(point);
				if(nowdist < dist)
				{
					dist = nowdist;
					pt = inter;
					Point3Dd a = edge->getNodeTo()->getPoint(), b = edge->getNodeFrom()->getPoint();
					if(isHeadingTo)
						(*isHeadingTo) = true;
				}

			}
		}
		return pt;
	}

	const Cuboidd RouteNode::getBoundingBox() const
	{
		Point3Dd a = this->getPoint();
		return Cuboidd(a.x(),a.x()+1,a.y(),a.y()+1,a.z(),a.z()+1);
	}

	bool RouteNode::isUnconnected() const
	{
		for(e_uint32 i = 0; i < mNumEdgesFrom; ++i)
			if(mEdgesFrom[i] != 0 && mEdgesFrom[i]->getType() != ROUTEEDGE_INVALID)
				return false;
		for(e_uint32 i = 0; i < mNumEdgesTo; ++i)
			if(mEdgesTo[i] != 0 && mEdgesTo[i]->getType() != ROUTEEDGE_INVALID)
				return false;
		return true;
	}

	void RouteNode::clear()
	{
		//TODO: How about invalidate all the active edges instead?
		delete mEdgesFrom;
		delete mEdgesTo;
		mNumEdgesFrom = 0;
		mNumEdgesTo = 0;
	}
}
