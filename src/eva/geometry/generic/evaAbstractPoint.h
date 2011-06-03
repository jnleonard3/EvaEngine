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

#ifndef EVA_ABSTRACTPOINT_H_
#define EVA_ABSTRACTPOINT_H_

#include "eva/Typedefs.h"
#include "eva/structures/evaFixedArray.h"
#include "eva/math/evaMatrix.h"
#include "eva/math/evaAbstractVector.h"

#include <iostream>
#include "math.h"

namespace eva
{
	template <class T, e_uchar8 N>
	class AbstractPoint : public FixedArray<T,N>
	{
		public:
			AbstractPoint():FixedArray<T,N>(){};
			AbstractPoint(const AbstractPoint<T,N>& pt, const FixedArray<T,N>& vec)
			:FixedArray<T,N>(pt)
			{
				for(e_uchar8 i = 0; i < N; ++i)
					(*this)[i] += vec[i];
			}
			AbstractPoint(const FixedArray<T,N>& array)
			:FixedArray<T,N>(array){};

			virtual ~AbstractPoint(){};

			void rotate(const Matrix<T>& rotMat)
			{
				Matrix<T> mult = rotMat*this->toMatrix();
				if(mult.getNumRows() != 0)
				{
					for(e_uchar8 i = 0; i < N; ++i)
						(*this)[i] = mult.at(i);
				}
			}

			AbstractPoint<T,N>& operator+=(const FixedArray<T,N>& rhs)
			{
				for(e_uchar8 i = 0; i < N; ++i)
					(*this)[i] += rhs[i];
				return *this;
			}

			T distance(const AbstractPoint& pt) const
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

			const AbstractPoint<T,N> projectOnto(const AbstractPoint<T,N> &vecPt, const AbstractVector<T,N> &vec) const
			{
				AbstractVector<T,N> vecPtVec(vecPt), thisPtVec(*this), vecNorm = vec;
				vecNorm.normalize();
				thisPtVec -= vecPtVec;
				T scale = thisPtVec.dotProduct(vec)/vec.magnitude();
				return AbstractPoint<T,N>(vecPt,vecNorm * scale);
			}

			template <e_uchar8 M>
			const AbstractPoint<T,M> toPoint() const
			{
				AbstractPoint<T,M> pt;
				e_uchar8 min = N;
				if(M < N)
					min = M;
				for(e_uchar8 i = 0; i < min; ++i)
					pt[i] = (*this)[i];
				return pt;
			}

			const AbstractVector<T,N> toVector() const
			{
				return AbstractVector<T,N>(*this);
			}

			const AbstractVector<T,N> toVector(const AbstractPoint<T,N>& to) const
			{
				AbstractVector<T,N> result = to.toVector() - this->toVector();
				return result;
			}

			bool isEqual(const AbstractPoint<T,N> &rhs, e_float32 threshold) const
			{
				for(e_uchar8 i = 0; i < N; ++i)
					if(abs((*this)[i] - rhs[i]) > threshold)
						return false;
				return true;
			}

			bool isEqual(const AbstractPoint<T,N> &rhs) const
			{
				return this->isEqual(rhs, 0.0f);
			};

			const AbstractPoint<T,N> operator+(const FixedArray<T,N> &rhs) const
			{
				AbstractPoint<T,N> result = *this;
				for(e_uchar8 i = 0; i < N; ++i)
					result[i] += rhs[i];
				return result;
			}

			const AbstractPoint<T,N> operator-(const FixedArray<T,N> &rhs) const
			{
				AbstractPoint<T,N> result = *this;
				for(e_uchar8 i = 0; i < N; ++i)
					result[i] -= rhs[i];
				return result;
			}

			const AbstractPoint<T,N> operator*(e_double64 scale) const
			{
				AbstractPoint<T,N> result = *this;
				for(e_uchar8 i = 0; i < N; ++i)
					result[i] *= scale;
				return result;
			}

			bool operator==(const AbstractPoint<T,N> &rhs) const
			{
				return this->isEqual(rhs);
			};

			friend std::ostream& operator<<(std::ostream &stream, const AbstractPoint<T,N> &a)
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
}

#endif
