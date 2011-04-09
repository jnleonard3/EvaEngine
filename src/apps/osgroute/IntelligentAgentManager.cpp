/*
 * IntelligentAgentManager.cpp
 *
 *  Created on: Apr 4, 2011
 *      Author: Jon
 */

#include "IntelligentAgentManager.h"

void IntelligentAgent::act()
{
	// If we are waiting, remain waiting until orders come in
	if(mState == STATE_WAITING)
		return;

	// Do a Line Of Sight Check

}

void IntelligentAgent::updatePosition()
{
	mPosition.transpose(mVelocityVector.i(),mVelocityVector.j(),mVelocityVector.k());
	eva::Vector3Dd totalAcceleration = mAccelerationVector;
	mVelocityVector += totalAcceleration;
}

IntelligentAgentManager::IntelligentAgentManager() {
	// TODO Auto-generated constructor stub

}

IntelligentAgentManager::~IntelligentAgentManager() {
	// TODO Auto-generated destructor stub
}
