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
		return mTo->getPoint().distance(mFrom->getPoint());
	}

	Vector3Dd RouteGraphEdge::toVector() const
	{
		if(mTo && mFrom && (this->getType() != ROUTEEDGE_INVALID))
		{
			Point3Dd to = mTo->getPoint(), from = mFrom->getPoint();
			return Vector3Dd(to.x()-from.x(),to.y()-from.y(),to.z()-from.z());
		}
		return Vector3Dd(0.0,0.0,0.0);
	}

	const Line3Dd RouteGraphEdge::toLine() const
	{
		if(mTo && mFrom && (this->getType() != ROUTEEDGE_INVALID))
			return Line3Dd(mFrom->getPoint(),mTo->getPoint());
		return Line3Dd(0.0,0.0,0.0,0.0,0.0,0.0);
	}

	const Cuboidd RouteGraphEdge::getBoundingBox() const
	{
		Line3Dd line = this->toLine();
		Point3Dd a = line.getFrom(), b = line.getTo();
		return Cuboidd(std::min(a.x(),b.x()),std::max(a.x(),b.x())+1,std::min(a.y(),b.y()),std::max(a.y(),b.y())+1,std::min(a.z(),b.z()),std::max(a.z(),b.z())+1);
	}

	void RouteGraphEdge::clear()
	{
		delete mConnectedEdges;
		mNumEdgesConnected = 0;
	}

}
