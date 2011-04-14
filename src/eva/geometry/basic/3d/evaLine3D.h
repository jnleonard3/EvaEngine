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

#ifndef EVA_LINE3D_H_
#define EVA_LINE3D_H_

#include "eva/Typedefs.h"
#include "math.h"
#include "eva/geometry/basic/3d/evaPoint3D.h"
#include "eva/math/evaMathCommon.h"

namespace eva
{
	template <class T>
	class Line3D
	{
		public:
			Line3D(T x1, T y1, T z1, T x2, T y2, T z2):mFrom(x1,y1,z1),mTo(x2,y2,z2){};
			Line3D(const Point3D<T> &lhs, const Point3D<T> &rhs):mFrom(lhs),mTo(rhs){};

			const Point3D<T>& from() const { return mFrom; };
			const Point3D<T>& to() const { return mTo; };

			T length() const { return mFrom.distance(mTo); };

		private:
			Point3D<T> mFrom, mTo;
	};

	typedef Line3D<e_uchar8> Line3Duc;
	typedef Line3D<e_char8> Line3Dc;
	typedef Line3D<e_uint32> Line3Dui;
	typedef Line3D<e_int32> Line3Di;
	typedef Line3D<e_float32> Line3Df;
	typedef Line3D<e_double64> Line3Dd;
}

#endif /* EVALINE3D_H_ */
