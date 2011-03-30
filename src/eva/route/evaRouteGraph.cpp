#include "evaRouteGraph.h"
#include "eva/route/evaRouteNode.h"
#include "eva/route/evaRouteGraphEdge.h"

namespace eva
{
	RouteGraph::RouteGraph()
	:mMaxNodeEdgesFrom(5), mMaxNodeEdgesTo(5),mMaxNodeEdges(10),mMaxEdgeEdges(10), mEdgesPtrArray(0), mAvailableElements()
	{
	}

	RouteGraph::RouteGraph(e_uint32 maxNodeEdges, e_uint32 maxEdgeEdges)
	:mMaxNodeEdgesFrom(maxNodeEdges/2), mMaxNodeEdgesTo(maxNodeEdges/2),mMaxNodeEdges(maxNodeEdges),mMaxEdgeEdges(maxEdgeEdges), mEdgesPtrArray(0), mAvailableElements()
	{
	}

	RouteNode& RouteGraph::createNode(Point3Dd pt)
	{
		// Allocate memory for edge pointers
		RouteGraphEdge **edgeMemoryStart = allocateSpace(mMaxNodeEdges);
		RouteGraphEdge **fromEdgePtr = edgeMemoryStart, **toEdgePtr = &edgeMemoryStart[mMaxNodeEdgesFrom];
		// Insert into array
		mNodes.pushBack(RouteNode(pt, mMaxNodeEdgesFrom, fromEdgePtr, mMaxNodeEdgesTo, toEdgePtr));
		// Insert into R-Tree
		return mNodes.at(mNodes.getSize()-1);
	}

	RouteGraphEdge& RouteGraph::connectNodes(RouteNode &nodeFrom, RouteNode &nodeTo)
	{
		std::cout << "CONNECTING: "<< &nodeFrom << " TO " << &nodeTo << "\n";
		RouteGraphEdge **edgeMemoryStart = allocateSpace(mMaxEdgeEdges);
		RouteGraphEdge &edge = mGraphEdges.pushBack(RouteGraphEdge(ROUTEEDGE_DIRECT, &nodeTo, &nodeFrom, mMaxEdgeEdges, edgeMemoryStart));

		e_uchar8 to = 255,from = 255;
		for(e_uchar8 i = 0; i < nodeFrom.getNumEdgesTo(); ++i)
			if(nodeFrom.getToEdge(i) == 0 || nodeFrom.getToEdge(i)->getType() == ROUTEEDGE_INVALID)
			{
				nodeFrom.setToEdge(i, &edge);
				to = i;
				break;
			}

		for(e_uchar8 i = 0; i < nodeTo.getNumEdgesFrom(); ++i)
			if(nodeTo.getFromEdge(i) == 0 || nodeTo.getFromEdge(i)->getType() == ROUTEEDGE_INVALID)
			{
				nodeTo.setFromEdge(i, &edge);
				from = i;
				break;
			}

		return edge;
	}

	void RouteGraph::disconnectNodes(RouteNode &nodeFrom, RouteNode &nodeTo)
	{
		std::cout << "DISCONNECTING: "<< &nodeFrom << " TO " << &nodeTo << "\n";
		for(e_uchar8 i = 0; i < nodeFrom.getNumEdgesTo(); ++i)
			if(nodeFrom.getToEdge(i) != 0 && nodeFrom.getToEdge(i)->getType() != ROUTEEDGE_INVALID)
			{
				for(e_uchar8 j = 0; j < nodeTo.getNumEdgesFrom(); ++j)
				{
					std::cout << nodeFrom.getToEdge(i) << ", " << nodeTo.getFromEdge(j) << "\n";
					if(nodeFrom.getToEdge(i) == nodeTo.getFromEdge(j))
					{
						std::cout << "DISCONNECT.\n";
						nodeFrom.getToEdge(i)->invalidate();
						return;
					}
				}
			}
		std::cout << "Couldn't find the edge that connects.\n";
	}

	RouteGraphEdge** RouteGraph::allocateSpace(e_uint32 size)
	{
		for(std::list<Range>::iterator i = mAvailableElements.begin(); i != mAvailableElements.end(); ++i)
		{
			e_uint32 spaceAvail = (i->mHigh+1)-i->mLow;
			if(spaceAvail > size)
			{
				RouteGraphEdge** ptr = &mEdgesPtrArray.at(i->mLow);
				i->mLow = i->mLow + size + 1;
				if(i->mLow > i->mHigh)
					mAvailableElements.erase(i);
				return ptr;
			}
		}

		// Ideally this part is rarely reached
		e_uint32 originalSize = mEdgesPtrArray.getSize();
		mEdgesPtrArray.doubleSize();
		mAvailableElements.push_back(Range(originalSize,mEdgesPtrArray.getSize()-1));
		this->combineSpace();
		return this->allocateSpace(size);
	}

	void RouteGraph::combineSpace()
	{
		for(std::list<Range>::iterator i = mAvailableElements.begin(); i != mAvailableElements.end(); ++i)
		{
			std::list<Range>::iterator next = i;
			next++;
			if(next != mAvailableElements.end())
			{
				if((i->mHigh + 1) == next->mLow)
				{
					i->mHigh = next->mHigh;
					mAvailableElements.erase(next);
					// Is this necessary? I just want to make sure we don't go on to a deleted element
					i--;
					i++;
				}
			}
		}
	}
}
