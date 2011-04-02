#ifndef EVA_ROUTENODE_H_
#define EVA_ROUTENODE_H_

#include "eva/Typedefs.h"
#include "eva/geometry/evaPoint3D.h"

namespace eva
{
	class RouteGraphEdge;

	class RouteNode
	{
		public:
			RouteNode():mNumEdgesFrom(0),mNumEdgesTo(0){};
			RouteNode(Point3Dd pt):mPoint(pt),mNumEdgesFrom(0),mNumEdgesTo(0){};
			RouteNode(Point3Dd pt, e_uchar8 numfromto, RouteGraphEdge **from, RouteGraphEdge **to):mPoint(pt),mNumEdgesFrom(numfromto),mNumEdgesTo(numfromto),mEdgesFrom(from),mEdgesTo(to){};
			RouteNode(Point3Dd pt, e_uchar8 numfrom, RouteGraphEdge **from, e_uchar8 numto, RouteGraphEdge **to):mPoint(pt),mNumEdgesFrom(numfrom),mNumEdgesTo(numto),mEdgesFrom(from),mEdgesTo(to){};

			Point3Dd getPoint() const {return mPoint;};

			e_uchar8 getNumEdgesFrom() const { return mNumEdgesFrom; };
			e_uchar8 getNumEdgesTo() const { return mNumEdgesTo; };

			RouteGraphEdge* getFromEdge(e_uchar8 index) const;
			RouteGraphEdge* getToEdge(e_uchar8 index) const;

			bool setFromEdge(e_uchar8 index, RouteGraphEdge *fromEdge);
			bool setToEdge(e_uchar8 index, RouteGraphEdge *toEdge);

			void setEdgesFrom(e_uchar8 count, RouteGraphEdge **from);
			void setEdgesTo(e_uchar8 count, RouteGraphEdge **to);

			bool isUnconnected() const;
			void clear();

		private:
			Point3Dd mPoint;
			e_uchar8 mNumEdgesFrom, mNumEdgesTo;
			RouteGraphEdge **mEdgesFrom, **mEdgesTo;
	};
}

#endif
