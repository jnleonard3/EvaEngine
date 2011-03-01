#ifndef EVA_ROUTENODE_H_
#define EVA_ROUTENODE_H_

#include "eva/Typedefs.h"
#include "geometry/evaPoint3D.h"

namespace eva
{
	class RouteNode
	{
		public:
			RouteNode():mId(0),mNumNodesFrom(0),mNumNodesTo(0){};
			RouteNode(e_uint32 id, Point3Df pt):mId(id),mPoint(pt),mNumNodesFrom(0),mNumNodesTo(0){};
			RouteNode(e_uint32 id, Point3Df pt, e_uchar8 numfromto, RouteNode **from, RouteNode **to):mId(id),mPoint(pt),mNumNodesFrom(numfromto),mNumNodesTo(numfromto),mNodesFrom(from),mNodesTo(to){};
			RouteNode(e_uint32 id, Point3Df pt, e_uchar8 numfrom, RouteNode **from, e_uchar8 numto, RouteNode **to):mId(id),mPoint(pt),mNumNodesFrom(numfrom),mNumNodesTo(numto),mNodesFrom(from),mNodesTo(to){};

			e_uint32 getId() const {return mId;};
			Point3Df getPoint() const {return mPoint;};

			e_uchar8 getNumNodesFrom() const { return mNumNodesFrom; };
			e_uchar8 getNumNodesTo() const { return mNumNodesTo; };

			RouteNode* getFromNode(e_uchar8 index) const;
			RouteNode* getToNode(e_uchar8 index) const;

			void setFromNode(e_uchar8 index, RouteNode *fromNode);
			void setToNode(e_uchar8 index, RouteNode *toNode);

			void setNodesFrom(e_uchar8 count, RouteNode **from);
			void setNodesTo(e_uchar8 count, RouteNode **to);

		private:
			const e_uint32 mId;
			const Point3Df mPoint;
			e_uchar8 mNumNodesFrom, mNumNodesTo;
			RouteNode **mNodesFrom, **mNodesTo;
	};
}

#endif
