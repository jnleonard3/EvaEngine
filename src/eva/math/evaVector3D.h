#ifndef EVA_VECTOR3D_H_
#define EVA_VECTOR3D_H_

#include "eva/Typedefs.h"
#include "eva/geometry/evaPoint3D.h"
#include "math.h"

namespace eva
{
	template <class T>
	class Vector3D
	{
		public:
			Vector3D():mI(0),mJ(0),mK(0){};
			Vector3D(T i, T j, T k):mI(i),mJ(j),mK(k){};
			Vector3D(Point3D<T> &p1, Point3D<T> &p2):mI(p2.x()-p1.x()),mJ(p2.y()-p1.y()),mK(p2.z()-p1.z()){};

			const T& i() const { return mI; };
			const T& j() const { return mJ; };
			const T& k() const { return mK; };

			void scale(e_float32 factor)
			{
				mI *= factor;
				mJ *= factor;
				mK *= factor;
			}

			void normalize()
			{
				T length = sqrt(pow(mI,2) + pow(mJ,2) + pow(mK,2));
				mI /= length;
				mJ /= length;
				mK /= length;
			}

			Vector3D crossProduct(const Vector3D &rhs) const
			{
				return Vector3D((this->j()*rhs.k())-(rhs.j()*this->k()),(this->i()*rhs.k())-(rhs.i()*this->k()),(this->i()*rhs.j())-(rhs.i()*this->j()));
			}

			bool isZeroVector() const
			{
				return mI == 0 && mJ == 0 && mK == 0;
			}

		private:
			T mI, mJ, mK;
	};

	typedef Vector3D<e_uchar8> Vector3Duc;
	typedef Vector3D<e_char8> Vector3Dc;
	typedef Vector3D<e_uint32> Vector3Dui;
	typedef Vector3D<e_int32> Vector3Di;
	typedef Vector3D<e_float32> Vector3Df;
	typedef Vector3D<e_double64> Vector3Dd;
}

#endif
