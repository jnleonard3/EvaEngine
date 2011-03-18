#ifndef EVA_ROUTEGRAPHEDGE_H_
#define EVA_ROUTEGRAPHEDGE_H_

#include "eva/Typedefs.h"

namespace eva
{
	class RouteNode;

	enum RouteGraphEdgeTypes
	{
		ROUTEEDGE_INVALID = 0,
		ROUTEEDGE_DIRECT,
		ROUTEEDGE_SEMIDIRECT,
		ROUTEEDGE_INDIRECT
	};

	class RouteGraphEdge
	{
		public:
			RouteGraphEdge():mType(ROUTEEDGE_INVALID),mTo(0),mFrom(0),mNumEdgesConnected(0),mConnectedEdges(0){};
			RouteGraphEdge(e_uchar8 type, RouteNode *to, RouteNode *from):mType(type),mTo(to),mFrom(from),mNumEdgesConnected(0),mConnectedEdges(0){};;
			RouteGraphEdge(e_uchar8 type, RouteNode *to, RouteNode *from, e_uchar8 numconnected, RouteGraphEdge **connected):mType(type),mTo(to),mFrom(from),mNumEdgesConnected(numconnected),mConnectedEdges(connected){};

			RouteNode* getNodeTo() const { return mTo; };
			RouteNode* getNodeFrom() const { return mFrom; };

			e_uchar8 getNumEdgesConnected() const { return mNumEdgesConnected; };

			RouteGraphEdge* getConnectedEdge(e_uchar8 index) const;

			bool setConnectedEdge(e_uchar8 index, RouteGraphEdge *edge);

			void setConnectedEdges(e_uchar8 count, RouteGraphEdge **edges);

			void clear();

		private:
			const e_uchar8 mType;
			RouteNode * const mTo, * const mFrom;
			e_uchar8 mNumEdgesConnected;
			RouteGraphEdge **mConnectedEdges;
	};

}


#endif
