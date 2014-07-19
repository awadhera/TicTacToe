/******************************************************************************/
/*!
\file Transforms.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Interface for transform classes
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef TRANSFORMS_H
#define TRANSFORMS_H

#include "../Math/Matrix.h"
#include<vector>

//!  SingleTransform class
/*!
Class for implmenting a single transformation
*/
class SingleTransform
{
public:
	virtual void update();
	virtual Matrix returnMatrix() const = 0; /*!< Pure virtual function for returning a matrix */
	virtual ~SingleTransform();
};

//!  Rotate class
/*!
Class for implmenting a rotation transformation
*/
class Rotate : public SingleTransform
{
	Matrix m;	/*!< Variable of type Matrix */
	float angle; /*!< Angle of rotation */
	float x; /*!< x component of axis of rotation */
	float y; /*!< y component of axis of rotation */
	float z; /*!< z component of axis of rotation */
	float angle_diff; /*!< Difference between angles in two frames*/
	float threshold_min; /*!< Minimum angle threshold */
	float threshold_max; /*!< Maximum angle threshold */
	bool cycle_flag;	/*!< flag to indicate if rotation can cycle through */
	bool update_flag;	/*!< flag to indicate if the transformation should be updated every frame */
	bool cycle_assist;	/*!< assist variable for a cyclic rotation */
public:
	void upload(const float &ang,const float &arg_x,const float &arg_y,const float &arg_z);
	void setUpdate(const float &ang_diff,const float &t_min,const float &t_max,const bool &cycle);
	void update();
	Matrix returnMatrix() const;
};

//!  Scale class
/*!
Class for implmenting a scale transformation
*/
class Scale : public SingleTransform
{
	Matrix m;	/*!< Variable of type Matrix */
	bool update_flag;	/*!< flag to indicate if the transformation should be updated every frame */
public:
	float x; /*!< x scale factor */
	float y; /*!< y scale factor */
	float z; /*!< z scale factor */
	void upload(const float &arg_x,const float &arg_y,const float &arg_z);
	void setUpdate();
	void update();
	Matrix returnMatrix() const;
};

//!  Translate class
/*!
Class for implmenting a Translate transformation
*/
class Translate : public SingleTransform
{
	Matrix m;	/*!< Variable of type Matrix */
	bool update_flag;	/*!< flag to indicate if the transformation should be updated every frame */
public:
	float x; /*!< x translate component */
	float y; /*!< y translate component */
	float z; /*!< z translate component */
	void upload(const float &arg_x,const float &arg_y,const float &arg_z);
	void setUpdate();
	void update();
	Matrix returnMatrix() const;
};

//!  AffineTransform class
/*!
Class for implmenting an affine transformation
*/
class AffineTransform
{
	std::vector<SingleTransform *> collection;	/*!< Vector of pointers of type SingleTransform */
public:
	Matrix m;	/*!< Variable of type Matrix */
	void add(SingleTransform *t);
	void remove();
	void update();
};

Matrix my_Translate(const float &arg_x,const float &arg_y,const float &arg_z);
Matrix my_Scale(const float &arg_x,const float &arg_y,const float &arg_z);
Matrix my_Rotate(const float &angle,const float &arg_x,const float &arg_y,const float &arg_z);
Matrix my_Perspective(const float &fovy,const float &aratio,const float &znear,const float &zfar);
Matrix my_frustum(const float &left,const float &right,const float &bottom,const float &top,const float &znear,const float &zfar);
Matrix my_Ortho(const float &left,const float &right,const float &bottom,const float &top,const float &nearVal,const float &farVal);
Matrix my_lookAt(const float &eye_x,const float &eye_y,const float &eye_z,
	const float &target_x,const float &target_y,const float &target_z,
	const float &up_x,const float &up_y,const float &up_z);
bool unProject(const float &x,const float &y,const float &z,const Matrix &mv,const Matrix &pr,const unsigned long &width,const unsigned long &height,float *objx,float *objy,float *objz);

#endif