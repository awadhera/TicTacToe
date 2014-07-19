/******************************************************************************/
/*!
\file LevelBuild.h
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

#ifndef _LEVELBUILD_H_
#define _LEVELBUILD_H_

#include "MotionPlanning.h"
#include "ObjectWrapper.h"
#include <sstream>	

void InitStaticScene(MotionPlanning_PointRobot & mp);
void CreateLevelPath_MotionPlanning(MotionPlanning_PointRobot & mp);
void DrawStaticScene(MotionPlanning_PointRobot & mp,MatrixStack &mv_s);

#endif