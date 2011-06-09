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

#ifndef EVA_INTERSECTION2D_H_
#define EVA_INTERSECTION2D_H_

#include "eva/math/evaVector.h"
#include "eva/math/evaMathCommon.h"
#include "eva/math/evaPoint.h"
#include "eva/geometry/evaLine.h"
#include "eva/geometry/basic/2d/evaTetragon.h"

#include <iostream>

namespace eva
{
	class Intersection2D
	{
		public:
			// Point-Line Intersection
			template <class T>
			static bool Intersects(const Point2D<T>& pt, const Line2D<T>& line)
			{ return Intersection2D::Intersects<T>(line,pt); };
			template <class T>
			static bool Intersects(const Line2D<T>& line, const Point2D<T>& pt)
			{
				Point2D<T> fromPt = Point2D<T>(line.getFrom()), toPt = Point2D<T>(line.getTo());
				Vector2D<T> lineVec(fromPt.x(),fromPt.y(),toPt.x(),toPt.y());
				if(lineVec.i() != 0)
				{
					T y = ((lineVec.j()/lineVec.i())*pt.x() - fromPt.x()) + fromPt.y();
					if(y == pt.y())
						if(MathCommon::isWithin<T>(fromPt.x(),toPt.x(),pt.x()))
							if(MathCommon::isWithin<T>(fromPt.y(),toPt.y(),pt.y()))
								return true;
				}
				return false;
			}

			// Line-Line Intersection
			template <class T>
			static bool Intersects(const Line2D<T>& a, const Line2D<T>& b)
			{ Point2D<T> pt; return Intersection2D::Intersection<T>(a,b,pt); }
			template <class T>
			static bool Intersection(const Line2D<T>& a, const Line2D<T>& b, Point2D<T>& pt)
			{
				Point2D<T> aFromPt = Point2D<T>(a.getFrom()), aToPt = Point2D<T>(a.getTo());
				Point2D<T> bFromPt = Point2D<T>(b.getFrom()), bToPt = Point2D<T>(b.getTo());
				e_double64 AOne = aToPt.y() - aFromPt.y();
				e_double64 BOne = aFromPt.x() - aToPt.x();
				e_double64 COne = AOne*aFromPt.x()+BOne*aFromPt.y();
				e_double64 ATwo = bToPt.y() - bFromPt.y();
				e_double64 BTwo = bFromPt.x() - bToPt.x();
				e_double64 CTwo = ATwo*bFromPt.x()+BTwo*bFromPt.y();
				e_double64 det = AOne*BTwo-ATwo*BOne;
				if(det == 0.0)
					return false;
				else
				{
					T x = (BTwo*COne-BOne*CTwo)/det;
					T y = (AOne*CTwo-ATwo*COne)/det;
					if(MathCommon::isWithin<T>(aFromPt.x(),aToPt.x(),x) && MathCommon::isWithin<T>(aFromPt.y(),aToPt.y(),y))
						if(MathCommon::isWithin<T>(bFromPt.x(),bToPt.x(),x) && MathCommon::isWithin<T>(bFromPt.y(),bToPt.y(),y))
						{
							pt.x() = x;
							pt.y() = y;
							return true;
						}
				}
				return false;
			}

			// Point-Simple Tetragon Intersection
			template <class T>
			static bool Intersects(const Point2D<T>& pt, const Tetragon<T>& tetra)
			{ return Intersection2D::Intersects<T>(tetra,pt); }
			template <class T>
			static bool Intersects(const Tetragon<T>& tetra, const Point2D<T>& pt)
			{
				const Line2D<T>* const edges = tetra.getEdges();
				if(Intersection2D::liesBetween(edges[0],edges[2],pt))
					if(Intersection2D::liesBetween(edges[1],edges[3],pt))
						return true;
				return false;
			}

			// Line-Simple Tetragon Intersection
			template <class T>
			static bool Intersects(const Line2D<T>& line, const Tetragon<T>& tetra)
			{ return Intersection2D::Intersects<T>(tetra,line); }
			template <class T>
			static bool Intersects(const Tetragon<T>& tetra,const Line2D<T>& line)
			{ Point2D<T> pt; return Intersection2D::Intersection<T>(tetra,line,pt); }
			template <class T>
			static bool Intersection(const Line2D<T>& line, const Tetragon<T>& tetra, Point2D<T> &pt)
			{ return Intersection2D::Intersection<T>(tetra,line,pt); }
			template <class T>
			static bool Intersection(const Tetragon<T>& tetra, const Line2D<T>& line, Point2D<T> &pt)
			{
				if(Intersection2D::Intersects<T>(Point2D<T>(line.getFrom()),tetra))
				{
					pt = Point2D<T>(line.getFrom());
					return true;
				}

				const Line2D<T>* const edges = tetra.getEdges();
				for(e_uchar8 i = 0; i < 4; ++i)
					if(Intersection2D::Intersection<T>(line,edges[i],pt))
						return true;

				return false;
			}

			//Simple Tetragon-Simple Tetragon Intersection
			template <class T>
			static bool Intersects(const Tetragon<T>& a, const Tetragon<T>& b)
			{
				const Point2D<T>* const aPts = a.getVerticies();
				for(e_uchar8 i = 0; i < 4; ++i)
					if(Intersection2D::Intersects<T>(b,aPts[i]))
						return true;

				const Point2D<T>* const bPts = b.getVerticies();
				for(e_uchar8 i = 0; i < 4; ++i)
					if(Intersection2D::Intersects<T>(a,bPts[i]))
						return true;

				return false;
			}

		private:
			Intersection2D();
			virtual ~Intersection2D();

			template <class T>
			static bool liesBetween(const Line2D<T>& a, const Line2D<T>& b, const Point2D<T>& pt)
			{
				Vector2D<T> lineAVec = a.toVector();
				Vector2D<T> lineBVec = b.toVector();

				Point2D<T> aFromPt = Point2D<T>(a.getFrom());
				Point2D<T> bFromPt = Point2D<T>(b.getFrom());

				if(lineAVec.i() != 0 && lineBVec.i() != 0)
				{
					T yA = (lineAVec.j()/lineAVec.i())*(pt.x() - aFromPt.x()) + aFromPt.y();
					T yB = (lineBVec.j()/lineBVec.i())*(pt.x() - bFromPt.x()) + bFromPt.y();
					if(MathCommon::isWithin<T>(yA,yB,pt.y()))
						return true;
				}
				else
				{
					if(MathCommon::isWithin<T>(aFromPt.x(),bFromPt.x(),pt.x()))
						return true;
				}

				if(lineAVec.j() != 0 && lineBVec.j() != 0)
				{
					T xA = (lineAVec.i()/lineAVec.j())*(pt.y() - aFromPt.y()) + aFromPt.x();
					T xB = (lineBVec.i()/lineBVec.j())*(pt.y() - bFromPt.y()) + bFromPt.x();
					if(MathCommon::isWithin<T>(xA,xB,pt.x()))
						return true;
				}
				else
				{
					if(MathCommon::isWithin<T>(aFromPt.y(),bFromPt.y(),pt.y()))
						return true;
				}
				return false;
			}
	};

}

#endif
