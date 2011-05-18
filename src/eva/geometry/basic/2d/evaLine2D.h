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
#include "eva/geometry/basic/2d/evaPoint2D.h"
#include "eva/math/evaVector2D.h"
#include "eva/math/evaMathCommon.h"
#include "math.h"

namespace eva
{
	template <class T>
	class Line2D
	{
		public:
			Line2D():mFrom(),mTo(){};
			Line2D(T x1, T y1, T x2, T y2):mFrom(x1,y1),mTo(x2,y2){};
			Line2D(const Point2D<T> &lhs, const Point2D<T> &rhs):mFrom(lhs),mTo(rhs){};

			const Point2D<T>& from() const { return mFrom; };
			const Point2D<T>& to() const { return mTo; };

			T length() const { return mFrom.distance(mTo); };

			const Point2D<T> projectOnto(const Point2D<T>& pt) const
			{
				Point2D<T> resPt = pt.projectOnto(this->from(),this->toVector());
				T length = this->length();
				if(resPt.distance(this->from()) > length)
					return this->to();
				else if(resPt.distance(this->to()) > length)
					return this->from();
				return resPt;
			}

			Vector2D<T> toVector() const
			{
				return Vector2D<T>(mTo.x()-mFrom.x(),mTo.y()-mFrom.y());
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
