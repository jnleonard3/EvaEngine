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

#ifndef EVAABSTRACTVECTOR_H_
#define EVAABSTRACTVECTOR_H_

#include "eva/Typedefs.h"
#include "eva/structures/evaFixedArray.h"
#include "eva/math/evaMatrix.h"
#include "math.h"

namespace eva
{
	template <class T, e_uchar8 N>
	class AbstractVector : public FixedArray<T,N>
	{
		public:
			AbstractVector():FixedArray<T,N>(){};
			AbstractVector(const FixedArray<T,N>& array)
			:FixedArray<T,N>(array){};

			void normalize()
			{
				T mag = this->magnitude();
				for(e_uchar8 i = 0; i < N; ++i)
					(*this)[i] /= mag;
			}

			void rotate(const Matrix<T>& rotMat)
			{
				Matrix<T> mult = rotMat*this->toMatrix();
				if(mult.getNumRows() != 0)
				{
					for(e_uchar8 i = 0; i < N; ++i)
						(*this)[i] = mult.at(i);
				}
			}

			AbstractVector<T,N>& operator+=(const FixedArray<T,N>& rhs)
			{
				for(e_uchar8 i = 0; i < N; ++i)
					(*this)[i] += rhs[i];
				return *this;
			}

			AbstractVector<T,N>& operator-=(const FixedArray<T,N>& rhs)
			{
				for(e_uchar8 i = 0; i < N; ++i)
					(*this)[i] -= rhs[i];
				return *this;
			}

			AbstractVector<T,N>& operator*=(e_int32 scale)
			{
				for(e_uchar8 i = 0; i < N; ++i)
					(*this)[i] *= scale;
				return *this;
			}

			bool isZero() const
			{
				for(e_uchar8 i = 0; i < N; ++i)
					if((*this)[i] != 0)
						return false;
				return true;
			}

			T magnitude() const
			{
				T val = 0;
				for(e_uchar8 i = 0; i < N; ++i)
					val += (*this)[i]*(*this)[i];
				val = sqrt(val);
				return val;
			}

			T dotProduct(const AbstractVector<T,N>& rhs) const
			{
				T val = 0;
				for(e_uchar8 i = 0; i < N; ++i)
					val += (*this)[i]*rhs[i];
				return val;
			}

			const AbstractVector<T,N> crossProduct(const AbstractVector<T,N>& rhs) const
			{
				if(N != 3)
					return AbstractVector<T,N>();

				AbstractVector<T,N> cross;
				cross[0] = ((*this)[1]*rhs[2])-(rhs[1]*(*this)[2]);
				cross[1] = ((*this)[0]*rhs[2])-(rhs[0]*(*this)[2]);
				cross[2] = ((*this)[0]*rhs[1])-(rhs[0]*(*this)[1]);
				return cross;
			}

			T angleBetween(const AbstractVector<T,N>& rhs) const
			{
				return acos(this->dotProduct(rhs)/(this->magnitude()*rhs.magnitude()));
			}

			Matrix<T> toMatrix() const
			{
				return Matrix<T>(N,1,&(*this)[0]);
			}

			const AbstractVector<T,N> operator+(const FixedArray<T,N>& rhs) const
			{
				AbstractVector<T,N> result = *this;
				result += rhs;
				return result;
			}

			const AbstractVector<T,N> operator-(const FixedArray<T,N>& rhs) const
			{
				AbstractVector<T,N> result = *this;
				result -= rhs;
				return result;
			}

			const AbstractVector<T,N> operator*(e_int32 scale) const
			{
				AbstractVector<T,N> result = *this;
				result *= scale;
				return result;
			}
	};
};

#endif
