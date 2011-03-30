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
