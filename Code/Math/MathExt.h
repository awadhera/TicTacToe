/******************************************************************************/
/*!
\file Math.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Interface for Math operations
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef MATH_H
#define MATH_H

#include <cmath>
#include<GL/glew.h>

/*! PI value */
#ifndef M_PI
#define M_PI 3.14159265f 
#endif

//!  Structure Point
/*!
Structure for a 3D Point in game world
*/

struct Vector;
//!  Structure Point
/*!
Structure for a 3D Point in game world
*/
struct Point
{
	float x; /*!< x coordinate as float */
	float y; /*!< y coordinate as float */
	float z; /*!< z coordinate as float */
	Point();
	Point(const float &arg_x,const float &arg_y,const float &arg_z);
	Point operator + (const Vector &v);
};

//!  Structure Vector
/*!
Structure for a 3D Vector in game world
*/
struct Vector
{
	float x; /*!< x coordinate as float */
	float y; /*!< x coordinate as float */
	float z; /*!< x coordinate as float */
	Vector();
	Vector(const float &arg_x,const float &arg_y,const float &arg_z);
	Vector operator * (const float &num);
	Vector operator + (const Vector &v);
	void Normalize();
	float Magnitude() const;
};

void crossProduct(const Vector &v1,const Vector &v2,Vector &v3);
float dotProduct(const Vector &v1,const Vector &v2);
float angleBetVectors(const Vector &v1,const Vector &v2);
float distanceBetweenTwoPoints(const Point &p1,const Point &p2);

#endif