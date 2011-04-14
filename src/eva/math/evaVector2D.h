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

#ifndef EVA_VECTOR2D_H_
#define EVA_VECTOR2D_H_

#include "eva/Typedefs.h"
#include "eva/structures/evaFixedArray.h"
#include "math.h"

namespace eva
{
	template <class T>
	class Vector2D : public FixedArray<T,2>
	{
		public:
			Vector2D(){};
			Vector2D(T x, T y){this->i() = x; this->j() = y;};
			Vector2D(T xF, T yF, T xT, T yT){this->i() = xT - xF; this->j() = yT - yF;};

			T& i(){return (*this)[0];};
			const T& i() const {return (*this)[0];};

			T& j(){return (*this)[1];};
			const T& j() const {return (*this)[1];};

			Vector2D<T> rotate(e_float32 radians) const
			{
				return Vector2D<T>(this->x()*cos(radians)-this->y()*sin(radians),this->x()*sin(radians)+this->y()*cos(radians));
			}

			T magnitude() const
			{
				return sqrt(pow(this->i(),2)+pow(this->j(),2));
			}

			Vector2D<T>& operator*=(e_int32 scale)
			{
				this->i() *= scale;
				this->j() *= scale;
				return *this;
			}

			const Vector2D<T> operator*(e_int32 scale) const
			{
				Vector2D<T> result = *this;
				result *= scale;
				return result;
			}
	};

	typedef Vector2D<e_uchar8> Vector2Duc;
	typedef Vector2D<e_char8> Vector2Dc;
	typedef Vector2D<e_uint32> Vector2Dui;
	typedef Vector2D<e_int32> Vector2Di;
	typedef Vector2D<e_float32> Vector2Df;
	typedef Vector2D<e_double64> Vector2Dd;
}

#endif
