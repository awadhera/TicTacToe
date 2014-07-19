/******************************************************************************/
/*!
\file MotionPlanning.h
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

#ifndef _MOTIONPLANNING_H_
#define _MOTIONPLANNING_H_

#include <list>
#include <vector>
#include "MathUtility.h"
#include "Graph.h"
#include<iostream>

#define BOUNDING_RECT_MARGIN	25

class AG_State;

class Trapezoid
{
public:
	Trapezoid();
	~Trapezoid();
	Trapezoid(const Trapezoid & tra);
	Trapezoid & operator=(const Trapezoid & tra);
	bool operator==(Trapezoid tra);
	
	LineSegment2D top;
	LineSegment2D bottom;
	Point2D leftPoint;
	Point2D rightPoint;
	Trapezoid * neighbours[4];
	//NOTES
	//neighbours[0] = upper left neighbour
	//neighbours[1] = lower left neighbour
	//neighbours[2] = upper right neighbour
	//neighbours[3] = lower right neighbour

	bool neighbourUsed[4];//for the graph (build)
	int stateIndex;//for the graph (navigation) - the state index of the state created in the middle of the trapezoid
	
	//Optional - to add any helper variables
	AG_State *treeNode;
	bool isVisited;
};



//Directed acyclic graph for point trapezoidal location problem
enum AG_TYPE {AG_XNODE,AG_YNODE,AG_LEAF};



//Directed acyclic graph State class
class AG_State
{
public:
	AG_State();
	~AG_State();
	AG_State(const AG_State & st);
	AG_State & operator=(const AG_State & st);	
	
	AG_State *left;
	AG_State *right;

	AG_TYPE type;

	Point2D point;
	LineSegment2D lineSegment;
	Trapezoid * trapezoid;//the trapezoid pointing into trapezoidalList

	std::vector<AG_State*> parents;
};



class MotionPlanning_PointRobot
{	
	std::list<Trapezoid> trapezoidalList;
	std::list<Trapezoid> unusedTrapezoidalList;//this is used only to display the unused trapezoids inside solid areas

	//To add any new private member data and member function if needed here
	
public:
	MotionPlanning_PointRobot();
	~MotionPlanning_PointRobot();

	Path * path;
	std::vector<LineSegment2D> segmentList;
	Trapezoid boundingRect;
	//searchStructure = point location data structure = search structure = 
	//directed acyclic graph with a single root and exactly one leaf for every trapezoid of the trapezoidal map.
	AG_State searchStructure;

	void Calculate_LineSegments_Normals();//this function computes the normal vectors of the line segments
	void Adjust_StartEnd_PointsSegment();//Adjust the start and end points of the line segments so that the start is always on the left.
	void TrapezoidalMap();//compute the trapezoids that form the free space and link them together
	void AdjustRemainingTrapezoids();//IS IMPLEMENTED - It adjusts the top and bottom line segments of every trapezoid
	void RemoveUnusedTrapezoids();//Remove the unused (solid areas) trapezoids
	void PathBuilding();// generates a graph with states and edges

	void StatesBuild(Trapezoid * next,int previousState);//to be used inside PathBuilding() fct - It is a recursive function that builds the states and edges of the graph
	Trapezoid * TrapezoidSearch(AG_State * searchS,Point2D pointS,Point2D pointE);//IS IMPLEMENTED
	Trapezoid Calculate_BoundingRectangle();//IS IMPLEMENTED
	
	//To add any new public member data and member function if needed here
	std::vector<Trapezoid *> trapezoidIntersections;
	unsigned int tcount;
	float distanceBetweenTwoPoints(const Point2D &p1,const Point2D &p2);
	unsigned int sizeOftrapezoidalList();
	unsigned int sizeOfunusedTrapezoidalList();
	std::list<Trapezoid> retrieveTrapezoidalList();
	std::list<Trapezoid> retrieveUnusedTrapezoidalList();
	bool draw[7];
};


#endif