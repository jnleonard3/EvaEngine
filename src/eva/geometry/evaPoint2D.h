#ifndef EVA_POINT2D_H_
#define EVA_POINT2D_H_

#include "eva/Typedefs.h"
#include "math.h"

namespace eva
{
	template <class T>
	class Point2D
	{
		public:
			Point2D():mX(0),mY(0){};
			Point2D(T x, T y):mX(x),mY(y){};

			T& x(){return mX;};
			const T& x() const {return mX;};

			T& y(){return mY;};
			const T& y() const {return mY;};

			T distanceFrom(Point2D<T> point) const;

		private:
			T mX, mY;
	};

	template <class T>
	T Point2D<T>::distanceFrom(Point2D<T> point) const
	{
		return sqrt(pow(this->x()-point.x(),2)+pow(this->y()-point.y(),2));
	}

	typedef Point2D<e_uchar8> Point2Duc;
	typedef Point2D<e_char8> Point2Dc;
	typedef Point2D<e_uint32> Point2Dui;
	typedef Point2D<e_int32> Point2Di;
	typedef Point2D<e_float32> Point2Df;
	typedef Point2D<e_double64> Point2Dd;
}

#endif
