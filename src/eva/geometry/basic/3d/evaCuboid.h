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

#ifndef EVA_CUBOID_H_
#define EVA_CUBOID_H_

#include "eva/geometry/generic/evaFixedPolyhedron.h"
	
namespace eva
{
	template <class T>
	class Cuboid : public FixedPolyhedron<T,8>
	{
		public:
			Cuboid(){};
			Cuboid(T xMin, T xMax, T yMin, T yMax, T zMin, T zMax)
			:FixedPolyhedron<T,8>(Point3D<T>((xMax+xMin)/2.0,(yMax+yMin)/2.0,(zMax+zMin)/2.0))
			{
				Vector3D<T>* vectors = this->getVectors();
				vectors[0] = Point3D<T>(xMin,yMin,zMin);
				vectors[1] = Point3D<T>(xMax,yMin,zMin);
				vectors[2] = Point3D<T>(xMax,yMax,zMin);
				vectors[3] = Point3D<T>(xMin,yMax,zMin);
				vectors[4] = Point3D<T>(xMin,yMin,zMax);
				vectors[5] = Point3D<T>(xMax,yMin,zMax);
				vectors[6] = Point3D<T>(xMax,yMax,zMax);
				vectors[7] = Point3D<T>(xMin,yMax,zMax);
				this->recalculate();
			}
			Cuboid(const Point3D<T> &pt, T radius)
			:FixedPolyhedron<T,8>(pt)
			{
				Vector3D<T>* vectors = this->getVectors();
				vectors[0] = Point3D<T>(pt.x()-radius,pt.y()-radius,pt.z()-radius);
				vectors[1] = Point3D<T>(pt.x()+radius,pt.y()-radius,pt.z()-radius);
				vectors[2] = Point3D<T>(pt.x()+radius,pt.y()+radius,pt.z()-radius);
				vectors[3] = Point3D<T>(pt.x()-radius,pt.y()+radius,pt.z()-radius);
				vectors[4] = Point3D<T>(pt.x()-radius,pt.y()-radius,pt.z()+radius);
				vectors[5] = Point3D<T>(pt.x()+radius,pt.y()-radius,pt.z()+radius);
				vectors[6] = Point3D<T>(pt.x()+radius,pt.y()+radius,pt.z()+radius);
				vectors[7] = Point3D<T>(pt.x()-radius,pt.y()+radius,pt.z()+radius);
				this->recalculate();
			}
			virtual ~Cuboid(){};

	};

	typedef Cuboid<e_uchar8> Cuboiduc;
	typedef Cuboid<e_char8> Cuboidc;
	typedef Cuboid<e_uint32> Cuboidui;
	typedef Cuboid<e_int32> Cuboidi;
	typedef Cuboid<e_float32> Cuboidf;
	typedef Cuboid<e_double64> Cuboidd;
};

#endif
