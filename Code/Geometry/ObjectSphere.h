/******************************************************************************/
/*!
\file ObjectSphere.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #02
\date 09/03/2012
\brief
Interface for Sphere mesh
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef OBJECTSPHERE_H
#define OBJECTSPHERE_H

#include "ObjectBase.h"

//!  Sphere class
/*!
Class for implementing Sphere geometry Object
*/
class Sphere : public BaseGeometry
{
	Sphere(const Sphere &);
	unsigned short slices,stacks;	/*!< Number of slices and stacks*/
	unsigned short NO_INDICES_STRIPS;	/*!< Total number of indices for triangle strips */
	unsigned short NO_INDICES_FANS;	/*!< Total number of indices for triangle fans */
	enum {	VBO,IBO,NUM_BUFFERS };	/*!< Generic enum. VBO = 0, IBO = 1, NUM_BUFFERS = 2 */
	unsigned int vbo[NUM_BUFFERS];	/*!< Vertex and Indices Buffer objects */
	unsigned int g_vao[1];	/*!< Vertex array object */
	ModelSpace mSpace;	/*!< ModelSpace of the mesh */
public:
	Sphere(const unsigned short &stk = 40,const unsigned short &sli = 40,const float &l = -0.5f,const float &r = 0.5f);
	~Sphere();
	void createGeometry(const unsigned char &red,const unsigned char &green,const unsigned char &blue);
	void uploadGeometry();
	void drawGeometry();
	void destroyGeometry();
	void drawTransformedGeometry(MatrixStack &mv_s);
};

#endif
