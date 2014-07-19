/******************************************************************************/
/*!
\file LevelBuild.cpp
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


#include "LevelBuild.h"

typedef std::stringstream StringStream_t;

#define LINESEGMENT_NUMBER1	26

void InitStaticScene(MotionPlanning_PointRobot & mp)
{
	LineSegment2D segmentList[LINESEGMENT_NUMBER1];

	//Set of non-crossing line segments
	//set1
	segmentList[0].start.x = 70;
	segmentList[0].start.y = 210;
	segmentList[0].end.x = 120;
	segmentList[0].end.y = 80;

	segmentList[1].start.x = 120;
	segmentList[1].start.y = 80;
	segmentList[1].end.x = 180;
	segmentList[1].end.y = 80;

	segmentList[2].start.x = 180;
	segmentList[2].start.y = 80;
	segmentList[2].end.x = 380;
	segmentList[2].end.y = 130;

	segmentList[3].start.x = 380;
	segmentList[3].start.y = 130;
	segmentList[3].end.x = 360;
	segmentList[3].end.y = 175;

	segmentList[4].start.x = 360;
	segmentList[4].start.y = 175;
	segmentList[4].end.x = 160;
	segmentList[4].end.y = 175;

	segmentList[5].start.x = 160;
	segmentList[5].start.y = 175;
	segmentList[5].end.x = 70;
	segmentList[5].end.y = 210;

	//set2
	segmentList[6].start.x = 335;
	segmentList[6].start.y = 275;
	segmentList[6].end.x = 345;
	segmentList[6].end.y = 601;

	segmentList[7].start.x = 345;
	segmentList[7].start.y = 601;
	segmentList[7].end.x = 455;
	segmentList[7].end.y = 425;

	segmentList[8].start.x = 455;
	segmentList[8].start.y = 425;
	segmentList[8].end.x = 475;
	segmentList[8].end.y = 605;

	segmentList[9].start.x = 475;
	segmentList[9].start.y = 605;
	segmentList[9].end.x = 395;
	segmentList[9].end.y = 705;

	segmentList[10].start.x = 395;
	segmentList[10].start.y = 705;
	segmentList[10].end.x = 315;
	segmentList[10].end.y = 595;

	segmentList[11].start.x = 315;
	segmentList[11].start.y = 595;
	segmentList[11].end.x = 95;
	segmentList[11].end.y = 700;

	segmentList[12].start.x = 95;
	segmentList[12].start.y = 700;
	segmentList[12].end.x = 255;
	segmentList[12].end.y = 525;

	segmentList[13].start.x = 255;
	segmentList[13].start.y = 525;
	segmentList[13].end.x = 335;
	segmentList[13].end.y = 275;

	//set3
	segmentList[14].start.x = 785;
	segmentList[14].start.y = 75;
	segmentList[14].end.x = 865;
	segmentList[14].end.y = 45;

	segmentList[15].start.x = 865;
	segmentList[15].start.y = 45;
	segmentList[15].end.x = 930;
	segmentList[15].end.y = 100;

	segmentList[16].start.x = 930;
	segmentList[16].start.y = 100;
	segmentList[16].end.x = 860;
	segmentList[16].end.y = 100;

	segmentList[17].start.x = 860;
	segmentList[17].start.y = 100;
	segmentList[17].end.x = 920;
	segmentList[17].end.y = 120;

	segmentList[18].start.x = 920;
	segmentList[18].start.y = 120;
	segmentList[18].end.x = 841;
	segmentList[18].end.y = 190;

	segmentList[19].start.x = 841;
	segmentList[19].start.y = 190;
	segmentList[19].end.x = 960;
	segmentList[19].end.y = 250;

	segmentList[20].start.x = 960;
	segmentList[20].start.y = 250;
	segmentList[20].end.x = 820;
	segmentList[20].end.y = 250;

	segmentList[21].start.x = 820;
	segmentList[21].start.y = 250;
	segmentList[21].end.x = 885;
	segmentList[21].end.y = 650;

	segmentList[22].start.x = 885;
	segmentList[22].start.y = 650;
	segmentList[22].end.x = 620;
	segmentList[22].end.y = 660;

	segmentList[23].start.x = 620;
	segmentList[23].start.y = 660;
	segmentList[23].end.x = 740;
	segmentList[23].end.y = 250;

	segmentList[24].start.x = 740;
	segmentList[24].start.y = 250;
	segmentList[24].end.x = 870;
	segmentList[24].end.y = 130;

	segmentList[25].start.x = 870;
	segmentList[25].start.y = 130;
	segmentList[25].end.x = 785;
	segmentList[25].end.y = 75;

	for(int i = 0; i < LINESEGMENT_NUMBER1; i++)
		mp.segmentList.push_back(segmentList[i]);
}

void CreateLevelPath_MotionPlanning(MotionPlanning_PointRobot & mp)
{
	InitStaticScene(mp);

	//Compute and Save the normals of the segments in a normal-segment data for later use
	mp.Calculate_LineSegments_Normals();

	//Adjust start and end points of all initial segments so that the start is always on the left.
	mp.Adjust_StartEnd_PointsSegment();

	//Compute Free Space
	//Divide the space to trapezoids linked together
	mp.TrapezoidalMap();

	//Line-01
	//Adjust top - bottom line segments
	mp.AdjustRemainingTrapezoids();

	//Line-02
	//Notes Line-02 must be before Line-01. It is after only so we can display correctly the unused trapezoids
	//Remove the unused (solid location) trapezoids
	mp.RemoveUnusedTrapezoids();

	//Compute Path
	//state generation and edge generation
	mp.PathBuilding();	 
}

void DrawStaticScene(MotionPlanning_PointRobot & mp,MatrixStack &mv_s)
{
	StringStream_t ss;
	Wrapper &wrapper = Wrapper::Instance();
	if(mp.draw[0])
	{
		for(int i = 0;i < 4;++i)
		{
			wrapper.bdLines[i].drawTransformedGeometry(mv_s);
		}
	}
	if(mp.draw[1])
	{
		for(unsigned int i = 0;i < wrapper.motionPlanning.segmentList.size();++i)
		{
			wrapper.geoLines[i].drawTransformedGeometry(mv_s);
		}
	}
	if(mp.draw[2])
	{
		for(unsigned int i = wrapper.motionPlanning.segmentList.size();i < wrapper.motionPlanning.segmentList.size()+(wrapper.motionPlanning.sizeOftrapezoidalList()*4);++i)
		{
			wrapper.geoLines[i].drawTransformedGeometry(mv_s);
		}
	}
	if(mp.draw[3])
	{
		for(unsigned int i = wrapper.motionPlanning.segmentList.size()+(wrapper.motionPlanning.sizeOftrapezoidalList()*4);i < wrapper.motionPlanning.segmentList.size()+(wrapper.motionPlanning.sizeOftrapezoidalList()*4)+(wrapper.motionPlanning.sizeOfunusedTrapezoidalList()*4);++i)
		{
			wrapper.geoLines[i].drawTransformedGeometry(mv_s);
		}
	}
	if(mp.draw[5])
	{
		for(unsigned int i = 0;i < wrapper.totalEdges;++i)
		{
			wrapper.geoPathLines[i].drawTransformedGeometry(mv_s);
		}
	}
	if(mp.draw[4])
	{
		for(unsigned int i = 0;i < wrapper.motionPlanning.path->states.size();++i)
		{
			wrapper.geoStates[i].drawTransformedGeometry(mv_s);
		}
	}
	if(mp.draw[6])
	{
		for(unsigned int i = 0;i < wrapper.motionPlanning.path->states.size();i++)
		{
			ss.str("");
			ss<<i;
			std::string displayStr = ss.str();
			glLoadIdentity();
			glTranslatef(wrapper.motionPlanning.path->states[i].worldPositionX,wrapper.motionPlanning.boundingRect.bottom.start.y - wrapper.motionPlanning.path->states[i].worldPositionY,0.0f);
			glRasterPos2f(-(wrapper.motionPlanning.boundingRect.leftPoint.x+wrapper.motionPlanning.boundingRect.rightPoint.x)/2.0f,-(wrapper.motionPlanning.boundingRect.top.start.y+wrapper.motionPlanning.boundingRect.bottom.start.y)/2.0f);
			glColor3f(0.6f,0.6f,0.6f);
			glListBase (1000);
			const char *strToChar = displayStr.c_str();
			glCallLists(displayStr.length(), GL_UNSIGNED_BYTE, LPCTSTR(strToChar));
		}
	}
}