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

#ifndef EVA_LINE2D_H_
#define EVA_LINE2D_H_

#include "eva/Typedefs.h"
#include "eva/geometry/evaPoint2D.h"
#include "eva/geometry/evaRectangle2D.h"
#include "eva/math/evaMathCommon.h"
#include "math.h"

namespace eva
{
	template <class T>
	class Line2D
	{
		public:
			Line2D(T x1, T y1, T x2, T y2):mFrom(x1,y1),mTo(x2,y2){};
			Line2D(const Point2D<T> &lhs, const Point2D<T> &rhs):mFrom(lhs),mTo(rhs){};

			const Point2D<T>& from() const { return mFrom; };
			const Point2D<T>& to() const { return mTo; };

			T length() const { return mFrom.distance(mTo); };

			bool intersects(const Line2D<T>& line, Point2D<T> &intersection) const
			{
				double AOne = line.to().y() - line.from().y();
				double BOne = line.from().x() - line.to().x();
				double COne = AOne*line.from().x()+BOne*line.from().y();
				double ATwo = this->to().y() - this->from().y();
				double BTwo = this->from().x() - this->to().x();
				double CTwo = ATwo*this->from().x()+BTwo*this->from().y();
				double det = AOne*BTwo-ATwo*BOne;
				if(det == 0)
					return false;
				else
				{
					double x = (BTwo*COne-BOne*CTwo)/det;
					double y = (AOne*CTwo-ATwo*COne)/det;
					if(MathCommon::isWithin<e_double64>(line.from().x(),line.to().x(),x) && MathCommon::isWithin<e_double64>(line.from().y(),line.to().y(),y))
						if(MathCommon::isWithin<e_double64>(this->from().x(),this->to().x(),x) && MathCommon::isWithin<e_double64>(this->from().y(),this->to().y(),y))
						{
							intersection.x() = x;
							intersection.y() = y;
							return true;
						}
				}
				return false;
			}

			bool intersects(const Line2D<T>& line) const
			{
				Point2D<T> temp;
				return this->intersects(line,temp);
			}

			bool intersects(const Rectangle2D<T>& rect, Point2D<T> &intersection) const
			{
				if(rect.intersects(this->from()))
				{
					intersection = this->from();
					return true;
				}

				Point2D<T> bR = Point2D<T>(rect.to().x(),rect.from().y()), tL = Point2D<T>(rect.from().x(),rect.to().y());
				Line2D<T> first(rect.from(),bR), second(rect.from(),tL), third(rect.to(),bR), fourth(rect.to(),tL);
				bool intersected = false;
				intersection = this->to();
				Point2D<T> potentialIntersection;
				if(this->intersects(first,potentialIntersection))
				{
					if(potentialIntersection.distance(this->from()) < intersection.distance(this->from()))
						intersection = potentialIntersection;
					intersected = true;
				}
				if(this->intersects(second,potentialIntersection))
				{
					if(potentialIntersection.distance(this->from()) < intersection.distance(this->from()))
						intersection = potentialIntersection;
					intersected = true;
				}
				if(this->intersects(third,potentialIntersection))
				{
					if(potentialIntersection.distance(this->from()) < intersection.distance(this->from()))
						intersection = potentialIntersection;
					intersected = true;
				}
				if(this->intersects(fourth,potentialIntersection))
				{
					if(potentialIntersection.distance(this->from()) < intersection.distance(this->from()))
						intersection = potentialIntersection;
					intersected = true;
				}
				return intersected;
			}

			bool intersects(const Rectangle2D<T>& rect) const
			{
				Point2D<T> temp;
				return this->intersects(rect,temp);
			}

		private:
			Point2D<T> mFrom, mTo;
	};

	typedef Line2D<e_uchar8> Line2Duc;
	typedef Line2D<e_char8> Line2Dc;
	typedef Line2D<e_uint32> Line2Dui;
	typedef Line2D<e_int32> Line2Di;
	typedef Line2D<e_float32> Line2Df;
	typedef Line2D<e_double64> Line2Dd;
}

#endif /* EVALINE3D_H_ */
