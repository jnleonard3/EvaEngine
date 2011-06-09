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

#ifndef EVA_ROUTEGRAPH_H_
#define EVA_ROUTEGRAPH_H_

#include "eva/Typedefs.h"
#include "eva/geometry/evaLine.h"
#include "eva/math/evaPoint.h"
#include "eva/structures/evaSimpleDynamicArray.h"
#include "eva/route/evaRouteGraphEdge.h"
#include "eva/route/evaRouteNode.h"
#include "eva/structures/evaBlockArray.h"
#include "eva/structures/RStarTree.h"
#include "eva/route/evaPath.h"

#include <list>

namespace eva
{
	struct RouteElement
	{
		union RouteElements
		{
			RouteNode *mNode;
			RouteGraphEdge *mEdge;
		};

		RouteElement():isNode(true){};

		bool isValid() const { if(data.mNode) return true; return false; }

		RouteNode& getNode() { return *(data.mNode); };
		const RouteNode& getNode() const { return *(data.mNode); };
		RouteGraphEdge& getEdge() { return *(data.mEdge); }
		const RouteGraphEdge& getEdge() const { return *(data.mEdge); }

		void setNode(RouteNode& node) { data.mNode = &node; };
		void setEdge(RouteGraphEdge& edge) { isNode = false; data.mEdge = &edge; };

		bool isNode;
		RouteElements data;
	};

	class RouteGraph : public PathFinder
	{
		public:
			RouteGraph();
			RouteGraph(e_uint32 maxNodeEdges, e_uint32 maxEdgeEdges);

			RouteNode& createNode(Point3Dd pt);

			RouteGraphEdge& connectNodes(RouteNode &nodeFrom, RouteNode &nodeTo);
			RouteGraphEdge& connectNodes(e_uchar8 edgeType, RouteNode &nodeFrom, RouteNode &nodeTo);

			void disconnectNodes(RouteNode &nodeFrom, RouteNode &nodeTo);

			std::vector<RouteNode*> queryNodesInArea(e_uint32 x1, e_uint32 x2, e_uint32 y1, e_uint32 y2, e_uint32 z1, e_uint32 z2);
			std::vector<RouteNode*> queryNodesInArea(e_uint32 x, e_uint32 y, e_uint32 z, e_uint32 radius);

			RouteNode const * findClosest(const Point3Dd& pt) const;
			RouteGraphEdge const * findClosestEdge(const Point3Dd& pt, Point3Dd* intersect) const;

			PathNode* findPath(const Point3Dd &ptFrom, const Point3Dd &ptTo) const;
			PathNode* findPath(const Point3Dd &ptFrom, const Point3Dd &ptTo, PathParameters &parameters) const;

			const BlockArray<RouteNode>& getNodes() const { return mNodes; };
			const BlockArray<RouteGraphEdge>& getEdges() const { return mGraphEdges; };

			typedef RStarTree<RouteElement,3,5,50> RouteGraphRTree;

		private:
			// General Functions
			bool isNodeInGraph(const RouteNode &node) const;

			// Storage for elements of the graph
			const e_uint32 mMaxNodeEdgesFrom;
			const e_uint32 mMaxNodeEdgesTo;

			BlockArray<RouteNode> mNodes;
			BlockArray<RouteGraphEdge> mGraphEdges;

			// Storage for the array of pointers to edges and related structures and functions
			struct Range
			{
				Range(e_uint32 low, e_uint32 high):mLow(low),mHigh(high){};
				e_uint32 mLow, mHigh;
			};

			RouteGraphEdge** allocateSpace(e_uint32 size);
			void deallocateSpace(RouteGraphEdge** address, e_uint32 size);
			void combineSpace();

			const e_uint32 mMaxNodeEdges;
			const e_uint32 mMaxEdgeEdges;

			SimpleDynamicArray<RouteGraphEdge*> mEdgesPtrArray;
			std::list<Range> mAvailableElements;

			RouteGraphRTree mRTree;
	};
}

#endif
