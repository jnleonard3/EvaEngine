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

#include "evaRouteGraph.h"
#include "eva/route/evaRouteNode.h"
#include "eva/route/evaTemporaryRouteNode.h"
#include "eva/route/evaRouteGraphEdge.h"

namespace eva
{
	struct NearestNeighborQuery
	{
		bool queryNode;
		RouteNode const *mClosestNode;
		RouteGraphEdge const *mClosestEdge;

		Point3Dd mQueryPoint, mIntersectionPoint;
		e_double64 mMaxNodeDistance;

		NearestNeighborQuery():queryNode(true),mQueryPoint(),mMaxNodeDistance(1000000.0){};
		bool updateNodeDistance(e_double64 distance){ if(distance < mMaxNodeDistance) { mMaxNodeDistance = distance; return true; } return false; };
	};

	template <typename Node, typename Leaf>
	struct NearestNeighborAcceptor
	{
		const NearestNeighborQuery &mQuery;
		NearestNeighborAcceptor(const NearestNeighborQuery &query):mQuery(query){};

		typename Node::BoundingBox updateBounds() const
		{
			typename Node::BoundingBox bounds;
			e_uint32 dist = (e_uint32)mQuery.mMaxNodeDistance + 1;
			bounds.edges[0].first = (e_uint32)mQuery.mQueryPoint.x() - dist;
			bounds.edges[0].second = (e_uint32)mQuery.mQueryPoint.x() + dist;
			bounds.edges[1].first = (e_uint32)mQuery.mQueryPoint.y() - dist;
			bounds.edges[1].second = (e_uint32)mQuery.mQueryPoint.y() + dist;
			bounds.edges[2].first = (e_uint32)mQuery.mQueryPoint.z() - dist;
			bounds.edges[2].second = (e_uint32)mQuery.mQueryPoint.z() + dist;
			return bounds;
		}

		bool operator()(const Node * const node) const
		{
			return updateBounds().overlaps(node->bound);
		}

		bool operator()(const Leaf * const leaf) const
		{
			return updateBounds().encloses(leaf->bound);
		}

		private: NearestNeighborAcceptor(){}
	};

	struct NearestNeighborVisitor : public NearestNeighborQuery
	{
		bool ContinueVisiting;
		NearestNeighborVisitor(Point3Dd pt):ContinueVisiting(true){ mQueryPoint = pt; };

		bool operator()(RouteGraph::RouteGraphRTree::Leaf const * const leaf)
		{
			if(queryNode && leaf->leaf.isNode)
			{
				const eva::RouteNode &node = leaf->leaf.getNode();
				if(updateNodeDistance(mQueryPoint.distance(node.getPoint())))
					mClosestNode = &node;
			}
			else if(!queryNode && !leaf->leaf.isNode)
			{
				const eva::RouteGraphEdge &edge = leaf->leaf.getEdge();
				Point3Dd inter = edge.toLine().projectOnto(mQueryPoint);
				if(updateNodeDistance(mQueryPoint.distance(inter)))
				{
					//std::cout << edge.getNodeTo()->getPoint().x() << "," << edge.getNodeTo()->getPoint().y() <<"~~\n";
					//std::cout << inter.x() << "," << inter.y() << ":" << mMaxNodeDistance <<"~\n";
					mClosestEdge = &edge;
					mIntersectionPoint = inter;
				}
			}
			return true;
		}
	};

	template <typename Leaf>
	struct GatherNodesVisitor
	{
		std::vector<Leaf*> mNodes;
		bool ContinueVisiting;
		GatherNodesVisitor() : ContinueVisiting(true) {};
		~GatherNodesVisitor() {};

		std::vector<Leaf*> getResults() { return mNodes; };

		bool operator()(Leaf* leaf)
		{
			mNodes.push_back(leaf);
			return true;
		}
	};

	const RouteGraph::RouteGraphRTree::BoundingBox cuboidToBoundingBox(const Cuboidd &cube)
	{
		const Point3Dd* const points = cube.getVerticies();
		RouteGraph::RouteGraphRTree::BoundingBox box;
		box.edges[0].first = points[0].x();
		box.edges[0].second = points[6].x();
		box.edges[1].first = points[0].y();
		box.edges[1].second = points[6].y();
		box.edges[2].first = points[0].z();
		box.edges[2].second = points[6].z();
		return box;
	}

	const RouteGraph::RouteGraphRTree::BoundingBox getRouteElemBounds(const RouteElement& elem)
	{
		if(elem.isValid())
		{
			Cuboidd cubeoid;
			if(elem.isNode)
				cubeoid = elem.getNode().getBoundingBox();
			else
				cubeoid = elem.getEdge().getBoundingBox();
			return cuboidToBoundingBox(cubeoid);
		}
		return RouteGraph::RouteGraphRTree::BoundingBox();
	}

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
		RouteElement leaf;
		leaf.setNode(node);
		mRTree.Insert(leaf,getRouteElemBounds(leaf));

		return node;
	}

	RouteGraphEdge& RouteGraph::connectNodes(RouteNode &nodeFrom, RouteNode &nodeTo)
	{
		RouteGraphEdge **edgeMemoryStart = allocateSpace(mMaxEdgeEdges);
		RouteGraphEdge &edge = mGraphEdges.pushBack(RouteGraphEdge(ROUTEEDGE_DIRECT, &nodeTo, &nodeFrom, mMaxEdgeEdges, edgeMemoryStart));

		RouteElement leaf;
		leaf.setEdge(edge);
		mRTree.Insert(leaf,getRouteElemBounds(leaf));

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

		Cuboidd box(x1,x2,y1,y2,z1,z2);
		RouteGraphRTree::AcceptOverlapping acceptor(cuboidToBoundingBox(box));
		GatherNodesVisitor<RouteGraphRTree::Leaf> visitor;

		visitor = mRTree.Query<RouteGraphRTree::AcceptOverlapping,GatherNodesVisitor<RouteGraphRTree::Leaf> >(acceptor,visitor);

		for(std::vector<RouteGraphRTree::Leaf*>::iterator i = visitor.getResults().begin(); i != visitor.getResults().end(); ++i)
			if((*i)->leaf.isNode)
				results.push_back((*i)->leaf.data.mNode);

		return results;
	}

	std::vector<RouteNode*> RouteGraph::queryNodesInArea(e_uint32 x, e_uint32 y, e_uint32 z, e_uint32 radius)
	{
		std::vector<RouteNode*> results;

		Cuboidd box(Point3Dd(x,y,z),radius);
		RouteGraphRTree::AcceptEnclosing acceptor(cuboidToBoundingBox(box));
		GatherNodesVisitor<RouteGraphRTree::Leaf> visitor;

		visitor = mRTree.Query<RouteGraphRTree::AcceptEnclosing,GatherNodesVisitor<RouteGraphRTree::Leaf> >(acceptor,visitor);

		for(std::vector<RouteGraphRTree::Leaf*>::iterator i = visitor.mNodes.begin(); i != visitor.mNodes.end(); ++i)
			if((*i)->leaf.isNode)
				results.push_back((*i)->leaf.data.mNode);

		return results;
	}

	RouteNode const * RouteGraph::findClosest(const Point3Dd& pt) const
	{
		NearestNeighborVisitor visitor(pt);
		NearestNeighborAcceptor<RouteGraphRTree::Node, RouteGraphRTree::Leaf> acceptor(visitor);

		visitor = mRTree.Query<NearestNeighborAcceptor<RouteGraphRTree::Node,RouteGraphRTree::Leaf>,NearestNeighborVisitor>(acceptor,visitor);

		return visitor.mClosestNode;
	}

	RouteGraphEdge const * RouteGraph::findClosestEdge(const Point3Dd& pt, Point3Dd* intersect) const
	{
		NearestNeighborVisitor visitor(pt);
		visitor.queryNode = false;
		NearestNeighborAcceptor<RouteGraphRTree::Node, RouteGraphRTree::Leaf> acceptor(visitor);

		visitor = mRTree.Query<NearestNeighborAcceptor<RouteGraphRTree::Node,RouteGraphRTree::Leaf>,NearestNeighborVisitor>(acceptor,visitor);

		if(intersect)
			(*intersect) = visitor.mIntersectionPoint;

		return visitor.mClosestEdge;
	}

	struct RouteNodeRecord
	{
		RouteNodeRecord():mNode(0),mTravelEdge(0),mCostSoFar(0),mEstimatedTotalCost(0),mPreviousRecord(0){};
		RouteNodeRecord(RouteNode const *node,RouteGraphEdge const *traveledge)
		:mNode(node),mTravelEdge(traveledge),mCostSoFar(0.0),mEstimatedTotalCost(0.0),mPreviousRecord(0){};
		RouteNodeRecord(RouteNode const *node,RouteGraphEdge const *traveledge,e_double64 costsofar,e_double64 estimatedtotalcost,RouteNodeRecord *previous)
		:mNode(node),mTravelEdge(traveledge),mCostSoFar(costsofar),mEstimatedTotalCost(estimatedtotalcost),mPreviousRecord(previous){};
		RouteNode const *mNode;
		RouteGraphEdge const *mTravelEdge;
		e_double64 mCostSoFar;
		e_double64 mEstimatedTotalCost;
		RouteNodeRecord *mPreviousRecord;
	};

	PathNode* RouteGraph::findPath(const Point3Dd &ptFrom, const Point3Dd &ptTo) const
	{
		Point3Dd fromNearest, toNearest;
		RouteGraphEdge const * edgeFrom = this->findClosestEdge(ptFrom,&fromNearest), *edgeTo = this->findClosestEdge(ptTo,&toNearest);
		std::cout << "From: " << ptFrom << ", To: " << ptTo << "\n";
		std::cout << "EdgeFrom:" << edgeFrom->toLine() << ", EdgeTo: " << edgeTo->toLine() << "\n";
		const RouteNode &nodeFrom = *(edgeFrom->getNodeTo()), &nodeTo = *(edgeTo->getNodeFrom());
		std::vector<RouteNodeRecord*> closedSet, openSet;
		openSet.push_back(new RouteNodeRecord(&nodeFrom,0,0,nodeFrom.getPoint().distance(nodeTo.getPoint()),0));
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

			const RouteNode &currentNode = *currentRecord->mNode;

			if(&currentNode == &nodeTo)
				break;

			for(e_uchar8 i = 0; i < currentNode.getNumEdgesFrom(); ++i)
			{
				if(currentNode.getFromEdge(i) && currentNode.getFromEdge(i)->getType() == ROUTEEDGE_DIRECT)
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
						e_double64 endNodeHeuristic = endNodeRecord->mNode->getPoint().distance(nodeTo.getPoint());
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

		PathNode *pathResult = 0;
		if(currentRecord->mNode == &nodeTo)
		{
			pathResult = new PathNode(toNearest);

			while(currentRecord != 0 && currentRecord->mNode != &nodeFrom)
			{
				PathNode *prevNode = new PathNode(currentRecord->mNode->getPoint());
				prevNode->mNext = new PathEdge();
				prevNode->mNext->mNext = pathResult;
				pathResult = prevNode;
				currentRecord = currentRecord->mPreviousRecord;
			}

			if(currentRecord)
			{
				PathNode *fromNode = new PathNode(currentRecord->mNode->getPoint());
				fromNode->mNext = new PathEdge();
				fromNode->mNext->mNext = pathResult;
				pathResult = fromNode;
			}

			PathNode *firstNode = new PathNode(fromNearest);
			firstNode->mNext = new PathEdge();
			firstNode->mNext->mNext = pathResult;
			pathResult = firstNode;
		}

		for(e_uint32 i = 0; i < openSet.size(); ++i)
			delete openSet[i];

		for(e_uint32 i = 0; i < closedSet.size(); ++i)
			delete closedSet[i];

		return pathResult;
	}

	PathNode* RouteGraph::findPath(const Point3Dd &ptFrom, const Point3Dd &ptTo, PathParameters &parameters) const
	{
		return false;
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
