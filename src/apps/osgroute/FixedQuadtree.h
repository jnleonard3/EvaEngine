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

#ifndef FIXEDQUADTREE_H_
#define FIXEDQUADTREE_H_

#include "eva/Typedefs.h"
#include "eva/geometry/evaPoint2D.h"
#include "eva/geometry/evaLine2D.h"
#include "eva/geometry/evaRectangle2D.h"

#include <vector>
#include <list>
#include <iostream>

struct QuadAppearance
{
	QuadAppearance(eva::Square2Dd quad, e_uchar8 level):mQuad(quad),mLevel(level){};
	eva::Square2Dd mQuad;
	e_uchar8 mLevel;
};

template <typename T>
struct BasicQuadtreeLineIntersectVisitor
{
	bool operator()(const T* const node) const
	{
		return true;
	}
};

template <class T>
class FixedQuadtree
{
	public:
		FixedQuadtree(eva::Square2Dd area, e_float32 minRadius):mMinimumRadius(minRadius){mRoot = new Quad(area);};
		virtual ~FixedQuadtree(){};

		bool insert(T* data, eva::Point2Dd pos)
		{return insert(data,pos,mRoot);};

		bool move(T* data, eva::Point2Dd oldPos, eva::Point2Dd newPos)
		{ return move(data,oldPos,newPos,mRoot); };

		template <typename Visitor>
		bool lineOfSightQuery(eva::Line2Dd query, Visitor &visit) const
		{ if(mRoot && query.intersects(mRoot->mQuad)) return lineOfSightQuery<Visitor>(query,visit,mRoot); return false; }

		std::vector<QuadAppearance> getAppearance() const
		{
			std::vector<QuadAppearance> vec;
			getAppearance(vec,mRoot,0);
			return vec;
		}

	private:
		struct Quad
		{
			Quad(eva::Square2Dd quad):mQuad(quad),mTR(0),mBR(0),mBL(0),mTL(0){};
			std::list<T*> mItems;
			eva::Square2Dd mQuad;
			Quad *mTR, *mBR, *mBL, *mTL;
		};

		bool insert(T* data, eva::Point2Dd pos, Quad *quad)
		{
			if(quad && quad->mQuad.intersects(pos))
			{
				e_double64 radius = quad->mQuad.radius();
				if(radius < mMinimumRadius)
				{
					if(data)
						quad->mItems.push_back(data);
					return true;
				}

				const eva::Point2Dd &center = quad->mQuad.center();
				if(!quad->mTR)
				{
					e_double64 halfRadius = radius/2.0;
					quad->mTR = new Quad(eva::Square2Dd(center.transpose(halfRadius,halfRadius),halfRadius));
					quad->mBR = new Quad(eva::Square2Dd(center.transpose(halfRadius,-halfRadius),halfRadius));
					quad->mBL = new Quad(eva::Square2Dd(center.transpose(-halfRadius,-halfRadius),halfRadius));
					quad->mTL = new Quad(eva::Square2Dd(center.transpose(-halfRadius,halfRadius),halfRadius));
				}

				switch(center.quadrant(pos))
				{
					case 1:
						return this->insert(data,pos,quad->mTR); break;
					case 2:
						return this->insert(data,pos,quad->mTL); break;
					case 3:
						return this->insert(data,pos,quad->mBL); break;
					case 4:
						return this->insert(data,pos,quad->mBR); break;
					default:
						return false; break;
				}
			}
			return false;
		}

		bool move(T* data, eva::Point2Dd oldPos, eva::Point2Dd newPos, Quad *quad)
		{
			if(quad && quad->mQuad.intersects(oldPos))
			{
				eva::Point2Dd &center = quad->mQuad.center();
				if(quad->mTR)
				{
					bool result = false;
					switch(center.quadrant(oldPos))
					{
						case 1:
							result = this->move(data,oldPos,newPos,quad->mTR); break;
						case 2:
							result = this->move(data,oldPos,newPos,quad->mTL); break;
						case 3:
							result = this->move(data,oldPos,newPos,quad->mBL); break;
						case 4:
							result = this->move(data,oldPos,newPos,quad->mBR); break;
					}
					if(result)
						return true;
				}
				else
				{
					for(typename std::list<T*>::iterator i = quad->mItems.begin(); i != quad->mItems.end(); ++i)
						if((*i) == data)
						{
							quad->mItems.erase(i);
							break;
						}
				}

				if(quad->mQuad.intersects(newPos))
				{
					if(quad->mTR)
					{
						switch(center.quadrant(newPos))
						{
							case 1:
								return this->insert(data,newPos,quad->mTR); break;
							case 2:
								return this->insert(data,newPos,quad->mTL); break;
							case 3:
								return this->insert(data,newPos,quad->mBL); break;
							case 4:
								return this->insert(data,newPos,quad->mBR); break;
						}
					}
					else
					{
						return this->insert(data,newPos,quad);
					}
				}
			}
			return false;
		};

		template <typename Visitor>
		bool lineOfSightQuery(eva::Line2Dd query, Visitor &visit, Quad* quad) const
		{
			if(quad)
			{
				if(quad->mTR)
				{
					Quad* quads[4];
					e_double64 distance[4];
					for(e_uchar8 i = 0; i < 4; ++i)
					{
						quads[i] = 0;
						distance[i] = 0.0;
					}

					eva::Point2Dd intersection;
					if(query.intersects(quad->mTR->mQuad,intersection))
					{
						quads[0] = quad->mTR;
						distance[0] = query.from().distance(intersection);
					}

					if(query.intersects(quad->mTL->mQuad, intersection))
					{
						quads[1] = quad->mTL;
						distance[1] = query.from().distance(intersection);
					}

					if(query.intersects(quad->mBL->mQuad, intersection))
					{
						quads[2] = quad->mBL;
						distance[2] = query.from().distance(intersection);
					}

					if(query.intersects(quad->mBR->mQuad, intersection))
					{
						quads[3] = quad->mBR;
						distance[3] = query.from().distance(intersection);
					}

					for(e_uchar8 i = 0; i < 4; ++i)
					{
						e_uchar8 minIndex = i;
						for(e_uchar8 j = i; j < 4; ++j)
						{
							if(quads[j])
							{
								if(quads[minIndex])
								{
									if(distance[j] < distance[minIndex])
										minIndex = j;
								}
								else
								{
									minIndex = j;
								}
							}
						}
						Quad* tmpQuad = quads[i];
						e_double64 tmpDist = distance[i];
						quads[i] = quads[minIndex];
						distance[i] = distance[minIndex];
						quads[minIndex] = tmpQuad;
						distance[minIndex] = tmpDist;
					}

					for(e_uchar8 i = 0; i < 4; ++i)
					{
						if(quads[i])
						{
							if(this->lineOfSightQuery<Visitor>(query, visit, quads[i]))
								return true;
						}
					}
				}
				else
				{
					bool intersects = false;
					for(typename std::list<T*>::iterator i = quad->mItems.begin(); i != quad->mItems.end(); ++i)
						intersects |= visit((*i));
					if(intersects)
						return true;
				}
			}
			return false;
		}

		void getAppearance(std::vector<QuadAppearance> &toFill, Quad* quad, e_uchar8 level) const
		{
			if(quad)
			{
				toFill.push_back(QuadAppearance(quad->mQuad,level));

				if(quad->mTR)
				{
					getAppearance(toFill,quad->mTR,level+1);
					getAppearance(toFill,quad->mBR,level+1);
					getAppearance(toFill,quad->mBL,level+1);
					getAppearance(toFill,quad->mTL,level+1);
				}
			}
		}

		e_float32 mMinimumRadius;
		Quad *mRoot;
};
#endif
