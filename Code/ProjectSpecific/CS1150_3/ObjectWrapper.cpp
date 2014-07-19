/******************************************************************************/
/*!
\file ObjectWrapper.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #03
\date 22/03/2012
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
	for(int i = 0;i < 4;++i)
		GeoLines[i].createGeometry(255,255,255);
	for(int i = 0;i < 9;++i)
	{
		GeoX[i].addTexture("Code/Files/TGA/x.tga");
		GeoO[i].addTexture("Code/Files/TGA/o.tga");
		GeoX[i].createGeometry(0,255,255);
		GeoO[i].createGeometry(0,255,255);
	}
	t[0].upload(-1.0f,1.0f,0.0f);
	t[1].upload(0.0f,1.0f,0.0f);
	t[2].upload(1.0f,1.0f,0.0f);
	t[3].upload(-1.0f,0.0f,0.0f);
	t[4].upload(0.0f,0.0f,0.0f);
	t[5].upload(1.0f,0.0f,0.0f);
	t[6].upload(-1.0f,-1.0f,0.0f);
	t[7].upload(0.0f,-1.0f,0.0f);
	t[8].upload(1.0f,-1.0f,0.0f);
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
	for(int i = 0;i < 4;++i)
		GeoLines[i].uploadGeometry();
	for(int i = 0;i < 9; ++i)
	{
		GeoX[i].uploadGeometry();
		GeoO[i].uploadGeometry();	
	}
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
	for(int i = 0; i < 4;++i)
		GeoLines[i].destroyGeometry();
	for(int i = 0;i < 9; ++i)
	{
		GeoX[i].destroyGeometry();
		GeoO[i].destroyGeometry();
	}
}

/******************************************************************************/
/*!
Create game objects
\return
void
*/
/******************************************************************************/
void Wrapper::createObjects()
{
	for(int i = 0;i < 4; ++i)
		Lines[i].setGeometryAndPhysicalType(&GeoLines[i],line);
	for(int i = 0; i < 9; ++i)
	{
		X[i].setGeometryAndPhysicalType(&GeoX[i],square);
		O[i].setGeometryAndPhysicalType(&GeoO[i],square);
	}
}

/******************************************************************************/
/*!
Initialize game objects
\return
void
*/
/******************************************************************************/
void Wrapper::InitObjects()
{
	mSpace.limit_l = -0.5f;
	mSpace.limit_r = 0.5f;
	for(int i = 0; i < 9; ++i)
	{
		X[i].translate.upload(t[i].x,t[i].y,t[i].z);
		O[i].translate.upload(t[i].x,t[i].y,t[i].z);
		X[i].geoType->local.add(&X[i].translate);
		O[i].geoType->local.add(&O[i].translate);
		X[i].p.x = t[i].x;
		X[i].p.y = t[i].y;
		X[i].p.z = t[i].z;
		O[i].p.x = t[i].x;
		O[i].p.y = t[i].y;
		O[i].p.z = t[i].z;
	}
	Lines[0].V0.x = -0.5f;
	Lines[0].V0.y = 1.5f;
	Lines[0].V0.z = 0.0f;
	Lines[0].V1.x = -0.5f;
	Lines[0].V1.y = -1.5f;
	Lines[0].V1.z = 0.0f;

	Lines[1].V0.x = 0.5f;
	Lines[1].V0.y = 1.5f;
	Lines[1].V0.z = 0.0f;
	Lines[1].V1.x = 0.5f;
	Lines[1].V1.y = -1.5f;
	Lines[1].V1.z = 0.0f;

	Lines[2].V0.x = -1.5f;
	Lines[2].V0.y = 0.5f;
	Lines[2].V0.z = 0.0f;
	Lines[2].V1.x = 1.5f;
	Lines[2].V1.y = 0.5f;
	Lines[2].V1.z = 0.0f;

	Lines[3].V0.x = -1.5f;
	Lines[3].V0.y = -0.5f;
	Lines[3].V0.z = 0.0f;
	Lines[3].V1.x = 1.5f;
	Lines[3].V1.y = -0.5f;
	Lines[3].V1.z = 0.0f;

	for(int i = 0; i < 4; ++i)
	{
		Lines[i].p.x = (Lines[i].V0.x+Lines[i].V1.x)/2;
		Lines[i].p.y = (Lines[i].V0.y+Lines[i].V1.y)/2;
		Lines[i].p.z = (Lines[i].V0.z+Lines[i].V1.z)/2;
		float length = sqrt(((Lines[i].V1.x-Lines[i].V0.x)*(Lines[i].V1.x-Lines[i].V0.x))
			+((Lines[i].V1.y-Lines[i].V0.y)*(Lines[i].V1.y-Lines[i].V0.y))
			+((Lines[i].V1.y-Lines[i].V0.y)*(Lines[i].V1.z-Lines[i].V0.z)));
		/// Find the scaling factor for objects based on object radius and geometric modelspace
		float scaleFactor = length/(mSpace.limit_r - mSpace.limit_l);
		/// Upload the scale,translate and rotate transforms for the objects
		Lines[i].scale.upload(scaleFactor,scaleFactor,scaleFactor);
		Lines[i].translate.upload(Lines[i].p.x,Lines[i].p.y,Lines[i].p.z);
		Lines[i].rotate.upload(atan((Lines[i].V1.y - Lines[i].V0.y)/(Lines[i].V1.x - Lines[i].V0.x))*180.0f/M_PI,0.0f,0.0f,1.0f);
		Lines[i].geoType->local.add(&Lines[i].translate);
		Lines[i].geoType->local.add(&Lines[i].rotate);
		Lines[i].geoType->local.add(&Lines[i].scale);
	}
}

/******************************************************************************/
/*!
Reinitialize board
\return
void
*/
/******************************************************************************/
void Wrapper::reinitBoard()
{
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
			Place[i][j] = EMPTY;
	game.reInitBoard();
}

/******************************************************************************/
/*!
Process input click in game
\param [in] posX
X coordinate in world
\param [in] posY
Y coordinate in world
\return
int - the index of board position where clicked
*/
/******************************************************************************/
int Wrapper::processInputClick(const float posX,const float posY)
{
	if(posY>=0.5f && posY<=1.5f)
	{
		if(posX>=-1.5f && posX<=-0.5f)
		{
			return 0;
		}
		else if(posX>=-0.5f && posX<=0.5f)
		{
			return 1;
		}
		else if(posX>=0.5f && posX<=1.5f)
		{
			return 2;
		}
		else
		{
			return -1;
		}
	}
	else if(posY>=-0.5f && posY<=0.5f)
	{
		if(posX>=-1.5f && posX<=-0.5f)
		{
			return 3;
		}
		else if(posX>=-0.5f && posX<=0.5f)
		{
			return 4;
		}
		else if(posX>=0.5f && posX<=1.5f)
		{
			return 5;
		}
		else
		{
			return -1;
		}
	}
	else if(posY>=-1.5f && posY<=-0.5f)
	{
		if(posX>=-1.5f && posX<=-0.5f)
		{
			return 6;
		}
		else if(posX>=-0.5f && posX<=0.5f)
		{
			return 7;
		}
		else if(posX>=0.5f && posX<=1.5f)
		{
			return 8;
		}
		else
		{
			return -1;
		}
	}
	else 
		return -1;
}
