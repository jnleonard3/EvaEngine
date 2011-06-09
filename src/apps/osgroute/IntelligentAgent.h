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

#ifndef INTELLIGENTAGENT_H_
#define INTELLIGENTAGENT_H_

#include "RoadCommon.h"

#include "eva/math/evaPoint.h"
#include "eva/geometry/evaLine.h"
#include "eva/geometry/basic/2d/evaRectangle.h"
#include "eva/route/evaRouteGraph.h"

class IntelligentAgentManager;

enum IntelligentAgentState
{
	STATE_STEADY,
	STATE_ACCELERATING,
	STATE_DECELERTATNG,
	STATE_STOPPED,
	STATE_WAITING
};

class IntelligentAgent
{
	public:
		static const e_double64 LOOKAHEAD_CONSTANT;
		static const e_double64 MAX_VELOCITY_VALUE;
		static const e_double64 ACCELERATION_INCREASE_VALUE;
		static const e_double64 MAX_DECELERATION_VALUE;
		static const e_double64 MAX_ACCELERATION_VALUE;

		IntelligentAgent(IntelligentAgentManager &manager)
		:mManager(manager),mState(STATE_WAITING),mBounds(eva::Point2Dd(),eva::Vector2Dd(2.0,5.0)),
		 mVelocityMagnitude(0.0f),mAccelerationMagnitude(0.0f),mSteeringWheelOffset(0.0f),mCurrentDirections(0){};
		IntelligentAgent(IntelligentAgentManager &manager, eva::Point2Dd position, e_float32 orientation)
		:mManager(manager),mState(STATE_WAITING),mBounds(position,eva::Vector2Dd(2.0,5.0)),
		 mVelocityMagnitude(0.0f),mAccelerationMagnitude(0.0f),mSteeringWheelOffset(0.0f),mCurrentDirections(0){mBounds.rotate(orientation);};
		IntelligentAgent(const IntelligentAgent& agent)
		:mManager(agent.mManager),mState(STATE_WAITING),mBounds(agent.mBounds),
		mVelocityMagnitude(0.0f),mAccelerationMagnitude(0.0f),mSteeringWheelOffset(0.0f),mCurrentDirections(0){};

		void act();

		void giveDirections(eva::PathNode *node);

		void updatePosition(e_float32 secondsElapsed);

		const eva::Point2Dd getPosition() const { return mBounds.constRef().getCenter(); };
		const e_float32& getOrientation() const { return mBounds.constRef().getOrientation(); };

		const eva::Rectangled getBounds() const { return mBounds; };

		IntelligentAgent& operator=(const IntelligentAgent& rhs)
		{
			if(this != &rhs)
				*this = IntelligentAgent(*this);
			return *this;
		}

	private:
		IntelligentAgentManager &mManager;
		e_uchar8 mState;
		eva::Rectangled mBounds;
		e_float32 mVelocityMagnitude, mAccelerationMagnitude, mSteeringWheelOffset;
		eva::PathNode *mCurrentDirections;

		const IntelligentAgentManager& getManager() const { return mManager; };

		bool losQuery(const eva::Line2Dd &line, eva::Point2Dd &hit) const;
};

#endif /* INTELLIGENTAGENT_H_ */
