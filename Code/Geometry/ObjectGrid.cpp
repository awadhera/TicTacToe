/******************************************************************************/
/*!
\file ObjectGrid.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Implementation of functions for Grid Class
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#include "ObjectGrid.h"

/******************************************************************************/
/*!
Constructor for Grid class
\param s_x
subdivisions along x axis
\param s_y
subdivisions along y axis
\param l
Modelspace left limit
\param r
Modelspace right limit
*/
/******************************************************************************/
Grid::Grid(const unsigned short &s_x,const unsigned short &s_y,const float &l,const float &r)
{
	mSpace.limit_l = l;
	mSpace.limit_r = r;
	subd_x = s_x;
	subd_y = s_y;
	/// Evaluate total number of vertices
	totVertices = ((subd_x+1)*(subd_y+1));
	g_VertexArray = new Vertex[totVertices];
	/// Evaluate total number of indices
	NUMBER_OF_INDICES = (((subd_x+1)*2)*subd_y)+subd_y+(((subd_y+1)*2)*subd_x)+subd_x-1;
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
void Grid::createGeometry(const unsigned char &red,const unsigned char &green,const unsigned char &blue)
{
	float inc_x = (mSpace.limit_r-mSpace.limit_l)/subd_x;
	float inc_y = (mSpace.limit_r-mSpace.limit_l)/subd_y;

	float inc_tex_x = 1.0f/subd_x;
	float inc_tex_y = 1.0f/subd_y;

	/// Generate Vertices and assign color for each vertex
	for(unsigned short j = 0;j <= subd_y; ++j)
	{
		for(unsigned short i = 0;i <= subd_x; ++i)
		{
			g_VertexArray[j*(subd_x+1)+i].p.x = mSpace.limit_l+(inc_x*i); 
			g_VertexArray[j*(subd_x+1)+i].p.y = mSpace.limit_l+(inc_y*j);
			g_VertexArray[j*(subd_x+1)+i].p.z = 0;
			g_VertexArray[j*(subd_x+1)+i].c.r = red;
			g_VertexArray[j*(subd_x+1)+i].c.g = green;
			g_VertexArray[j*(subd_x+1)+i].c.b = blue;
			g_VertexArray[j*(subd_x+1)+i].tc.s = (inc_tex_x*i);
			g_VertexArray[j*(subd_x+1)+i].tc.t = (inc_tex_y*j);
		}
	}

	/// Generate Indices for drawing this object through TRIANGLE_STRIPS
	unsigned short indexToIndices = 0;
	for(unsigned short i = 0;i < subd_x; ++i)
	{
		for(unsigned short j = 0;j <= subd_y; ++j)
		{
			g_indices[indexToIndices] = ((subd_x+1)*j)+i;
			++indexToIndices;
			g_indices[indexToIndices] = g_indices[indexToIndices-1]+1;
			++indexToIndices;
		}
		g_indices[indexToIndices] = PRIMITIVE_RESTART_INDEX;
		++indexToIndices;
	}
	for(unsigned short i = 0;i < subd_y;++i)
	{
		for(unsigned short j = 0;j <= subd_x;++j)
		{
			g_indices[indexToIndices] = (i*(subd_x+1))+j;
			++indexToIndices;
			g_indices[indexToIndices] = g_indices[indexToIndices-1]+(subd_x+1);
			++indexToIndices;
		}
		if(i!=(subd_y-1))
		{
			g_indices[indexToIndices] = PRIMITIVE_RESTART_INDEX;
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
void Grid::uploadGeometry()
{
	const int ColorOffset = 0;
	const int PositionOffset = sizeof( Color4 );
	const int TexCoordOffset = sizeof( Color4 ) + sizeof( Point );
	const unsigned int POSITION_INDEX = 0;	
	const unsigned int COLOR0_INDEX = 3;
	const unsigned int TEXCOORD_INDEX = 8;
	/// Generate and bind vertex array object for the grid
	glGenVertexArrays(1, g_vao);
	(Logger::Instance()).checkAndReportGLError("Failed to generate Vertex Arrays",'e',4,__LINE__,__FILE__);
	glBindVertexArray( g_vao[0] );
	(Logger::Instance()).checkAndReportGLError("Failed to bind Vertex Arrays",'e',4,__LINE__,__FILE__);
	/// Generate buffer objects for vertices and indices

	glGenTextures(1, texName);
	(Logger::Instance()).checkAndReportGLError(" failed to gen textures",'e',4,__LINE__,__FILE__);
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	(Logger::Instance()).checkAndReportGLError(" fail 2d ",'e',4,__LINE__,__FILE__);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	(Logger::Instance()).checkAndReportGLError(" fail wrap s",'e',4,__LINE__,__FILE__);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	(Logger::Instance()).checkAndReportGLError(" fail wrap t",'e',4,__LINE__,__FILE__);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	if(tgaObj.pixelSizeInBytes == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tgaObj.width,
			tgaObj.height, 0, GL_BGR, GL_UNSIGNED_BYTE,
			tgaObj.data);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tgaObj.width,
			tgaObj.height, 0, GL_BGRA, GL_UNSIGNED_BYTE,
			tgaObj.data);
	}
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
	glVertexAttribPointer(
		TEXCOORD_INDEX,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof( Vertex ),
		BUFFER_OFFSET( TexCoordOffset ) );
	(Logger::Instance()).checkAndReportGLError("Failed to run glVertexAttribPointer for TexCoord",'e',4,__LINE__,__FILE__);
	glEnableVertexAttribArray( TEXCOORD_INDEX );
	(Logger::Instance()).checkAndReportGLError("Failed to enable VertexAttribArray for TexCoord",'e',4,__LINE__,__FILE__);
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
Render the GRID
\return
void
*/
/******************************************************************************/
void Grid::drawGeometry()
{
	glBindVertexArray( g_vao[0] );
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glDrawElements( GL_TRIANGLE_STRIP, NUMBER_OF_INDICES, GL_UNSIGNED_SHORT, (void*)0 );
	(Logger::Instance()).checkAndReportGLError("Failed to render the grid",'e',4,__LINE__,__FILE__);
}

/******************************************************************************/
/*!
Render the transformed GRID. Calls drawGeometry() internally
\return
void
*/
/******************************************************************************/
void Grid::drawTransformedGeometry(MatrixStack &mv_s)
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
Destructor for Grid class
*/
/******************************************************************************/
Grid::~Grid()
{
}

/******************************************************************************/
/*!
Unbind buffers and delete vertex and index data
\return
void
*/
/******************************************************************************/
void Grid::destroyGeometry()
{
	/// Delete vertex array object
	glDeleteVertexArrays( 0 , g_vao );
	(Logger::Instance()).checkAndReportGLError("Failed to delete Vertex array",'e',4,__LINE__,__FILE__);
	/// Delete Vertex buffer object and Index buffer object
	glDeleteBuffers( NUM_BUFFERS, vbo );
	(Logger::Instance()).checkAndReportGLError("Failed to delete buffers",'e',4,__LINE__,__FILE__);
	delete [] g_VertexArray;
	delete [] g_indices; 
} 

/******************************************************************************/
/*!
Add Texture to the grid
\param imageFile
char pointer pointing to image file name
\return
bool
*/
/******************************************************************************/
bool Grid::addTexture(char *imageFile)
{
	std::ifstream in;
	in.open(imageFile,std::ios::binary);
	if(in.fail())
	{
		return false;
	}
	char *imageType = new char[3];
	in.read(imageType,3);
	in.seekg (9, std::ios::cur);
	char *imageInfo = new char[6];
	in.read(imageInfo,6);
	if(imageType[1]!=0 || (imageType[2]!=2 && imageType[3]!=3))
	{
		delete [] imageType;
		delete [] imageInfo;
		in.close();
		return false;
	}
	tgaObj.width = imageInfo[0] + imageInfo[1]*256;
	tgaObj.height = imageInfo[2] + imageInfo[3]*256;
	tgaObj.pixelSizeInBytes = imageInfo[4]/8;
	switch(tgaObj.pixelSizeInBytes)
	{
	case 3:
		tgaObj.cType = RGB;
		break;
	case 4:
		tgaObj.cType = RGBA;
		break;
	default:
		delete [] imageType;
		delete [] imageInfo;
		in.close();
		return false;
	}
	unsigned long imageSize = tgaObj.width * tgaObj.height * tgaObj.pixelSizeInBytes;
	tgaObj.data = new char[imageSize];
	in.read(tgaObj.data,imageSize);
	if(in.fail())
	{
		return false;
	}
	delete [] imageType;
	delete [] imageInfo;
	in.close();
	return true;
}