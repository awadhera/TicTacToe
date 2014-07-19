/******************************************************************************/
/*!
\file ObjectBase.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Implementation for BaseGeometry Class
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#include "ObjectBase.h"

/******************************************************************************/
/*!
Virtual function. Placeholder function for inherited geometry type. Used for building
the geometry
\return
void
*/
/******************************************************************************/
void BaseGeometry::createGeometry(const unsigned char &,const unsigned char &,const unsigned char &)
{
}

/******************************************************************************/
/*!
Virtual function. Placeholder function for inherited geometry type. Used for uploading
geometry to the GPU
\return
void
*/
/******************************************************************************/
void BaseGeometry::uploadGeometry()
{
}

/******************************************************************************/
/*!
Virtual function. Placeholder function for inherited geometry type. Used for rendering
the object
\return
void
*/
/******************************************************************************/
void BaseGeometry::drawGeometry()
{
}

/******************************************************************************/
/*!
Virtual function. Placeholder function for inherited geometry type. Used for drawing
the transformed geometry. ( Internally calls drawGeometry())
\return
void
*/
/******************************************************************************/
void BaseGeometry::drawTransformedGeometry(MatrixStack &)
{
}

/******************************************************************************/
/*!
Virtual function. Placeholder function for inherited geometry type. Used for destroying
the geometry( Unbinding the buffers and destroying vertices and indices data)
\return
void
*/
/******************************************************************************/
void BaseGeometry::destroyGeometry()
{
}
/******************************************************************************/
/*!
Virtual Destructor for BaseGeometry class
*/
/******************************************************************************/
BaseGeometry::~BaseGeometry() 
{
}
