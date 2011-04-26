/*
 * Copyright (c) 2011, EvaEngine Project
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

#ifndef EVA_FIXEDARRAY_H_
#define EVA_FIXEDARRAY_H_

#include "eva/Typedefs.h"

namespace eva
{
	template <typename T, e_uchar8 N>
	class FixedArray
	{
		public:
			FixedArray(){};
			FixedArray(const FixedArray& array)
			{
				for(e_uchar8 i = 0; i < N; ++i)
					(*this)[i] = array[i];
			}

			virtual ~FixedArray(){};

			T& operator[] (e_uint32 index) { return this->mElements[index]; };
			const T& operator[] (e_uint32 index) const { return this->mElements[index]; };

			FixedArray<T,N>& operator=(const FixedArray<T,N> &rhs)
			{
			    // Check for self-assignment!
			    if (this == &rhs)      // Same object?
			      return *this;        // Yes, so skip assignment, and just return *this.

				for(e_uchar8 i = 0; i < N; ++i)
					(*this)[i] = rhs[i];
			    return *this;
			}

		protected:
			T& at(e_uint32 index) { return this->mElements[index]; };

		private:
			T mElements[N];
	};
}

#endif /* FIXEDARRAY_H_ */
