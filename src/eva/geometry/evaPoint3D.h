/*
 * Copyright (c) 2011, EvaEngine Project
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

#ifndef EVA_POINT3D_H_
#define EVA_POINT3D_H_

#include "eva/Typedefs.h"
#include "eva/structures/evaFixedArray.h"
#include "math.h"

namespace eva
{
	template <class T>
	class Point3D : public FixedArray<T,3>
	{
		public:
			Point3D(){};
			Point3D(T x, T y, T z){this->x() = x; this->y() = y; this->z() = z;};

			T& x(){return (*this)[0];};
			const T& x() const {return (*this)[0];};

			T& y(){return (*this)[1];};
			const T& y() const {return (*this)[1];};

			T& z(){return (*this)[2];};
			const T& z() const {return (*this)[2];};

			void transpose(T x, T y, T z)
			{
				this->x() += x;
				this->y() += y;
				this->z() += z;
			};

			T distance(const Point3D<T>& point) const
			{
				return sqrt(pow(this->x()-point.x(),2)+pow(this->y()-point.y(),2)+pow(this->z()-point.z(),2));
			};

			Point3D<T> difference(const Point3D<T>& point) const
			{
				return Point3D<T>(abs(this->x()-point.x()),abs(this->y()-point.y()),abs(this->z()-point.z()));
			};

			bool isEqual(const Point3D<T> &rhs, e_float32 threshold) const
			{
				if(abs(this->x()-rhs.x()) < threshold)
					if(abs(this->y()-rhs.y()) < threshold)
						if(abs(this->z()-rhs.z()) < threshold)
							return true;
				return false;
			};

			bool isEqual(const Point3D<T> &rhs) const
			{
				return this->isEqual(rhs, 0.0f);
			};

			bool operator==(const Point3D<T> &rhs) const
			{
				return this->isEqual(rhs);
			};
	};

	typedef Point3D<e_uchar8> Point3Duc;
	typedef Point3D<e_char8> Point3Dc;
	typedef Point3D<e_uint32> Point3Dui;
	typedef Point3D<e_int32> Point3Di;
	typedef Point3D<e_float32> Point3Df;
	typedef Point3D<e_double64> Point3Dd;
}

#endif
