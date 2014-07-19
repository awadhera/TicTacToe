/******************************************************************************/
/*!
\file Transforms.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Implementation of classes related to transforms
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#include "Transforms.h"

/******************************************************************************/
/*!
update function for class SingleTransform
\return
void
*/
/******************************************************************************/
void SingleTransform::update()
{
}

/******************************************************************************/
/*!
Destructor for class:SingleTransform
\return
void
*/
/******************************************************************************/
SingleTransform::~SingleTransform()
{
}

/******************************************************************************/
/*!
upload the rotation transform specific values 
\param ang
Angle of rotation
\param arg_x
x component of vector of axis of rotation
\param arg_y
y component of vector of axis of rotation
\param arg_z
z component of vector of axis of rotation
\return
void
*/
/******************************************************************************/
void Rotate::upload(const float &ang,const float &arg_x,const float &arg_y,const float &arg_z)
{
	angle = ang;
	x = arg_x;
	y = arg_y;
	z = arg_z;
	m = my_Rotate(angle,x,y,z);
	/// update_flag specifies if the transform has to be updated every frame
	update_flag = false;
}

/******************************************************************************/
/*!
setUpdate sets the transform to be updated every frame
\param ang_diff
Difference in angle of rotation between frames
\param t_min
Minimum angle threshold
\param t_max
Maximum angle threshold
\param cycle
Specify the cycle-through flag
\return
void
*/
/******************************************************************************/
void Rotate::setUpdate(const float &ang_diff,const float &t_min,const float &t_max,const bool &cycle)
{
	update_flag = true;
	cycle_flag = cycle;
	cycle_assist = true;
	angle_diff = ang_diff;
	threshold_min = t_min;
	threshold_max = t_max;
}

/******************************************************************************/
/*!
Update the transform
\return
void
*/
/******************************************************************************/
void Rotate::update()
{
	if(update_flag) 
	{
		/// cycle_flag indicates if the rotation is to-fro type or in single direction
		/// if TRUE rotation is in single direction
		if(cycle_flag)
		{
			/// increment the angle by angle_diff and check for max threshold
			angle+=angle_diff;
			if(angle>threshold_max)
				angle = threshold_min;
		}
		else
		{
			/// Manage to-fro rotation
			if(cycle_assist)
			{
				angle+=angle_diff;
			}
			else
			{	
				angle-=angle_diff;
			}
			if(angle>threshold_max)
			{
				angle = threshold_max;
				cycle_assist = false;
			}
			else if(angle<threshold_min)
			{
				angle = threshold_min;
				cycle_assist = true;
			}
		}
		/// Evaluate the rotation matrix
		m = my_Rotate(angle,x,y,z);
	}
}

/******************************************************************************/
/*!
Return the matrix transformation
\return
Instance of type Matrix
*/
/******************************************************************************/
Matrix Rotate::returnMatrix() const
{
	return m;
}

/******************************************************************************/
/*!
Upload the scale tranform specific values
\param arg_x
x scale component
\param arg_y
y scale component
\param arg_z
z scale component
\return
void
*/
/******************************************************************************/
void Scale::upload(const float &arg_x,const float &arg_y,const float &arg_z)
{
	x = arg_x;
	y = arg_y;
	z = arg_z;
	m = my_Scale(x,y,z);
	///update_flag specifies if the transform must be updated every frame
	update_flag = false;
}

/******************************************************************************/
/*!
Sets the transform to be updated every frame
\return
void
*/
/******************************************************************************/
void Scale::setUpdate()
{
	update_flag = true;
}

/******************************************************************************/
/*!
Update the transform
\return
void
*/
/******************************************************************************/
void Scale::update()
{
	if(update_flag)
	{
	}
}

/******************************************************************************/
/*!
Return the matrix transformation
\return
Instance of type Matrix
*/
/******************************************************************************/
Matrix Scale::returnMatrix() const
{
	return m;
}

/******************************************************************************/
/*!
Upload the translate tranform specific values
\param arg_x
x translate component
\param arg_y
y translate component
\param arg_z
z translate component
\return
void
*/
/******************************************************************************/
void Translate::upload(const float &arg_x,const float &arg_y,const float &arg_z)
{
	x = arg_x;
	y = arg_y;
	z = arg_z;
	m = my_Translate(x,y,z);
	///update_flag specifies if the transform must be updated every frame
	update_flag = false;
}

/******************************************************************************/
/*!
Sets the transform to be updated every frame
\return
void
*/
/******************************************************************************/
void Translate::setUpdate()
{
	update_flag = true;
}

/******************************************************************************/
/*!
Update the transform
\return
void
*/
/******************************************************************************/
void Translate::update()
{
	if(update_flag)
	{
	}
	m = my_Translate(x,y,z);
}

/******************************************************************************/
/*!
Return the matrix transformation
\return
Instance of type Matrix
*/
/******************************************************************************/
Matrix Translate::returnMatrix() const
{
	return m;
}

/******************************************************************************/
/*!
add a SingleTransform instance to AffineTransform
\param t
Pointer to SingleTransform
\return
void
*/
/******************************************************************************/
void AffineTransform::add(SingleTransform *t)
{
	/// Push the transform to vector:collection
	collection.push_back(t);
	/// Multiply the current affinetransform with the argument transform
	m.MultMatrix(t->returnMatrix());
}

void AffineTransform::remove()
{
	collection.pop_back();
}

/******************************************************************************/
/*!
update the affine transform
\return
void
*/
/******************************************************************************/
void AffineTransform::update()
{
	if(!collection.empty())
	{
		Matrix n;
		std::vector<SingleTransform *>::iterator iter;
		/// iterate through all the transforms and update them
		for(iter = collection.begin();iter!=collection.end();iter++)
		{
			(*iter)->update();
			n.MultMatrix((*iter)->returnMatrix());
		}
		m = n;
	}
}

/******************************************************************************/
/*!
Provide the translation transform matrix
\param arg_x
x translate componenet
\param arg_y
y translate component
\param arg_z
z translate component
\return
Instance of type Matrix
*/
/******************************************************************************/
Matrix my_Translate(const float &arg_x,const float &arg_y,const float &arg_z)
{
	float tr[16] = {1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0,
		arg_x,arg_y,arg_z,1.0};
	Matrix trans(tr);
	return trans;
}
/******************************************************************************/
/*!
Provide the scale transform matrix
\param arg_x
x scale component
\param arg_y
y scale component
\param arg_z
z scale component
\return
Instance of type Matrix
*/
/******************************************************************************/
Matrix my_Scale(const float &arg_x,const float &arg_y,const float &arg_z)
{
	float sc[16] =  {arg_x,0.0,0.0,0.0,
		0.0,arg_y,0.0,0.0,
		0.0,0.0,arg_z,0.0,
		0.0,0.0,0.0,1.0};
	Matrix scale(sc);
	return scale;
}

/******************************************************************************/
/*!
Provide the rotation transform matrix
\param angle
Angle of rotation in degrees
\param arg_x
x translate component
\param arg_y
y translate component
\param arg_z
z translate component
\return
Instance of type Matrix
*/
/******************************************************************************/
Matrix my_Rotate(const float &angle,const float &arg_x,const float &arg_y,const float &arg_z)
{
	float rad = angle*(M_PI/180.0f);
	float cos_a = std::cos(rad); 
	float sin_a = std::sin(rad);
	Vector v_n(arg_x,arg_y,arg_z);
	v_n.Normalize();
	float rot[16] = 
	{ cos_a+((1-cos_a)*v_n.x*v_n.x),  ((1-cos_a)*v_n.x*v_n.y)+(v_n.z*sin_a),((1-cos_a)*v_n.x*v_n.z)-(v_n.y*sin_a), 0.0,
	((1-cos_a)*v_n.x*v_n.y)-(v_n.z*sin_a), cos_a+((1-cos_a)*v_n.y*v_n.y),((1-cos_a)*v_n.y*v_n.z)+(v_n.x*sin_a), 0.0,
	((1-cos_a)*v_n.x*v_n.z)+(v_n.y*sin_a), ((1-cos_a)*v_n.y*v_n.z) - (v_n.x*sin_a),  cos_a+((1-cos_a)*v_n.z*v_n.z),0.0,
	0.0, 0.0, 0.0, 1.0
	};
	Matrix rotate(rot);
	return rotate;
}

/******************************************************************************/
/*!
Provide the perspective transform matrix
\param fovy
Field of view angle in degrees
\param aratio
Aspect ratio
\param znear
Z-near plane
\param zfar
Z-far plane
\return
Instance of type Matrix
*/
/******************************************************************************/
Matrix my_Perspective(const float &fovy,const float &aratio,const float &znear,const float &zfar)
{
	float fovy_rad = fovy*(M_PI/180);
	float f = 1/(tan(fovy_rad/2));

	float pers[16] = 
	{ f/aratio, 0.0, 0.0, 0.0,
	0.0, f, 0.0, 0.0,
	0.0, 0.0, (zfar+znear)/(znear-zfar), -1,
	0.0, 0.0, (2*zfar*znear)/(znear - zfar), 0.0 };

	Matrix perspective(pers);
	return perspective;
}

/******************************************************************************/
/*!
Provide the frustum transform matrix
\param left
left plane
\param right 
right plane
\param bottom
bottom plane
\param top
top plane
\param znear
Z-near plane
\param zfar
Z-far plane
\return
Instance of type Matrix
*/
/******************************************************************************/
Matrix my_frustum(const float &left,const float &right,const float &bottom,const float &top,const float &znear,const float &zfar)
{
	float frust[16] = 
	{(2*znear)/(right-left), 0.0, 0.0, 0.0,
	0.0, (2*znear)/(top-bottom), 0.0, 0.0,
	(right + left)/(right-left), (top + bottom)/(top-bottom),  (-zfar - znear)/(zfar - znear),-1.0,
	0.0,0.0, ((-(2*znear))*zfar)/(zfar-znear), 0.0};

	Matrix frustum(frust);
	return frustum;
}

/******************************************************************************/
/*!
Provide the lookAt orientation for the camera
\param eye_x
x coordinate of camera
\param eye_y
y coordinate of camera
\param eye_z
z coordinate of camera
\param target_x
x coordinate of target
\param target_y
y coordinate of target
\param target_z
z coordinate of target
\param up_x
x component of up vector
\param up_y
y component of up vector
\param up_z
z component of up vector
\return
Instance of type Matrix
*/
/******************************************************************************/
Matrix my_lookAt(const float &eye_x,const float &eye_y,const float &eye_z,
	const float &target_x,const float &target_y,const float &target_z,
	const float &up_x,const float &up_y,const float &up_z)
{
	Vector view,side,up;
	/// Evaluate view vector
	view.x = target_x - eye_x;
	view.y = target_y - eye_y;
	view.z = target_z - eye_z;
	up.x = up_x;
	up.y = up_y;
	up.z = up_z;
	/// Normalize view vector
	view.Normalize();
	/// Evaluate side vector
	crossProduct(view,up,side);
	/// Normalize side vector	
	side.Normalize();
	/// Evaluate new up vector
	crossProduct(side,view,up);
	Matrix lookAt;
	/// Set the view Matrix values
	lookAt.t[0][0] = side.x;
	lookAt.t[1][0] = side.y;
	lookAt.t[2][0] = side.z;
	lookAt.t[0][1] = up.x;
	lookAt.t[1][1] = up.y;
	lookAt.t[2][1] = up.z;
	lookAt.t[0][2] = -view.x;
	lookAt.t[1][2] = -view.y;
	lookAt.t[2][2] = -view.z;
	lookAt.t[0][3] = lookAt.t[1][3] = lookAt.t[2][3] = 	lookAt.t[3][0] = lookAt.t[3][1] = lookAt.t[3][2] = 0;
	lookAt.t[3][3] = 1;
	/// Translate the view matrix by eye position
	lookAt.MultMatrix(my_Translate(-eye_x,-eye_y,-eye_z));
	return lookAt;
}

/******************************************************************************/
/*!
Provide the upprojection matrix for use in camera
\param x
x window coordinate
\param y
y window coordinate
\param z
z window coordinate
\param mv
ModelView matrix
\param pr
Projection matrix
\param width
Width of the viewport
\param height
Height of the viewport
\param [out] objx
x coordinate of object
\param [out] objy
y coordinate of object
\param [out] objz
z coordiante of object
\return
bool - indicating if unprojection was successful
*/
/******************************************************************************/
bool unProject(const float &x,const float &y,const float &z,const Matrix &mv,const Matrix &pr,const unsigned long &width,const unsigned long &height,float *objx,float *objy,float *objz)
{
	Matrix PMV;
	float VParr[4];
	float xn,yn,zn;
	/// Multiply projection matrix by modelview matrix. Result is PMV
	PMV.MultMatrix(pr);
	PMV.MultMatrix(mv);
	Matrix InvPMV;
	/// calculate the inverse of PMV
	bool val = Inverse(PMV,InvPMV);
	if(val == false)
		return val;
	else
	{
		///Multiply Inverse of PMV with normalized device coordinates
		xn = ((2*x)/width)-1;
		yn = ((2*y)/height)-1;
		zn = ((2*z)-1);
		VParr[0] = xn;
		VParr[1] = yn;
		VParr[2] = zn;
		VParr[3] = 1;
		InvPMV.MultMatrix(VParr,4);
		/// Set the object coordinates in world
		*objx = InvPMV.t[0][0]/InvPMV.t[0][3];
		*objy = InvPMV.t[0][1]/InvPMV.t[0][3];
		*objz = InvPMV.t[0][2]/InvPMV.t[0][3];
		return true;
	}
}

/******************************************************************************/
/*!
Evaluate the orthographic projection matrix
\param left
Left plane
\param right
Right plane
\param bottom
Bottom plane
\param top
Top plane
\param nearVal
Z-near plane
\param farVal
Z-far plane
\return
Instance of type Matrix
*/
/******************************************************************************/
Matrix my_Ortho(const float &left,const float &right,const float &bottom,const float &top,const float &nearVal,const float &farVal)
{
	float ortho[16] = 
	{2/(right-left),0,0,0,
	0,2/(top-bottom),0,0,
	0,0,(-2)/(farVal-nearVal),0,
	-(right+left)/(right-left),-(top+bottom)/(top-bottom),-(farVal+nearVal)/(farVal-nearVal),1};

	Matrix orthographic(ortho);
	return orthographic;
}