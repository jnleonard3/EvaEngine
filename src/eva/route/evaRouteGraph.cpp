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
		RouteNode &node = mNodes.pushBack(RouteNode(pt, mMaxNodeEdgesFrom, fromEdgePtr, mMaxNodeEdgesTo, toEdgePtr));

		// Insert into R-Tree
		RTreeLeaf leaf(true,(e_uint32)pt.x(),(e_uint32)pt.y(),(e_uint32)pt.z());
		leaf.data.mNode = &node;
		mRTree.Insert(leaf,leaf.boundingBox);

		return node;
	}

	RouteGraphEdge& RouteGraph::connectNodes(RouteNode &nodeFrom, RouteNode &nodeTo)
	{
		RouteGraphEdge **edgeMemoryStart = allocateSpace(mMaxEdgeEdges);
		RouteGraphEdge &edge = mGraphEdges.pushBack(RouteGraphEdge(ROUTEEDGE_DIRECT, &nodeTo, &nodeFrom, mMaxEdgeEdges, edgeMemoryStart));

		for(e_uchar8 i = 0; i < nodeFrom.getNumEdgesFrom(); ++i)
			if(nodeFrom.getFromEdge(i) == 0 || nodeFrom.getFromEdge(i)->getType() == ROUTEEDGE_INVALID)
			{
				nodeFrom.setFromEdge(i, &edge);
				break;
			}

		for(e_uchar8 i = 0; i < nodeTo.getNumEdgesTo(); ++i)
			if(nodeTo.getToEdge(i) == 0 || nodeTo.getToEdge(i)->getType() == ROUTEEDGE_INVALID)
			{
				nodeTo.setToEdge(i, &edge);
				break;
			}

		return edge;
	}

	void RouteGraph::disconnectNodes(RouteNode &nodeFrom, RouteNode &nodeTo)
	{
		for(e_uchar8 i = 0; i < nodeFrom.getNumEdgesFrom(); ++i)
			if(nodeFrom.getFromEdge(i) != 0 && nodeFrom.getFromEdge(i)->getType() != ROUTEEDGE_INVALID)
			{
				for(e_uchar8 j = 0; j < nodeTo.getNumEdgesTo(); ++j)
				{
					if(nodeFrom.getFromEdge(i) == nodeTo.getToEdge(j))
					{
						nodeFrom.getFromEdge(i)->invalidate();
						return;
					}
				}
			}
	}

	std::vector<RouteNode*> RouteGraph::queryNodesInArea(e_uint32 x1, e_uint32 x2, e_uint32 y1, e_uint32 y2, e_uint32 z1, e_uint32 z2)
	{
		std::vector<RouteNode*> results;

		RouteGraphRTree::AcceptOverlapping acceptor(createBoundingBox(x1,x2,y1,y2,z1,z2));
		RouteGraphGatherNodesVisitor visitor;

		visitor = mRTree.Query<RouteGraphRTree::AcceptOverlapping,RouteGraphGatherNodesVisitor>(acceptor,visitor);

		for(std::vector<RouteGraphRTree::Leaf*>::iterator i = visitor.getResults().begin(); i != visitor.getResults().end(); ++i)
			if((*i)->leaf.isNode)
				results.push_back((*i)->leaf.data.mNode);

		return results;
	}

	std::vector<RouteNode*> RouteGraph::queryNodesInArea(e_uint32 x, e_uint32 y, e_uint32 z, e_uint32 radius)
	{
		std::vector<RouteNode*> results;

		RouteGraphRTree::AcceptEnclosing acceptor(createBoundingBox(x-radius,x+radius,y-radius,y+radius,z-radius,z+radius));
		RouteGraphGatherNodesVisitor visitor;

		visitor = mRTree.Query<RouteGraphRTree::AcceptEnclosing,RouteGraphGatherNodesVisitor>(acceptor,visitor);

		for(std::vector<RouteGraphRTree::Leaf*>::iterator i = visitor.mNodes.begin(); i != visitor.mNodes.end(); ++i)
			if((*i)->leaf.isNode)
				results.push_back((*i)->leaf.data.mNode);

		return results;
	}

	RouteNode* RouteGraph::findClosest(e_double64 x, e_double64 y, e_double64 z)
	{
		RouteGraphNearestNeighborVisitor visitor(Point3Dd(x,y,z));
		RouteGraphNearestNeighborAcceptor acceptor(visitor);

		visitor = mRTree.Query<RouteGraphNearestNeighborAcceptor,RouteGraphNearestNeighborVisitor>(acceptor,visitor);

		return visitor.mClosest;
	}

	bool RouteGraph::findPath(RouteNode &nodeFrom, RouteNode &nodeTo, std::list<RoutePathElement> &pathResult) const
	{
		bool pathFound = false;
		std::vector<RouteNodeRecord*> closedSet, openSet;
		openSet.push_back(new RouteNodeRecord(&nodeFrom,0,0,nodeFrom.getPoint().distanceFrom(nodeTo.getPoint()),0));
		RouteNodeRecord *currentRecord = openSet.front();
		while(openSet.size() > 0)
		{
			currentRecord = openSet.front();
			e_uint32 currentRecordIndex = 0;
			for(e_uint32 i = 0; i < openSet.size(); ++i)
				if(currentRecord->mEstimatedTotalCost > openSet[i]->mEstimatedTotalCost)
				{
					currentRecord = openSet[i];
					currentRecordIndex = i;
				}

			RouteNode &currentNode = *currentRecord->mNode;

			if(&currentNode == &nodeTo)
				break;

			for(e_uchar8 i = 0; i < currentNode.getNumEdgesFrom(); ++i)
			{
				if(currentNode.getFromEdge(i) != 0 && currentNode.getFromEdge(i)->getType() == ROUTEEDGE_DIRECT)
				{
					bool isInOpenSet = false, isInClosedSet = false, skip = false;

					RouteNode &currentEndNode = *currentNode.getFromEdge(i)->getNodeTo();
					RouteNodeRecord *endNodeRecord = 0;
					e_double64 endNodeCost = currentRecord->mCostSoFar + currentNode.getFromEdge(i)->distance();

					for(e_uint32 j = 0; j < closedSet.size(); ++j)
					{
						if(closedSet.at(j)->mNode == currentNode.getFromEdge(i)->getNodeTo())
						{
							isInClosedSet = true;
							endNodeRecord = closedSet[j];

							if(endNodeRecord->mCostSoFar > endNodeCost)
							{
								closedSet[j] = closedSet.back();
								closedSet.pop_back();
							}
							else
								skip = true;

							break;
						}
					}

					if(!isInClosedSet)
					{
						for(e_uint32 j = 0; j < openSet.size(); ++j)
						{
							if(openSet.at(j)->mNode == currentNode.getFromEdge(i)->getNodeTo())
							{
								isInOpenSet = true;
								endNodeRecord = openSet[j];
								break;
							}
						}
					}

					if(!isInClosedSet && !isInOpenSet)
						endNodeRecord = new RouteNodeRecord(currentNode.getFromEdge(i)->getNodeTo(), currentNode.getFromEdge(i));

					if(!skip)
					{
						e_double64 endNodeHeuristic = endNodeRecord->mNode->getPoint().distanceFrom(nodeTo.getPoint());
						endNodeRecord->mCostSoFar = endNodeCost;
						endNodeRecord->mTravelEdge = currentNode.getFromEdge(i);
						endNodeRecord->mEstimatedTotalCost = endNodeCost + endNodeHeuristic;
						endNodeRecord->mPreviousRecord = currentRecord;

						if(!isInOpenSet)
							openSet.push_back(endNodeRecord);
					}
				}
			}

			openSet[currentRecordIndex] = openSet.back();
			openSet.pop_back();
			closedSet.push_back(currentRecord);
		}

		if(currentRecord->mNode == &nodeTo)
		{
			pathFound = true;
			while(currentRecord != 0 && currentRecord->mNode != &nodeFrom)
			{
				pathResult.push_front(RoutePathElement(currentRecord->mNode,currentRecord->mTravelEdge));
				currentRecord = currentRecord->mPreviousRecord;
			}
		}

		for(e_uint32 i = 0; i < openSet.size(); ++i)
			delete openSet[i];

		for(e_uint32 i = 0; i < closedSet.size(); ++i)
			delete closedSet[i];

		return pathFound;
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

	RouteGraph::RouteGraphRTree::BoundingBox RouteGraph::createBoundingBox(e_int32 x1, e_int32 x2, e_int32 y1, e_int32 y2, e_int32 z1, e_int32 z2)
	{
		RouteGraphRTree::BoundingBox boundingBox;
		boundingBox.edges[0].first = std::min(x1,x2);
		boundingBox.edges[0].second = std::max(x1,x2);
		boundingBox.edges[1].first = std::min(y1,y2);
		boundingBox.edges[1].second = std::max(y1,y2);
		boundingBox.edges[2].first = std::min(z1,z2);
		boundingBox.edges[2].second = std::max(z1,z2);

		return boundingBox;
	}
}
