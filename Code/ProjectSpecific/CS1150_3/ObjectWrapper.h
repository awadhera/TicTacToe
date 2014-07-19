/******************************************************************************/
/*!
\file ObjectWrapper.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #03
\date 22/03/2012
\brief
Interface for Wrapper of geometric objects and their transforms for this project
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef OBJECTWRAPPER_H
#define OBJECTWRAPPER_H

#include "../../Geometry/ObjectLine.h"
#include "../../Geometry/ObjectGrid.h"
#include "../../Camera/Camera.h"
#include "../../GameObject/GameObject.h"
#include "../../AI/AIUtilities.h"
#include "MiniMaxSearch.h"
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
	Grid GeoX[9],GeoO[9];
	Line GeoLines[4];
	GameObject X[9],O[9];
	GameObject Lines[4];
	Translate t[9];
	int Place[3][3];
	TicTacToe game;
	enum TicTacToeCellValue{o,x,EMPTY};
	int gameInAIMode;
	long playFirst;
	int depth;
	Camera camera;			/*!< Camera object of the project */
	ModelSpace mSpace;		/*!< ModelSpace of the geometries */
	static void create();
	static void destroy();
	static Wrapper &Instance();
	void createGeometries();
	void uploadGeometries();
	void destroyGeometries();
	void createObjects();
	void InitObjects();
	void reinitBoard();
	int processInputClick(const float posX,const float posY);
};

#endif