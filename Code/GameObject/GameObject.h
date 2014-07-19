/******************************************************************************/
/*!
\file GameObject.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #02
\date 09/03/2012
\brief
GameObject class
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Geometry/ObjectBase.h"
#include "Math/MathExt.h"

//!  Physics type enum
/*!
enum for type of objects in terms of its physics
*/
enum physicsType{ ball,straightwall,circularwall,square,line,circle };

//!  GameObject class
/*!
Class for game object representing physical properties
*/
class GameObject
{
	typedef Point Position;		/*!< Typedef for Point */
	typedef Vector Direction;	/*!< Typedef for Vector */
	typedef Point Destination;  /*!< Typedef for Point */
public:
	BaseGeometry *geoType;		/*!< Geometry type */
	Translate translate;	/*!< Translate transform for the geometry */
	Scale scale;			/*!< Scale transform for the geometry*/
	Rotate rotate;			/*!< Rotate transform for the geometry */
	physicsType shape;	/*!< Shape of the object */
	float mass;			/*!< Mass of the object */
	float speed;		/*!< Speed of the object */
	Position p;			/*!< Position of the object in world */
	Direction v;		/*!< Direction of the object */
	Destination d;		/*!< Destination of the object */
	bool dynamic;		/*!< True if object is dynamic,false if static */
	float radius;		/*!< Radius of the object(if circular)*/
	Position V0;		/*!< Starting point of a object(if straight wall)*/
	Position V1;		/*!< End point of a object(if straight wall)*/
	bool collisionFlag;	/*!< Check if object has already collided with something else in the frame(Used by driver)*/
	GameObject(BaseGeometry *type,physicsType s);
	GameObject();
	void setGeometryAndPhysicalType(BaseGeometry *type,physicsType s);
};

void updatePositionwrtVelocity(GameObject &go,const float &dt);

#endif