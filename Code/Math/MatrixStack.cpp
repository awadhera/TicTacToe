/******************************************************************************/
/*!
\file MatrixStack.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Implementation of Matrix Stack related operations
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#include "MatrixStack.h"

/******************************************************************************/
/*!
Push the top element on the stack
\return
void
*/
/******************************************************************************/
void MatrixStack::PushMatrix()
{
	if(stack.size()!=0)
	{
		stack.push_back(stack.back());
	}
}

/******************************************************************************/
/*!
Pop the top element from the stack
\return
void
*/
/******************************************************************************/
void MatrixStack::PopMatrix()
{
	if(stack.size()!=0)
	{
		stack.pop_back();
	}
}

/******************************************************************************/
/*!
Load the argument matrix on top of stack
\param n
Reference to type Matrix
\return
void
*/
/******************************************************************************/
void MatrixStack::LoadMatrix(const Matrix &n)
{
	if(stack.size() == 0)
	{
		stack.push_back(n);
	}
	else
	{
		stack.back() = n;
	}
}

/******************************************************************************/
/*!
Multiply the top of matrix with argument matrix
\param n
Reference to a constant of type:Matrix
\return
void
*/
/******************************************************************************/
void MatrixStack::MultMatrix(const Matrix &n)
{
	Matrix t = stack.back();
	t.MultMatrix(n);
	stack.back() = t;
}

/******************************************************************************/
/*!
Load the Identity matrix on top of stack
\return
void
*/
/******************************************************************************/
void MatrixStack::LoadIdentity()
{
	float ID[16] = {1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0,
		0.0,0.0,0.0,1.0};
	Matrix t(ID);
	LoadMatrix(t);
}

/******************************************************************************/
/*!
Return the top element of the stack as a pointer to an array of 16 floats
\return
Pointer to an array of 16 floats
*/
/******************************************************************************/
GLfloat * MatrixStack::last()
{
	return &(stack.back().t[0][0]);
}

/******************************************************************************/
/*!
Return the top element of the stack
\return
Instance of type Matrix
*/
/******************************************************************************/
Matrix MatrixStack::top()
{
	return stack.back();
}