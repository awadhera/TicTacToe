/******************************************************************************/
/*!
\file MathUtility.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #02
\date 09/03/2012
\brief
Implementation of Wrapper of game objects for this project
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/
#include "MathUtility.h"

bool PointOnLineSegment(Point2D & pnt,LineSegment2D & lns)
{
	float res1 = lns.end.y - lns.start.y;
	float res2 = lns.end.x - lns.start.x;
	float res3 = pnt.x - lns.start.x;
	float res4 = pnt.y - lns.start.y;
	float t;
	if(res3*res1 == res4*res2)//the point is on the line of the segment
	{		
		if(res1 != 0.0f)
			t = res4/res1;
		else //if(res2 != 0)
			t = res3/res2;
		
		if((t >= 0.0f) && (t <= 1.0f))
			return true;
	}
	return false;
}

CLASSIFY Classify_Point_To_LineSegment(Point2D pnt,LineSegment2D lns)
{
    //return Vector3D(Y*v.Z - Z*v.Y,Z*v.X - X*v.Z,X*v.Y - Y*v.X);
	Vector3D v1(pnt.x-lns.start.x,pnt.y-lns.start.y,0);
	Vector3D v2(lns.end.x-lns.start.x,lns.end.y-lns.start.y,0);
	Vector3D cross;
	cross.z = v1.x*v2.y - v1.y*v2.x;
	if(cross.z < 0.0f)
        return CL_UNDER;
	else if(cross.z > 0.0f)
        return CL_ABOVE;
	return CL_ON;
}