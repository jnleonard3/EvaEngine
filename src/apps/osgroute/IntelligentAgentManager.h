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

#ifndef INTELLIGENTAGENTMANAGER_H_
#define INTELLIGENTAGENTMANAGER_H_

#include "RoadCommon.h"
#include "FixedQuadtree.h"
#include "IntelligentAgent.h"

#include "eva/geometry/basic/2d/evaPoint2D.h"
#include "eva/geometry/basic/2d/evaSquare.h"
#include "eva/math/evaVector3D.h"

#include <list>

namespace eva
{
	class RouteGraph;
}

enum QuadtreeElementTypes
{
	INVALID_QUADTREEELEM,
	INTELLIGENTAGENT_QUADTREEELEM,
};

union QuadtreeElement
{
	IntelligentAgent *mAgent;
};

struct QuadtreeData
{
	QuadtreeData():mType(INVALID_QUADTREEELEM){};
	QuadtreeData(e_uchar8 type):mType(type){};
	e_uchar8 mType;
	QuadtreeElement mElement;
};

struct IntelligentAgentQuadtreeVisitor
{
	eva::Point2Dd hitPoint;
	bool operator()(const QuadtreeData* const node)
	{
		return true;
	}
};

class IntelligentAgentManager
{
	public:
		IntelligentAgentManager(const eva::RouteGraph &graph, eva::Squared effectiveArea)
		:DEFAULT_AGENT(*this),mAgentVector(0,DEFAULT_AGENT),mQuadtree(effectiveArea,15),mRouteGraph(graph){};
		virtual ~IntelligentAgentManager(){};

		IntelligentAgent& addNewAgent(e_double64 x, e_double64 y, e_float32 orientation);

		void everybodyAct();

		void updatePositions(e_float32 secondsElapsed);

		bool losQuery(eva::Line2Dd line, eva::Point2Dd &hit) const;

		const std::vector<IntelligentAgent>& getAgents() const { return mAgentVector; };

	private:
		const IntelligentAgent DEFAULT_AGENT;
		std::vector<IntelligentAgent> mAgentVector;
		FixedQuadtree<QuadtreeData> mQuadtree;
		const eva::RouteGraph &mRouteGraph;
};

#endif /* INTELLIGENTAGENT_H_ */
