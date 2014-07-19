/******************************************************************************/
/*!
\file ObjectComposite.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Implementation for Composite Class
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#include "ObjectComposite.h"

/******************************************************************************/
/*!
Render the transformed geometry object(Internally calls drawGeometry for the specific
object type)
\param mv_s
Reference to the modelview matrix stack
\return
void
*/
/******************************************************************************/
void Composite::drawTransformedGeometry(MatrixStack &mv_s)
{
	std::vector<BaseGeometry *>::iterator iter1 = geoList.begin();
	std::vector<BaseGeometry *>::iterator iter2;
	/// Update the parent-to-child and local transform for this entity
	parentChild.update();
	local.update();
	/// Save state of the matrix stack
	mv_s.PushMatrix();
	/// Multiply the top of matrix stack with the parent-to-child and local transforms
	mv_s.MultMatrix(parentChild.m);
	mv_s.MultMatrix(local.m);
	/// Call drawTransformedGeometry for each child whose parent is this entity
	for(iter2=(geoList.begin());iter2!=geoList.end();iter2++)
	{
		(*iter2)->drawTransformedGeometry(mv_s);
	}
	/// Restore state of the matrix stack
	mv_s.PopMatrix();
}

/******************************************************************************/
/*!
Add an entity to the composite object.(Entity may be a geometric object or can
itself be a composite object)
\param b
Pointer to BaseGeometry type
\return
void
*/
/******************************************************************************/
void Composite::add(BaseGeometry *b)
{
	geoList.push_back(b);
}