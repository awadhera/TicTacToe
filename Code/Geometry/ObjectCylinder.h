/******************************************************************************/
/*!
\file ObjectCylinder.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1050
\par Project #01
\date 21/12/2011
\brief
Interface for Cylinder mesh
Copyright (C) 2011 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/


#ifndef OBJECTCYLINDER_H
#define OBJECTCYLINDER_H

#include "ObjectBase.h"

//!  Cylinder class
/*!
Class for implementing Cylinder geometry Object
*/
class Cylinder : public BaseGeometry
{
	Cylinder(const Cylinder &);
	unsigned short slices,stacks;	/*!< Number of slices and stacks*/
	unsigned short NO_INDICES_STRIPS;	/*!< Total number of indices for triangle strips */
	unsigned short NO_INDICES_FANS;	/*!< Total number of indices for triangle fans */
	enum {	VBO,IBO,NUM_BUFFERS };	/*!< Generic enum. VBO = 0, IBO = 1, NUM_BUFFERS = 2 */
	unsigned int vbo[NUM_BUFFERS];	/*!< Vertex and Indices Buffer objects */
	unsigned int g_vao[1];	/*!< Vertex array object */
	ModelSpace mSpace;	/*!< ModelSpace of the mesh */
public:
	Cylinder(const unsigned short &stk = 20,const unsigned short &sli = 20,const float &l = -0.5f,const float &r = 0.5f);
	~Cylinder();
	void createGeometry(const unsigned char &red,const unsigned char &green,const unsigned char &blue);
	void uploadGeometry();
	void drawGeometry();
	void destroyGeometry();
	void drawTransformedGeometry(MatrixStack &mv_s);
};



#endif
