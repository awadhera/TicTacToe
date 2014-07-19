/******************************************************************************/
/*!
\file MatrixStack.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Interface for handling stack of matrices
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include<GL/glew.h>
#include<vector>
#include "Matrix.h"

//!  MatrixStack class
/*!
Class for implementing MatrixStack operations
*/
class MatrixStack
{
	std::vector<Matrix> stack;	/*!< stack defined as vector of elements of type Matrix */
public:
	void PushMatrix();
	void PopMatrix();
	void LoadMatrix(const Matrix &n);
	void MultMatrix(const Matrix &n);
	void LoadIdentity();
	float * last();
	Matrix top();
};



#endif