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

#ifndef EVA_POINT2D_H_
#define EVA_POINT2D_H_

#include "eva/Typedefs.h"
#include "eva/structures/evaFixedArray.h"
#include "eva/math/evaVector2D.h"

#include "math.h"

namespace eva
{
	template <class T>
	class Point2D : public FixedArray<T,2>
	{
		public:
			Point2D(){};
			Point2D(T x, T y){this->x() = x; this->y() = y;};
			Point2D(const Point2D<T> &pt, const Vector2D<T> &vec){this->x() = pt.x() + vec.i(); this->y() = pt.y() + vec.j();};

			T& x(){return (*this)[0];};
			const T& x() const {return (*this)[0];};

			T& y(){return (*this)[1];};
			const T& y() const {return (*this)[1];};

			void transpose(T x, T y) { this->x() += x; this->y() += y; };
			Point2D<T> transpose(T x, T y) const { return Point2D<T>(this->x() + x, this->y() + y); };
			Point2D<T> transpose(FixedArray<T,2> &array) const { return Point2D<T>(this->x() + array[0], this->y() + array[1]); };

			T distance(Point2D<T> point) const
			{
				return sqrt(pow(this->x()-point.x(),2)+pow(this->y()-point.y(),2));
			}

			e_char8 quadrant(Point2D<T> point) const
			{
				if(point.x() >= this->x() && point.y() > this->y())
					return 1;
				else if(point.x() < this->x() && point.y() >= this->y())
					return 2;
				else if(point.x() <= this->x() && point.y() < this->y())
					return 3;
				else if(point.x() > this->x() && point.y() <= this->y())
					return 4;
				else
					return 0;
			}

			Point2D<T> rotate(e_float32 radians) const
			{
				return Point2D<T>(this->x()*cos(radians)-this->y()*sin(radians),this->x()*sin(radians)+this->y()*cos(radians));
			}

			Point2D<T> rotate(Point2D<T> origin, e_float32 radians) const
			{
				T xDiff = origin.x() - this->x(), yDiff = origin.y() - this->y();
				return Point2D<T>((xDiff*cos(radians)-yDiff*sin(radians))+this->x(),(xDiff*sin(radians)+yDiff*cos(radians))+this->y());
			}

			Vector2D<T> toVector(const Point2D<T> &to) const
			{
				return Vector2D<T>(this->x(),this->y(),to.x(),to.y());
			}

			const Point2D<T> operator+(const Point2D<T> &rhs) const
			{
				Point2D<T> result = *this;
				result.x() += rhs.x();
				result.y() += rhs.y();
				return result;
			}

			const Point2D<T> operator+(const Vector2D<T> &other) const
			{
				Point2D<T> result = *this;
				result.x() += other.i();
				result.y() += other.j();
				return result;
			}
	};

	typedef Point2D<e_uchar8> Point2Duc;
	typedef Point2D<e_char8> Point2Dc;
	typedef Point2D<e_uint32> Point2Dui;
	typedef Point2D<e_int32> Point2Di;
	typedef Point2D<e_float32> Point2Df;
	typedef Point2D<e_double64> Point2Dd;
}

#endif