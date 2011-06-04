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

#ifndef EVA_POINT_H_
#define EVA_POINT_H_

#include "eva/Typedefs.h"
#include "eva/structures/evaFixedArray.h"
#include "eva/math/evaMatrix.h"
#include "eva/math/evaVector.h"

#include <iostream>
#include "math.h"

namespace eva
{
	template <class T, e_uchar8 N>
	class GenericPoint : public FixedArray<T,N>
	{
		public:
			GenericPoint():FixedArray<T,N>(){};
			GenericPoint(const GenericPoint<T,N>& pt, const FixedArray<T,N>& vec)
			:FixedArray<T,N>(pt)
			{
				for(e_uchar8 i = 0; i < N; ++i)
					(*this)[i] += vec[i];
			}
			GenericPoint(const FixedArray<T,N>& array)
			:FixedArray<T,N>(array){};

			virtual ~GenericPoint(){};

			void rotate(const Matrix<T>& rotMat)
			{
				Matrix<T> mult = rotMat*this->toMatrix();
				if(mult.getNumRows() != 0)
				{
					for(e_uchar8 i = 0; i < N; ++i)
						(*this)[i] = mult.at(i);
				}
			}

			GenericPoint<T,N>& operator+=(const FixedArray<T,N>& rhs)
			{
				for(e_uchar8 i = 0; i < N; ++i)
					(*this)[i] += rhs[i];
				return *this;
			}

			T distance(const GenericPoint& pt) const
			{
				T val = 0;
				for(e_uchar8 i = 0; i < N; ++i)
				{
					T tmp = pt[i] - (*this)[i];
					val += tmp*tmp;
				}
				val = sqrt(val);
				return val;
			}

			const GenericPoint<T,N> projectOnto(const GenericPoint<T,N> &vecPt, const GenericVector<T,N> &vec) const
			{
				GenericVector<T,N> vecPtVec(vecPt), thisPtVec(*this), vecNorm = vec.normalized();
				thisPtVec -= vecPtVec;
				T scale = thisPtVec.dotProduct(vec)/vec.magnitude();
				return GenericPoint<T,N>(vecPt,vecNorm.scaled(scale));
			}

			template <e_uchar8 M>
			const GenericPoint<T,M> toPoint() const
			{
				GenericPoint<T,M> pt;
				e_uchar8 min = N;
				if(M < N)
					min = M;
				for(e_uchar8 i = 0; i < min; ++i)
					pt[i] = (*this)[i];
				return pt;
			}

			const GenericVector<T,N> toVector() const
			{
				return GenericVector<T,N>(*this);
			}

			const GenericVector<T,N> toVector(const GenericPoint<T,N>& to) const
			{
				GenericVector<T,N> result = to.toVector() - this->toVector();
				return result;
			}

			bool isEqual(const GenericPoint<T,N> &rhs, e_float32 threshold) const
			{
				for(e_uchar8 i = 0; i < N; ++i)
					if(abs((*this)[i] - rhs[i]) > threshold)
						return false;
				return true;
			}

			bool isEqual(const GenericPoint<T,N> &rhs) const
			{
				return this->isEqual(rhs, 0.0f);
			};

			const GenericPoint<T,N> operator+(const FixedArray<T,N> &rhs) const
			{
				GenericPoint<T,N> result = *this;
				for(e_uchar8 i = 0; i < N; ++i)
					result[i] += rhs[i];
				return result;
			}

			const GenericPoint<T,N> operator-(const FixedArray<T,N> &rhs) const
			{
				GenericPoint<T,N> result = *this;
				for(e_uchar8 i = 0; i < N; ++i)
					result[i] -= rhs[i];
				return result;
			}

			const GenericPoint<T,N> operator*(e_double64 scale) const
			{
				GenericPoint<T,N> result = *this;
				for(e_uchar8 i = 0; i < N; ++i)
					result[i] *= scale;
				return result;
			}

			bool operator==(const GenericPoint<T,N> &rhs) const
			{
				return this->isEqual(rhs);
			};

			friend std::ostream& operator<<(std::ostream &stream, const GenericPoint<T,N> &a)
			{
				stream << "(";
				for(e_uchar8 i = 0; i < N-1; ++i)
					stream << a[i] << ", ";
				stream << a[N-1];
				stream << ")";
				return stream;
			}

		private:
			Matrix<T> toMatrix() const
			{
				return Matrix<T>(N,1,&(*this)[0]);
			}

	};

	template <class T>
	class Point2D : public GenericPoint<T,2>
	{
		public:
			Point2D()
			:GenericPoint<T,2>(){};
			Point2D(T x, T y)
			:GenericPoint<T,2>(){this->x() = x; this->y() = y;};
			Point2D(const Point2D<T> &pt, const FixedArray<T,2> &vec)
			:GenericPoint<T,2>(pt,vec){};
			Point2D(const FixedArray<T,2>& array)
			:GenericPoint<T,2>(array){};

			T& x(){return (*this)[0];};
			const T& x() const {return (*this)[0];};

			T& y(){return (*this)[1];};
			const T& y() const {return (*this)[1];};

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

			const Point2D<T> rotate(e_float32 radians) const
			{
				return Point2D<T>(this->x()*cos(radians)-this->y()*sin(radians),this->x()*sin(radians)+this->y()*cos(radians));
			}

			const Point2D<T> rotate(Point2D<T> origin, e_float32 radians) const
			{
				T xDiff = origin.x() - this->x(), yDiff = origin.y() - this->y();
				return Point2D<T>((xDiff*cos(radians)-yDiff*sin(radians))+this->x(),(xDiff*sin(radians)+yDiff*cos(radians))+this->y());
			}
	};

	typedef Point2D<e_int32> Point2Di;
	typedef Point2D<e_float32> Point2Df;
	typedef Point2D<e_double64> Point2Dd;

	template <class T>
	class Point3D : public GenericPoint<T,3>
	{
		public:
			Point3D()
			:GenericPoint<T,3>(){};
			Point3D(T x, T y, T z)
			:GenericPoint<T,3>(){this->x() = x; this->y() = y; this->z() = z;};
			Point3D(const Point3D<T> &pt, const FixedArray<T,3> &vec)
			:GenericPoint<T,3>(pt,vec){};
			Point3D(const FixedArray<T,3>& array)
			:GenericPoint<T,3>(array){};
			virtual ~Point3D(){};

			T& x(){return (*this)[0];};
			const T& x() const {return (*this)[0];};

			T& y(){return (*this)[1];};
			const T& y() const {return (*this)[1];};

			T& z(){return (*this)[2];};
			const T& z() const {return (*this)[2];};
	};

	typedef Point3D<e_int32> Point3Di;
	typedef Point3D<e_float32> Point3Df;
	typedef Point3D<e_double64> Point3Dd;

}

#endif
