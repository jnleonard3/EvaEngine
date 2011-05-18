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
/*
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

			Vector3D<T>& operator+=(const Vector3D<T> &rhs)
			{
			    mI += rhs.i();
			    mJ += rhs.j();
			    mK += rhs.k();
			    return *this;
			}

			const Vector3D<T> operator+(const Vector3D<T> &rhs) const
			{
			    return Vector3D<T>(*this) += rhs;
			}

		private:
			T mI, mJ, mK;
*/
	};

	typedef Vector3D<e_uchar8> Vector3Duc;
	typedef Vector3D<e_char8> Vector3Dc;
	typedef Vector3D<e_uint32> Vector3Dui;
	typedef Vector3D<e_int32> Vector3Di;
	typedef Vector3D<e_float32> Vector3Df;
	typedef Vector3D<e_double64> Vector3Dd;
}

#endif
