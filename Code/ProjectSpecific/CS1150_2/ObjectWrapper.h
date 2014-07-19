/******************************************************************************/
/*!
\file ObjectWrapper.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #02
\date 09/03/2012
\brief
Interface for Wrapper of geometric objects and their transforms for this project
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef OBJECTWRAPPER_H
#define OBJECTWRAPPER_H

#include "../../Geometry/ObjectLine.h"
#include "../../Geometry/ObjectSphere.h"
#include "../../Camera/Camera.h"
#include "../../GameObject/GameObject.h"
#include "../../AI/AIUtilities.h"
#include "Character.h"
#include "LevelBuild.h"
#include <sstream>

class Character;
//!  Wrapper class
/*!
Class for implmenting wrapper of geomtric objects and transforms
*/
class Wrapper
{
	static Wrapper *s_pInstance;	/*! Pointer to a Wrapper object which is static*/
	int count;	/*! Variable to keep track of total number of geometric objects*/
	Wrapper();
	~Wrapper();
	Wrapper(Wrapper &);
	Wrapper & operator = (Wrapper &);
public:
	MotionPlanning_PointRobot motionPlanning;
	Line *geoLines;
	GameObject *goLines;
	Line *geoPathLines;
	GameObject *goPathLines;
	Sphere *geoStates;
	GameObject *goStates;
	unsigned int totalEdges;
	Camera camera;	/*!< Camera object of the project */
	ModelSpace mSpace;		/*!< ModelSpace of the geometries */
	Character character;
	Sphere charPosition[4];
	int mouseMoveFix;
	GameObject *charPos;
	long long diffcount;
	long long diffcount1;
	bool dragFlag;
	float olddt;
	bool startSet;
	bool endSet;
	bool gameInAIState;
	GameObject *bd;
	Line *bdLines;
	static void create();
	static void destroy();
	static Wrapper &Instance();
	void createLevelPath();
	void allocateGeometriesAndCreateGameObjects();
	void createGeometries();
	void uploadGeometries();
	void InitializePhysics();
	void destroyGeometries();
};

#endif