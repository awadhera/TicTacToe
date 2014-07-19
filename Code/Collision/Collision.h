/******************************************************************************/
/*!
\file Collision.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1250
\par Project #01
\date 02/08/2012
\brief
Interface for collision detection and response
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef COLLISION_H
#define COLLISION_H

#include "../GameObject/GameObject.h"

bool collisionBallStraightWall(GameObject &g1,GameObject &g2,const float &Ts,const float &dt);
bool collisionBallCircularWall(GameObject &g1,GameObject &g2,const float &Ts,const float &dt);
bool collisionBallBall(GameObject &g1,GameObject &g2,const float &Ts,const float &dt);

#endif
