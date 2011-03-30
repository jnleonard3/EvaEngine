#ifndef EVA_ROUTEGRAPH_H_
#define EVA_ROUTEGRAPH_H_

#include "eva/Typedefs.h"
#include "eva/geometry/evaPoint3D.h"
#include "eva/structures/evaSimpleDynamicArray.h"
#include "eva/structures/evaBlockArray.h"
#include "eva/structures/RStarTree.h"

#include <list>
#include <map>

namespace eva
{
	class RouteGraphEdge;
	class RouteNode;

	class RouteGraph
	{
		public:
			RouteGraph();
			RouteGraph(e_uint32 maxNodeEdges, e_uint32 maxEdgeEdges);

			RouteNode& createNode(Point3Dd pt);

			RouteGraphEdge& connectNodes(RouteNode &nodeFrom, RouteNode &nodeTo);
			RouteGraphEdge& connectNodes(e_uchar8 edgeType, RouteNode &nodeFrom, RouteNode &nodeTo);

			void disconnectNodes(RouteNode &nodeFrom, RouteNode &nodeTo);

			struct RoutePathElement
			{
				RouteNode &mNode;
				RouteGraphEdge &mTravelEdge;
			};

			struct RoutePathParameters
			{
				bool directionIgnored;
				bool useSemidirectEdges;
				bool takeIndirectEdges;
			};

			bool findPath(RouteNode &nodeFrom, RouteNode &nodeTo, std::list<RoutePathElement> &pathResult) const;
			bool findPath(RouteNode &nodeFrom, RouteNode &nodeTo, std::list<RoutePathElement> &pathResult, RoutePathParameters &parameters) const;

			const BlockArray<RouteNode>& getNodes() const { return mNodes; };
			const BlockArray<RouteGraphEdge>& getEdges() const { return mGraphEdges; };

		private:
			// General Functions

			bool isNodeInGraph(RouteNode &node) const;

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

			// Storage and helper functions for retrieving cached paths of indirect edges

			struct MapKey
			{
				Point3Dd mFrom, mTo;
			};

			std::vector<std::list<RoutePathElement> > mCachedPaths;
			std::map<MapKey,std::list<RoutePathElement>&> mIndirectPathMap;

			// R-Tree structure and helper functions for spatial querying (nearest neighbor, area query)

			union RTreeLeafData
			{
				RouteNode *mNode;
				RouteGraphEdge *mEdge;
			};

			struct RTreeLeaf
			{
				bool isNode;
				RTreeLeafData data;
			};

			RStarTree<RTreeLeaf,3,5,50> mRTree;
	};
}

#endif
