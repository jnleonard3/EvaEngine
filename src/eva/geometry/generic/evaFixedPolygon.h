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

#ifndef EVA_FIXEDPOLYGON_H_
#define EVA_FIXEDPOLYGON_H_

#include "eva/Typedefs.h"
#include "eva/math/evaMathDefines.h"
#include "eva/math/evaVector.h"
#include "eva/math/evaPoint.h"
#include "eva/geometry/evaLine.h"

namespace eva
{
	template <class T, e_uchar8 N>
	class FixedPolygon
	{
		public:
			FixedPolygon():mCenter(),mOrientation(0){};
			FixedPolygon(const Point2D<T>& center):mCenter(center),mOrientation(0){};
			virtual ~FixedPolygon(){};

			const Point2D<T>& getCenter() const { return mCenter; };
			const e_float32& getOrientation() const { return mOrientation; };
			const Point2D<T>* const getVerticies() const { return mVerticies; };
			const Line2D<T>* const getEdges() const { return mEdges; };
			e_uchar8 getNumOfEdges() const { return N; };

			void move(const Point2D<T>& to)
			{
				this->getCenter() = to;
				this->recalculate();
			}

			void move(const Vector2D<T>& trans)
			{
				this->getCenter() += trans;
				this->recalculate();
			}

			void rotate(e_double64 angle)
			{
				Vector2D<T>* vectors = this->getVectors();
				for(e_uchar8 i = 0; i < this->getNumOfEdges(); ++i)
					vectors[i].rotate(angle);
				this->getOrientation() += angle;
				this->recalculate();
			}

			void orientTowards(const Point2D<T>& to)
			{
				std::cout << this->getCenter().x() << "," << this->getCenter().y() << " -> " << to.x() << "," << to.y() << "\n";
				Vector2D<T> vec(this->getCenter(),to), forward(cos(this->getOrientation()),sin(this->getOrientation()));
				std::cout << this->getOrientation() << "(theta)\n";
				std::cout << vec.angleBetween(forward) << "(theta)\n";
				std::cout << vec.i() << "," << vec.j() << "\n";
				std::cout << forward.i() << "," << forward.j() << "\n";
				std::cout << vec.angle() << "|" << forward.angle() << "\n";
				vec.normalize();
				this->rotate(vec.angleBetween(forward));
			}

			const FixedPolygon<T,N>& constRef() const { return *this; };

		protected:
			virtual Vector2D<T>* getVectors() { return mPointVectors; };
			virtual const Vector2D<T>* const getVectors() const { return mPointVectors; };
			Point2D<T>& getCenter() { return mCenter; };
			e_float32& getOrientation() { return mOrientation; };
			Point2D<T>* getVerticies() { return mVerticies; };
			Line2D<T>* getEdges() { return mEdges; };

			virtual void recalculate()
			{
				Vector2D<T>* vectors = this->getVectors();
				Point2D<T>* verticies = this->getVerticies();
				for(e_uchar8 i = 0; i < this->getNumOfEdges(); ++i)
					verticies[i] = this->getCenter() + vectors[i];

				Line2D<T>* edges = this->getEdges();
				for(e_uchar8 i = 0; i+1 < this->getNumOfEdges(); ++i)
					edges[i] = Line2D<T>(verticies[i],verticies[i+1]);
				edges[N-1] = Line2D<T>(verticies[N-1],verticies[0]);
			}

		private:
			// Structural Information
			Point2D<T> mCenter;
			Vector2D<T> mPointVectors[N];
			e_float32 mOrientation;
			// Calculated Values
			Point2D<T> mVerticies[N];
			Line2D<T> mEdges[N];
	};
}

#endif
