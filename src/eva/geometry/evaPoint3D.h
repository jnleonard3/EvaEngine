#ifndef EVA_POINT3D_H_
#define EVA_POINT3D_H_

#include "eva/Typedefs.h"
#include "math.h"

namespace eva
{
	template <class T>
	class Point3D
	{
		public:
			Point3D():mX(0),mY(0),mZ(0){};
			Point3D(T x, T y, T z):mX(x),mY(y),mZ(z){};

			T& x(){return mX;};
			const T& x() const {return mX;};

			T& y(){return mY;};
			const T& y() const {return mY;};

			T& z(){return mZ;};
			const T& z() const {return mZ;};

			T distance(const Point3D<T>& point) const;

			Point3D<T> difference(const Point3D<T>& point) const;

			bool isEqual(const Point3D<T> &rhs, e_float32 threshold) const;
			bool isEqual(const Point3D<T> &rhs) const { return this->isEqual(rhs, 0.0f); };

			bool operator==(const Point3D<T> &rhs) const { return this->isEqual(rhs); };

		private:
			T mX, mY, mZ;
	};

	template <class T>
	T Point3D<T>::distance(const Point3D<T>& point) const
	{
		return sqrt(pow(this->x()-point.x(),2)+pow(this->y()-point.y(),2)+pow(this->z()-point.z(),2));
	}

	template <class T>
	Point3D<T> Point3D<T>::difference(const Point3D<T>& point) const
	{
		return Point3D<T>(abs(this->x()-point.x()),abs(this->y()-point.y()),abs(this->z()-point.z()));
	}

	template <class T>
	bool Point3D<T>::isEqual(const Point3D<T> &rhs, e_float32 threshold) const
	{
		if(abs(this->x()-rhs.x()) < threshold)
			if(abs(this->y()-rhs.y()) < threshold)
				if(abs(this->z()-rhs.z()) < threshold)
					return true;
		return false;
	}

	typedef Point3D<e_uchar8> Point3Duc;
	typedef Point3D<e_char8> Point3Dc;
	typedef Point3D<e_uint32> Point3Dui;
	typedef Point3D<e_int32> Point3Di;
	typedef Point3D<e_float32> Point3Df;
	typedef Point3D<e_double64> Point3Dd;
}

#endif
