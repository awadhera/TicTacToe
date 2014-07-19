/******************************************************************************/
/*!
\file ObjectUtility.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Interface for basic mesh utilities 
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef OBJECTUTILITY_H
#define OBJECTUTILITY_H

#include "Math/MathExt.h"

/*! PRIMITIVE RESTART INDEX */
#ifndef PRIMITIVE_RESTART_INDEX
#define PRIMITIVE_RESTART_INDEX 0xFFFF 
#endif

/*! BUFFER OFFSET - used while binding openGL buffers */
#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET( nBytes ) ( reinterpret_cast<const unsigned char *>( 0 ) + nBytes ) 
#endif

//!  Color4 structure
/*!
Structure for RGB type color
*/
struct Color4
{
	unsigned char r;	/*!< R value of the color */
	unsigned char g;	/*!< G value of the color */
	unsigned char b;	/*!< B value of the color */
	unsigned char a;	/*!< A value of the color */
	Color4( void );
};

//!  ModelSpace structure
/*!
Structure for modelspace limits
*/
struct ModelSpace
{
	float limit_l; /*!< Modelspace left limit */
	float limit_r; /*!< Modelspace right limit */
};

struct TexCoord
{
	float s;
	float t;
};

//!  Vertex structure
/*!
Structure for mesh vertices
*/
struct Vertex
{
	Color4 c;	/*!< Color of the Vertex */
	Point p;	/*!< Vertex position */
	TexCoord tc;
	Vertex( void );
};



#endif