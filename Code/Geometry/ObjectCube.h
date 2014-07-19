/******************************************************************************/
/*!
\file ObjectCube.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1050
\par Project #01
\date 21/12/2011
\brief
Interface for Cube mesh
Copyright (C) 2011 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef OBJECTCUBE_H
#define OBJECTCUBE_H

#include "ObjectBase.h"
#include "../Logger/Logger.h"

//!  Cube class
/*!
Class for implementing Cube geometry Object
*/
class Cube : public BaseGeometry
{
	Cube(const Cube &);
	unsigned short subd;			/*!< Number of subdivisions in the cube */
	unsigned short NUMBER_OF_INDICES;	/*!< Total number of indices */
	enum {	VBO,IBO,NUM_BUFFERS }; /*!< Generic enum. VBO = 0, IBO = 1, NUM_BUFFERS = 2 */
	unsigned int vbo[NUM_BUFFERS];	 /*!< Vertex and Indices Buffer objects*/
	unsigned int g_vao[1];	/*!< Vertex array object */
	ModelSpace mSpace;	/*!< ModelSpace of the mesh */
public:
	Cube(const unsigned short &sd = 5,const float &l = -0.5f,const float &r = 0.5f);
	~Cube();
	void createGeometry(const unsigned char &red,const unsigned char &green,const unsigned char &blue);
	void uploadGeometry();
	void drawGeometry();
	void destroyGeometry();
	void transformGridInCube(const Matrix &m,Vertex *storeVertex,const unsigned long &size,Vertex *g_Vertex_Array,unsigned long &index);
	void drawTransformedGeometry(MatrixStack &mv_s);
};



#endif