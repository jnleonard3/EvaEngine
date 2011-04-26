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

#ifndef EVA_MATRIX_H_
#define EVA_MATRIX_H_

#include "eva/Typedefs.h"

#include <string.h>
	
namespace eva
{
	template <class T>
	class Matrix
	{
		public:
			Matrix():mMatrix(0),mRows(0),mCols(0){};
			Matrix(e_uint32 rows, e_uint32 cols)
			:mMatrix(0),mRows(rows),mCols(cols)
			{
				mMatrix = new T[mRows*mCols];
			};
			Matrix(e_uint32 rows, e_uint32 cols, T* elements)
			:mMatrix(0),mRows(rows),mCols(cols)
			{
				mMatrix = new T[mRows*mCols];
				memcpy(mMatrix,elements,mRows*mCols);
			};
			Matrix(const Matrix& mat)
			:mMatrix(0),mRows(mat.getNumRows()),mCols(mat.getNumCols())
			{
				mMatrix = new T[mRows*mCols];
				memcpy(mMatrix,&mat.at(0),mRows*mCols);
			}

			virtual ~Matrix(){delete mMatrix;};

			e_uint32 getNumRows() const { return mRows; };
			e_uint32 getNumCols() const { return mCols; };

			T& at(e_uint32 i) { return mMatrix[i]; };
			const T& at(e_uint32 i) const { return mMatrix[i]; };

			T& at(e_uint32 i, e_uint32 j) { return mMatrix[i+(j*mRows)]; };
			const T& at(e_uint32 i, e_uint32 j) const { return mMatrix[i+(j*mRows)]; };

			const Matrix<T> minorMatrix(e_uint32 i, e_uint32 j) const
			{
				e_uint32 skiprow = 0, skipcol = 0;
				Matrix<T> minorMat(this->getNumRows()-1,this->getNumCols()-1);
				for(e_uint32 i2 = 0; i2 < this->getNumRows(); ++i2)
				{
					if(i2 != i)
					{
						skipcol = 0;
						for(e_uint32 j2 = 0; j2 < this->getNumCols(); ++j2)
						{
							if(j2 != j)
								minorMat.at(i2-skiprow,j2-skipcol) = this->at(i2,j2);
							else
								skipcol = 1;
						}
					}
					else
						skiprow = 1;
				}
				return minorMat;
			}

			T determinant() const
			{
				if(this->getNumRows() != this->getNumCols())
					return 0;
				else if(this->getNumRows() == 1)
					return this->at(0);

				T val = 0;
				bool isNeg = false;
				for(e_uint32 i = 0; i < this->getNumRows(); ++i)
					for(e_uint32 j = 0; j < this->getNumCols(); ++j)
					{
						val += (-1*isNeg)*this->minorMatrix(i,j).determinant();
						isNeg = !isNeg;
					}
				return val;
			}

			const Matrix<T> operator*(const Matrix<T> &rhs) const
			{
				if(this->getNumCols() != rhs.getNumRows())
					return Matrix<T>();

				Matrix<T> mult(this->getNumRows(),rhs.getNumCols());

				for(e_uint32 i = 0; i < mult.getNumRows(); ++i)
					for(e_uint32 j = 0; j < mult.getNumCols(); ++j)
					{
						T val = 0;
						for(e_uint32 k = 0; k < this->getNumCols(); ++k)
							val += this->at(i,k)*rhs.at(k,j);
						mult.at(i,j) = val;
					}
				return mult;
			}

		private:

			T* mMatrix;
			const e_uint32 mRows, mCols;
	};
};

#endif
