#include "evaRouteGraphEdge.h"
#include "evaRouteNode.h"

namespace eva
{
	RouteGraphEdge* RouteGraphEdge::getConnectedEdge(e_uchar8 index) const
	{
		if(index < mNumEdgesConnected)
			return mConnectedEdges[index];
		return 0;
	}

	bool RouteGraphEdge::setConnectedEdge(e_uchar8 index, RouteGraphEdge *edge)
	{
		if(index < mNumEdgesConnected)
		{
			mConnectedEdges[index] = edge;
			return true;
		}
		return false;
	}

	void RouteGraphEdge::setConnectedEdges(e_uchar8 count, RouteGraphEdge **edges)
	{
		mNumEdgesConnected = count;
		mConnectedEdges = edges;
	}

	e_double64 RouteGraphEdge::distance() const
	{
		if(mTo == 0 || mFrom == 0)
			return 0.0;
		return mTo->getPoint().distanceFrom(mFrom->getPoint());
	}

	void RouteGraphEdge::clear()
	{
		delete mConnectedEdges;
		mNumEdgesConnected = 0;
	}

}
