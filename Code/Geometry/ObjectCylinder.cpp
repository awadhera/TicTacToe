/******************************************************************************/
/*!
\file ObjectCylinder.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1050
\par Project #01
\date 21/12/2011
\brief
Implementation of functions for Cylinder class
Copyright (C) 2011 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#include "ObjectCylinder.h"

/******************************************************************************/
/*!
Constructor for Cylinder class
\param stk
Number of stacks
\param sli
Number of slices
\param l
Modelspace left limit
\param r
Modelspace right limit
*/
/******************************************************************************/
Cylinder::Cylinder(const unsigned short &stk,const unsigned short &sli,const float &l,const float &r)
{
	mSpace.limit_l = l;
	mSpace.limit_r = r;
	stacks = stk;
	slices = sli;
	/// Total number of vertices
	totVertices = (slices+1)*(stacks+1)+2;
	g_VertexArray = new Vertex[totVertices];
	/// Total number of indices used for drawing strips and fans
	NO_INDICES_STRIPS = ((((2*(slices+1))+1)*stacks)-1);
	NO_INDICES_FANS = (2*(slices+1))+3;
	g_indices = new GLushort[NO_INDICES_STRIPS+NO_INDICES_FANS];
}

/******************************************************************************/
/*!
Destructor for Cylinder class
*/
/******************************************************************************/
Cylinder::~Cylinder()
{
}

/******************************************************************************/
/*!
Unbind buffers and delete vertex and index data
\return
void
*/
/******************************************************************************/
void Cylinder::destroyGeometry()
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
void Cylinder::createGeometry(const unsigned char &red,const unsigned char &green,const unsigned char &blue)
{
	float angle_d = 360.0f/slices;
	float angle_r = (angle_d*M_PI)/180;
	float inc_y = (mSpace.limit_r-mSpace.limit_l)/stacks;
	float radius = (mSpace.limit_r-mSpace.limit_l)/2;

	/// Generate Vertices and assign color
	for(unsigned short i = 0;i <= stacks; ++i)
	{	
		unsigned short j;
		float k;
		for(j = 0,k = 0;j <= slices; ++j,k+=angle_r)
		{
			if(j==slices)
			{
				g_VertexArray[(i*(slices+1))+j].p.x = g_VertexArray[i*(slices+1)].p.x;
				g_VertexArray[(i*(slices+1))+j].p.y = g_VertexArray[i*(slices+1)].p.y;
				g_VertexArray[(i*(slices+1))+j].p.z = g_VertexArray[i*(slices+1)].p.z;
				g_VertexArray[(i*(slices+1))+j].c.r = g_VertexArray[i*(slices+1)].c.r;
				g_VertexArray[(i*(slices+1))+j].c.g = g_VertexArray[i*(slices+1)].c.g;
				g_VertexArray[(i*(slices+1))+j].c.b = g_VertexArray[i*(slices+1)].c.b;
			}
			else
			{
				g_VertexArray[(i*(slices+1))+j].p.x = radius*cos(k);
				g_VertexArray[(i*(slices+1))+j].p.y = mSpace.limit_l+(inc_y*i);
				g_VertexArray[(i*(slices+1))+j].p.z = radius*sin(k)*(-1);
				g_VertexArray[(i*(slices+1))+j].c.r = red;
				g_VertexArray[(i*(slices+1))+j].c.g = green;
				g_VertexArray[(i*(slices+1))+j].c.b = blue;
			}
		}
	}
	g_VertexArray[totVertices-2].p.x = 0.0f;
	g_VertexArray[totVertices-2].p.y = mSpace.limit_l;
	g_VertexArray[totVertices-2].p.z = 0.0f;
	g_VertexArray[totVertices-2].c.r = red;
	g_VertexArray[totVertices-2].c.g = green;
	g_VertexArray[totVertices-2].c.b = blue;
	g_VertexArray[totVertices-1].p.x = 0.0f;
	g_VertexArray[totVertices-1].p.y = mSpace.limit_r;
	g_VertexArray[totVertices-1].p.z = 0.0f;
	g_VertexArray[totVertices-1].c.r = red;
	g_VertexArray[totVertices-1].c.g = green;
	g_VertexArray[totVertices-1].c.b = blue;

	unsigned short totVertStrips = totVertices-2;

	/// Generate indices that will be used to draw the TRIANGLE_STRIPS
	unsigned short indexToIndices = 0;
	for(unsigned short i = 0;i < ((totVertStrips-1)-slices);i+=(slices+1))
	{
		for(unsigned short k = 0;k <= slices;k++)
		{
			g_indices[indexToIndices] = k+i+(slices+1);
			++indexToIndices;
			g_indices[indexToIndices] = k+i;
			++indexToIndices;
		}
		if(i<((totVertStrips)-(2*(slices+1))))
		{
			g_indices[indexToIndices] = PRIMITIVE_RESTART_INDEX;
			++indexToIndices;
		}
	}
	g_indices[indexToIndices] = totVertices-2;
	++indexToIndices;

	/// Generate indices that will be used to draw the TRIANGLE_FANS
	for(unsigned short k = 0;k < totVertStrips;k+=((slices+1)*stacks))
	{
		for(unsigned short i = 0;i <= slices;++i)
		{
			if(k==0)
			{
				g_indices[indexToIndices] = k+(slices-i);
			}
			else
			{
				g_indices[indexToIndices] = k+i;
			}
			++indexToIndices;
		}
		if(k==0)
		{
			g_indices[indexToIndices] = PRIMITIVE_RESTART_INDEX;
			++indexToIndices;
			g_indices[indexToIndices] = totVertices-1;
			++indexToIndices;
		}
	}
}

/******************************************************************************/
/*!
Bind buffers for vertices and indices
\return
void
*/
/******************************************************************************/
void Cylinder::uploadGeometry()
{
	const int ColorOffset = 0;
	const int PositionOffset = sizeof( Color4 );
	const unsigned int POSITION_INDEX = 0;	
	const unsigned int COLOR0_INDEX = 3;
	/// Generate and bind vertex array object for Cylinder
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
	unsigned short NUMBER_OF_INDICES;
	NUMBER_OF_INDICES = NO_INDICES_STRIPS+NO_INDICES_FANS;
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(GLushort) * NUMBER_OF_INDICES,
		reinterpret_cast<const GLvoid*>( g_indices ),
		GL_STATIC_DRAW );
	(Logger::Instance()).checkAndReportGLError("Failed to provide buffer data for indices",'e',4,__LINE__,__FILE__);
}

/******************************************************************************/
/*!
Render the Cylinder
\return
void
*/
/******************************************************************************/
void Cylinder::drawGeometry()
{
	glBindVertexArray( g_vao[0] );
	glDrawElements( GL_TRIANGLE_STRIP, NO_INDICES_STRIPS, GL_UNSIGNED_SHORT, (void*)0 );
	glDrawElements( GL_TRIANGLE_FAN, NO_INDICES_FANS, GL_UNSIGNED_SHORT, BUFFER_OFFSET(sizeof(GLushort)*NO_INDICES_STRIPS));
	(Logger::Instance()).checkAndReportGLError("Failed to render the cylinder",'e',4,__LINE__,__FILE__);
}

/******************************************************************************/
/*!
Render the transformed Cylinder. Calls drawGeometry() internally
\return
void
*/
/******************************************************************************/
void Cylinder::drawTransformedGeometry(MatrixStack &mv_s)
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