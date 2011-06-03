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

#include "IntelligentAgentManager.h"

IntelligentAgent& IntelligentAgentManager::addNewAgent(e_double64 x, e_double64 y, e_float32 orientation)
{
	mAgentVector.push_back(IntelligentAgent(*this, eva::Point2Dd(x,y), orientation));
	return mAgentVector.back();
}

void IntelligentAgentManager::everybodyAct()
{
	for(e_uint32 i = 0; i < mAgentVector.size(); ++i)
		mAgentVector[i].act();
}

void IntelligentAgentManager::updatePositions(e_float32 secondsElapsed)
{
	for(e_uint32 i = 0; i < mAgentVector.size(); ++i)
		mAgentVector[i].updatePosition(secondsElapsed);
}

bool IntelligentAgentManager::losQuery(eva::Line2Dd line, eva::Point2Dd &hit) const
{
	IntelligentAgentQuadtreeVisitor visitor;
	bool result = mQuadtree.lineOfSightQuery<IntelligentAgentQuadtreeVisitor>(line,visitor);
	if(result)
		hit = visitor.hitPoint;
	return result;
}
