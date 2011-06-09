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

#ifndef EVA_LINE_H_
#define EVA_LINE_H_

#include "eva/Typedefs.h"
#include "math.h"
#include "eva/math/evaPoint.h"
#include "eva/math/evaMathCommon.h"
#include "eva/math/evaVector.h"

#include <iostream>

namespace eva
{
	template <class T, e_uchar8 N>
	class GenericLine
	{
		public:
			GenericLine():mFrom(),mTo(){};
			GenericLine(const GenericPoint<T,N> &lhs, const GenericPoint<T,N> &rhs):mFrom(lhs),mTo(rhs){};
			virtual ~GenericLine(){};

			const GenericPoint<T,N>& getFrom() const { return mFrom; };
			const GenericPoint<T,N>& getTo() const { return mTo; };

			void setFrom(const GenericPoint<T,N>& from) { mFrom = from; };
			void setTo(const GenericPoint<T,N>& to) { mTo = to; };

			T length() const { return mFrom.distance(mTo); };

			const GenericPoint<T,N> projectOnto(const GenericPoint<T,N>& pt) const
			{
				GenericVector<T,N> vec = this->toVector();
				const GenericPoint<T,N> resPt = pt.projectOnto(this->getFrom(),vec);
				T length = this->length();
				if(resPt.distance(this->getFrom()) > length)
					return this->getTo();
				else if(resPt.distance(this->getTo()) > length)
					return this->getFrom();
				return resPt;
			}

			const GenericVector<T,N> toVector() const
			{
				return GenericVector<T,N>(mFrom,mTo);
			}

			friend std::ostream& operator<<(std::ostream &stream, const GenericLine<T,N> &a)
			{
				stream << "[" << a.getFrom() << " -> " << a.getTo() << "]";
				return stream;
			}

		private:
			GenericPoint<T,N> mFrom, mTo;
	};

	template <class T>
	class Line2D : public GenericLine<T,2>
	{
		public:
			Line2D():GenericLine<T,2>(){};
			Line2D(T x1, T y1, T x2, T y2)
			:GenericLine<T,2>()
			{
				this->setFrom(Point2D<T>(x1,y1));
				this->setTo(Point2D<T>(x2,y2));
			};
			Line2D(const GenericPoint<T,2> &lhs, const GenericPoint<T,2> &rhs):GenericLine<T,2>(lhs,rhs){};
	};

	typedef Line2D<e_float32> Line2Df;
	typedef Line2D<e_double64> Line2Dd;

	template <class T>
	class Line3D : public GenericLine<T,3>
	{
		public:
			Line3D():GenericLine<T,3>(){};
			Line3D(T x1, T y1, T z1, T x2, T y2, T z2):GenericLine<T,3>()
			{
				this->setFrom(Point3D<T>(x1,y1,z1));
				this->setTo(Point3D<T>(x2,y2,z2));
			};
			Line3D(const Point3D<T> &lhs, const Point3D<T> &rhs):GenericLine<T,3>(lhs,rhs){};
	};

	typedef Line3D<e_float32> Line3Df;
	typedef Line3D<e_double64> Line3Dd;
}

#endif
