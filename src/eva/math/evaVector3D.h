#ifndef EVA_VECTOR3D_H_
#define EVA_VECTOR3D_H_

#include "eva/Typedefs.h"
#include "eva/math/evaAbstractVector.h"
#include "math.h"

namespace eva
{
	template <class T>
	class Vector3D : public AbstractVector<T,3>
	{
		public:
			Vector3D()
			:AbstractVector<T,3>(){};
			Vector3D(T i, T j, T k)
			:AbstractVector<T,3>(){this->i() = i; this->j() = j; this->k() = k;};
			Vector3D(const FixedArray<T,3> &a1, const FixedArray<T,3> &a2)
			:AbstractVector<T,3>(){this->i() = a2[0] - a1[0]; this->j() = a2[1] - a1[1]; this->k() = a2[2] - a1[2];};
			Vector3D(const FixedArray<T,3> &array)
			:AbstractVector<T,3>(array){};

			T& i() { return (*this)[0]; };
			const T& i() const { return (*this)[0]; };
			T& j() { return (*this)[1]; };
			const T& j() const { return (*this)[1]; };
			T& k() { return (*this)[2]; };
			const T& k() const { return (*this)[2]; };

			const Vector3D crossProduct(const Vector3D& rhs) const
			{
				Vector3D cross;
				cross.i() = (this->j()*rhs.k())-(rhs.j()*this->k());
				cross.j() = ((*this)[0]*rhs[2])-(rhs[0]*(*this)[2]);
				cross.k() = ((*this)[0]*rhs[1])-(rhs[0]*(*this)[1]);
				return cross;
			}
	};

	typedef Vector3D<e_uchar8> Vector3Duc;
	typedef Vector3D<e_char8> Vector3Dc;
	typedef Vector3D<e_uint32> Vector3Dui;
	typedef Vector3D<e_int32> Vector3Di;
	typedef Vector3D<e_float32> Vector3Df;
	typedef Vector3D<e_double64> Vector3Dd;
}

#endif
