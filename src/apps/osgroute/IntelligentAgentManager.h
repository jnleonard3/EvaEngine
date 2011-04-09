#ifndef INTELLIGENTAGENT_H_
#define INTELLIGENTAGENT_H_

#include "RoadCommon.h"

#include "eva/geometry/evaPoint3D.h"
#include "eva/math/evaVector3D.h"

#include <list>

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

		IntelligentAgent(eva::Point3Dd position, e_float32 orientationAngle)
		:mState(STATE_WAITING),mPosition(position),mVelocityVector(),mAccelerationVector(),
		 mOrientationAngle(orientationAngle){};

		void act();

		void giveDirections(std::list<eva::RoutePathElement> &path);

		void updatePosition();

		const eva::Point3Dd& getPosition() const { return mPosition; };
		const e_float32& getOrientation() const { return mOrientationAngle; };

	private:
		e_uchar8 mState;
		eva::Point3Dd mPosition;
		eva::Vector3Dd mVelocityVector, mAccelerationVector;
		e_float32 mOrientationAngle, mSteeringWheelOffset;
		std::list<eva::RoutePathElement> &mCurrentDirections;
};

class IntelligentAgentManager
{
	public:
		IntelligentAgentManager();
		virtual ~IntelligentAgentManager();
};

#endif /* INTELLIGENTAGENT_H_ */
