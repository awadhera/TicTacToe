/******************************************************************************/
/*!
\file Character.cpp
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


#include "Character.h"

Character::Character()
{
	speed = 0.0f;
	motionPlanning_PointRobot = NULL;
}

Character::~Character()
{
}

void Character::Set_MotionPlanning_PointRobot(MotionPlanning_PointRobot * mp)
{
	motionPlanning_PointRobot = mp;
}

void Character::SetSpeed(float sp)
{
	speed = sp;
	goCharacter.dynamic = true;
	goCharacter.speed = speed;
	vSet = false;
	stateRadius = ((speed/20.0f) );
}

int Character::GetNearestTrapezoidState(Trapezoid * trapezoid)
{
	return trapezoid->stateIndex;
}

bool Character::checkWithinSolidArea(const Point2D &pos)
{
	std::list<Trapezoid> unusedTrapezoidalList = motionPlanning_PointRobot->retrieveUnusedTrapezoidalList();
	std::list<Trapezoid>::iterator it;
	for(it = unusedTrapezoidalList.begin();it != unusedTrapezoidalList.end();it++)
	{
		if(checkWithinTrapezoid(&(*it),pos))
			return true;
	}
	return false;
}

bool Character::checkWithinTrapezoid(Trapezoid *trapezoid,const Point2D &pos)
{
	if((pos.x >= trapezoid->leftPoint.x && pos.x <= trapezoid->rightPoint.x))
	{
		if((Classify_Point_To_LineSegment(pos,trapezoid->top) == (CL_UNDER) || Classify_Point_To_LineSegment(pos,trapezoid->top) == (CL_ON))
			&& (Classify_Point_To_LineSegment(pos,trapezoid->bottom) == (CL_ABOVE) || Classify_Point_To_LineSegment(pos,trapezoid->bottom) == (CL_ON)))
			return true;
		else
			return false;
	}
	else
		return false;
}

bool Character::setStartPosition(const Point2D &pos)
{
	if(!checkWithinSolidArea(pos))
	{
		startPosition = pos;
		return true;
	}
	return false;
}

bool Character::setEndPosition(const Point2D &pos)
{
	if(!checkWithinSolidArea(pos))
	{
		endPosition = pos;
		return true;
	}
	return false;
}

void Character::findTrapezoidAndStartState()
{
	std::list<Trapezoid> trapezoidalList = motionPlanning_PointRobot->retrieveTrapezoidalList();
	std::list<Trapezoid>::iterator it;
	for(it = trapezoidalList.begin();it != trapezoidalList.end();it++)
	{
		if(checkWithinTrapezoid(&(*it),startPosition))
		{
			startStateIndex = GetNearestTrapezoidState(&(*it));
			startMiddleStateIndex = startStateIndex;
			Point2D startPoint = startPosition;
			Point2D	endPoint(motionPlanning_PointRobot->path->states[startStateIndex].worldPositionX,motionPlanning_PointRobot->path->states[startStateIndex].worldPositionY);
			float shortestDistance = motionPlanning_PointRobot->distanceBetweenTwoPoints(startPoint,endPoint);
			for(unsigned int i = 0;i < motionPlanning_PointRobot->path->states[startMiddleStateIndex].edges.size();i++)
			{
				Point2D endPoint(motionPlanning_PointRobot->path->states[motionPlanning_PointRobot->path->states[startMiddleStateIndex].edges[i].to].worldPositionX,
					motionPlanning_PointRobot->path->states[motionPlanning_PointRobot->path->states[startMiddleStateIndex].edges[i].to].worldPositionY);
				float distance = motionPlanning_PointRobot->distanceBetweenTwoPoints(startPoint,endPoint);
				if(distance < shortestDistance)
				{
					shortestDistance = distance;
					startStateIndex = motionPlanning_PointRobot->path->states[startMiddleStateIndex].edges[i].to;
				}
			}
		}
	}
}

void Character::findTrapezoidAndEndState()
{
	std::list<Trapezoid> trapezoidalList = motionPlanning_PointRobot->retrieveTrapezoidalList();
	std::list<Trapezoid>::iterator it;
	for(it = trapezoidalList.begin();it != trapezoidalList.end();it++)
	{
		if(checkWithinTrapezoid(&(*it),endPosition))
		{
			endStateIndex = GetNearestTrapezoidState(&(*it));
		}
	}
}


void Character::followShortestPath()
{
	shortestPath.clear();
	shortestPath.push_back(startStateIndex);
	shortestPath.push_back(endStateIndex);
	bool found = false;
	std::vector<int>::iterator i,j;
	while(!found)
	{
		found = true;
		i = shortestPath.begin();
		while(i!=(shortestPath.end()-1))
		{
			j = i+1;
			if(motionPlanning_PointRobot->path->PATH_MATRIX[*i][*j].first==SHORTERPATH)
			{
				i = shortestPath.insert(j,motionPlanning_PointRobot->path->PATH_MATRIX[*i][*j].second);
				i++;
				found = false;
			}
			else 
				i++;
		}
	}
}

bool Character::checkIfStartEndSameTrapezoid()
{
	if(startMiddleStateIndex == endStateIndex)
		return true;
	return false;
}

void Character::InitializePhysics(const ModelSpace &mSpace)
{
	goCharacter.p.x = 0.0f;
	goCharacter.p.y = 0.0f;
	goCharacter.p.z = 0.0f;
	goCharacter.radius = 5.0f;
	float scaleFactor = (goCharacter.radius*2.0f)/(mSpace.limit_r - mSpace.limit_l);
	/// Upload the scale and translate transforms for the objects
	goCharacter.scale.upload(scaleFactor,scaleFactor,scaleFactor);
	goCharacter.translate.upload(goCharacter.p.x,goCharacter.p.y,goCharacter.p.z);
	goCharacter.geoType->local.add(&goCharacter.translate);
	goCharacter.geoType->local.add(&goCharacter.scale);
}

void Character::move()
{
	if(!vSet && startMiddleStateIndex == endStateIndex)
	{
		goCharacter.p.x = startPosition.x;
		goCharacter.p.y = motionPlanning_PointRobot->boundingRect.bottom.start.y - startPosition.y;
		goCharacter.p.z = 0.0f;
		goCharacter.v.x = endPosition.x - startPosition.x;
		goCharacter.v.y = motionPlanning_PointRobot->boundingRect.bottom.start.y - endPosition.y - motionPlanning_PointRobot->boundingRect.bottom.start.y + startPosition.y;
		goCharacter.v.z = 0.0f;
		goCharacter.v.Normalize();
		vSet = true;
	}
	else if(startMiddleStateIndex != endStateIndex)
	{
		if(!vSet && !endReached)
		{
			parsePath = 0;
			interStart = startPosition;
			interEnd =  Point2D(motionPlanning_PointRobot->path->states[shortestPath[parsePath]].worldPositionX,motionPlanning_PointRobot->path->states[shortestPath[parsePath]].worldPositionY);
			goCharacter.p.x = interStart.x;
			goCharacter.p.y =  motionPlanning_PointRobot->boundingRect.bottom.start.y - interStart.y;
			goCharacter.p.z = 0.0f;
			goCharacter.v.x = interEnd.x - interStart.x;
			goCharacter.v.y = motionPlanning_PointRobot->boundingRect.bottom.start.y - interEnd.y - motionPlanning_PointRobot->boundingRect.bottom.start.y + interStart.y;
			goCharacter.v.z = 0.0f;
			goCharacter.v.Normalize();
			parsePath++;
			vSet = true;
			interEndReached = false;
		}
		else if(vSet && interEndReached)
		{
			interStart = interEnd;
			if(unsigned(parsePath) != shortestPath.size()-1)
			{
				interEnd = Point2D(motionPlanning_PointRobot->path->states[shortestPath[parsePath]].worldPositionX,motionPlanning_PointRobot->path->states[shortestPath[parsePath]].worldPositionY);
			}
			else
				interEnd = endPosition;
			goCharacter.p.x = interStart.x;
			goCharacter.p.y =  motionPlanning_PointRobot->boundingRect.bottom.start.y - interStart.y;
			goCharacter.p.z = 0.0f;
			goCharacter.v.x = interEnd.x - interStart.x;
			goCharacter.v.y = motionPlanning_PointRobot->boundingRect.bottom.start.y - interEnd.y - motionPlanning_PointRobot->boundingRect.bottom.start.y + interStart.y;
			goCharacter.v.z = 0.0f;
			goCharacter.v.Normalize();
			parsePath++;
			interEndReached = false;
		}
	}
	updatePositionwrtVelocity(goCharacter,dt);
	if(startMiddleStateIndex == endStateIndex)
	{
	//	float distance2 = motionPlanning_PointRobot->distanceBetweenTwoPoints(Point2D(startPosition.x,motionPlanning_PointRobot->boundingRect.bottom.start.y - startPosition.y),Point2D(endPosition.x,motionPlanning_PointRobot->boundingRect.bottom.start.y - endPosition.y));
		float distance2 = motionPlanning_PointRobot->distanceBetweenTwoPoints(Point2D(endPosition.x,motionPlanning_PointRobot->boundingRect.bottom.start.y - endPosition.y),Point2D(goCharacter.p.x,goCharacter.p.y));
		//if(distance1>distance2)
		if(distance2<stateRadius)
		{
			endReached = true;
			vSet = false;
		}
	}
	else if(startMiddleStateIndex != endStateIndex)
	{
	//	float distance2 = motionPlanning_PointRobot->distanceBetweenTwoPoints(Point2D(interStart.x,motionPlanning_PointRobot->boundingRect.bottom.start.y - interStart.y),Point2D(interEnd.x,motionPlanning_PointRobot->boundingRect.bottom.start.y - interEnd.y));
		float distance2 = motionPlanning_PointRobot->distanceBetweenTwoPoints(Point2D(interEnd.x,motionPlanning_PointRobot->boundingRect.bottom.start.y - interEnd.y),Point2D(goCharacter.p.x,goCharacter.p.y));
		//if(distance1>distance2)
		if(distance2<stateRadius)
		{
			interEndReached = true;
		}
		if(interEndReached && unsigned(parsePath)==shortestPath.size())
		{
			vSet = false;
			endReached = true;
		}
	}

}