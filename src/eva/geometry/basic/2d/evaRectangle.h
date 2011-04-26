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

#ifndef EVA_RECTANGLE_H_
#define EVA_RECTANGLE_H_

#include "eva/Typedefs.h"
#include "eva/geometry/basic/2d/evaTetragon.h"

namespace eva
{
	template <class T>
	class Rectangle : public Tetragon<T>
	{
		public:
			Rectangle(){};
			Rectangle(T fX, T fY, T tX, T tY):Tetragon<T>(Point2D<T>(fX,fY),Point2D<T>(tX,tY)){};
			Rectangle(const Point2D<T>& from, const Point2D<T>& to):Tetragon<T>(from,to){};
			Rectangle(const Point2D<T>& center, const Vector2D<T>& tl):Tetragon<T>(center,tl){};
			Rectangle(const Point2D<T>& center, const Vector2D<T>& tl, const Vector2D<T>& bl):Tetragon<T>(center,tl,bl){};
	};

	typedef Rectangle<e_uchar8> Rectangleuc;
	typedef Rectangle<e_char8> Rectanglec;
	typedef Rectangle<e_uint32> Rectangleui;
	typedef Rectangle<e_int32> Rectanglei;
	typedef Rectangle<e_float32> Rectanglef;
	typedef Rectangle<e_double64> Rectangled;
}
#endif /* EVARECTANGLE2D_H_ */
