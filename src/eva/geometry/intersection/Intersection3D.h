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

#ifndef EVA_INTERSECTION3D_H_
#define EVA_INTERSECTION3D_H_

#include "eva/math/evaMathCommon.h"
#include "eva/math/evaPoint.h"
#include "eva/geometry/evaLine.h"

namespace eva
{
	class Intersection3D
	{
		public:
			// Line-Line Intersection, 2d for now, pay no attention to that
			template <class T>
			static bool Intersects(const Line3D<T>& a, const Line3D<T>& b)
			{ Point3D<T> pt; return Intersection3D::Intersection<T>(a,b,pt); }
			template <class T>
			static bool Intersection(const Line3D<T>& a, const Line3D<T>& b, Point3D<T>& pt)
			{
				Point3D<T> aFromPt = Point3D<T>(a.getFrom()), aToPt = Point3D<T>(a.getTo());
				Point3D<T> bFromPt = Point3D<T>(b.getFrom()), bToPt = Point3D<T>(b.getTo());
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

		private:
			Intersection3D();
			virtual ~Intersection3D();
	};

}

#endif
