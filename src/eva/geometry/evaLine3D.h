
#ifndef EVALINE3D_H_
#define EVALINE3D_H_

#include "eva/Typedefs.h"
#include "math.h"
#include "evaPoint3D.h"
#include "eva/math/evaMathCommon.h"

namespace eva
{
	template <class T>
	class Line3D
	{
		public:
			Line3D(T x1, T y1, T z1, T x2, T y2, T z2):mFrom(x1,y1,z1),mTo(x2,y2,z2){};
			Line3D(const Point3D<T> &lhs, const Point3D<T> &rhs):mFrom(lhs),mTo(rhs){};

			const Point3D<T>& from() const { return mFrom; };
			const Point3D<T>& to() const { return mTo; };

			T length() const { mFrom.distance(mTo); };

			bool intersects(const Line3D<T>& line, Point3D<T> &intersection) const
			{
				double AOne = line.to().y() - line.from().y();
				double BOne = line.from().x() - line.to().x();
				double COne = AOne*line.from().x()+BOne*line.from().y();
				double ATwo = this->to().y() - this->from().y();
				double BTwo = this->from().x() - this->to().x();
				double CTwo = ATwo*this->from().x()+BTwo*this->from().y();
				double det = AOne*BTwo-ATwo*BOne;
				if(det == 0)
					return false;
				else
				{
					double x = (BTwo*COne-BOne*CTwo)/det;
					double y = (AOne*CTwo-ATwo*COne)/det;
					if(MathCommon::isWithin<e_double64>(line.from().x(),line.to().x(),x) && MathCommon::isWithin<e_double64>(line.from().y(),line.to().y(),y))
						if(MathCommon::isWithin<e_double64>(this->from().x(),this->to().x(),x) && MathCommon::isWithin<e_double64>(this->from().y(),this->to().y(),y))
						{
							intersection.x() = x;
							intersection.y() = y;
							return true;
						}
				}
				return false;
			}

			bool intersects(const Line3D<T>& line) const
			{
				Point3D<T> temp;
				return this->intersects(line,temp);
			}

		private:
			Point3D<T> mFrom, mTo;
	};

	typedef Line3D<e_uchar8> Line3Duc;
	typedef Line3D<e_char8> Line3Dc;
	typedef Line3D<e_uint32> Line3Dui;
	typedef Line3D<e_int32> Line3Di;
	typedef Line3D<e_float32> Line3Df;
	typedef Line3D<e_double64> Line3Dd;
}

#endif /* EVALINE3D_H_ */
