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

#include "eva/geometry/basic/2d/evaPoint2D.h"
#include "eva/geometry/basic/3d/evaPoint3D.h"
#include "eva/geometry/basic/2d/evaLine2D.h"
#include "eva/math/evaVector3D.h"
#include "eva/route/evaRouteGraph.h"

#include <list>

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
		static const e_double64 MAX_DECELERATION_VALUE;

		IntelligentAgent(IntelligentAgentManager &manager):mManager(manager),mState(STATE_WAITING),mBounds(0.0,0.0,2.0,5.0){};
		IntelligentAgent(IntelligentAgentManager &manager, eva::Point3Dd position, e_float32 orientationAngle)
		:mManager(manager),mState(STATE_WAITING),mPosition(position),mVelocityVector(),mAccelerationVector(),
		 mOrientationAngle(orientationAngle),mBounds(0.0,0.0,2.0,5.0){};

		void act();

		void giveDirections(std::list<eva::RoutePathElement> &path);

		void updatePosition(e_float32 secondsElapsed);

		const eva::Point3Dd& getPosition() const { return mPosition; };
		const e_float32& getOrientation() const { return mOrientationAngle; };

		eva::Rectangled getBounds() const;

	private:
		IntelligentAgentManager &mManager;
		e_uchar8 mState;
		eva::Point3Dd mPosition;
		eva::Vector3Dd mVelocityVector, mAccelerationVector;
		e_float32 mOrientationAngle, mSteeringWheelOffset;
		std::list<eva::RoutePathElement> mCurrentDirections;
		eva::Rectangled mBounds;

		const IntelligentAgentManager& getManager() const { return mManager; };

		bool losQuery(eva::Line2Dd line, eva::Point2Dd &hit) const;
};

#endif /* INTELLIGENTAGENT_H_ */
