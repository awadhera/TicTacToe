/******************************************************************************/
/*!
\file Camera.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Implementation for a game camera
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/
#include "Camera.h"

/******************************************************************************/
/*!
Constructor for Camera class
*/
/******************************************************************************/
Camera::Camera()
{
	/// Initialize the mouse current and previous positions to 0
	mCurr.x = 0;
	mCurr.y = 0;
	mPrev.x = 0;
	mPrev.y = 0;
}

/******************************************************************************/
/*!
setCamera sets the default eye and target position for the camera along with
the up vector
\param eye_x
x coordinate of eye position
\param eye_y
y coordinate of eye position
\param eye_z
z coordinate of eye position
\param target_x
x coordinate of target position
\param target_y
y coordinate of target position
\param target_z
z coordinate of target position
\param up_x
x component for up vector
\param up_y
y component for up vector
\param up_z
z component for up vector
\return
void
*/
/******************************************************************************/
void Camera::setCamera(const float &eye_x,const float &eye_y,const float &eye_z,
	const float &target_x,const float &target_y,const float &target_z,
	const float &up_x, const float &up_y, const float &up_z)
{
	eye.x = eye_x;
	eye.y = eye_y;
	eye.z = eye_z;
	target.x = target_x;
	target.y = target_y;
	target.z = target_z;
	prevTarget.x = target_x;
	prevTarget.y = target_y;
	prevTarget.z = target_z;
	up.x = up_x;
	up.y = up_y;
	up.z = up_z;
}

/******************************************************************************/
/*!
moveFront moves the camera to the front position
\return
void
*/
/******************************************************************************/
void Camera::moveFront()
{
	/// Move the camera in the diretion of View Vector(except along y axis)
	evalViewVector();
	view.Normalize();
	eye.x+=(view.x)*0.1f;
	target.x+=(view.x)*0.1f;
	eye.z+=(view.z)*0.1f;
	target.z+=(view.z)*0.1f;
	prevTarget.z = target.z;
	prevTarget.x = target.x;
}

/******************************************************************************/
/*!
moveBack moves the camera to the back position
\return
void
*/
/******************************************************************************/
void Camera::moveBack()
{
	/// Move the camera in the diretion of View Vector(except along y axis)
	evalViewVector();
	view.Normalize();
	eye.x-=(view.x)*0.1f;
	target.x-=(view.x)*0.1f;
	eye.z-=(view.z)*0.1f;
	target.z-=(view.z)*0.1f;
	prevTarget.z = target.z;
	prevTarget.x = target.x;
}

/******************************************************************************/
/*!
moveLeft moves the camera to the left
\return
void
*/
/******************************************************************************/
void Camera::moveLeft()
{
	/// Move the camera in the diretion of side Vector(except along y axis)
	fixUpVector();
	evalViewVector();
	view.Normalize();
	crossProduct(view,up,side);
	side.Normalize();
	eye.x-=(side.x)*0.1f;
	target.x-=(side.x)*0.1f;
	eye.z-=(side.z)*0.1f;
	target.z-=(side.z)*0.1f;
	prevTarget.z = target.z;
	prevTarget.x = target.x;
}

/******************************************************************************/
/*!
moveLeft moves the camera to the right
\return
void
*/
/******************************************************************************/
void Camera::moveRight()
{
	/// Move the camera in the diretion of side Vector(except along y axis)
	fixUpVector();
	evalViewVector();
	view.Normalize();
	crossProduct(view,up,side);
	side.Normalize();
	eye.x+=(side.x)*0.1f;
	target.x+=(side.x)*0.1f;
	eye.z+=(side.z)*0.1f;
	target.z+=(side.z)*0.1f;
	prevTarget.z = target.z;
	prevTarget.x = target.x;
}

/******************************************************************************/
/*!
moveLeft moves the camera up
\return
void
*/
/******************************************************************************/
void Camera::moveUp()
{
	eye.y+=0.05f;
	target.y+=0.05f;
	prevTarget.y = target.y;
}

/******************************************************************************/
/*!
moveLeft moves the camera down
\return
void
*/
/******************************************************************************/
void Camera::moveDown()
{
	eye.y-=0.05f;
	target.y-=0.05f;
	prevTarget.y = target.y;
}

/******************************************************************************/
/*!
lookAt modifies the orientation of the camera
\return
void
*/
/******************************************************************************/
void Camera::lookAt()
{
	/// set the up vector
	fixUpVector();
	/// set the view matrix to identity
	viewMtx.setIdentity();
	/// evaluate the view vector
	evalViewVector();
	float angle = angleBetVectors(view,up);
	if(angle<1.0f || angle>179.0f)
	{
		target.x = prevTarget.x;
		target.y = prevTarget.y;
		target.z = prevTarget.z;
		evalViewVector();
	}
	/// Normalize the view vector
	view.Normalize();
	/// Compute side vector
	crossProduct(view,up,side);
	/// Normalize side vector
	side.Normalize();
	/// Recompute up vector
	crossProduct(side,view,up);
	/// Set the view Matrix values
	viewMtx.t[0][0] = side.x;
	viewMtx.t[1][0] = side.y;
	viewMtx.t[2][0] = side.z;
	viewMtx.t[0][1] = up.x;
	viewMtx.t[1][1] = up.y;
	viewMtx.t[2][1] = up.z;
	viewMtx.t[0][2] = -view.x;
	viewMtx.t[1][2] = -view.y;
	viewMtx.t[2][2] = -view.z;
	viewMtx.t[0][3] = viewMtx.t[1][3] = viewMtx.t[2][3] = 
		viewMtx.t[3][0] = viewMtx.t[3][1] = viewMtx.t[3][2] = 0;
	viewMtx.t[3][3] = 1;
	/// Translate the view matrix by eye position
	viewMtx.MultMatrix(my_Translate(-eye.x,-eye.y,-eye.z));
	prevTarget.x = target.x;
	prevTarget.y = target.y;
	prevTarget.z = target.z;
}

/******************************************************************************/
/*!
Affix the UP vector of camera to normalized y axis
\return
void
*/
/******************************************************************************/
void Camera::fixUpVector()
{
	up.x = 0;
	up.y = 1;
	up.z = 0;
}

/******************************************************************************/
/*!
Evaluate the new view vector
\return
void
*/
/******************************************************************************/
void Camera::evalViewVector()
{
	view.x = target.x - eye.x;
	view.y = target.y - eye.y;
	view.z = target.z - eye.z;
}