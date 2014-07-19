/******************************************************************************/
/*!
\file Matrix.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Implementation of Matrix related operations
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#include "Matrix.h"

/******************************************************************************/
/*!
Set the Matrix to Identity
\return
void
*/
/******************************************************************************/
void Matrix::setIdentity()
{
	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			t[i][j] = (i==j)?1.0f:0.0f;			
		}
	}
}

/******************************************************************************/
/*!
Constructor for Matrix class
*/
/******************************************************************************/
Matrix::Matrix()
{
	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			t[i][j] = (i==j)?1.0f:0.0f;			
		}
	}
}

/******************************************************************************/
/*!
Multiply the matrix by a vector
\param v
Reference to Vector
\return
void
*/
/******************************************************************************/
void Matrix::MultMatrix(const Vector &v)
{
	float vMatrix[4] = {v.x,v.y,v.z,0};
	MultMatrix(vMatrix,4);
}

/******************************************************************************/
/*!
Multiply the matrix by a point
\param p
Reference to Point
\return
void
*/
/******************************************************************************/
void Matrix::MultMatrix(const Point &p)
{
	float vMatrix[4] = {p.x,p.y,p.z,1};
	MultMatrix(vMatrix,4);
}

/******************************************************************************/
/*!
Multiply the matrix by another matrix represented by array of floats
\param right
Pointer to float
\param size
Size of the float array pointed by right
\return
void
*/
/******************************************************************************/
void Matrix::MultMatrix(const float *right,const unsigned int &size)
{
	float add[4][4];
	unsigned int row = size/4;
	for(unsigned short i = 0;i < row; ++i)
	{
		for(unsigned short j = 0;j < 4; ++j)
		{
			add[i][j] = 0;
			for(unsigned short k = 0;k < 4; ++k)
			{
				add[i][j]+=(t[k][j]*(right[(i*4)+k]));
			}
		}
	}
	for(unsigned short i = 0;i < row; ++i)
	{
		for(unsigned short j = 0;j < 4; ++j)
		{
			t[i][j] = add[i][j];
		}
	}

}

/******************************************************************************/
/*!
Multiply the matrix by another matrix
\param n
Reference to a constant of type Matrix
\return
void
*/
/******************************************************************************/
void Matrix::MultMatrix(const Matrix &n)
{
	float add[4][4];
	for(unsigned int i = 0;i < 4; ++i)
	{
		for(unsigned int j = 0;j < 4; ++j)
		{
			add[i][j] = 0;
			for(unsigned int k = 0; k < 4; k++)
			{
				add[i][j]+=(t[k][j]*n.t[i][k]);
			}
		}
	}
	for(unsigned short i = 0;i < 4; ++i)
	{
		for(unsigned short j = 0;j < 4; ++j)
		{
			t[i][j] = add[i][j];
		}
	}

}

/******************************************************************************/
/*!
Parameterized constructor of class Matrix
\param load
Pointer to array of 16 floats
\return
void
*/
/******************************************************************************/
Matrix::Matrix(const float *load)
{
	for(unsigned int i = 0;i < 4; ++i)
	{
		for(unsigned int j = 0;j< 4; ++j)
		{
			t[i][j] = load[(i*4)+j];
		}
	}
}

/******************************************************************************/
/*!
Evaluate the determinant of Matrix
\return
float
*/
/******************************************************************************/
float Matrix::Determinant() const
{
	return(t[0][0]*t[1][1]*t[2][2]*t[3][3]+t[0][0]*t[1][2]*t[2][3]*t[3][1]+t[0][0]*t[1][3]*t[2][1]*t[3][2]+
		t[0][1]*t[1][0]*t[2][3]*t[3][2]+t[0][1]*t[1][2]*t[2][0]*t[3][3]+t[0][1]*t[1][3]*t[2][2]*t[3][0]+
		t[0][2]*t[1][0]*t[2][1]*t[3][3]+t[0][2]*t[1][1]*t[2][3]*t[3][0]+t[0][2]*t[1][3]*t[2][0]*t[3][1]+
		t[0][3]*t[1][0]*t[2][2]*t[3][1]+t[0][3]*t[1][1]*t[2][0]*t[3][2]+t[0][3]*t[1][2]*t[2][1]*t[3][0]-
		t[0][0]*t[1][1]*t[2][3]*t[3][2]-t[0][0]*t[1][2]*t[2][1]*t[3][3]-t[0][0]*t[1][3]*t[2][2]*t[3][1]-
		t[0][1]*t[1][0]*t[2][2]*t[3][3]-t[0][1]*t[1][2]*t[2][3]*t[3][0]-t[0][1]*t[1][3]*t[2][0]*t[3][2]-	
		t[0][2]*t[1][0]*t[2][3]*t[3][1]-t[0][2]*t[1][1]*t[2][0]*t[3][3]-t[0][2]*t[1][3]*t[2][1]*t[3][0]-
		t[0][3]*t[1][0]*t[2][1]*t[3][2]-t[0][3]*t[1][1]*t[2][2]*t[3][0]-t[0][3]*t[1][2]*t[2][0]*t[3][1]);
}

/******************************************************************************/
/*!
Evaluate the transpose of the matrix
\param [in] in
Reference to a constant of type Matrix
\param [out] out
Reference to type Matrix - Holds the transpose
\return
void
/******************************************************************************/
void Transpose(const Matrix &in,Matrix &out)
{
	for(unsigned int i = 0; i < 4; ++i)
	{
		for(unsigned int j = 0; j < 4; ++j)
		{
			out.t[i][j] = in.t[j][i];
		}
	}
}

/******************************************************************************/
/*!
Evaluate the inverse of the matrix
\param [in] in
Reference to a constant of type Matrix
\param [out] out
Reference to type Matrix - Holds the inverse
\return
void
/******************************************************************************/
bool Inverse(const Matrix &in,Matrix &out)
{
	float det = in.Determinant();
	if(det==0)
		return false;
	else
	{
		out.t[0][0] = (in.t[1][1]*in.t[2][2]*in.t[3][3]+in.t[1][2]*in.t[2][3]*in.t[3][1]+in.t[1][3]*in.t[2][1]*in.t[3][2]-in.t[1][1]*in.t[2][3]*in.t[3][2]-in.t[1][2]*in.t[2][1]*in.t[3][3]-in.t[1][3]*in.t[2][2]*in.t[3][1])/det;
		out.t[0][1] = (in.t[0][1]*in.t[2][3]*in.t[3][2]+in.t[0][2]*in.t[2][1]*in.t[3][3]+in.t[0][3]*in.t[2][2]*in.t[3][1]-in.t[0][1]*in.t[2][2]*in.t[3][3]-in.t[0][2]*in.t[2][3]*in.t[3][1]-in.t[0][3]*in.t[2][1]*in.t[3][2])/det;
		out.t[0][2] = (in.t[0][1]*in.t[1][2]*in.t[3][3]+in.t[0][2]*in.t[1][3]*in.t[3][1]+in.t[0][3]*in.t[1][1]*in.t[3][2]-in.t[0][1]*in.t[1][3]*in.t[3][2]-in.t[0][2]*in.t[1][1]*in.t[3][3]-in.t[0][3]*in.t[1][2]*in.t[3][1])/det;
		out.t[0][3] = (in.t[0][1]*in.t[1][3]*in.t[2][2]+in.t[0][2]*in.t[1][1]*in.t[2][3]+in.t[0][3]*in.t[1][2]*in.t[2][1]-in.t[0][1]*in.t[1][2]*in.t[2][3]-in.t[0][2]*in.t[1][3]*in.t[2][1]-in.t[0][3]*in.t[1][1]*in.t[2][2])/det;
		out.t[1][0] = (in.t[1][0]*in.t[2][3]*in.t[3][2]+in.t[1][2]*in.t[2][0]*in.t[3][3]+in.t[1][3]*in.t[2][2]*in.t[3][0]-in.t[1][0]*in.t[2][2]*in.t[3][3]-in.t[1][2]*in.t[2][3]*in.t[3][0]-in.t[1][3]*in.t[2][0]*in.t[3][2])/det;
		out.t[1][1] = (in.t[0][0]*in.t[2][2]*in.t[3][3]+in.t[0][2]*in.t[2][3]*in.t[3][0]+in.t[0][3]*in.t[2][0]*in.t[3][2]-in.t[0][0]*in.t[2][3]*in.t[3][2]-in.t[0][2]*in.t[2][0]*in.t[3][3]-in.t[0][3]*in.t[2][2]*in.t[3][0])/det;
		out.t[1][2] = (in.t[0][0]*in.t[1][3]*in.t[3][2]+in.t[0][2]*in.t[1][0]*in.t[3][3]+in.t[0][3]*in.t[1][2]*in.t[3][0]-in.t[0][0]*in.t[1][2]*in.t[3][3]-in.t[0][2]*in.t[1][3]*in.t[3][0]-in.t[0][3]*in.t[1][0]*in.t[3][2])/det;
		out.t[1][3] = (in.t[0][0]*in.t[1][2]*in.t[2][3]+in.t[0][2]*in.t[1][3]*in.t[2][0]+in.t[0][3]*in.t[1][0]*in.t[2][2]-in.t[0][0]*in.t[1][3]*in.t[2][2]-in.t[0][2]*in.t[1][0]*in.t[2][3]-in.t[0][3]*in.t[1][2]*in.t[2][0])/det;
		out.t[2][0] = (in.t[1][0]*in.t[2][1]*in.t[3][3]+in.t[1][1]*in.t[2][3]*in.t[3][0]+in.t[1][3]*in.t[2][0]*in.t[3][1]-in.t[1][0]*in.t[2][3]*in.t[3][1]-in.t[1][1]*in.t[2][0]*in.t[3][3]-in.t[1][3]*in.t[2][1]*in.t[3][0])/det;
		out.t[2][1] = (in.t[0][0]*in.t[2][3]*in.t[3][1]+in.t[0][1]*in.t[2][0]*in.t[3][3]+in.t[0][3]*in.t[2][1]*in.t[3][0]-in.t[0][0]*in.t[2][1]*in.t[3][3]-in.t[0][1]*in.t[2][3]*in.t[3][0]-in.t[0][3]*in.t[2][0]*in.t[3][1])/det;
		out.t[2][2] = (in.t[0][0]*in.t[1][1]*in.t[3][3]+in.t[0][1]*in.t[1][3]*in.t[3][0]+in.t[0][3]*in.t[1][0]*in.t[3][1]-in.t[0][0]*in.t[1][3]*in.t[3][1]-in.t[0][1]*in.t[1][0]*in.t[3][3]-in.t[0][3]*in.t[1][1]*in.t[3][0])/det;
		out.t[2][3] = (in.t[0][0]*in.t[1][3]*in.t[2][1]+in.t[0][1]*in.t[1][0]*in.t[2][3]+in.t[0][3]*in.t[1][1]*in.t[2][0]-in.t[0][0]*in.t[1][1]*in.t[2][3]-in.t[0][1]*in.t[1][3]*in.t[2][0]-in.t[0][3]*in.t[1][0]*in.t[2][1])/det;
		out.t[3][0] = (in.t[1][0]*in.t[2][2]*in.t[3][1]+in.t[1][1]*in.t[2][0]*in.t[3][2]+in.t[1][2]*in.t[2][1]*in.t[3][0]-in.t[1][0]*in.t[2][1]*in.t[3][2]-in.t[1][1]*in.t[2][2]*in.t[3][0]-in.t[1][2]*in.t[2][0]*in.t[3][1])/det;
		out.t[3][1] = (in.t[0][0]*in.t[2][1]*in.t[3][2]+in.t[0][1]*in.t[2][2]*in.t[3][0]+in.t[0][2]*in.t[2][0]*in.t[3][1]-in.t[0][0]*in.t[2][2]*in.t[3][1]-in.t[0][1]*in.t[2][0]*in.t[3][2]-in.t[0][2]*in.t[2][1]*in.t[3][0])/det;
		out.t[3][2] = (in.t[0][0]*in.t[1][2]*in.t[3][1]+in.t[0][1]*in.t[1][0]*in.t[3][2]+in.t[0][2]*in.t[1][1]*in.t[3][0]-in.t[0][0]*in.t[1][1]*in.t[3][2]-in.t[0][1]*in.t[1][2]*in.t[3][0]-in.t[0][2]*in.t[1][0]*in.t[3][1])/det;
		out.t[3][3] = (in.t[0][0]*in.t[1][1]*in.t[2][2]+in.t[0][1]*in.t[1][2]*in.t[2][0]+in.t[0][2]*in.t[1][0]*in.t[2][1]-in.t[0][0]*in.t[1][2]*in.t[2][1]-in.t[0][1]*in.t[1][0]*in.t[2][2]-in.t[0][2]*in.t[1][1]*in.t[2][0])/det;
		return true;
	}
}