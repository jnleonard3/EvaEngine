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

#ifndef EVA_RECTANGLE2D_H_
#define EVA_RECTANGLE2D_H_

#include "eva/Typedefs.h"
#include "evaPoint2D.h"
#include "eva/math/evaMathCommon.h"

namespace eva
{
	template <class T>
	class Rectangle2D
	{
		public:
			Rectangle2D(){};
			Rectangle2D(Point2D<T> from, Point2D<T> to):mFrom(from),mTo(to){};

			const Point2D<T>& from() const { return mFrom; };
			const Point2D<T>& to() const { return mTo; };

			Point2D<T> center() const {return Point2D<T>(((this->to().x()-this->from().x())/2)+this->from().x(), ((this->to().y()-this->from().y())/2)+this->from().y());};

			bool intersects(Point2D<T> pt) const
			{
				if(MathCommon::isWithin<T>(mFrom.x(),mTo.x(),pt.x()))
					if(MathCommon::isWithin<T>(mFrom.y(),mTo.y(),pt.y()))
						return true;
				return false;
			}

		private:
			Point2D<T> mFrom, mTo;
	};

	template <class T>
	class Square2D : public Rectangle2D<T>
	{
		public:
			Square2D():Rectangle2D<T>(){};
			Square2D(Point2D<T> center, T radius):Rectangle2D<T>(Point2D<T>(center.x()-radius,center.y()-radius),Point2D<T>(center.x()+radius,center.y()+radius)){};

			T radius() const {return (this->to().x()-this->from().x())/2.0;};
	};

	typedef Square2D<e_uchar8> Square2Duc;
	typedef Square2D<e_char8> Square2Dc;
	typedef Square2D<e_uint32> Square2Dui;
	typedef Square2D<e_int32> Square2Di;
	typedef Square2D<e_float32> Square2Df;
	typedef Square2D<e_double64> Square2Dd;

}


#endif /* EVARECTANGLE2D_H_ */
