/******************************************************************************/
/*!
\file Camera.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Interface for a Game Camera
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef CAMERA_H
#define CAMERA_H

#include "../Geometry/Transforms.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/wglew.h>


//!  Camera class
/*!
Class for simple (First-person) camera management(UP,DOWN,LEFT,RIGHT,LOOKAT)
*/
class Camera	
{
	Vector up;	/*!< Up vector for camera */
	Vector view;	/*!< View vector for camera */
	Vector side;	/*!< Side vector for camera */
	Point prevTarget; /*!< Target(previous) position for camera */
	void fixUpVector();
	void evalViewVector();
public:
	Matrix viewMtx;			/*!< View Transform matrix */
	Point eye;	/*!< Eye position for camera */
	Point target; /*!< Target position for camera */
	POINT mCurr;		/*!< Current Mouse positions on the window*/
	POINT mPrev;	/*!< Previous Mouse positions on the window*/
	unsigned long vpWidth;		/*!< View port width */
	unsigned long vpHeight;		/*!< View port height */
	Camera();
	void setCamera(const float &eye_x,const float &eye_y,const float &eye_z,
		const float &target_x,const float &target_y,const float &target_z,
		const float &up_x, const float &up_y, const float &up_z);
	void moveFront();
	void moveBack();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void lookAt();
};

#endif