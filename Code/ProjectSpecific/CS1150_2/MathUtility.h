/******************************************************************************/
/*!
\file MathUtlity.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 09/03/2012
\brief
Implementation of Wrapper of game objects for this project
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/


#ifndef _MATHUTILITY_H_
#define _MATHUTILITY_H_

struct Point2D
{
	Point2D() {};
	Point2D(float _x,float _y) {x=_x;y=_y;};
	Point2D & operator=(Point2D pnt) {x = pnt.x; y = pnt.y; return *this;};
	bool operator==(Point2D pnt) {return (x == pnt.x) && (y == pnt.y);};
	float x;
	float y;
};

struct Vector2D
{
	Vector2D() {x=0;y=0;};
	Vector2D(float _x,float _y) {x=_x;y=_y;};
	Vector2D & operator=(Vector2D vec) {x=vec.x;y=vec.y;return *this;};
	bool operator==(Vector2D vec) {return (x==vec.x) && (y==vec.y);};
	float x;
	float y;
};

struct LineSegment2D
{
	LineSegment2D() {};
	LineSegment2D(Point2D s,Point2D e) {start = s;end = e;};
	LineSegment2D & operator=(LineSegment2D lns) {start = lns.start;end = lns.end;normal = lns.normal;return *this;};
	bool operator==(LineSegment2D lns) {return (start == lns.start) && (end == lns.end);};
	Point2D start;
	Point2D end;
	
	Vector2D normal;
};

struct Vector3D
{
	Vector3D() {x=0;y=0;z=0;};
	Vector3D(float _x,float _y,float _z) {x=_x;y=_y;z=_z;};
	Vector3D & operator=(Vector3D vec) {x=vec.x;y=vec.y;z=vec.z;return *this;};
	bool operator==(Vector3D vec) {return (x==vec.x) && (y==vec.y) && (z==vec.z);};
	float x;
	float y;
	float z;
};

enum CLASSIFY {CL_ABOVE,CL_UNDER,CL_ON};

bool PointOnLineSegment(Point2D & pnt,LineSegment2D & lns);
CLASSIFY Classify_Point_To_LineSegment(Point2D pnt,LineSegment2D lns);

#endif