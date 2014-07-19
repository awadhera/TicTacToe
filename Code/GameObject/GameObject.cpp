/******************************************************************************/
/*!
\file GameObject.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #02
\date 09/03/2012
\brief
Implementation of GameObject class
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#include "GameObject.h"

/******************************************************************************/
/*!
Constructor for GameObject class
\param [in] type
Pointer to BaseGeometry type 
\param [in] s
Type of physics object
*/
/******************************************************************************/
GameObject::GameObject(BaseGeometry *type,physicsType s) : geoType(type), shape(s)
{
}

/******************************************************************************/
/*!
Update the position of object with respect to its velocity
\param [in] go
Reference to a Game Object
\param [in] dt
Reference to a constant float representing time taken by frame
\return
void
*/
/******************************************************************************/
void updatePositionwrtVelocity(GameObject &go,const float &dt)
{
	if(go.dynamic)
	{
		/// Find velocityVector*dt
		float speedTime = go.speed*dt;
		Vector velocity = go.v*speedTime;
		/// Update the position
		go.p = go.p + velocity;
		/// Translate the geometric object for this physics object through translate trasform
		go.translate.x = go.p.x;
		go.translate.y = go.p.y;
		go.translate.z = go.p.z;
	}
}

void GameObject::setGeometryAndPhysicalType(BaseGeometry *type,physicsType s)
{
	geoType = type;
	shape = s;
}

GameObject::GameObject()
{
}
