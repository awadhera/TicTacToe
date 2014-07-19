/******************************************************************************/
/*!
\file ObjectGrid.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Interface for Grid mesh
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef OBJECTGRID_H
#define OBJECTGRID_H

#include "ObjectBase.h"
#include<fstream>
#include<iostream>
#include "../FileProcessors/TGALoader.h"

//!  Grid class
/*!
Class for implementing Grid geometry Object
*/
class Grid : public BaseGeometry
{
	Grid(const Grid &);
	unsigned short subd_x,subd_y;	/*!< Number of subdivisions along x and y axis */
	unsigned short NUMBER_OF_INDICES;	/*!< Total number of indices */
	enum {	VBO,IBO,NUM_BUFFERS };	/*!< Generic enum. VBO = 0, IBO = 1, NUM_BUFFERS = 2 */
	unsigned int vbo[NUM_BUFFERS];	/*!< Vertex and Indices Buffer objects*/
	unsigned int g_vao[1];	/*!< Vertex array object */
	ModelSpace mSpace;	/*!< ModelSpace of the mesh */
	unsigned int texName[1];	/*!< TextureID for the mesh */

public:
	TGA tgaObj; /*!< TGA object for the texture map */
	Grid(const unsigned short &s_x = 3,const unsigned short &s_y = 3,const float &l = -0.5f,const float &r = 0.5f);
	~Grid();
	void createGeometry(const unsigned char &red,const unsigned char &green,const unsigned char &blue);
	void uploadGeometry();
	void drawGeometry();
	void destroyGeometry();
	void drawTransformedGeometry(MatrixStack &mv_s);
	bool addTexture(char *fileName);
};

#endif