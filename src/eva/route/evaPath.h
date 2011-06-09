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

#ifndef EVA_PATH_H_
#define EVA_PATH_H_

#include "eva/Typedefs.h"
#include "eva/math/evaPoint.h"
#include "eva/geometry/evaLine.h"
	
namespace eva
{
	struct PathNode;
	struct PathEdge;

	struct PathNode
	{
		PathNode(const Point3Dd& pt):mPoint(pt),mNext(0){};
		Point3Dd mPoint;
		PathEdge *mNext;
	};

	enum PathEdgeTypes
	{
		PATH_EDGE = 0,
		PATH_MERGE_EDGE,
		PATH_EDGE_COUNT,
	};

	struct PathEdge
	{
		virtual e_uchar8 getType() const { return PATH_EDGE; };
		PathNode *mNext;
	};

	struct MergePathEdge : public PathEdge
	{
		virtual e_uchar8 getType() const { return PATH_MERGE_EDGE; };
		e_uchar8 mNumPaths;
		Line3Dd *mPaths;
	};

	struct PathParameters
	{
		bool directionIgnored;
		bool useSemidirectEdges;
		bool takeIndirectEdges;
	};

	class PathFinder
	{
		public:
			virtual PathNode* findPath(const Point3Dd &ptFrom, const Point3Dd &ptTo) const = 0;
			virtual PathNode* findPath(const Point3Dd &ptFrom, const Point3Dd &ptTo, PathParameters &parameters) const = 0;
	};
};

#endif
