/******************************************************************************/
/*!
\file Graph.cpp
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

#include "Graph.h"

//Edge class
Edge::Edge()
{
}

Edge::~Edge()
{
}

Edge::Edge(const Edge & ed)
{
	from = ed.from;
	to = ed.to;		
}

Edge & Edge::operator=(const Edge & ed)
{
	if(this != &ed)
	{
		from = ed.from;
		to = ed.to;
	}
	return *this;
}

//State class
State::State()
{
}

State::~State()
{
}

State::State(const State & st)
{
	worldPositionX = st.worldPositionX;
	worldPositionY = st.worldPositionY;
	
	edges = st.edges;		
}

State & State::operator=(const State & st)
{
	if(this != &st)
	{
		worldPositionX = st.worldPositionX;
		worldPositionY = st.worldPositionY;
	
		edges = st.edges;		
	}
	return *this;
}

//Path class
Path::Path()
{
}

Path::~Path()
{
}

Path & Path::operator=(Path & pa1)
{
	if(this != &pa1)
	{
	}
	return *this;
}

void Path::FloydAlgorithm()
{
	for(unsigned int b = 0;b < states.size(); ++b)
	{
		for(unsigned int a = 0;a < states.size();++a)
		{
			for(unsigned int c = 0;c < states.size();++c)
			{
				if(DM_MATRIX[a][b].first != VIRTUALMAX && DM_MATRIX[b][c].first != VIRTUALMAX)
				{
					if(DM_MATRIX[a][c].second > (DM_MATRIX[a][b].second + DM_MATRIX[b][c].second))
					{
						DM_MATRIX[a][c].second = DM_MATRIX[a][b].second + DM_MATRIX[b][c].second;
						DM_MATRIX[a][c].first = DEFINED;
						PATH_MATRIX[a][c].second = b;
						PATH_MATRIX[a][c].first = SHORTERPATH;
					}
				}
			}
		}
	}
}
