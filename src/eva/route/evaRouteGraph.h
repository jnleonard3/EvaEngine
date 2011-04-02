#ifndef EVA_ROUTEGRAPH_H_
#define EVA_ROUTEGRAPH_H_

#include "eva/Typedefs.h"
#include "eva/geometry/evaPoint3D.h"
#include "eva/structures/evaSimpleDynamicArray.h"
#include "eva/route/evaRouteGraphEdge.h"
#include "eva/route/evaRouteNode.h"
#include "eva/structures/evaBlockArray.h"
#include "eva/structures/RStarTree.h"

#include <list>
#include <map>

namespace eva
{
	class RouteGraphEdge;
	class RouteNode;

	struct RoutePathElement
	{
		RoutePathElement(RouteNode *node, RouteGraphEdge *edge):mNode(node),mTravelEdge(edge){};
		RouteNode *mNode;
		RouteGraphEdge *mTravelEdge;
	};

	class RouteGraph
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

			RouteNode* findClosest(e_double64 x, e_double64 y, e_double64 z);

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
			std::map<MapKey,std::list<RoutePathElement>*> mIndirectPathMap;

			struct RouteNodeRecord : public RoutePathElement
			{
				RouteNodeRecord():RoutePathElement(0,0),mCostSoFar(0),mEstimatedTotalCost(0),mPreviousRecord(0){};
				RouteNodeRecord(RouteNode *node,RouteGraphEdge *traveledge)
				: RoutePathElement(node,traveledge),mCostSoFar(0.0),mEstimatedTotalCost(0.0),mPreviousRecord(0){};
				RouteNodeRecord(RouteNode *node,RouteGraphEdge *traveledge,e_double64 costsofar,e_double64 estimatedtotalcost,RouteNodeRecord *previous)
				: RoutePathElement(node,traveledge),mCostSoFar(costsofar),mEstimatedTotalCost(estimatedtotalcost),mPreviousRecord(previous){};
				e_double64 mCostSoFar;
				e_double64 mEstimatedTotalCost;
				RouteNodeRecord *mPreviousRecord;
			};

			// R-Tree structure and helper functions for spatial querying (nearest neighbor, area query)

			union RTreeLeafData
			{
				RouteNode *mNode;
				RouteGraphEdge *mEdge;
			};

			struct RTreeLeaf
			{
				RTreeLeaf():isNode(true){};
				RTreeLeaf(bool isnode, e_int32 x, e_int32 y, e_int32 z):isNode(isnode){	boundingBox.edges[0].first = x-1;
																							boundingBox.edges[0].second = x+1;
																							boundingBox.edges[1].first = y-1;
																							boundingBox.edges[1].second = y+1;
																							boundingBox.edges[2].first = z-1;
																							boundingBox.edges[2].second = z+1;
				};
				RTreeLeaf(bool isnode, e_int32 x1, e_int32 x2, e_int32 y1, e_int32 y2, e_int32 z1, e_int32 z2):isNode(isnode){	boundingBox.edges[0].first = std::min(x1,x2);
																																boundingBox.edges[0].second = std::max(x1,x2);
																																boundingBox.edges[1].first = std::min(y1,y2);
																																boundingBox.edges[1].second = std::max(y1,y2);
																																boundingBox.edges[2].first = std::min(z1,z2);
																																boundingBox.edges[2].second = std::max(z1,z2);
				};
				bool isNode;
				RStarTree<RTreeLeaf,3,5,50>::BoundingBox boundingBox;
				RTreeLeafData data;
			};

			struct NearestNeighborQuery
			{
				RouteNode *mClosest;
				Point3Dd mQueryPoint;
				e_double64 mMaxDistance;

				NearestNeighborQuery():mClosest(0),mQueryPoint(),mMaxDistance(1000000.0){};
				bool updateDistance(e_double64 distance){ if(distance < mMaxDistance) { mMaxDistance = distance; return true; } return false; };
			};

			template <typename Node, typename Leaf>
			struct NearestNeighborAcceptor
			{
				const NearestNeighborQuery &mQuery;
				NearestNeighborAcceptor(const NearestNeighborQuery &query):mQuery(query){};

				typename Node::BoundingBox updateBounds() const
				{
					typename Node::BoundingBox bounds;
					e_uint32 dist = (e_uint32)mQuery.mMaxDistance;
					bounds.edges[0].first = mQuery.mQueryPoint.x() - dist;
					bounds.edges[0].second = mQuery.mQueryPoint.x() + dist;
					bounds.edges[1].first = mQuery.mQueryPoint.y() - dist;
					bounds.edges[1].second = mQuery.mQueryPoint.y() + dist;
					bounds.edges[2].first = mQuery.mQueryPoint.z() - dist;
					bounds.edges[2].second = mQuery.mQueryPoint.z() + dist;
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

			template <typename Leaf>
			struct NearestNeighborVisitor : public NearestNeighborQuery
			{
				bool ContinueVisiting;
				NearestNeighborVisitor(Point3Dd pt) : ContinueVisiting(true) { mQueryPoint = pt; };

				bool operator()(Leaf* leaf)
				{
					RTreeLeaf &rleaf = leaf->leaf;
					if(rleaf.isNode)
					{
						RouteNode *node = rleaf.data.mNode;
						if(updateDistance(mQueryPoint.distanceFrom(node->getPoint())))
							mClosest = node;
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

			typedef RStarTree<RTreeLeaf,3,5,50> RouteGraphRTree;
			typedef GatherNodesVisitor<RouteGraphRTree::Leaf> RouteGraphGatherNodesVisitor;
			typedef NearestNeighborAcceptor<RouteGraphRTree::Node, RouteGraphRTree::Leaf> RouteGraphNearestNeighborAcceptor;
			typedef NearestNeighborVisitor<RouteGraphRTree::Leaf> RouteGraphNearestNeighborVisitor;

			static RouteGraphRTree::BoundingBox createBoundingBox(e_int32 x1, e_int32 x2, e_int32 y1, e_int32 y2, e_int32 z1, e_int32 z2);

			RouteGraphRTree mRTree;
	};
}

#endif
