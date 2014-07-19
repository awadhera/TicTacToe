/******************************************************************************/
/*!
\file Collision.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1250
\par Project #01
\date 02/08/2012
\brief
Implementation of collision detection and response functions
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#include "Collision.h"

/******************************************************************************/
/*!
Function to detect if there is a collision between a ball and straight wall
and if so, find the collision response
\param [in] g1
Reference to a GameObject representing a dynamic ball
\param [in] g2
Reference to a GameObject representing a straight wall
\param [in] Ts
Reference to a constant float representing frame start time
\param [in] dt
Reference to a constant float representing time taken to complete a frame
\return
bool - True if collision is detected,false otherwise
*/
/******************************************************************************/
bool collisionBallStraightWall(GameObject &g1,GameObject &g2,const float &Ts,const float &dt)
{
	/// Normalize the direction vector of ball.
	g1.v.Normalize();
	Vector normal_g2;
	/// Find normal of the wall.
	normal_g2.x = (g2.V1.y - g2.V0.y);
	normal_g2.y = -(g2.V1.x - g2.V0.x);
	normal_g2.z = 0.0f;
	normal_g2.Normalize();
	Point Bs = g1.p;
	Vector v_Bs(g1.p.x,g1.p.y,g1.p.z);
	float speedTime = g1.speed*dt;
	Vector velocity = g1.v*speedTime;
	/// Find the expected end position of ball at end of frame if it does not collides
	Point Be = g1.p + velocity;
	Vector v_Be(Be.x,Be.y,Be.z);
	Vector v_V0(g2.V0.x,g2.V0.y,g2.V0.z);
	float n_dot_Bs,n_dot_V0,n_dot_Be;
	n_dot_Bs = dotProduct(normal_g2,v_Bs);
	n_dot_V0 = dotProduct(normal_g2,v_V0);
	n_dot_Be = dotProduct(normal_g2,v_Be);
	/// Trivial rejection checks
	if((((n_dot_Bs - n_dot_V0 + g1.radius) < 0.0f) && ((n_dot_Be - n_dot_V0 + g1.radius) < 0.0f)) ||
		(((n_dot_Bs - n_dot_V0 - g1.radius) > 0.0f) && ((n_dot_Be - n_dot_V0 - g1.radius) > 0.0f)))
	{
		return false;
	}
	else
	{
		/// Find whether ball is inside or outside the wall
		float D = ((n_dot_Bs - n_dot_V0 + g1.radius) < 0.0f)?-(g1.radius):g1.radius;
		Vector c = g1.v*g1.speed;
		float n_dot_c = dotProduct(normal_g2,c);
		/// Find the collision time
		float col_dt = ((n_dot_V0 - n_dot_Bs + D)/n_dot_c);
		Point Bh;
		float Th;
		speedTime = g1.speed*col_dt;
		velocity = g1.v*speedTime;
		Th = Ts + col_dt; 
		/// Point of collision		
		Bh = Bs + velocity;
		Vector Bh_V0(Bh.x - g2.V0.x,Bh.y - g2.V0.y,Bh.z - g2.V0.z);
		Vector V1_V0(g2.V1.x - g2.V0.x,g2.V1.y - g2.V0.y,g2.V1.z - g2.V0.z);
		float Bh_V0_dot_V1_V0 = dotProduct(Bh_V0,V1_V0);
		float V1_V0_dot_V1_V0 = dotProduct(V1_V0,V1_V0);
		/// Check if ball collides with infinite extension of the wall
		if((Bh_V0_dot_V1_V0 - V1_V0_dot_V1_V0 > 0.0f) || (Bh_V0_dot_V1_V0 < 0.0f))
			return false;
		else if((Th-Ts >= 0.0f) && (Th-(Ts+dt) <= 0.0f))
		{
			/// Collision Response
			Vector i(Be.x - Bh.x,Be.y - Bh.y,Be.z - Bh.z);
			float i_dot_n = dotProduct(i,normal_g2);
			Vector r;
			float mult = (-2.0f)*(i_dot_n);
			Vector temp = normal_g2*mult; 
			r = i + temp;
			Point Ber = Bh + r;
			g1.p = Ber;
			/// Translate the geometric object for this physics object through translate trasform
			g1.translate.x = Ber.x;
			g1.translate.y = Ber.y;
			g1.translate.z = Ber.z;
			g1.v = r;
			g1.v.Normalize();
			return true;
		}
		else
		{
			return false;
		}
	} 
}

/******************************************************************************/
/*!
Function to detect if there is a collision between a ball and circular wall
and if so, find the collision response
\param [in] g1
Reference to a GameObject representing a dynamic ball
\param [in] g2
Reference to a GameObject representing a circular wall
\param [in] Ts
Reference to a constant float representing frame start time
\param [in] dt
Reference to a constant float representing time taken to complete a frame
\return
bool - True if collision is detected,false otherwise
*/
/******************************************************************************/
bool collisionBallCircularWall(GameObject &g1,GameObject &g2,const float &Ts,const float &dt)
{
	/// Normalize the direction vector of the ball
	g1.v.Normalize();
	Vector Q_Bs(g2.p.x-g1.p.x,g2.p.y-g1.p.y,g2.p.z-g1.p.z);
	float m = dotProduct(Q_Bs,g1.v);
	float combinedRadius = g2.radius + g1.radius;
	float Q_Bs_Magnitude = Q_Bs.Magnitude();
	float n_Square = (Q_Bs_Magnitude*Q_Bs_Magnitude) - (m*m);
	float combinedRadius_Square = combinedRadius*combinedRadius;
	/// Trivial rejection checks
	if(((Q_Bs_Magnitude > combinedRadius)&&(m<0.0f)) || ((n_Square) > (combinedRadius_Square)))
	{
		return false;
	}
	else 
	{
		float s = sqrt(combinedRadius_Square - n_Square);
		float speedTime;
		Vector velocity;
		/// Find expected collision time
		float col_dt = ((m - s)/(g1.speed)); 
		float Th;
		Point Bh;
		Th = Ts + col_dt;
		speedTime = g1.speed*col_dt;
		velocity = g1.v*speedTime;
		/// Find expected point of collision
		Bh = g1.p + velocity;
		if(((Th-Ts >= 0.0f) && (Th-(Ts+dt) <= 0.0f)))
		{
			/// Collision Response
			Vector ref_n(Bh.x - g2.p.x,Bh.y - g2.p.y,Bh.z - g2.p.z);
			ref_n.Normalize();
			Vector ref_m(g1.p.x - Bh.x,g1.p.y - Bh.y,g1.p.z - Bh.z);
			float ref_n_dot_ref_m = dotProduct(ref_n,ref_m);
			Vector ref_r = ref_n*(2.0f*ref_n_dot_ref_m) + (ref_m*(-1.0f));
			ref_r.Normalize();
			speedTime = g1.speed * (Ts+dt-Th);
			velocity = ref_r*speedTime;
			Point Ber = Bh + velocity;
			g1.p = Ber;
			/// Translate the geometric object for this physics object through translate trasform
			g1.translate.x = Ber.x;
			g1.translate.y = Ber.y;
			g1.translate.z = Ber.z;
			g1.v = ref_r;
			g1.v.Normalize();
			return true;
		}
		else
		{
			return false;
		}
	}
}

/******************************************************************************/
/*!
Function to detect if there is a collision between two dynamic balls
and if so, find the collision response
\param [in] g1
Reference to a GameObject representing a dynamic ball
\param [in] g2
Reference to a GameObject representing another dynamic wall
\param [in] Ts
Reference to a constant float representing frame start time
\param [in] dt
Reference to a constant float representing time taken to complete a frame
\return
bool - True if collision is detected,false otherwise
*/
/******************************************************************************/
bool collisionBallBall(GameObject &g1,GameObject &g2,const float &Ts,const float &dt)
{
	/// Save the direction vector and speed of the first ball.
	Vector storeDirection = g1.v;
	float storeSpeed = g1.speed;
	Vector cA_velocity = g1.v*g1.speed;
	Vector cB_velocity = g2.v*g2.speed;
	/// Relative velocity of the first ball.
	Vector cA_cB = cA_velocity + (cB_velocity*(-1.0f));
	/// Check for collision between a ball(first) and circular wall(second)
	g1.speed = cA_cB.Magnitude();
	g1.v = cA_cB;
	g1.v.Normalize();
	Vector Q_Bs(g2.p.x-g1.p.x,g2.p.y-g1.p.y,g2.p.z-g1.p.z);
	float m = dotProduct(Q_Bs,g1.v);
	float combinedRadius = g2.radius + g1.radius;
	float Q_Bs_Magnitude = Q_Bs.Magnitude();
	float n_Square = (Q_Bs_Magnitude*Q_Bs_Magnitude) - (m*m);
	float combinedRadius_Square = combinedRadius*combinedRadius;
	/// Trivial rejection checks
	if(((Q_Bs_Magnitude > combinedRadius)&&(m<0.0f)) || ((n_Square) > (combinedRadius_Square)))
	{
		/// Restore direction and speed of the first ball
		g1.v = storeDirection;
		g1.speed = storeSpeed;
		return false;
	}
	else 
	{
		/// Find collision time
		float s = sqrt(combinedRadius_Square - n_Square);
		float col_dt = ((m - s)/(g1.speed)); 
		float Th;
		Point Bh;
		Th = Ts + col_dt;
		if(((Th-Ts >= 0.0f) && (Th-(Ts+dt) <= 0.0f)))
		{
			/// Collision Response
			g1.v = storeDirection;
			g1.speed = storeSpeed;
			Point BhA,BhB;
			BhA = g1.p + cA_velocity*(Th);
			BhB = g2.p + cB_velocity*(Th);
			Vector d(BhA.x - BhB.x,BhA.y - BhB.y,BhA.z - BhB.z);
			d.Normalize();
			float aA = dotProduct(cA_velocity,d);
			float aB = dotProduct(cB_velocity,d);
			Vector cA_new = cA_velocity + d*((-2.0f)*(((aA-aB)/(g1.mass+g2.mass))*g2.mass));
			Vector cB_new = cB_velocity + d*((((2.0f*(aA-aB))/(g1.mass+g2.mass))*g1.mass));
			g1.speed = cA_new.Magnitude();
			g2.speed = cB_new.Magnitude();
			cA_new.Normalize();
			cB_new.Normalize();
			g1.p = BhA + (cA_new)*((g1.speed)*(Ts+dt-Th));
			g2.p = BhB + (cB_new)*((g2.speed)*(Ts+dt-Th));
			/// Translate the geometric object for this physics object through translate trasform
			g1.translate.x = g1.p.x;
			g1.translate.y = g1.p.y;
			g1.translate.z = g1.p.z;
			g2.translate.x = g2.p.x;
			g2.translate.y = g2.p.y;
			g2.translate.z = g2.p.z;
			g1.v = cA_new;
			g2.v = cB_new;
			return true;
		}
		else
		{
			/// Restore the direction and speed of the first ball.
			g1.v = storeDirection;
			g1.speed = storeSpeed;
			return false;
		}
	}
}