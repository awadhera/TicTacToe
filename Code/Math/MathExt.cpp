/******************************************************************************/
/*!
\file Math.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Implementation of Math library
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/
#include "MathExt.h"

/******************************************************************************/
/*!
Constructor for Point structure
*/
/******************************************************************************/
Point::Point()
{
}

/******************************************************************************/
/*!
Parameterized constructor for Point structure
*/
/******************************************************************************/
Point::Point(const float &arg_x,const float &arg_y,const float &arg_z)
{
	/// x,y,z coordinates of the point
	x = arg_x;
	y = arg_y;
	z = arg_z;
}

/******************************************************************************/
/*!
Overloaded Assignment Operator implementation for Point+Vector
\param [in] v
Reference to a constant Vector
\return
Point
*/
/******************************************************************************/
Point Point::operator + (const Vector &v)
{
	Point temp;
	temp.x = x+v.x;
	temp.y = y+v.y;
	temp.z = z+v.z;
	return temp;
}

/******************************************************************************/
/*!
Constructor for Vector structure
*/
/******************************************************************************/
Vector::Vector()
{
}

/******************************************************************************/
/*!
Parameterized constructor for Point structure
*/
/******************************************************************************/
Vector::Vector(const float &arg_x,const float &arg_y,const float &arg_z)
{
	/// x,y,z components of the vector
	x = arg_x;
	y = arg_y;
	z = arg_z;
}
/******************************************************************************/
/*!
Normalize the vector
\return
void
*/
/******************************************************************************/
void Vector::Normalize()
{
	/// Evaluate the magnitude of the vector
	float mag = Magnitude();
	/// Divide the x,y,z components by magnitude to normalize
	x = x/mag;
	y = y/mag;
	z = z/mag;
}

/******************************************************************************/
/*!
Evaluate magnitude of the vector
\return
float
*/
/******************************************************************************/
float Vector::Magnitude() const
{
	return(sqrt((x*x)+(y*y)+(z*z)));
}

/******************************************************************************/
/*!
Overloaded Assignment Operator implementation for Vector*float
\param [in] num
Reference to a constant float
\return
Vector
*/
/******************************************************************************/
Vector Vector::operator * (const float &num)
{
	Vector temp;
	temp.x = x*num;
	temp.y = y*num;
	temp.z = z*num;
	return temp;
}

/******************************************************************************/
/*!
Overloaded Assignment Operator implementation for Vector+Vector
\param [in] v
Reference to a constant Vector
\return
Vector
*/
/******************************************************************************/
Vector Vector::operator + (const Vector &v)
{
	Vector temp;
	temp.x = x+v.x;
	temp.y = y+v.y;
	temp.z = z+v.z;
	return temp;
}

/******************************************************************************/
/*!
Evaluate the cross product of 2 vectors
\param [in] v1
Reference to a constant Vector
\param [in] v2
Reference to a constant Vector
\param [out] v3
Reference to a Vector which is the cross product of v1 and v2
\return
void
*/
/******************************************************************************/
void crossProduct(const Vector &v1,const Vector &v2,Vector &v3)
{
	v3.x = ((v1.y*v2.z) - (v1.z*v2.y));
	v3.y = ((v1.z*v2.x) - (v1.x*v2.z));
	v3.z = ((v1.x*v2.y) - (v1.y*v2.x));
}

/******************************************************************************/
/*!
Evaluate the dot product
\param v1
Reference to a constant vector
\param v2
Reference to a constant vector
\return
float
*/
/******************************************************************************/
float dotProduct(const Vector &v1,const Vector &v2)
{
	return((v1.x*v2.x)+(v1.y*v2.y)+(v1.z*v2.z));
}

/******************************************************************************/
/*!
Evaluate the angle between 2 vectors
\param v1
Reference to a constant vector
\param v2
Reference to a constant vector
\return
float - The angle between 2 vectors expressed in degrees
*/
/******************************************************************************/
float angleBetVectors(const Vector &v1,const Vector &v2)
{
	return((acos(dotProduct(v1,v2)/(v1.Magnitude()*v2.Magnitude())))*180.0f/M_PI);
}

/******************************************************************************/
/*!
Finds distance between two Points
\param p1
Reference to a constant point
\param p2
Reference to a constant point
\return
float - The distance between two points
*/
/******************************************************************************/
float distanceBetweenTwoPoints(const Point &p1,const Point &p2)
{
	return(sqrt(((p2.x-p1.x)*(p2.x-p1.x))+((p2.y-p1.y)*(p2.y-p1.y))+((p2.z-p1.z)*(p2.z-p1.z))));
}
