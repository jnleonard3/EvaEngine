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

#ifndef EVA_BEZIERCURVE_H_
#define EVA_BEZIERCURVE_H_

#include "eva/Typedefs.h"
#include "eva/geometry/generic/evaAbstractPoint.h"

#include "math.h"
	
namespace eva
{
	template <class T, e_uchar8 N>
	class BezierCurve
	{
		public:
			BezierCurve(const AbstractPoint<T,N>& p0, const AbstractPoint<T,N>& p1, const AbstractPoint<T,N>& p2)
			:mP0(p0),mP1(p1),mP2(p2),mNumPoints(5),mBezierPoints(0)
			{
				this->recalulate(mNumPoints);
			}

			BezierCurve(e_uchar8 points, const AbstractPoint<T,N>& p0, const AbstractPoint<T,N>& p1, const AbstractPoint<T,N>& p2)
			:mP0(p0),mP1(p1),mP2(p2),mNumPoints(points),mBezierPoints(0)
			{
				this->recalulate(mNumPoints);
			}

			virtual ~BezierCurve()
			{
				delete mBezierPoints;
			}

			void recalculate(e_uchar8 points)
			{
				mBezierPoints = new AbstractPoint<T,N>[points];
				e_double64 inc = 1.0/(e_double64)points;

				e_uchar8 i = 0;
				for(e_double64 t = 0; t <= 1.0; t += inc)
				{
					e_double64 t0 = pow(1.0-t,2);
					e_double64 t1 = 2.0*(1.0-t)*t;
					e_double64 t2 = pow(t,2);
					mBezierPoints[i] = mP0 * t0 + mP1 * t1 + mP2 * t2;
					++i;
				}
				mNumPoints = points;
			}

			const e_uchar8 getNumPoints() const { return mNumPoints; };
			const AbstractPoint<T,N> getPoint(e_uchar8 index) const { return mBezierPoints[index]; };

			const AbstractPoint<T,N> getValue(e_float32 t) const
			{
				e_double64 t0 = pow(1.0-t,2);
				e_double64 t1 = 2.0*(1.0-t)*t;
				e_double64 t2 = pow(t,2);
				return mP0 * t0 + mP1 * t1 + mP2 * t2;
			}

		private:
			AbstractPoint<T,N> mP0, mP1, mP2;

			e_uchar8 mNumPoints;
			AbstractPoint<T,N>* mBezierPoints;
	};
};

#endif
