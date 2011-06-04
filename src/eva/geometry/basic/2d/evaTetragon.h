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

#ifndef EVA_TETRAGON_H_
#define EVA_TETRAGON_H_

#include "eva/Typedefs.h"
#include "eva/geometry/generic/evaFixedPolygon.h"
#include "eva/math/evaPoint.h"

namespace eva
{
	template <class T>
	class Tetragon : public FixedPolygon<T,4>
	{
		public:
			Tetragon(){};
			Tetragon(const Point2D<T>& center):FixedPolygon<T,4>(center){};
			Tetragon(const Point2D<T>& center, const Vector2D<T>& first, const Vector2D<T>& second, const Vector2D<T>& third, const Vector2D<T>& fourth)
			:FixedPolygon<T,4>(center)
			{
				Vector2D<T>* vectors = this->getVectors();
				vectors[0] = first;
				vectors[1] = second;
				vectors[2] = third;
				vectors[3] = fourth;
				this->recalculate();
			};
			Tetragon(const Point2D<T>& from, const Point2D<T>& to)
			:FixedPolygon<T,4>()
			{
				this->getCenter() = from + Point2D<T>((to.x()-from.x())/2,(to.y()-from.y())/2);
				Vector2D<T>* vectors = this->getVectors();
				vectors[0] = Vector2D<T>(to.x()-this->getCenter().x(),to.y()-this->getCenter().y());
				vectors[1] = Vector2D<T>(vectors[0].i(),-vectors[0].j());
				vectors[2] = vectors[0].scaled(-1.0);
				vectors[3] = vectors[1].scaled(-1.0);
				this->recalculate();
			};
			Tetragon(const Point2D<T>& center, const Vector2D<T>& tl)
			:FixedPolygon<T,4>(center)
			{
				Vector2D<T>* vectors = this->getVectors();
				vectors[0] = tl;
				vectors[1] = Vector2D<T>(tl.i(),-tl.j());
				vectors[2] = vectors[0].scaled(-1.0);
				vectors[3] = vectors[1].scaled(-1.0);
				this->recalculate();
			};
			Tetragon(const Point2D<T>& center, const Vector2D<T>& tl, const Vector2D<T>& bl)
			:FixedPolygon<T,4>(center)
			{
				Vector2D<T>* vectors = this->getVectors();
				vectors[0] = tl;
				vectors[1] = bl;
				vectors[2] = vectors[0].scaled(-1.0);
				vectors[3] = vectors[1].scaled(-1.0);
				this->recalculate();
			};
			Tetragon(const Point2D<T>& center, T radius)
			:FixedPolygon<T,4>(center)
			{
				Vector2D<T>* vectors = this->getVectors();
				vectors[0] = Vector2D<T>(radius,radius);
				vectors[1] = Vector2D<T>(radius,-radius);
				vectors[2] = vectors[0].scaled(-1.0);
				vectors[3] = vectors[1].scaled(-1.0);
				this->recalculate();
			}
			virtual ~Tetragon(){};

		protected:
			virtual Vector2D<T>* getVectors() { return FixedPolygon<T,4>::getVectors(); };
			virtual const Vector2D<T>* const getVectors() const { return FixedPolygon<T,4>::getVectors(); };
	};
}

#endif /* EVATETRAGON_H_ */
