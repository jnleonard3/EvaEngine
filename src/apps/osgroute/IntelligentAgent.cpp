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

#include "IntelligentAgent.h"
#include "IntelligentAgentManager.h"

#include "eva/math/evaMathDefines.h"
#include "eva/math/evaVector2D.h"

#include "math.h"

const e_double64 IntelligentAgent::LOOKAHEAD_CONSTANT = 1.5;
const e_double64 IntelligentAgent::MAX_DECELERATION_VALUE = 8.0;

const eva::Vector2Dd getOrientedVector(e_float32 orient)
{
	return eva::Vector2Dd(cos(orient),sin(orient));
}

void IntelligentAgent::act()
{
	// If we are waiting, remain waiting until orders come in
	if(mState == STATE_WAITING)
		return;

	// Do a Line Of Sight Check
	e_double64 losDistance = 10.0;
	losDistance += mVelocityMagnitude*LOOKAHEAD_CONSTANT;
	eva::Vector2Dd losVec = getOrientedVector(this->getOrientation());
	losVec *= losDistance;

	eva::Point2Dd losStart(this->getPosition()), losEnd(losStart,losVec), hit;
	eva::Line2Dd losLine(losStart,losEnd);
	if(this->losQuery(losLine,hit))
	{

	}
	else
	{
		//if(mState == STATE_STOPPED)
	}

}

void IntelligentAgent::updatePosition(e_float32 secondsElapsed)
{
	eva::Vector2Dd start = getOrientedVector(this->getOrientation()), directionVector = start, turnVector = start;
	directionVector *= mVelocityMagnitude;
	turnVector.rotate(eva::E_PI/2.0f);
	turnVector *= mSteeringWheelOffset;
	directionVector += turnVector;
	// Units are meters/second, scale vector to actual amount of time elapsed since last update
	directionVector *= secondsElapsed;
	mBounds.move(directionVector);

	e_float32 angle = start.angleBetween(directionVector);
	if(mSteeringWheelOffset < 0)
		angle *= -1.0;
	mBounds.rotate(angle);

	mVelocityMagnitude += mAccelerationMagnitude;
	if(mVelocityMagnitude < 0.0)
	{
		mVelocityMagnitude = 0.0;
		mAccelerationMagnitude = 0.0;
	}
}

bool IntelligentAgent::losQuery(const eva::Line2Dd &line, eva::Point2Dd &hit) const
{
	return this->getManager().losQuery(line, hit);
	return false;
};
