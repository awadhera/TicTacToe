/******************************************************************************/
/*!
\file ObjectWrapper.cpp
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

#include "ObjectWrapper.h"
#include <string>

///Initialize static instance of Wrapper
Wrapper * Wrapper::s_pInstance = 0;
/******************************************************************************/
/*!
Constructor for Wrapper class
*/
/******************************************************************************/
Wrapper::Wrapper()
{
	count = 0;
}

/******************************************************************************/
/*!
Destructor for Wrapper class
*/
/******************************************************************************/
Wrapper::~Wrapper()
{
}


/******************************************************************************/
/*!
Create instance of Wrapper class
\return
void
*/
/******************************************************************************/
void Wrapper::create()
{
	s_pInstance = new Wrapper;
}

/******************************************************************************/
/*!
Destroy(deallocate) instance of Wrapper class
\return
void
*/
/******************************************************************************/
void Wrapper::destroy()
{
	delete s_pInstance;
}

/******************************************************************************/
/*!
Provide static instance of Wrapper class
\return
Reference to Wrapper instance
*/
/******************************************************************************/
Wrapper & Wrapper::Instance()
{
	return *s_pInstance;
}

/******************************************************************************/
/*!
Create the Vertices and Indices for all the geometric objects
Also create the transform objects
\return
void
*/
/******************************************************************************/
void Wrapper::createGeometries()
{
	unsigned int i;
	for(i = 0;i < motionPlanning.segmentList.size();++i)
	{
		geoLines[i].createGeometry(0,255,255);
	}
	for(i = motionPlanning.segmentList.size();i < motionPlanning.segmentList.size()+(motionPlanning.sizeOftrapezoidalList()*4);++i)
	{
		geoLines[i].createGeometry(255,255,0);
	}
	for(i = motionPlanning.segmentList.size()+(motionPlanning.sizeOftrapezoidalList()*4);i < motionPlanning.segmentList.size()+(motionPlanning.sizeOftrapezoidalList()*4)+(motionPlanning.sizeOfunusedTrapezoidalList()*4);++i)
	{
		geoLines[i].createGeometry(255,0,255);
	}
	for(i = 0;i < totalEdges;++i)
	{
		geoPathLines[i].createGeometry(255,255,255);
	}
	for(i = 0;i < motionPlanning.path->states.size();++i)
	{
		geoStates[i].createGeometry(255,0,0);
	}
	charPosition[0].createGeometry(255,255,255);
	charPosition[1].createGeometry(255,255,255);
	charPosition[2].createGeometry(255,255,255);
	charPosition[3].createGeometry(255,255,255);
	bdLines[0].createGeometry(0,0,255);
	bdLines[1].createGeometry(0,0,255);
	bdLines[2].createGeometry(0,0,255);
	bdLines[3].createGeometry(0,0,255);
}

/******************************************************************************/
/*!
Upload the geometries to the GPU
\return
void
*/
/******************************************************************************/
void Wrapper::uploadGeometries()
{
	int totalNumberOfLinesToDisplay = motionPlanning.segmentList.size()+(motionPlanning.sizeOftrapezoidalList()*4)+(motionPlanning.sizeOfunusedTrapezoidalList()*4);
	for(int i = 0;i < totalNumberOfLinesToDisplay;++i)
	{
		geoLines[i].uploadGeometry();
	}
	for(unsigned int i = 0;i < totalEdges;++i)
	{
		geoPathLines[i].uploadGeometry();
	}
	for(unsigned int i = 0;i < motionPlanning.path->states.size();++i)
	{
		geoStates[i].uploadGeometry();
	}
	charPosition[0].uploadGeometry();
	charPosition[1].uploadGeometry();
	charPosition[2].uploadGeometry();
	charPosition[3].uploadGeometry();
	bdLines[0].uploadGeometry();
	bdLines[1].uploadGeometry();
	bdLines[2].uploadGeometry();
	bdLines[3].uploadGeometry();
}

/******************************************************************************/
/*!
Destroy the geometries - Unbind the buffers, delete the vertices and indices info
\return
void
*/
/******************************************************************************/
void Wrapper::destroyGeometries()
{
	int totalNumberOfLinesToDisplay = motionPlanning.segmentList.size()+(motionPlanning.sizeOftrapezoidalList()*4)+(motionPlanning.sizeOfunusedTrapezoidalList()*4);
	for(int i = 0;i < totalNumberOfLinesToDisplay;++i)
	{
		geoLines[i].destroyGeometry();
	}
	for(unsigned int i = 0;i < totalEdges;++i)
	{
		geoPathLines[i].destroyGeometry();
	}
	for(unsigned int i = 0;i < motionPlanning.path->states.size();++i)
	{
		geoStates[i].destroyGeometry();
	}
	charPosition[0].destroyGeometry();
	charPosition[1].destroyGeometry();
	charPosition[2].destroyGeometry();
	charPosition[3].destroyGeometry();
	bdLines[0].destroyGeometry();
	bdLines[1].destroyGeometry();
	bdLines[2].destroyGeometry();
	bdLines[3].destroyGeometry();
}

void Wrapper::createLevelPath()
{
	CreateLevelPath_MotionPlanning(motionPlanning);
}

void Wrapper::allocateGeometriesAndCreateGameObjects()
{
	mSpace.limit_l = -0.5f;
	mSpace.limit_r = 0.5f;
	int totalNumberOfLinesToDisplay = motionPlanning.segmentList.size()+(motionPlanning.sizeOftrapezoidalList()*4)+(motionPlanning.sizeOfunusedTrapezoidalList()*4);
	geoLines = new Line[totalNumberOfLinesToDisplay];
	goLines = new GameObject[totalNumberOfLinesToDisplay];
	for(int i = 0; i < totalNumberOfLinesToDisplay; ++i)
	{
		goLines[i].setGeometryAndPhysicalType(&geoLines[i],line);
	}
	totalEdges = 0;
	for(unsigned int i = 0;i < motionPlanning.path->states.size(); ++i)
	{
		for(unsigned int j = 0;j < motionPlanning.path->states[i].edges.size();++j)
		{
			totalEdges++;
		}
	}
	geoPathLines = new Line[totalEdges];
	goPathLines = new GameObject[totalEdges];
	for(unsigned int i = 0; i < totalEdges; ++i)
	{
		goPathLines[i].setGeometryAndPhysicalType(&geoPathLines[i],line);
	}
	geoStates = new Sphere[motionPlanning.path->states.size()];
	goStates = new GameObject[motionPlanning.path->states.size()];
	for(unsigned int i = 0; i < motionPlanning.path->states.size(); ++i)
	{
		goStates[i].setGeometryAndPhysicalType(&geoStates[i],circle);
	}
	character.goCharacter.setGeometryAndPhysicalType(&charPosition[0],circle);
	charPos = new GameObject[3];
	charPos[0].setGeometryAndPhysicalType(&charPosition[1],circle);
	charPos[1].setGeometryAndPhysicalType(&charPosition[2],circle);
	charPos[2].setGeometryAndPhysicalType(&charPosition[3],circle);
	bd = new GameObject[4];
	bdLines = new Line[4];
	bd[0].setGeometryAndPhysicalType(&bdLines[0],line);
	bd[1].setGeometryAndPhysicalType(&bdLines[1],line);
	bd[2].setGeometryAndPhysicalType(&bdLines[2],line);
	bd[3].setGeometryAndPhysicalType(&bdLines[3],line);
}

void Wrapper::InitializePhysics()
{
	Trapezoid res = motionPlanning.Calculate_BoundingRectangle();
	unsigned int i;
	for(i = 0;i < motionPlanning.segmentList.size();++i)
	{
		goLines[i].V0.x = motionPlanning.segmentList[i].start.x;
		goLines[i].V0.y = res.bottom.start.y - motionPlanning.segmentList[i].start.y;
		goLines[i].V0.z = 0.0f;
		goLines[i].V1.x = motionPlanning.segmentList[i].end.x;
		goLines[i].V1.y = res.bottom.start.y - motionPlanning.segmentList[i].end.y;
		goLines[i].V1.z = 0.0f;
	}
	for(i = 0;i < motionPlanning.segmentList.size(); ++i)
	{
		goLines[i].dynamic = false;
		goLines[i].p.x = (goLines[i].V0.x+goLines[i].V1.x)/2;
		goLines[i].p.y = (goLines[i].V0.y+goLines[i].V1.y)/2;
		goLines[i].p.z = (goLines[i].V0.z+goLines[i].V1.z)/2;
		float length = sqrt(((goLines[i].V1.x-goLines[i].V0.x)*(goLines[i].V1.x-goLines[i].V0.x))
				   +((goLines[i].V1.y-goLines[i].V0.y)*(goLines[i].V1.y-goLines[i].V0.y))
				   +((goLines[i].V1.y-goLines[i].V0.y)*(goLines[i].V1.z-goLines[i].V0.z)));
		/// Find the scaling factor for objects based on object radius and geometric modelspace
		float scaleFactor = length/(mSpace.limit_r - mSpace.limit_l);
		/// Upload the scale,translate and rotate transforms for the objects
		goLines[i].scale.upload(scaleFactor,scaleFactor,scaleFactor);
		goLines[i].translate.upload(goLines[i].p.x,goLines[i].p.y,goLines[i].p.z);
		goLines[i].rotate.upload(atan((goLines[i].V1.y - goLines[i].V0.y)/(goLines[i].V1.x - goLines[i].V0.x))*180.0f/M_PI,0.0f,0.0f,1.0f);
		goLines[i].geoType->local.add(&goLines[i].translate);
		goLines[i].geoType->local.add(&goLines[i].rotate);
		goLines[i].geoType->local.add(&goLines[i].scale);
	}
	std::list<Trapezoid>::iterator j;
	std::list<Trapezoid> trapezoidalList = motionPlanning.retrieveTrapezoidalList();
	std::list<Trapezoid> unusedTrapezoidalList = motionPlanning.retrieveUnusedTrapezoidalList();
	for(j = trapezoidalList.begin(),i = motionPlanning.segmentList.size();j != trapezoidalList.end();j++)
	{
		goLines[i].V0.x = (*j).top.start.x;
		goLines[i].V0.y = res.bottom.start.y - (*j).top.start.y;
		goLines[i].V0.z = 0.0f;
		goLines[i].V1.x = (*j).top.end.x;
		goLines[i].V1.y = res.bottom.start.y - (*j).top.end.y;
		goLines[i].V1.z = 0.0f;
		i++;
		goLines[i].V0.x = (*j).top.end.x;
		goLines[i].V0.y = res.bottom.start.y - (*j).top.end.y;
		goLines[i].V0.z = 0.0f;
		goLines[i].V1.x = (*j).bottom.end.x;
		goLines[i].V1.y = res.bottom.start.y - (*j).bottom.end.y;
		goLines[i].V1.z = 0.0f;
		i++;
		goLines[i].V0.x = (*j).bottom.end.x;
		goLines[i].V0.y = res.bottom.start.y - (*j).bottom.end.y;
		goLines[i].V0.z = 0.0f;
		goLines[i].V1.x = (*j).bottom.start.x;
		goLines[i].V1.y = res.bottom.start.y - (*j).bottom.start.y;
		goLines[i].V1.z = 0.0f;
		i++;
		goLines[i].V0.x = (*j).bottom.start.x;
		goLines[i].V0.y = res.bottom.start.y - (*j).bottom.start.y;
		goLines[i].V0.z = 0.0f;
		goLines[i].V1.x = (*j).top.start.x;
		goLines[i].V1.y = res.bottom.start.y - (*j).top.start.y;
		goLines[i].V1.z = 0.0f;
		i++;
	}
	for(i = motionPlanning.segmentList.size();i < motionPlanning.segmentList.size()+(motionPlanning.sizeOftrapezoidalList()*4); ++i)
	{
		goLines[i].dynamic = false;
		goLines[i].p.x = (goLines[i].V0.x+goLines[i].V1.x)/2;
		goLines[i].p.y = (goLines[i].V0.y+goLines[i].V1.y)/2;
		goLines[i].p.z = (goLines[i].V0.z+goLines[i].V1.z)/2;
		float length = sqrt(((goLines[i].V1.x-goLines[i].V0.x)*(goLines[i].V1.x-goLines[i].V0.x))
				   +((goLines[i].V1.y-goLines[i].V0.y)*(goLines[i].V1.y-goLines[i].V0.y))
				   +((goLines[i].V1.y-goLines[i].V0.y)*(goLines[i].V1.z-goLines[i].V0.z)));
		/// Find the scaling factor for objects based on object radius and geometric modelspace
		float scaleFactor = length/(mSpace.limit_r - mSpace.limit_l);
		/// Upload the scale,translate and rotate transforms for the objects
		goLines[i].scale.upload(scaleFactor,scaleFactor,scaleFactor);
		goLines[i].translate.upload(goLines[i].p.x,goLines[i].p.y,goLines[i].p.z);
		goLines[i].rotate.upload(atan((goLines[i].V1.y - goLines[i].V0.y)/(goLines[i].V1.x - goLines[i].V0.x))*180.0f/M_PI,0.0f,0.0f,1.0f);
		goLines[i].geoType->local.add(&goLines[i].translate);
		goLines[i].geoType->local.add(&goLines[i].rotate);
		goLines[i].geoType->local.add(&goLines[i].scale);
	}
	for(j = unusedTrapezoidalList.begin(),i = motionPlanning.segmentList.size()+(motionPlanning.sizeOftrapezoidalList()*4);j != unusedTrapezoidalList.end();j++)
	{
		goLines[i].V0.x = (*j).top.start.x;
		goLines[i].V0.y = res.bottom.start.y - (*j).top.start.y;
		goLines[i].V0.z = 0.0f;
		goLines[i].V1.x = (*j).top.end.x;
		goLines[i].V1.y = res.bottom.start.y - (*j).top.end.y;
		goLines[i].V1.z = 0.0f;
		i++;
		goLines[i].V0.x = (*j).top.end.x;
		goLines[i].V0.y = res.bottom.start.y - (*j).top.end.y;
		goLines[i].V0.z = 0.0f;
		goLines[i].V1.x = (*j).bottom.end.x;
		goLines[i].V1.y = res.bottom.start.y - (*j).bottom.end.y;
		goLines[i].V1.z = 0.0f;
		i++;
		goLines[i].V0.x = (*j).bottom.end.x;
		goLines[i].V0.y = res.bottom.start.y - (*j).bottom.end.y;
		goLines[i].V0.z = 0.0f;
		goLines[i].V1.x = (*j).bottom.start.x;
		goLines[i].V1.y = res.bottom.start.y - (*j).bottom.start.y;
		goLines[i].V1.z = 0.0f;
		i++;
		goLines[i].V0.x = (*j).bottom.start.x;
		goLines[i].V0.y = res.bottom.start.y - (*j).bottom.start.y;
		goLines[i].V0.z = 0.0f;
		goLines[i].V1.x = (*j).top.start.x;
		goLines[i].V1.y = res.bottom.start.y - (*j).top.start.y;
		goLines[i].V1.z = 0.0f;
		i++;
	}
	for(i = motionPlanning.segmentList.size()+(motionPlanning.sizeOftrapezoidalList()*4);i < motionPlanning.segmentList.size()+(motionPlanning.sizeOftrapezoidalList()*4)+(motionPlanning.sizeOfunusedTrapezoidalList()*4); ++i)
	{
		goLines[i].dynamic = false;
		goLines[i].p.x = (goLines[i].V0.x+goLines[i].V1.x)/2;
		goLines[i].p.y = (goLines[i].V0.y+goLines[i].V1.y)/2;
		goLines[i].p.z = (goLines[i].V0.z+goLines[i].V1.z)/2;
		float length = sqrt(((goLines[i].V1.x-goLines[i].V0.x)*(goLines[i].V1.x-goLines[i].V0.x))
				   +((goLines[i].V1.y-goLines[i].V0.y)*(goLines[i].V1.y-goLines[i].V0.y))
				   +((goLines[i].V1.y-goLines[i].V0.y)*(goLines[i].V1.z-goLines[i].V0.z)));
		/// Find the scaling factor for objects based on object radius and geometric modelspace
		float scaleFactor = length/(mSpace.limit_r - mSpace.limit_l);
		/// Upload the scale,translate and rotate transforms for the objects
		goLines[i].scale.upload(scaleFactor,scaleFactor,scaleFactor);
		goLines[i].translate.upload(goLines[i].p.x,goLines[i].p.y,goLines[i].p.z);
		goLines[i].rotate.upload(atan((goLines[i].V1.y - goLines[i].V0.y)/(goLines[i].V1.x - goLines[i].V0.x))*180.0f/M_PI,0.0f,0.0f,1.0f);
		goLines[i].geoType->local.add(&goLines[i].translate);
		goLines[i].geoType->local.add(&goLines[i].rotate);
		goLines[i].geoType->local.add(&goLines[i].scale);
	}
	unsigned int k = 0;
	for(unsigned int i = 0;i < motionPlanning.path->states.size(); ++i)
	{
		for(unsigned int j = 0;j < motionPlanning.path->states[i].edges.size();++j)
		{
			goPathLines[k].V0.x = motionPlanning.path->states[motionPlanning.path->states[i].edges[j].from].worldPositionX;
			goPathLines[k].V0.y = res.bottom.start.y - motionPlanning.path->states[motionPlanning.path->states[i].edges[j].from].worldPositionY;
			goPathLines[k].V0.z = 0.0f;
			goPathLines[k].V1.x = motionPlanning.path->states[motionPlanning.path->states[i].edges[j].to].worldPositionX;
			goPathLines[k].V1.y = res.bottom.start.y - motionPlanning.path->states[motionPlanning.path->states[i].edges[j].to].worldPositionY;
			goPathLines[k].V1.z = 0.0f;
			k++;
		}
	}

	for(i = 0;i < totalEdges; ++i)
	{
		goPathLines[i].dynamic = false;
		goPathLines[i].p.x = (goPathLines[i].V0.x+goPathLines[i].V1.x)/2;
		goPathLines[i].p.y = (goPathLines[i].V0.y+goPathLines[i].V1.y)/2;
		goPathLines[i].p.z = (goPathLines[i].V0.z+goPathLines[i].V1.z)/2;
		float length = sqrt(((goPathLines[i].V1.x-goPathLines[i].V0.x)*(goPathLines[i].V1.x-goPathLines[i].V0.x))
				   +((goPathLines[i].V1.y-goPathLines[i].V0.y)*(goPathLines[i].V1.y-goPathLines[i].V0.y))
				   +((goPathLines[i].V1.y-goPathLines[i].V0.y)*(goPathLines[i].V1.z-goPathLines[i].V0.z)));
		/// Find the scaling factor for objects based on object radius and geometric modelspace
		float scaleFactor = length/(mSpace.limit_r - mSpace.limit_l);
		/// Upload the scale,translate and rotate transforms for the objects
		goPathLines[i].scale.upload(scaleFactor,scaleFactor,scaleFactor);
		goPathLines[i].translate.upload(goPathLines[i].p.x,goPathLines[i].p.y,goPathLines[i].p.z);
		goPathLines[i].rotate.upload(atan((goPathLines[i].V1.y - goPathLines[i].V0.y)/(goPathLines[i].V1.x - goPathLines[i].V0.x))*180.0f/M_PI,0.0f,0.0f,1.0f);
		goPathLines[i].geoType->local.add(&goPathLines[i].translate);
		goPathLines[i].geoType->local.add(&goPathLines[i].rotate);
		goPathLines[i].geoType->local.add(&goPathLines[i].scale);
	}
	for(i = 0;i < motionPlanning.path->states.size();i++)
	{
		goStates[i].p.x = motionPlanning.path->states[i].worldPositionX;
		goStates[i].p.y = res.bottom.start.y - motionPlanning.path->states[i].worldPositionY;
		goStates[i].p.z = 0.0f;
		goStates[i].radius = 3.0f;
		float scaleFactor = (goStates[i].radius*2.0f)/(mSpace.limit_r - mSpace.limit_l);
		/// Upload the scale and translate transforms for the objects
		goStates[i].scale.upload(scaleFactor,scaleFactor,scaleFactor);
		goStates[i].translate.upload(goStates[i].p.x,goStates[i].p.y,goStates[i].p.z);
		goStates[i].geoType->local.add(&goStates[i].translate);
		goStates[i].geoType->local.add(&goStates[i].scale);
	}
	for(i = 0;i < 3;i++)
	{
		charPos[i].p.x = 0.0f;
		charPos[i].p.y = 0.0f;
		charPos[i].p.z = 0.0f;
		charPos[i].radius = 5.0f;
		float scaleFactor = (charPos[i].radius*2.0f)/(mSpace.limit_r - mSpace.limit_l);
		/// Upload the scale and translate transforms for the objects
		charPos[i].scale.upload(scaleFactor,scaleFactor,scaleFactor);
		charPos[i].translate.upload(charPos[i].p.x,charPos[i].p.y,charPos[i].p.z);
		charPos[i].geoType->local.add(&charPos[i].translate);
		charPos[i].geoType->local.add(&charPos[i].scale);
	}
	bd[0].V0.x = motionPlanning.boundingRect.bottom.start.x;
	bd[0].V0.y = res.bottom.start.y - motionPlanning.boundingRect.bottom.start.y;
	bd[0].V0.z = 0.0f;
	bd[0].V1.x = motionPlanning.boundingRect.top.start.x;
	bd[0].V1.y = res.bottom.start.y - motionPlanning.boundingRect.top.start.y;
	bd[0].V1.z = 0.0f;

	bd[1].V0.x = motionPlanning.boundingRect.top.start.x;
	bd[1].V0.y = res.bottom.start.y - motionPlanning.boundingRect.top.start.y;
	bd[1].V0.z = 0.0f;
	bd[1].V1.x = motionPlanning.boundingRect.top.end.x;
	bd[1].V1.y = res.bottom.start.y - motionPlanning.boundingRect.top.end.y;
	bd[1].V1.z = 0.0f;

	bd[2].V0.x = motionPlanning.boundingRect.top.end.x;
	bd[2].V0.y = res.bottom.start.y - motionPlanning.boundingRect.top.end.y;
	bd[2].V0.z = 0.0f;
	bd[2].V1.x = motionPlanning.boundingRect.bottom.end.x;
	bd[2].V1.y = res.bottom.start.y - motionPlanning.boundingRect.bottom.end.y;
	bd[2].V1.z = 0.0f;

	bd[3].V0.x = motionPlanning.boundingRect.bottom.end.x;
	bd[3].V0.y = res.bottom.start.y - motionPlanning.boundingRect.bottom.end.y;
	bd[3].V0.z = 0.0f;
	bd[3].V1.x = motionPlanning.boundingRect.bottom.start.x;
	bd[3].V1.y = res.bottom.start.y - motionPlanning.boundingRect.bottom.start.y;
	bd[3].V1.z = 0.0f;
	for(i = 0;i < 4; ++i)
	{
		bd[i].dynamic = false;
		bd[i].p.x = (bd[i].V0.x+bd[i].V1.x)/2;
		bd[i].p.y = (bd[i].V0.y+bd[i].V1.y)/2;
		bd[i].p.z = (bd[i].V0.z+bd[i].V1.z)/2;
		float length = sqrt(((bd[i].V1.x-bd[i].V0.x)*(bd[i].V1.x-bd[i].V0.x))
				   +((bd[i].V1.y-bd[i].V0.y)*(bd[i].V1.y-bd[i].V0.y))
				   +((bd[i].V1.y-bd[i].V0.y)*(bd[i].V1.z-bd[i].V0.z)));
		/// Find the scaling factor for objects based on object radius and geometric modelspace
		float scaleFactor = length/(mSpace.limit_r - mSpace.limit_l);
		/// Upload the scale,translate and rotate transforms for the objects
		bd[i].scale.upload(scaleFactor,scaleFactor,scaleFactor);
		bd[i].translate.upload(bd[i].p.x,bd[i].p.y,bd[i].p.z);
		bd[i].rotate.upload(atan((bd[i].V1.y - bd[i].V0.y)/(bd[i].V1.x - bd[i].V0.x))*180.0f/M_PI,0.0f,0.0f,1.0f);
		bd[i].geoType->local.add(&bd[i].translate);
		bd[i].geoType->local.add(&bd[i].rotate);
		bd[i].geoType->local.add(&bd[i].scale);
	}
	character.InitializePhysics(mSpace);
}

