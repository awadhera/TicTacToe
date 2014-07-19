/******************************************************************************/
/*!
\file ObjectLine.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1250
\par Project #01
\date 02/08/2012
\brief
Interface for Line mesh
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef OBJECTLINE_H
#define OBJECTLINE_H

#include "ObjectBase.h"

//!  Line class
/*!
Class for implementing Line geometry Object
*/
class Line : public BaseGeometry
{
	Line(const Line &);
	enum {	VBO,IBO,NUM_BUFFERS };	/*!< Generic enum. VBO = 0, IBO = 1, NUM_BUFFERS = 2 */
	unsigned int vbo[NUM_BUFFERS];	/*!< Vertex and Indices Buffer objects*/
	unsigned short NUMBER_OF_INDICES; /*!< Total number of indices */
	unsigned int g_vao[1];	/*!< Vertex array object */
	ModelSpace mSpace;	/*!< ModelSpace of the mesh */
public:
	Line(const float &l = -0.5f,const float &r = 0.5f);
	~Line();
	void createGeometry(const unsigned char &red,const unsigned char &green,const unsigned char &blue);
	void uploadGeometry();
	void drawGeometry();
	void destroyGeometry();
	void drawTransformedGeometry(MatrixStack &mv_s);
};

#endif