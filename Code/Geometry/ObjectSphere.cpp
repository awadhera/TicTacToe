/******************************************************************************/
/*!
\file ObjectSphere.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1250
\par Project #01
\date 02/08/2012
\brief
Implementation of functions for Sphere class
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#include "ObjectSphere.h"

/******************************************************************************/
/*!
Constructor for Sphere class
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
Sphere::Sphere(const unsigned short &stk,const unsigned short &sli,const float &l,const float &r)
{	
	mSpace.limit_l = l;
	mSpace.limit_r = r;
	stacks = stk;
	slices = sli;
	/// Total number of vertices
	totVertices = ((slices+1)*(stacks-1))+2;
	g_VertexArray = new Vertex[totVertices];
	/// Total number of indices used for drawing strips and fans
	/// If stacks are just 2 - only triangle fans are used. 
	/// If stacks are more than 2 - only triangle strips are used as well
	if(stacks!=2)
	{
		NO_INDICES_STRIPS = (((((slices+1)*2)+1)*(stacks-2))-1);
	}
	else
	{
		NO_INDICES_STRIPS = 0;
	}
	NO_INDICES_FANS = (2*(slices+1))+3;
	g_indices = new GLushort[NO_INDICES_STRIPS+NO_INDICES_FANS];
}

/******************************************************************************/
/*!
Destructor for Sphere class
*/
/******************************************************************************/
Sphere::~Sphere()
{
}

/******************************************************************************/
/*!
Unbind buffers and delete vertex and index data
\return
void
*/
/******************************************************************************/
void Sphere::destroyGeometry()
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
void Sphere::createGeometry(const unsigned char &red,const unsigned char &green,const unsigned char &blue)
{
	float angle_d = 360.0f/slices;
	float angle_r = (angle_d*M_PI)/180.0f;
	float inc_y = (mSpace.limit_r-mSpace.limit_l)/stacks;
	float radius = (mSpace.limit_r-mSpace.limit_l)/2;
	float radTrav = 0.0f;

	/// Generate Vertices and assign color for the sphere
	for(unsigned short i = 0;i <= stacks; ++i)
	{	
		/// Generate Vertices for the tip and bottom of the sphere
		if(i==0)
		{
			g_VertexArray[i].p.x = 0.0f;
			g_VertexArray[i].p.y = mSpace.limit_l;
			g_VertexArray[i].p.z = 0.0f;
			g_VertexArray[i].c.r = red;
			g_VertexArray[i].c.g = green;
			g_VertexArray[i].c.b = blue;
		}
		else if(i==stacks)
		{
			g_VertexArray[totVertices-1].p.x = 0.0f;
			g_VertexArray[totVertices-1].p.y = mSpace.limit_r;
			g_VertexArray[totVertices-1].p.z= 0.0f;
			g_VertexArray[totVertices-1].c.r = red;
			g_VertexArray[totVertices-1].c.g = green;
			g_VertexArray[totVertices-1].c.b = blue;
		}
		/// Generate Vertices for the rest of the sphere
		else
		{
			unsigned short j;
			float k;
			for(j = 1,k = 0.0f;j <= (slices+1); ++j,k+=angle_r)
			{
				if(j==(slices+1))
				{
					g_VertexArray[((i-1)*(slices+1))+j].p.x = g_VertexArray[((i-1)*(slices+1))+1].p.x;
					g_VertexArray[((i-1)*(slices+1))+j].p.y = g_VertexArray[((i-1)*(slices+1))+1].p.y;
					g_VertexArray[((i-1)*(slices+1))+j].p.z = g_VertexArray[((i-1)*(slices+1))+1].p.z;
					g_VertexArray[((i-1)*(slices+1))+j].c = g_VertexArray[((i-1)*(slices+1))+1].c;
				}
				else
				{
					g_VertexArray[((i-1)*(slices+1))+j].p.x = radTrav*cos(k);
					g_VertexArray[((i-1)*(slices+1))+j].p.y = mSpace.limit_l+(inc_y*i);
					g_VertexArray[((i-1)*(slices+1))+j].p.z = radTrav*sin(k)*(-1);
					g_VertexArray[((i-1)*(slices+1))+j].c.r = red;
					g_VertexArray[((i-1)*(slices+1))+j].c.g = green;
					g_VertexArray[((i-1)*(slices+1))+j].c.b = blue;
				}
			}
		}
		if(i!=stacks) 
		{
			radTrav = sqrt((radius*radius)-((mSpace.limit_l+(inc_y*(i+1)))*(mSpace.limit_l+(inc_y*(i+1)))));
		}
	}
	unsigned short indexToIndices = 0;
	/// If number of stacks is 2, only triangle fans are used. Triangle strips are used along with
	/// triangle fans if number of stacks is more than 2
	if(stacks!=2)
	{
		/// Generate indices that will be used to draw the TRIANGLE_STRIPS
		unsigned short totVertStrips = totVertices-2;
		for(unsigned short i = 1;i < ((totVertStrips)-slices);i+=(slices+1))
		{
			for(unsigned short k = 0;k <= slices;k++)
			{
				g_indices[indexToIndices] = i+k+(slices+1);
				++indexToIndices;
				g_indices[indexToIndices] = i+k;
				++indexToIndices;
			}
			if(i<((totVertStrips)-(2*(slices+1))))
			{
				g_indices[indexToIndices] = PRIMITIVE_RESTART_INDEX;
				++indexToIndices;
			}
		}
		g_indices[indexToIndices] = 0;
		++indexToIndices;

		/// Generate indices that will be used to draw the TRIANGLE_FANS
		for(unsigned short k = 0;k < totVertStrips;k+=(((slices+1)*(stacks-2))+1))
		{
			for(unsigned short i = 1;i <= (slices+1);++i)
			{
				if(k==0)
				{
					g_indices[indexToIndices] = k+((slices+1)-(i-1));
				}
				else
				{
					g_indices[indexToIndices] = k+(i-1);
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
	else
	{
		/// If stacks is 2 - Generate indices for the triangle fans only
		g_indices[indexToIndices] = 0;
		++indexToIndices;
		for(unsigned short k=0;k<2;++k)
		{
			for(unsigned short i = 1;i <= (slices+1);++i)
			{	
				if(k==0)
				{
					g_indices[indexToIndices] = k+((slices+1)-(i-1));
				}
				else
				{
					g_indices[indexToIndices] = k+(i-1);
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
}

/******************************************************************************/
/*!
Bind buffers for vertices and indices
\return
void
*/
/******************************************************************************/
void Sphere::uploadGeometry()
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
	unsigned short NUMBER_OF_INDICES;
	if(stacks!=2)
	{
		NUMBER_OF_INDICES = NO_INDICES_STRIPS+NO_INDICES_FANS;
	}
	else
	{
		NUMBER_OF_INDICES = NO_INDICES_FANS;
	}
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(GLushort) * NUMBER_OF_INDICES,
		reinterpret_cast<const GLvoid*>( g_indices ),
		GL_STATIC_DRAW );
	(Logger::Instance()).checkAndReportGLError("Failed to provide buffer data for indices",'e',4,__LINE__,__FILE__);
}

/******************************************************************************/
/*!
Render the Sphere
\return
void
*/
/******************************************************************************/
void Sphere::drawGeometry()
{
	glBindVertexArray( g_vao[0] );
	if(stacks!=2)
	{
		glDrawElements( GL_TRIANGLE_STRIP, NO_INDICES_STRIPS, GL_UNSIGNED_SHORT, (void*)0 );
		glDrawElements( GL_TRIANGLE_FAN, NO_INDICES_FANS, GL_UNSIGNED_SHORT, BUFFER_OFFSET(sizeof(GLushort)*NO_INDICES_STRIPS));
	}
	else
	{
		glDrawElements( GL_TRIANGLE_FAN, NO_INDICES_FANS, GL_UNSIGNED_SHORT, (void*)0);
	}
	(Logger::Instance()).checkAndReportGLError("Failed to render the sphere",'e',4,__LINE__,__FILE__);
}

/******************************************************************************/
/*!
Render the transformed Sphere. Calls drawGeometry() internally
\param [in] mv_s
Reference to modelview stack object
\return
void
*/
/******************************************************************************/
void Sphere::drawTransformedGeometry(MatrixStack &mv_s)
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