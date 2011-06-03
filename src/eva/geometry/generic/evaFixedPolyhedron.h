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

#ifndef EVA_FIXEDPOLYHEDRON_H_
#define EVA_FIXEDPOLYHEDRON_H_
	
#include "eva/Typedefs.h"
#include "eva/math/evaVector.h"
#include "eva/geometry/basic/3d/evaPoint3D.h"
#include "eva/geometry/basic/3d/evaLine3D.h"

namespace eva
{
	template <class T, e_uchar8 N>
	class FixedPolyhedron
	{
		public:
			FixedPolyhedron():mCenter(),mOrientation(3,3),mNumEdges(0),mEdges(0){};
			FixedPolyhedron(const Point3D<T>& center):mCenter(center),mOrientation(3,3),mNumEdges(0),mEdges(0){};
			virtual ~FixedPolyhedron(){ delete mEdges; };

			const Point3D<T>& getCenter() const { return mCenter; };
			const Matrix<T>& getOrientation() const { return mOrientation; };
			const Point3D<T>* const getVerticies() const { return mVerticies; };
			const Line3D<T>* const getEdges() const { return mEdges; };
			e_uchar8 getNumOfEdges() const { return mNumEdges; };

			void move(const Point3D<T>& to)
			{
				this->getCenter() = to;
				this->recalculate();
			}

			void move(const Vector3D<T>& trans)
			{
				this->getCenter() += trans;
				this->recalculate();
			}

			void rotate(const Matrix<T> &mat)
			{
				Vector3D<T>* vectors = this->getVectors();
				for(e_uchar8 i = 0; i < N; ++i)
					vectors[i].rotate(mat);
				this->getOrientation() += mat;
				this->recalculate();
			}

			const FixedPolyhedron<T,N>& constRef() const { return *this; };

		protected:
			virtual Vector3D<T>* getVectors() { return mPointVectors; };
			virtual const Vector3D<T>* const getVectors() const { return mPointVectors; };
			Point3D<T>& getCenter() { return mCenter; };
			e_float32& getOrientation() { return mOrientation; };
			Point3D<T>* getVerticies() { return mVerticies; };
			Line3D<T>* getEdges() { return mEdges; };

			virtual void recalculate()
			{
				Vector3D<T>* vectors = this->getVectors();
				Point3D<T>* verticies = this->getVerticies();
				for(e_uchar8 i = 0; i < this->getNumOfEdges(); ++i)
					verticies[i] = this->getCenter() + vectors[i];

				this->recalculateEdges();
			}

			virtual void recalculateEdges(){};

		private:
			// Structural Information
			Point3D<T> mCenter;
			Vector3D<T> mPointVectors[N];
			Matrix<T> mOrientation;
			// Calculated Values
			Point3D<T> mVerticies[N];
			e_uchar8 mNumEdges;
			Line3D<T> *mEdges;
	};
}

#endif
