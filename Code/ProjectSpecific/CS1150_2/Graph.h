/******************************************************************************/
/*!
\file Graph.h
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


#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>

enum DM_TYPE {VIRTUALMAX,DEFINED};
enum PATH_TYPE {ADJACENT,NOPATH,SHORTERPATH};

//Edge
class Edge
{
public:
	Edge();
	~Edge();
	Edge(const Edge & ed);
	Edge & operator=(const Edge & ed);
	
	int from;
	int to;
};

//State
class State
{
public:
	State();
	~State();
	State(const State & st);
	State & operator=(const State & st);
	
	float worldPositionX;
	float worldPositionY;

	std::vector<Edge> edges;
};

#define MAX_DISTANCE	1000000

//Path
class Path
{
public:
	Path();
	~Path();
	Path & operator=(Path & pa1);
	std::vector<State> states;
	int currentState;
	std::vector<std::vector<std::pair<DM_TYPE,float>>> DM_MATRIX;
	std::vector<std::vector<std::pair<PATH_TYPE,int>>> PATH_MATRIX;
	void FloydAlgorithm();
	//to add Floyd's path algorithm here
};

#endif