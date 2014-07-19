/******************************************************************************/
/*!
\file ObjectComposite.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Interface for Composite object
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef OBJECTCOMPOSITE_H
#define OBJECTCOMPOSITE_H

#include "ObjectBase.h"

//!  Composite class
/*!
Class for implementing Composite geometry objects.
*/
class Composite : public BaseGeometry
{
	std::vector<BaseGeometry *> geoList;  /*!< vector of basegeometry pointers */
public:
	AffineTransform parentChild;  /*!< Transform for this composite object from its parent */
	AffineTransform local;	/*!< local transform for this object */	
	void add(BaseGeometry *b);
	void drawTransformedGeometry(MatrixStack &mv_s);
};


#endif