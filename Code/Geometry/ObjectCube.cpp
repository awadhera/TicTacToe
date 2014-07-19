/******************************************************************************/
/*!
\file ObjectCube.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1050
\par Project #01
\date 21/12/2011
\brief
Implementation of functions for Cube Class
Copyright (C) 2011 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/
#include "ObjectCube.h"

/******************************************************************************/
/*!
Constructor for Cube class
\param sd
subdivisions in the cube
\param l
Modelspace left limit
\param r
Modelspace right limit
*/
/******************************************************************************/
Cube::Cube(const unsigned short &sd,const float &l,const float &r)
{
	mSpace.limit_l = l;
	mSpace.limit_r = r;
	subd = sd;
	/// Total number of vertices
	totVertices = ((subd+1)*(subd+1)*6);
	g_VertexArray = new Vertex[totVertices];
	/// Total number of indices
	NUMBER_OF_INDICES = (((((subd+1)*2)*subd)+subd)*6)-1;
	g_indices = new GLushort[NUMBER_OF_INDICES];
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
void Cube::createGeometry(const unsigned char &red,const unsigned char &green,const unsigned char &blue)
{
	float inc = (mSpace.limit_r-mSpace.limit_l)/subd;
	Vertex *storeVertex = new Vertex[(subd+1)*(subd+1)];
	unsigned long ind = 0;

	/// Generate Vertices and assign color for a grid
	for(unsigned short j = 0;j <= subd; ++j)
	{
		for(unsigned short i = 0;i <= subd; ++i)
		{
			storeVertex[(j*(subd+1))+i].p.x = mSpace.limit_l+(inc*i); 
			storeVertex[(j*(subd+1))+i].p.y = mSpace.limit_l+(inc*j);
			storeVertex[(j*(subd+1))+i].p.z = 0.0f;
			storeVertex[(j*(subd+1))+i].c.r = red;
			storeVertex[(j*(subd+1))+i].c.b = blue;
			storeVertex[(j*(subd+1))+i].c.g = green;
		}
	}

	/// Transform and use this grid as different faces of the cube
	Matrix transform;

	transform.MultMatrix(my_Translate(0.0f,0.0f,mSpace.limit_r));
	transformGridInCube(transform,storeVertex,((subd+1)*(subd+1)),g_VertexArray,ind);

	transform.setIdentity();
	transform.MultMatrix(my_Translate(0.0f,0.0f,mSpace.limit_l));
	transform.MultMatrix(my_Rotate(180.0f, 0.0f, 1.0f, 0.0f));
	transformGridInCube(transform,storeVertex,((subd+1)*(subd+1)),g_VertexArray,ind);

	transform.setIdentity();
	transform.MultMatrix(my_Translate(0.0f,mSpace.limit_r,0.0f));
	transform.MultMatrix(my_Rotate(-90.0f, 1.0f, 0.0f, 0.0f));
	transformGridInCube(transform,storeVertex,((subd+1)*(subd+1)),g_VertexArray,ind);

	transform.setIdentity();
	transform.MultMatrix(my_Translate(0.0f,mSpace.limit_l,0.0f));
	transform.MultMatrix(my_Rotate(90.0f, 1.0f, 0.0f, 0.0f));
	transformGridInCube(transform,storeVertex,((subd+1)*(subd+1)),g_VertexArray,ind);

	transform.setIdentity();
	transform.MultMatrix(my_Translate(mSpace.limit_l,0.0f,0.0f));
	transform.MultMatrix(my_Rotate(-90.0f, 0.0f, 1.0f, 0.0f));
	transformGridInCube(transform,storeVertex,((subd+1)*(subd+1)),g_VertexArray,ind);

	transform.setIdentity();
	transform.MultMatrix(my_Translate(mSpace.limit_r,0.0f,0.0f));
	transform.MultMatrix(my_Rotate(90.0f, 0.0f, 1.0f, 0.0f));
	transformGridInCube(transform,storeVertex,((subd+1)*(subd+1)),g_VertexArray,ind);

	/// Generate Indices for drawing this object through TRIANGLE_STRIPS
	unsigned short indexToIndices = 0;
	for(unsigned short k = 0;k < totVertices; k+=((subd+1)*(subd+1)))
	{
		for(unsigned short i = 0;i < subd; ++i)
		{
			for(unsigned short j = 0;j <= subd; ++j)
			{
				g_indices[indexToIndices] = k+i+((subd+1)*j);
				++indexToIndices;
				g_indices[indexToIndices] = g_indices[indexToIndices-1]+1;
				++indexToIndices;
			}
			if(k>=(totVertices-((subd+1)*(subd+1)))&&(i==subd-1))
			{
				continue;
			}
			else
			{
				g_indices[indexToIndices] = PRIMITIVE_RESTART_INDEX;
				++indexToIndices;
			}
		}
	}
	delete [] storeVertex;
}

/******************************************************************************/
/*!
Bind buffers for vertices and indices
\return
void
*/
/******************************************************************************/
void Cube::uploadGeometry()
{
	const int ColorOffset = 0;
	const int PositionOffset = sizeof( Color4 );
	const unsigned int POSITION_INDEX = 0;	
	const unsigned int COLOR0_INDEX = 3;
	/// Generate and bind vertex array object for the Cube
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
Render the Cube
\return
void
*/
/******************************************************************************/
void Cube::drawGeometry()
{
	glBindVertexArray( g_vao[0] );
	glDrawElements( GL_TRIANGLE_STRIP, NUMBER_OF_INDICES, GL_UNSIGNED_SHORT, (void*)0 );
	(Logger::Instance()).checkAndReportGLError("Failed to render the cube",'e',4,__LINE__,__FILE__);
}

/******************************************************************************/
/*!
Render the transformed Cube. Calls drawGeometry() internally
\return
void
*/
/******************************************************************************/
void Cube::drawTransformedGeometry(MatrixStack &mv_s)
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

/******************************************************************************/
/*!
Destructor for Cube class
*/
/******************************************************************************/
Cube::~Cube()
{

}

/******************************************************************************/
/*!
Unbind buffers and delete vertex and index data
\return
void
*/
/******************************************************************************/
void Cube::destroyGeometry()
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
Transforms a GRID to form different faces of the cube
\param m
Reference to Matrix. Represents an affine transform
\param storeVertex
Vertex array for a grid
\param size
Size of the Vertex array for a grid
\param g_Vertex_Array
Vertex array for the cube
\param index
Starting index of the vertex array of cube where the vertices of the grid
will be inserted
\return
void
*/
/******************************************************************************/
void Cube::transformGridInCube(const Matrix &m,Vertex *storeVertex,const unsigned long &size,Vertex *g_Vertex_Array,unsigned long &index)
{
	/// Backup the transform
	Matrix n = m;
	for(unsigned long i = 0;i < size; ++i)
	{
		/// Form a Point struct for a vertex in grid
		Point p(storeVertex[i].p.x,storeVertex[i].p.y,storeVertex[i].p.z);
		/// transform this point
		n.MultMatrix(p);
		/// Insert the transformed point as a vertex to the cube
		g_Vertex_Array[index].p.x = n.t[0][0];
		g_Vertex_Array[index].p.y = n.t[0][1];
		g_Vertex_Array[index].p.z = n.t[0][2];
		g_Vertex_Array[index].c = storeVertex[i].c;
		++index;
		/// Restore the transform
		n = m;
	}
}