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

	void RouteNode::clear()
	{
		delete mEdgesFrom;
		delete mEdgesTo;
		mNumEdgesFrom = 0;
		mNumEdgesTo = 0;
	}
}
