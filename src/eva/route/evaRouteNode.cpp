#include "evaRouteNode.h"

namespace eva
{
	RouteNode* RouteNode::getFromNode(e_uchar8 index) const
	{
		if(index < mNumNodesFrom)
			return mNodesFrom[index];
		else
			return 0;
	}

	RouteNode* RouteNode::getToNode(e_uchar8 index) const
	{
		if(index < mNumNodesTo)
			return mNodesTo[index];
		else
			return 0;
	}

	void RouteNode::setFromNode(e_uchar8 index, RouteNode *fromNode)
	{
		if(index < mNumNodesFrom)
			mNodesFrom[index] = fromNode;
	}

	void RouteNode::setToNode(e_uchar8 index, RouteNode *toNode)
	{
		if(index < mNumNodesTo)
			mNodesTo[index] = toNode;
	}

	void RouteNode::setNodesFrom(e_uchar8 count, RouteNode **from)
	{
		mNumNodesFrom = count;
		mNodesFrom = from;
	}

	void RouteNode::setNodesTo(e_uchar8 count, RouteNode **to)
	{
		mNumNodesFrom = count;
		mNodesTo = to;
	}
}
