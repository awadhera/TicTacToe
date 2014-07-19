/******************************************************************************/
/*!
\file ObjectLine.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1250
\par Project #01
\date 02/08/2012
\brief
Implementation of functions for Line class
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#include "ObjectLine.h"

/******************************************************************************/
/*!
Constructor for Line class
\param l
Modelspace left limit
\param r
Modelspace right limit
*/
/******************************************************************************/
Line::Line(const float &l,const float &r)
{	
	mSpace.limit_l = l;
	mSpace.limit_r = r;
	totVertices = 2;
	NUMBER_OF_INDICES = 2;
	g_VertexArray = new Vertex[totVertices];
	g_indices = new GLushort[2];
}

/******************************************************************************/
/*!
Destructor for Line class
*/
/******************************************************************************/
Line::~Line()
{
}

/******************************************************************************/
/*!
Unbind buffers and delete vertex and index data
\return
void
*/
/******************************************************************************/
void Line::destroyGeometry()
{
	glDeleteVertexArrays( 0 , g_vao );
	(Logger::Instance()).checkAndReportGLError("Failed to delete Vertex array",'e',4,__LINE__,__FILE__);
	glDeleteBuffers( NUM_BUFFERS, vbo );
	(Logger::Instance()).checkAndReportGLError("Failed to delete buffers",'e',4,__LINE__,__FILE__);
	delete [] g_VertexArray;
	delete [] g_indices; 
}

/******************************************************************************/
/*!
Generate Vertices and Indices for the object
\param red
Red component in RGB
\param green
Green component in RGB
\param blue
Blue component in RGB
\return
void
*/
/******************************************************************************/
void Line::createGeometry(const unsigned char &red,const unsigned char &green,const unsigned char &blue)
{
	g_VertexArray[0].p.x = mSpace.limit_l;
	g_VertexArray[1].p.x = mSpace.limit_r;
	g_VertexArray[0].p.y = g_VertexArray[1].p.y = 0;
	g_VertexArray[0].p.z = g_VertexArray[1].p.z = 0;
	g_VertexArray[0].c.r = g_VertexArray[1].c.r = red;
	g_VertexArray[0].c.g = g_VertexArray[1].c.g = green;
	g_VertexArray[0].c.b = g_VertexArray[1].c.b = blue;
	g_indices[0] = 0;
	g_indices[1] = 1;
}

/******************************************************************************/
/*!
Bind buffers for vertices and indices
\return
void
*/
/******************************************************************************/
void Line::uploadGeometry()
{
	const int ColorOffset = 0;
	const int PositionOffset = sizeof( Color4 );
	const unsigned int POSITION_INDEX = 0;	
	const unsigned int COLOR0_INDEX = 3;
	/// Generate and bind vertex array object for Sphere
	glGenVertexArrays(1, g_vao);
	(Logger::Instance()).checkAndReportGLError("Failed to generate Vertex Arrays",'e',4,__LINE__,__FILE__);
	glBindVertexArray( g_vao[0] );
	(Logger::Instance()).checkAndReportGLError("Failed to bind Vertex Arrays",'e',4,__LINE__,__FILE__);
	/// Generate buffer objects for vertices and indices
	glGenBuffers( NUM_BUFFERS, vbo );
	(Logger::Instance()).checkAndReportGLError("Failed to generate Buffer objects",'e',4,__LINE__,__FILE__);
	/// Bind vertex buffer object
	glBindBuffer( GL_ARRAY_BUFFER, vbo[VBO] );
	(Logger::Instance()).checkAndReportGLError("Failed to bind Vertex Buffer object",'e',4,__LINE__,__FILE__);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(Vertex) * totVertices,
		reinterpret_cast<const GLvoid*>( g_VertexArray ),
		GL_STATIC_DRAW );
	(Logger::Instance()).checkAndReportGLError("Failed to provide Vertex Buffer data",'e',4,__LINE__,__FILE__);
	glVertexAttribPointer(
		POSITION_INDEX,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof( Vertex ),
		BUFFER_OFFSET( PositionOffset ) );
	(Logger::Instance()).checkAndReportGLError("Failed to run glVertexAttribPointer for vertex position",'e',4,__LINE__,__FILE__);
	glEnableVertexAttribArray( POSITION_INDEX );
	(Logger::Instance()).checkAndReportGLError("Failed to enable VertexAttribArray for vertex position",'e',4,__LINE__,__FILE__);
	glVertexAttribPointer(
		COLOR0_INDEX,
		4,
		GL_UNSIGNED_BYTE,
		GL_TRUE,
		sizeof( Vertex ),
		BUFFER_OFFSET( ColorOffset ) );
	(Logger::Instance()).checkAndReportGLError("Failed to run glVertexAttribPointer for color",'e',4,__LINE__,__FILE__);
	glEnableVertexAttribArray( COLOR0_INDEX );
	(Logger::Instance()).checkAndReportGLError("Failed to enable VertexAttribArray for color",'e',4,__LINE__,__FILE__);
	/// Bind Index buffer object
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbo[IBO] );
	(Logger::Instance()).checkAndReportGLError("Failed to bind Index Buffer object",'e',4,__LINE__,__FILE__);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(GLushort) * NUMBER_OF_INDICES,
		reinterpret_cast<const GLvoid*>( g_indices ),
		GL_STATIC_DRAW );
	(Logger::Instance()).checkAndReportGLError("Failed to provide buffer data for indices",'e',4,__LINE__,__FILE__);
}

/******************************************************************************/
/*!
Render the Line
\return
void
*/
/******************************************************************************/
void Line::drawGeometry()
{
	glBindVertexArray( g_vao[0] );
	glDrawElements( GL_LINES, NUMBER_OF_INDICES, GL_UNSIGNED_SHORT, (void*)0 );
	(Logger::Instance()).checkAndReportGLError("Failed to render the line",'e',4,__LINE__,__FILE__);
}

/******************************************************************************/
/*!
Render the transformed Line. Calls drawGeometry() internally
\param [in] mv_s
Reference to modelview stack object
\return
void
*/
/******************************************************************************/
void Line::drawTransformedGeometry(MatrixStack &mv_s)
{
	/// Update parent-to-child and local transforms
	parentChild.update();
	local.update();
	/// save state of matrixstack
	mv_s.PushMatrix();
	/// Evaluate the affine transform
	mv_s.MultMatrix(parentChild.m);
	mv_s.MultMatrix(local.m);
	glLoadMatrixf(mv_s.last());
	(Logger::Instance()).checkAndReportGLError("Failed to load matrix onto the openGL modelview matrixstack",'e',4,__LINE__,__FILE__);
	/// Render the object
	this->drawGeometry();
	/// Restore state of matrixstack
	mv_s.PopMatrix();
}