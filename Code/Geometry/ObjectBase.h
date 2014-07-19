/******************************************************************************/
/*!
\file ObjectBase.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Interface for base geometry
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#include "ObjectUtility.h"
#include "Transforms.h"
#include "../Math/MatrixStack.h"
#include "../Logger/Logger.h"

//!  BaseGeometry class
/*!
Class for implementing BaseGeometry Mesh
*/

class BaseGeometry
{
public:
	AffineTransform parentChild;	/*!< Transform for this composite object from its parent */
	AffineTransform local;	/*!< local transform for this object */
	Vertex *g_VertexArray;	/*!< Vertex Array - Pointer of type Vertex */
	unsigned short *g_indices;	/*!< Indices Array - Pointer of type short */
	unsigned short totVertices;	/*!< Total number of vertices */
	virtual void createGeometry(const unsigned char &,const unsigned char &,const unsigned char &); 
	virtual void uploadGeometry();
	virtual void drawGeometry();
	virtual void drawTransformedGeometry(MatrixStack &);	
	virtual void destroyGeometry();
	virtual ~BaseGeometry();
};

#endif