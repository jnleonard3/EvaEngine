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
#include "eva/math/evaVector.h"

#include "math.h"

#include <iostream>

const e_double64 IntelligentAgent::LOOKAHEAD_CONSTANT = 1.5;
const e_double64 IntelligentAgent::MAX_VELOCITY_VALUE = 15.0;
const e_double64 IntelligentAgent::MAX_ACCELERATION_VALUE = 1.0;
const e_double64 IntelligentAgent::MAX_DECELERATION_VALUE = 2.0;

const eva::Vector2Dd getOrientedVector(e_float32 orient)
{
	return eva::Vector2Dd(cos(orient),sin(orient));
}

void IntelligentAgent::act()
{
	if(mCurrentDirections && mState == STATE_WAITING)
		mState = STATE_STOPPED;
	else if(mState == STATE_WAITING) // If we are waiting, remain waiting until orders come in
		return;

	if(!mCurrentDirections)
	{
		if(mVelocityMagnitude > 0)
		{
			mState = STATE_DECELERTATNG;
			mAccelerationMagnitude = -0.75*MAX_DECELERATION_VALUE;
		}
		else
			mState = STATE_WAITING;
	}
	else if(this->getPosition().distance(mCurrentDirections->mPoint.toPoint<2>()) < 0.5)
	{
		std::cout << "Node Change!\n";
		if(mCurrentDirections->mNext == 0)
			mCurrentDirections = 0;
		else
		{
			mCurrentDirections = mCurrentDirections->mNext->mNext;
			mBounds.orientTowards(mCurrentDirections->mPoint.toPoint<2>());
		}
	}

	// Do a Line Of Sight Check
	e_double64 losDistance = 10.0;
	losDistance += mVelocityMagnitude*LOOKAHEAD_CONSTANT;
	eva::Vector2Dd losVec = getOrientedVector(this->getOrientation());
	losVec.scale(losDistance);

	eva::Point2Dd losStart(this->getPosition()), losEnd(losStart,losVec), hit;
	eva::Line2Dd losLine(losStart,losEnd);
	if(this->losQuery(losLine,hit))
	{
		mState = STATE_DECELERTATNG;
		mAccelerationMagnitude = MAX_DECELERATION_VALUE;
	}
	else
	{
		if(mState == STATE_STOPPED)
		{
			mBounds.orientTowards(mCurrentDirections->mPoint.toPoint<2>());
			mAccelerationMagnitude = 0.5*MAX_ACCELERATION_VALUE;
			mState = STATE_ACCELERATING;
		}

	}
}

void IntelligentAgent::giveDirections(eva::PathNode *node)
{
	if(mCurrentDirections)
	{
		mCurrentDirections->mNext = new eva::PathEdge();
		mCurrentDirections->mNext->mNext = node;
	}
	else
		mCurrentDirections = node;
}

void IntelligentAgent::updatePosition(e_float32 secondsElapsed)
{
	eva::Vector2Dd start = getOrientedVector(this->getOrientation()), directionVector = start, turnVector = start;
	directionVector.scale(mVelocityMagnitude);
	turnVector.rotate(eva::E_PI/2.0f);
	turnVector.scale(mSteeringWheelOffset);
	directionVector += turnVector;
	// Units are meters/second, scale vector to actual amount of time elapsed since last update
	directionVector.scale(secondsElapsed);
	mBounds.move(directionVector);

	if(directionVector.magnitude() != 0.0)
	{
		//directionVector.normalize();
		//e_float32 angle = start.angleBetween(directionVector);
		//if(mSteeringWheelOffset < 0)
		//	angle *= -1.0;
		//mBounds.rotate(angle);
	}

	mVelocityMagnitude += mAccelerationMagnitude*secondsElapsed;
	if(mVelocityMagnitude < 0.0)
	{
		mVelocityMagnitude = 0.0;
		mAccelerationMagnitude = 0.0;
		mState = STATE_STOPPED;
	}
	else if(mVelocityMagnitude > MAX_VELOCITY_VALUE)
	{
		mVelocityMagnitude = MAX_VELOCITY_VALUE;
		mAccelerationMagnitude = 0.0;
		mState = STATE_STEADY;
	}
}

bool IntelligentAgent::losQuery(const eva::Line2Dd &line, eva::Point2Dd &hit) const
{
	return this->getManager().losQuery(line, hit);
};
