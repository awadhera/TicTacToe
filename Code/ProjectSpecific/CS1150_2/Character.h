/******************************************************************************/
/*!
\file Character.h
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

#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "MotionPlanning.h"
#include "../../Geometry/ObjectUtility.h"
#include "../../GameObject/GameObject.h"

class Character
{
	float speed;	
	float stateRadius;//for collision
	
	MotionPlanning_PointRobot * motionPlanning_PointRobot;

	//locate the nearest trapezoid to where the character is plotted
	int GetNearestTrapezoidState(Trapezoid * trapezoid);
	
	//To add any new private member data and member function here
	
public:
	 
	Character();
	~Character();
	
	void Set_MotionPlanning_PointRobot(MotionPlanning_PointRobot * mp);
	void SetSpeed(float sp);
	
	//To add any new public member data and member function here

	Point2D startPosition;
	Point2D endPosition;
	Point2D interStart;
	Point2D interEnd;
	bool interEndReached;
	int parsePath;
	int startStateIndex;
	int endStateIndex;
	int startMiddleStateIndex;
	std::vector<int> shortestPath;
	GameObject goCharacter;
	bool vSet;
	bool endReached;
	float dt;
	bool setStartPosition(const Point2D &pos);
	bool setEndPosition(const Point2D &pos);
	bool checkWithinSolidArea(const Point2D &pos); 
	bool checkWithinTrapezoid(Trapezoid *trapezoid,const Point2D &pos);
	void findTrapezoidAndStartState();
	void findTrapezoidAndEndState();
	void followShortestPath();
	bool checkIfStartEndSameTrapezoid();
	void InitializePhysics(const ModelSpace &mSpace);
	void move();
};

#endif