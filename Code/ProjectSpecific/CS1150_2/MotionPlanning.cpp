#include "MotionPlanning.h"
#include <math.h>

//Trapezoid class
Trapezoid::Trapezoid()
{
	neighbours[0]=NULL;
	neighbours[1]=NULL;
	neighbours[2]=NULL;
	neighbours[3]=NULL;	

	neighbourUsed[0] = false;
	neighbourUsed[1] = false;
	neighbourUsed[2] = false;
	neighbourUsed[3] = false;

	isVisited = false;

	stateIndex = -1;

	treeNode = NULL;
}

Trapezoid::~Trapezoid()
{
}

Trapezoid & Trapezoid::operator=(const Trapezoid & tra)
{
	top = tra.top;
	bottom = tra.bottom;
	leftPoint = tra.leftPoint;
	rightPoint = tra.rightPoint;
	neighbours[0] = tra.neighbours[0];
	neighbours[1] = tra.neighbours[1];
	neighbours[2] = tra.neighbours[2];
	neighbours[3] = tra.neighbours[3];
	neighbourUsed[0] = tra.neighbourUsed[0];
	neighbourUsed[1] = tra.neighbourUsed[1];
	neighbourUsed[2] = tra.neighbourUsed[2];
	neighbourUsed[3] = tra.neighbourUsed[3];
	stateIndex = tra.stateIndex;
	isVisited = tra.isVisited;
	treeNode = tra.treeNode;
	return *this;
}

bool Trapezoid::operator==(Trapezoid tra)
{
	return ( (top == tra.top) &&
			 (bottom == tra.bottom) &&
			 (leftPoint == tra.leftPoint) &&
			 (rightPoint==tra.rightPoint) );//&&
			// (neighbours[0]==tra.neighbours[0]) &&
			// (neighbours[1]==tra.neighbours[1]) &&
			// (neighbours[2]==tra.neighbours[2]) &&
			// (neighbours[3]==tra.neighbours[3]) );			 
}

Trapezoid::Trapezoid(const Trapezoid & tra)
{
	top=tra.top;
	bottom=tra.bottom;
	leftPoint=tra.leftPoint;
	rightPoint=tra.rightPoint;
	neighbours[0]=tra.neighbours[0];
	neighbours[1]=tra.neighbours[1];
	neighbours[2]=tra.neighbours[2];
	neighbours[3]=tra.neighbours[3];
	neighbourUsed[0] = tra.neighbourUsed[0];
	neighbourUsed[1] = tra.neighbourUsed[1];
	neighbourUsed[2] = tra.neighbourUsed[2];
	neighbourUsed[3] = tra.neighbourUsed[3];
	stateIndex = tra.stateIndex;
	isVisited = tra.isVisited;
	treeNode = tra.treeNode;
}
//End Trapezoid class

//AG_State class
AG_State::AG_State()
{
	left = NULL;
	right = NULL;
	type = AG_LEAF;
	point = Point2D(0,0);
	lineSegment = LineSegment2D(Point2D(0,0),Point2D(0,0));
	trapezoid = NULL;
}

AG_State::~AG_State()
{
}

AG_State::AG_State(const AG_State & st)
{
	left = st.left;
	right = st.right;
	type = st.type;
	point = st.point;
	lineSegment = st.lineSegment;
	trapezoid = st.trapezoid;
	for(unsigned int i = 0; i < st.parents.size(); i++)
		parents.push_back(st.parents[i]);
}
AG_State & AG_State::operator=(const AG_State & st)
{
	left = st.left;
	right = st.right;
	type = st.type;
	point = st.point;
	lineSegment = st.lineSegment;
	trapezoid = st.trapezoid;
	for(unsigned int i = 0; i < st.parents.size(); i++)
		parents.push_back(st.parents[i]);

	return *this;
}	
//End AG_State class


//MotionPlanning_PointRobot class
MotionPlanning_PointRobot::MotionPlanning_PointRobot()
{
	path = NULL;
}

MotionPlanning_PointRobot::~MotionPlanning_PointRobot()
{
	if(path)
	{
		delete path;
        path = NULL;
	}
}

void MotionPlanning_PointRobot::Calculate_LineSegments_Normals()
{
	std::vector<LineSegment2D>::iterator it;
	for(it=segmentList.begin();it!=segmentList.end();++it)
	{
		(*it).normal.x = (*it).end.y - (*it).start.y;
		(*it).normal.y = -((*it).end.x - (*it).start.x);
	}
}

//this will put the point with the lower x-value to the start point
//end the point with the higher x-value to the end point
void MotionPlanning_PointRobot::Adjust_StartEnd_PointsSegment()
{
	std::vector<LineSegment2D>::iterator it;
	for(it=segmentList.begin();it!=segmentList.end();++it)
	{
		if((*it).end.x < (*it).start.x)
		{
			Point2D temp;
			temp = (*it).start;
			(*it).start = (*it).end;
			(*it).end = temp;
		}
	}
}

//We give to this function a start point-segment and shall return the current
//trapezoid in the search tree that holds this point (pointS).
//(pointE) help us to classify the line segment (pointS,pointE) if pointS lies on a line-segment.
Trapezoid * MotionPlanning_PointRobot::TrapezoidSearch(AG_State * searchS,Point2D pointS,Point2D pointE)
{
	AG_State * temp = searchS;
	while(temp)
	{
		switch(temp->type)
		{
		case AG_XNODE:
			//NOTES
			//because of the line segments general positions strategy
			//the points should not have the same x.

			if(pointS.x < temp->point.x)
				temp = temp->left;
			else
				temp = temp->right;
			break;
		case AG_YNODE:
			//NOTES
			//if p lies on the current segment (this can only happen if both segments
			//share their left end point)
			//so we only compare their slopes to classify the point

			switch(Classify_Point_To_LineSegment(pointS,temp->lineSegment))
			{
			case CL_ABOVE:
				temp = temp->left;
				break;
			case CL_UNDER:
				temp = temp->right;
				break;
			case CL_ON:
				if(Classify_Point_To_LineSegment(pointE,temp->lineSegment) == CL_ABOVE)
					temp = temp->left;
				else
					temp = temp->right;
				break;
			}
			break;
		case AG_LEAF:
			return temp->trapezoid;
			break;
		}
	}
	return NULL;
}


//The following function is based on the general position strategy.
void MotionPlanning_PointRobot::TrapezoidalMap()
{
	//Initialization of the trapezoidal map structure
	//The first trapezoid will be the bounding rectangle that bound all of the input line-segments
	boundingRect = Calculate_BoundingRectangle();
	trapezoidalList.push_back(boundingRect);

	//Initialization of the search structure for the trapezoidal map
	//searchStructure : root of the directed acyclic binary search tree - (one single root for all)
	searchStructure.type = AG_LEAF;
	std::list<Trapezoid>::iterator temptr1;
	temptr1 = trapezoidalList.begin();
	searchStructure.trapezoid = &(*temptr1);
	(*temptr1).treeNode = &searchStructure;

	//continue here
	tcount = 0;
	std::vector<LineSegment2D>::iterator lsIt;
	std::vector<Point2D> usedPoints;
	std::vector<Point2D>::iterator ptIt;
	for(lsIt = segmentList.begin();lsIt != segmentList.end();++lsIt)
	{
		trapezoidIntersections.clear();
		bool startPointUsed = false;
		bool endPointUsed = false;
		for(ptIt = usedPoints.begin();ptIt != usedPoints.end();++ptIt)
		{
			if((*ptIt) == (*lsIt).start)
				startPointUsed = true;
			else if((*ptIt) == (*lsIt).end)
				endPointUsed = true;
		}
		if(!startPointUsed)
			usedPoints.push_back((*lsIt).start);
		if(!endPointUsed)
			usedPoints.push_back((*lsIt).end);
		Trapezoid *currentTrapezoid = TrapezoidSearch(&searchStructure,(*lsIt).start,(*lsIt).end);
		if(currentTrapezoid != NULL)
		{
			tcount = 1;
			trapezoidIntersections.push_back(currentTrapezoid);
			while((*lsIt).end.x > currentTrapezoid->rightPoint.x)
			{
				if(Classify_Point_To_LineSegment(currentTrapezoid->rightPoint,(*lsIt)) == CL_ABOVE) 
				{
					trapezoidIntersections.push_back(trapezoidIntersections[tcount-1]->neighbours[3]); 
				}
				else
				{
					trapezoidIntersections.push_back(trapezoidIntersections[tcount-1]->neighbours[2]);
				}
				currentTrapezoid = trapezoidIntersections[tcount];
				++tcount;
			}
			if(tcount == 1)
			{
				if(startPointUsed && endPointUsed)
				{
					Trapezoid tA,tB;
					tA.leftPoint = (*lsIt).start;
					tA.rightPoint = (*lsIt).end;
					tA.top = trapezoidIntersections[0]->top; 
					tA.bottom = (*lsIt); 
					tB.leftPoint = (*lsIt).start; 
					tB.rightPoint = (*lsIt).end; 
					tB.top = (*lsIt); 
					tB.bottom = trapezoidIntersections[0]->bottom;
					trapezoidalList.push_back(tA);
					std::list<Trapezoid>::iterator trapA = trapezoidalList.end();
					trapA--;
					trapezoidalList.push_back(tB);
					std::list<Trapezoid>::iterator trapB = trapezoidalList.end();
					trapB--;
					bool startOnTop = PointOnLineSegment((*lsIt).start,trapezoidIntersections[0]->top);
					bool startOnBottom = PointOnLineSegment((*lsIt).start,trapezoidIntersections[0]->bottom);
					bool endOnTop = PointOnLineSegment((*lsIt).end,trapezoidIntersections[0]->top);
					bool endOnBottom = PointOnLineSegment((*lsIt).end,trapezoidIntersections[0]->bottom);
					if(startOnBottom && endOnTop)
					{
						(*trapA).neighbours[0] = trapezoidIntersections[0]->neighbours[0];
						if((*trapA).neighbours[0]) 
							(*trapA).neighbours[0]->neighbours[2] = &(*trapA);
						(*trapB).neighbours[3] = trapezoidIntersections[0]->neighbours[3];
						if((*trapB).neighbours[3]) 
							(*trapB).neighbours[3]->neighbours[1] = &(*trapB);
					}
					else if(startOnTop && endOnBottom)
					{
						(*trapA).neighbours[2] = trapezoidIntersections[0]->neighbours[2];
						if((*trapA).neighbours[2])
							(*trapA).neighbours[2]->neighbours[0] = &(*trapA);
						(*trapB).neighbours[1] = trapezoidIntersections[0]->neighbours[1];
						if((*trapB).neighbours[1])
							(*trapB).neighbours[1]->neighbours[3] = &(*trapB);
					}
					else if(startOnTop)
					{
						(*trapA).neighbours[2] = trapezoidIntersections[0]->neighbours[2];
						if((*trapA).neighbours[2])
							(*trapA).neighbours[2]->neighbours[0] = &(*trapA);
						(*trapB).neighbours[1] = trapezoidIntersections[0]->neighbours[1];
						(*trapB).neighbours[3] = trapezoidIntersections[0]->neighbours[3];
						if((*trapB).neighbours[1])
							(*trapB).neighbours[1]->neighbours[3] = &(*trapB);
						if((*trapB).neighbours[3])
							(*trapB).neighbours[3]->neighbours[1] = &(*trapB);
					}
					else if(startOnBottom)
					{
						(*trapA).neighbours[0] = trapezoidIntersections[0]->neighbours[0];
						(*trapA).neighbours[2] = trapezoidIntersections[0]->neighbours[2];
						if((*trapA).neighbours[0])
							(*trapA).neighbours[0]->neighbours[2] = &(*trapA);
						if((*trapA).neighbours[2])
							(*trapA).neighbours[2]->neighbours[0] = &(*trapA);
						(*trapB).neighbours[3] = trapezoidIntersections[0]->neighbours[3];
						if((*trapB).neighbours[3])
							(*trapB).neighbours[3]->neighbours[1] = &(*trapB);
					}
					else if(endOnTop)
					{
						(*trapA).neighbours[0] = trapezoidIntersections[0]->neighbours[0];
						if((*trapA).neighbours[0])
							(*trapA).neighbours[0]->neighbours[2] = &(*trapA);
						(*trapB).neighbours[1] = trapezoidIntersections[0]->neighbours[1];
						(*trapB).neighbours[3] = trapezoidIntersections[0]->neighbours[3];
						if((*trapB).neighbours[1])
							(*trapB).neighbours[1]->neighbours[3] = &(*trapB);
						if((*trapB).neighbours[3])
							(*trapB).neighbours[3]->neighbours[1] = &(*trapB);
					}
					else if(endOnBottom)
					{
						(*trapA).neighbours[0] = trapezoidIntersections[0]->neighbours[0];
						(*trapA).neighbours[2] = trapezoidIntersections[0]->neighbours[2];
						if((*trapA).neighbours[0])
							(*trapA).neighbours[0]->neighbours[2] = &(*trapA);
						if((*trapA).neighbours[2])
							(*trapA).neighbours[2]->neighbours[0] = &(*trapA);
						(*trapB).neighbours[1] = trapezoidIntersections[0]->neighbours[1];
						if((*trapB).neighbours[1])
							(*trapB).neighbours[1]->neighbours[3] = &(*trapB);
					}
					else
					{
						(*trapA).neighbours[0] = trapezoidIntersections[0]->neighbours[0];
						(*trapA).neighbours[2] = trapezoidIntersections[0]->neighbours[2];
						if((*trapA).neighbours[0])
							(*trapA).neighbours[0]->neighbours[2] = &(*trapA);
						if((*trapA).neighbours[2])
							(*trapA).neighbours[2]->neighbours[0] = &(*trapA);
						(*trapB).neighbours[1] = trapezoidIntersections[0]->neighbours[1];
						(*trapB).neighbours[3] = trapezoidIntersections[0]->neighbours[3];
						if((*trapB).neighbours[1])
							(*trapB).neighbours[1]->neighbours[3] = &(*trapB);
						if((*trapB).neighbours[3])
							(*trapB).neighbours[3]->neighbours[1] = &(*trapB);
					}
					AG_State *replacement = (*trapezoidIntersections[0]).treeNode;
					replacement->type = AG_YNODE;
					replacement->lineSegment = *lsIt;
					replacement->left = new AG_State;
					replacement->left->type = AG_LEAF;
					replacement->left->trapezoid = &(*trapA);
					(*trapA).treeNode = replacement->left;
					replacement->right = new AG_State;
					replacement->right->type = AG_LEAF;
					replacement->right->trapezoid = &(*trapB);
					(*trapB).treeNode = replacement->right;
					replacement->left->parents.push_back(replacement);
					replacement->right->parents.push_back(replacement);
				}
				else if(startPointUsed)
				{
					Trapezoid tA,tB,tC;
					tA.leftPoint = (*lsIt).end;
					tA.rightPoint = trapezoidIntersections[0]->rightPoint;
					tA.top = trapezoidIntersections[0]->top;
					tA.bottom = trapezoidIntersections[0]->bottom;
					tB.leftPoint = (*lsIt).start;
					tB.rightPoint = (*lsIt).end;
					tB.top = trapezoidIntersections[0]->top;
					tB.bottom = (*lsIt);
					tC.leftPoint = (*lsIt).start;
					tC.rightPoint = (*lsIt).end;
					tC.top = (*lsIt);
					tC.bottom = trapezoidIntersections[0]->bottom;
					trapezoidalList.push_back(tA);
					std::list<Trapezoid>::iterator trapA = trapezoidalList.end();
					trapA--;
					trapezoidalList.push_back(tB);
					std::list<Trapezoid>::iterator trapB = trapezoidalList.end();
					trapB--;
					trapezoidalList.push_back(tC);
					std::list<Trapezoid>::iterator trapC = trapezoidalList.end();
					trapC--;
					if(PointOnLineSegment((*lsIt).start,trapezoidIntersections[0]->top))
					{
						(*trapA).neighbours[0] = &(*trapB);
						(*trapA).neighbours[1] = &(*trapC);
						(*trapA).neighbours[2] = trapezoidIntersections[0]->neighbours[2];
						(*trapA).neighbours[3] = trapezoidIntersections[0]->neighbours[3];
						if((*trapA).neighbours[2])
							(*trapA).neighbours[2]->neighbours[0] = &(*trapA);
						if((*trapA).neighbours[3])
							(*trapA).neighbours[3]->neighbours[1] = &(*trapA);
						(*trapB).neighbours[2] = &(*trapA);
						(*trapC).neighbours[1] = trapezoidIntersections[0]->neighbours[1];
						(*trapC).neighbours[3] = &(*trapA);
						if((*trapC).neighbours[1])
							(*trapC).neighbours[1]->neighbours[3] = &(*trapC);
					}
					else if(PointOnLineSegment((*lsIt).start,trapezoidIntersections[0]->bottom))
					{
						(*trapA).neighbours[0] = &(*trapB);
						(*trapA).neighbours[1] = &(*trapC);
						(*trapA).neighbours[2] = trapezoidIntersections[0]->neighbours[2];
						(*trapA).neighbours[3] = trapezoidIntersections[0]->neighbours[3];
						if((*trapA).neighbours[2])
							(*trapA).neighbours[2]->neighbours[0] = &(*trapA);
						if((*trapA).neighbours[3])
							(*trapA).neighbours[3]->neighbours[1] = &(*trapA);
						(*trapB).neighbours[0] = trapezoidIntersections[0]->neighbours[0];
						(*trapB).neighbours[2] = &(*trapA);
						if((*trapB).neighbours[0])
							(*trapB).neighbours[0]->neighbours[2] = &(*trapB);
						(*trapC).neighbours[3] = &(*trapA);
					}
					else
					{
						(*trapA).neighbours[0] = &(*trapB);
						(*trapA).neighbours[1] = &(*trapC);
						(*trapA).neighbours[2] = trapezoidIntersections[0]->neighbours[2];
						(*trapA).neighbours[3] = trapezoidIntersections[0]->neighbours[3];
						if((*trapA).neighbours[2])
							(*trapA).neighbours[2]->neighbours[0] = &(*trapA);
						if((*trapA).neighbours[3])
							(*trapA).neighbours[3]->neighbours[1] = &(*trapA);
						(*trapB).neighbours[2] = &(*trapA);
						(*trapB).neighbours[0] = trapezoidIntersections[0]->neighbours[0];
						if((*trapB).neighbours[0])
							(*trapB).neighbours[0]->neighbours[2] = &(*trapB);
						(*trapC).neighbours[3] = &(*trapA);
						(*trapC).neighbours[1] = trapezoidIntersections[0]->neighbours[1];
						if((*trapC).neighbours[1])
							(*trapC).neighbours[1]->neighbours[3] = &(*trapC);
					}
					AG_State *replacement = (*trapezoidIntersections[0]).treeNode;
					replacement->type = AG_XNODE;
					replacement->point = (*lsIt).end;
					replacement->left = new AG_State;
					replacement->left->type = AG_YNODE;
					replacement->left->lineSegment = (*lsIt);
					replacement->left->left = new AG_State;
					replacement->left->left->type = AG_LEAF;
					replacement->left->left->trapezoid = &(*trapB);
					(*trapB).treeNode = replacement->left->left;
					replacement->left->right = new AG_State;
					replacement->left->right->type = AG_LEAF;
					replacement->left->right->trapezoid = &(*trapC);
					(*trapC).treeNode = replacement->left->right;
					replacement->right = new AG_State;
					replacement->right->type = AG_LEAF;
					replacement->right->trapezoid = &(*trapA);
					(*trapA).treeNode = replacement->right;
					replacement->left->parents.push_back(replacement);
					replacement->right->parents.push_back(replacement);
					replacement->left->left->parents.push_back(replacement->left);
					replacement->left->right->parents.push_back(replacement->left);
				}
				else if(endPointUsed)
				{
					Trapezoid tA,tB,tC;
					tA.leftPoint = trapezoidIntersections[0]->leftPoint;
					tA.rightPoint = (*lsIt).start;
					tA.top = trapezoidIntersections[0]->top;
					tA.bottom = trapezoidIntersections[0]->bottom;
					tB.leftPoint = (*lsIt).start;
					tB.rightPoint = (*lsIt).end;
					tB.top = trapezoidIntersections[0]->top;
					tB.bottom = (*lsIt);
					tC.leftPoint = (*lsIt).start;
					tC.rightPoint = (*lsIt).end;
					tC.top = (*lsIt);
					tC.bottom = trapezoidIntersections[0]->bottom;
					trapezoidalList.push_back(tA);
					std::list<Trapezoid>::iterator trapA = trapezoidalList.end();
					trapA--;
					trapezoidalList.push_back(tB);
					std::list<Trapezoid>::iterator trapB = trapezoidalList.end();
					trapB--;
					trapezoidalList.push_back(tC);
					std::list<Trapezoid>::iterator trapC = trapezoidalList.end();
					trapC--;
					if(PointOnLineSegment((*lsIt).end,trapezoidIntersections[0]->top))
					{
						(*trapA).neighbours[1] = trapezoidIntersections[0]->neighbours[1];
						(*trapA).neighbours[2] = &(*trapB);
						(*trapA).neighbours[3] = &(*trapC);
						if((*trapA).neighbours[1])
							(*trapA).neighbours[1]->neighbours[3] = &(*trapA);
						(*trapB).neighbours[0] = &(*trapA);
						(*trapC).neighbours[1] = &(*trapA);
						(*trapC).neighbours[3] = trapezoidIntersections[0]->neighbours[3];
						if((*trapC).neighbours[3])
							(*trapC).neighbours[3]->neighbours[1] = &(*trapC);
					}
					else if(PointOnLineSegment((*lsIt).end,trapezoidIntersections[0]->bottom))
					{
						(*trapA).neighbours[0] = trapezoidIntersections[0]->neighbours[0];
						(*trapA).neighbours[2] = &(*trapB);
						(*trapA).neighbours[3] = &(*trapC);
						if((*trapA).neighbours[0])
							(*trapA).neighbours[0]->neighbours[2] = &(*trapA);
						(*trapB).neighbours[0] = &(*trapA);
						(*trapB).neighbours[2] = trapezoidIntersections[0]->neighbours[2];
						if((*trapB).neighbours[2])
							(*trapB).neighbours[2]->neighbours[0] = &(*trapB);
						(*trapC).neighbours[1] = &(*trapA);
					}
					else
					{
						(*trapA).neighbours[0] = trapezoidIntersections[0]->neighbours[0];
						(*trapA).neighbours[1] = trapezoidIntersections[0]->neighbours[1];
						(*trapA).neighbours[2] = &(*trapB);
						(*trapA).neighbours[3] = &(*trapC);
						if((*trapA).neighbours[0])
							(*trapA).neighbours[0]->neighbours[2] = &(*trapA);
						if((*trapA).neighbours[1])
							(*trapA).neighbours[1]->neighbours[3] = &(*trapA);
						(*trapB).neighbours[0] = &(*trapA);
						(*trapB).neighbours[2] = trapezoidIntersections[0]->neighbours[2];
						if((*trapB).neighbours[2])
							(*trapB).neighbours[2]->neighbours[0] = &(*trapB);
						(*trapC).neighbours[1] = &(*trapA);
						(*trapC).neighbours[3] = trapezoidIntersections[0]->neighbours[3];
						if((*trapC).neighbours[3])
							(*trapC).neighbours[3]->neighbours[1] = &(*trapC);
					}
					AG_State *replacement = (*trapezoidIntersections[0]).treeNode;
					replacement->type = AG_XNODE;
					replacement->point = (*lsIt).start;
					replacement->left = new AG_State;
					replacement->left->type = AG_LEAF;
					replacement->left->trapezoid = &(*trapA);
					(*trapA).treeNode = replacement->left;
					replacement->right = new AG_State;
					replacement->right->type = AG_YNODE;
					replacement->right->lineSegment = (*lsIt);
					replacement->right->left = new AG_State;
					replacement->right->left->type = AG_LEAF;
					replacement->right->left->trapezoid = &(*trapB);
					(*trapB).treeNode = replacement->right->left;
					replacement->right->right = new AG_State;
					replacement->right->right->type = AG_LEAF;
					replacement->right->right->trapezoid = &(*trapC);
					(*trapC).treeNode = replacement->right->right;
					replacement->left->parents.push_back(replacement);
					replacement->right->parents.push_back(replacement);
					replacement->right->left->parents.push_back(replacement->right);
					replacement->right->right->parents.push_back(replacement->right);
				}
				else
				{
					Trapezoid tA,tB,tC,tD;
					tA.leftPoint = trapezoidIntersections[0]->leftPoint;
					tA.rightPoint = (*lsIt).start;
					tA.top = trapezoidIntersections[0]->top;
					tA.bottom = trapezoidIntersections[0]->bottom;
					tB.leftPoint = (*lsIt).end;
					tB.rightPoint = trapezoidIntersections[0]->rightPoint;
					tB.top = trapezoidIntersections[0]->top;
					tB.bottom = trapezoidIntersections[0]->bottom;
					tC.leftPoint = (*lsIt).start;
					tC.rightPoint = (*lsIt).end;
					tC.top = trapezoidIntersections[0]->top;
					tC.bottom = (*lsIt);
					tD.leftPoint = (*lsIt).start;
					tD.rightPoint = (*lsIt).end;
					tD.top = (*lsIt);
					tD.bottom = trapezoidIntersections[0]->bottom;
					trapezoidalList.push_back(tA);
					std::list<Trapezoid>::iterator trapA = trapezoidalList.end();
					trapA--;
					trapezoidalList.push_back(tB);
					std::list<Trapezoid>::iterator trapB = trapezoidalList.end();
					trapB--;
					trapezoidalList.push_back(tC);
					std::list<Trapezoid>::iterator trapC = trapezoidalList.end();
					trapC--;
					trapezoidalList.push_back(tD);
					std::list<Trapezoid>::iterator trapD = trapezoidalList.end();
					trapD--;
					(*trapA).neighbours[0] = trapezoidIntersections[0]->neighbours[0];
					(*trapA).neighbours[1] = trapezoidIntersections[0]->neighbours[1];
					(*trapA).neighbours[2] = &(*trapC);
					(*trapA).neighbours[3] = &(*trapD);
					if((*trapA).neighbours[0])
						(*trapA).neighbours[0]->neighbours[2] = &(*trapA);
					if((*trapA).neighbours[1])
						(*trapA).neighbours[1]->neighbours[3] = &(*trapA);
					(*trapB).neighbours[0] = &(*trapC);
					(*trapB).neighbours[1] = &(*trapD);
					(*trapB).neighbours[2] = trapezoidIntersections[0]->neighbours[2];
					(*trapB).neighbours[3] = trapezoidIntersections[0]->neighbours[3];
					if((*trapB).neighbours[2])
						(*trapB).neighbours[2]->neighbours[0] = &(*trapB);
					if((*trapB).neighbours[3])
						(*trapB).neighbours[3]->neighbours[1] = &(*trapB);
					(*trapC).neighbours[0] = &(*trapA);
					(*trapC).neighbours[2] = &(*trapB);
					(*trapD).neighbours[1] = &(*trapA);
					(*trapD).neighbours[3] = &(*trapB);
					AG_State *replacement = (*trapezoidIntersections[0]).treeNode;
					replacement->type = AG_XNODE;
					replacement->point = (*lsIt).start;
					replacement->left = new AG_State;
					replacement->left->type = AG_LEAF;
					replacement->left->trapezoid = &(*trapA);
					(*trapA).treeNode = replacement->left;
					replacement->right = new AG_State;
					replacement->right->type = AG_XNODE;
					replacement->right->point = (*lsIt).end;
					replacement->right->right = new AG_State;
					replacement->right->right->type = AG_LEAF;
					replacement->right->right->trapezoid = &(*trapB);
					(*trapB).treeNode = replacement->right->right;
					replacement->right->left = new AG_State;
					replacement->right->left->type = AG_YNODE;
					replacement->right->left->lineSegment = (*lsIt);
					replacement->right->left->left = new AG_State;
					replacement->right->left->left->type = AG_LEAF;
					replacement->right->left->left->trapezoid = &(*trapC);
					(*trapC).treeNode = replacement->right->left->left;
					replacement->right->left->right = new AG_State;
					replacement->right->left->right->type = AG_LEAF;
					replacement->right->left->right->trapezoid = &(*trapD);
					(*trapD).treeNode = replacement->right->left->right;
					replacement->left->parents.push_back(replacement);
					replacement->right->parents.push_back(replacement);
					replacement->right->right->parents.push_back(replacement->right);
					replacement->right->left->parents.push_back(replacement->right);
					replacement->right->left->left->parents.push_back(replacement->right->left);
					replacement->right->left->right->parents.push_back(replacement->right->left);
				}
				trapezoidalList.remove(*trapezoidIntersections[0]);
			}
			else
			{
				Trapezoid *previousTopPosition,*previousBottomPosition;
				if(startPointUsed)
				{
					Trapezoid tA,tB;
					tA.leftPoint = (*lsIt).start; 
					tA.rightPoint = trapezoidIntersections[0]->rightPoint; 
					tA.top = trapezoidIntersections[0]->top; 
					tA.bottom = (*lsIt);
					tB.leftPoint = (*lsIt).start; 
					tB.rightPoint = trapezoidIntersections[0]->rightPoint;
					tB.top = (*lsIt); 
					tB.bottom = trapezoidIntersections[0]->bottom;
					trapezoidalList.push_back(tA);
					std::list<Trapezoid>::iterator trapA = trapezoidalList.end();
					trapA--;
					trapezoidalList.push_back(tB);
					std::list<Trapezoid>::iterator trapB = trapezoidalList.end();
					trapB--;
					previousTopPosition = &(*trapA);
					previousBottomPosition = &(*trapB);
					if(Classify_Point_To_LineSegment(trapezoidIntersections[0]-> rightPoint,(*lsIt)) == CL_ABOVE)
					{
						(*trapA).neighbours[0] = trapezoidIntersections[0]->neighbours[0]; 
						if((*trapA).neighbours[0]) 
							(*trapA).neighbours[0]->neighbours[2] = &(*trapA);
						(*trapA).neighbours[2] = trapezoidIntersections[0]->neighbours[2]; 
						if((*trapA).neighbours[2]) 
							(*trapA).neighbours[2]->neighbours[0] = &(*trapA);
						(*trapB).neighbours[1] = trapezoidIntersections[0]->neighbours[1];
						if((*trapB).neighbours[1])
							(*trapB).neighbours[1]->neighbours[3] = &(*trapB);
					}
					else
					{
						(*trapA).neighbours[0] = trapezoidIntersections[0]->neighbours[0];
						if((*trapA).neighbours[0])
							(*trapA).neighbours[0]->neighbours[2] = &(*trapA);
						(*trapB).neighbours[1] = trapezoidIntersections[0]->neighbours[1];
						if((*trapB).neighbours[1])
							(*trapB).neighbours[1]->neighbours[3] = &(*trapB);
						(*trapB).neighbours[3] = trapezoidIntersections[0]->neighbours[3];
						if((*trapB).neighbours[3])
							(*trapB).neighbours[3]->neighbours[1] = &(*trapB);
					}
					AG_State *replacement = (*trapezoidIntersections[0]).treeNode;
					replacement->type = AG_YNODE;
					replacement->lineSegment = (*lsIt);
					replacement->left = new AG_State;
					replacement->left->type = AG_LEAF;
					replacement->left->trapezoid = &(*trapA);
					(*trapA).treeNode = replacement->left;
					replacement->right = new AG_State;
					replacement->right->type = AG_LEAF;
					replacement->right->trapezoid = &(*trapB);
					(*trapB).treeNode = replacement->right;
					replacement->left->parents.push_back(replacement);
					replacement->right->parents.push_back(replacement);
				}
				else
				{
					Trapezoid tA,tB,tC;
					tA.leftPoint = trapezoidIntersections[0]->leftPoint;
					tA.rightPoint = (*lsIt).start;
					tA.top = trapezoidIntersections[0]->top;
					tA.bottom = trapezoidIntersections[0]->bottom;
					tB.leftPoint = (*lsIt).start;
					tB.rightPoint = trapezoidIntersections[0]->rightPoint;
					tB.top = trapezoidIntersections[0]->top;
					tB.bottom = (*lsIt);
					tC.leftPoint = (*lsIt).start;
					tC.rightPoint = trapezoidIntersections[0]->rightPoint;
					tC.top = (*lsIt);
					tC.bottom = trapezoidIntersections[0]->bottom;
					trapezoidalList.push_back(tA);
					std::list<Trapezoid>::iterator trapA = trapezoidalList.end();
					trapA--;
					trapezoidalList.push_back(tB);
					std::list<Trapezoid>::iterator trapB = trapezoidalList.end();
					trapB--;
					trapezoidalList.push_back(tC);
					std::list<Trapezoid>::iterator trapC = trapezoidalList.end();
					trapC--;
					previousTopPosition = &(*trapB);
					previousBottomPosition = &(*trapC);
					(*trapA).neighbours[0] = trapezoidIntersections[0]->neighbours[0];
					(*trapA).neighbours[1] = trapezoidIntersections[0]->neighbours[1];
					(*trapA).neighbours[2] = &(*trapB);
					(*trapA).neighbours[3] = &(*trapC);
					if((*trapA).neighbours[0])
						(*trapA).neighbours[0]->neighbours[2] = &(*trapA);
					if((*trapA).neighbours[1])
						(*trapA).neighbours[1]->neighbours[3] = &(*trapA);
					if(Classify_Point_To_LineSegment(trapezoidIntersections[0]->rightPoint,(*lsIt)) == CL_ABOVE)
					{
						(*trapB).neighbours[0] = &(*trapA);
						if((*trapB).neighbours[0])
							(*trapB).neighbours[0]->neighbours[2] = &(*trapB);
						(*trapB).neighbours[2] = trapezoidIntersections[0]->neighbours[2];
						if((*trapB).neighbours[2])
							(*trapB).neighbours[2]->neighbours[0] = &(*trapB);
						(*trapC).neighbours[1] = &(*trapA);
						if((*trapC).neighbours[1])
							(*trapC).neighbours[1]->neighbours[3] = &(*trapC);
					}
					else
					{
						(*trapB).neighbours[0] = &(*trapA);
						if((*trapB).neighbours[0])
							(*trapB).neighbours[0]->neighbours[2] = &(*trapB);
						(*trapC).neighbours[1] = &(*trapA);
						if((*trapC).neighbours[1])
							(*trapC).neighbours[1]->neighbours[3] = &(*trapC);
						(*trapC).neighbours[3] = trapezoidIntersections[0]->neighbours[3];
						if((*trapC).neighbours[3])
							(*trapC).neighbours[3]->neighbours[1] = &(*trapC);
					}
					AG_State *replacement = (*trapezoidIntersections[0]).treeNode;
					replacement->type = AG_XNODE;
					replacement->point = (*lsIt).start;
					replacement->left = new AG_State;
					replacement->left->type = AG_LEAF;
					replacement->left->trapezoid = &(*trapA);
					(*trapA).treeNode = replacement->left;
					replacement->right = new AG_State;
					replacement->right->type = AG_YNODE;
					replacement->right->lineSegment = (*lsIt);
					replacement->right->right = new AG_State;
					replacement->right->right->type = AG_LEAF;
					replacement->right->right->trapezoid = &(*trapC);
					(*trapC).treeNode = replacement->right->right;
					replacement->right->left = new AG_State;
					replacement->right->left->type = AG_LEAF;
					replacement->right->left->trapezoid = &(*trapB);
					(*trapB).treeNode = replacement->right->left;
					replacement->left->parents.push_back(replacement);
					replacement->right->parents.push_back(replacement);
					replacement->right->left->parents.push_back(replacement->right);
					replacement->right->right->parents.push_back(replacement->right);
				}
				for(unsigned int t = 1;t < tcount - 1; ++t)
				{
					if(Classify_Point_To_LineSegment(trapezoidIntersections[t]->leftPoint,(*lsIt)) == CL_ABOVE)
					{
						Trapezoid tA;
						tA.leftPoint = trapezoidIntersections[t]->leftPoint;
						tA.rightPoint = trapezoidIntersections[t]->rightPoint; 
						tA.top = trapezoidIntersections[t]->top; 
						tA.bottom = (*lsIt);
						trapezoidalList.push_back(tA);
						std::list<Trapezoid>::iterator trapA = trapezoidalList.end();
						trapA--;
						Trapezoid *trapB = previousBottomPosition;
						(*trapB).rightPoint = trapezoidIntersections[t]->rightPoint;
						(*trapA).neighbours[1] = previousTopPosition; 
						if((*trapA).neighbours[1]) 
							(*trapA).neighbours[1]->neighbours[3] = &(*trapA);
						(*trapA).neighbours[0] = trapezoidIntersections[t]->neighbours[0]; 
						if((*trapA).neighbours[0]) 
							(*trapA).neighbours[0]->neighbours[2] = &(*trapA);
						(*trapA).neighbours[2] = trapezoidIntersections[t]->neighbours[2]; 
						if((*trapA).neighbours[2]) 
							(*trapA).neighbours[2]->neighbours[0] = &(*trapA);
						(*trapB).neighbours[3] = trapezoidIntersections[t]->neighbours[3]; 
						if((*trapB).neighbours[3]) 
							(*trapB).neighbours[3]->neighbours[1] = &(*trapB);
						previousTopPosition = &(*trapA);
						AG_State *replacement = (*trapezoidIntersections[t]).treeNode;
						replacement->type = AG_YNODE;
						replacement->lineSegment = (*lsIt);
						replacement->left = new AG_State;
						replacement->left->type = AG_LEAF;
						replacement->left->trapezoid = &(*trapA);
						trapA->treeNode = replacement->left;
						replacement->left->parents.push_back(replacement);
						replacement->right = previousBottomPosition->treeNode;
						std::vector<AG_State *>::iterator stateIt;
						int flag = 1;
						for(stateIt = replacement->right->parents.begin();stateIt!=replacement->right->parents.end();stateIt++)
						{
							if(*stateIt == replacement)
							{
								flag = 0;
								break;
							}
						}
						if(flag == 1)
						{
							replacement->right->parents.push_back(replacement);
						}
					}
					else
					{
						Trapezoid tB;
						Trapezoid *trapA = previousTopPosition;
						(*trapA).rightPoint = trapezoidIntersections[t]->rightPoint;
						tB.leftPoint = trapezoidIntersections[t]->leftPoint;
						tB.rightPoint = trapezoidIntersections[t]->rightPoint;
						tB.top = (*lsIt);
						tB.bottom = trapezoidIntersections[t]->bottom;
						trapezoidalList.push_back(tB);
						std::list<Trapezoid>::iterator trapB = trapezoidalList.end();
						trapB--;
						(*trapA).neighbours[2] = trapezoidIntersections[t]->neighbours[2];
						if((*trapA).neighbours[2])
							(*trapA).neighbours[2]->neighbours[0] = &(*trapA);
						(*trapB).neighbours[0] = previousBottomPosition;//previous bottom partition
						if((*trapB).neighbours[0])
							(*trapB).neighbours[0]->neighbours[2] = &(*trapB);
						(*trapB).neighbours[1] = trapezoidIntersections[t]->neighbours[1];
						if((*trapB).neighbours[1])
							(*trapB).neighbours[1]->neighbours[3] = &(*trapB);
						(*trapB).neighbours[3] = trapezoidIntersections[t]->neighbours[3];
						if((*trapB).neighbours[3])
							(*trapB).neighbours[3]->neighbours[1] = &(*trapB);
						previousBottomPosition = &(*trapB);
						AG_State *replacement = (*trapezoidIntersections[t]).treeNode;
						replacement->type = AG_YNODE;
						replacement->lineSegment = (*lsIt);
						replacement->left = previousTopPosition->treeNode;
						std::vector<AG_State *>::iterator stateIt;
						int flag = 1;
						for(stateIt = replacement->left->parents.begin();stateIt!=replacement->left->parents.end();stateIt++)
						{
							if(*stateIt == replacement)
							{
								flag = 0;
								break;
							}
						}
						if(flag == 1)
						{
							replacement->left->parents.push_back(replacement);
						}
						replacement->right = new AG_State;
						replacement->right->type = AG_LEAF;
						replacement->right->trapezoid = &(*trapB);
						trapB->treeNode = replacement->right;
						replacement->right->parents.push_back(replacement);
					}
				}
				unsigned int tcount2 = tcount-1;
				if(endPointUsed) 
				{
					if(Classify_Point_To_LineSegment(trapezoidIntersections[tcount2]->leftPoint, (*lsIt)) == CL_ABOVE)
					{
						Trapezoid tA,*trapB;
						tA.leftPoint = trapezoidIntersections[tcount2]->leftPoint;
						tA.rightPoint = trapezoidIntersections[tcount2]->rightPoint; 
						tA.top = trapezoidIntersections[tcount2]->top;
						tA.bottom = (*lsIt);
						trapB = previousBottomPosition;
						(*trapB).rightPoint = trapezoidIntersections[tcount2]->rightPoint;
						trapezoidalList.push_back(tA);
						std::list<Trapezoid>::iterator trapA = trapezoidalList.end();
						trapA--;
						(*trapA).neighbours[0] = trapezoidIntersections[tcount2]->neighbours[0];
						if((*trapA).neighbours[0]) 
							(*trapA).neighbours[0]->neighbours[2] = &(*trapA); 
						(*trapA).neighbours[1] = previousTopPosition;
						if((*trapA).neighbours[1]) 
							(*trapA).neighbours[1]->neighbours[3] = &(*trapA);
						(*trapA).neighbours[2] = trapezoidIntersections[tcount2]->neighbours[2]; 
						if((*trapA).neighbours[2]) 
							(*trapA).neighbours[2]->neighbours[0] = &(*trapA);
						(*trapB).neighbours[3] = trapezoidIntersections[tcount2]->neighbours[3];
						if((*trapB).neighbours[3]) 
							(*trapB).neighbours[3]->neighbours[1] = &(*trapB);
						AG_State *replacement = (*trapezoidIntersections[tcount2]).treeNode;
						replacement->type = AG_YNODE;
						replacement->lineSegment = (*lsIt);
						replacement->left = new AG_State;
						replacement->left->type = AG_LEAF;
						replacement->left->trapezoid = &(*trapA);
						trapA->treeNode = replacement->left;
						replacement->left->parents.push_back(replacement);
						replacement->right = previousBottomPosition->treeNode;
						std::vector<AG_State *>::iterator stateIt;
						int flag = 1;
						for(stateIt = replacement->right->parents.begin();stateIt!=replacement->right->parents.end();stateIt++)
						{
							if(*stateIt == replacement)
							{
								flag = 0;
								break;
							}
						}
						if(flag == 1)
						{
							replacement->right->parents.push_back(replacement);
						}
					}
					else
					{
						Trapezoid *trapA,tB;
						trapA = previousTopPosition;
						(*trapA).rightPoint = trapezoidIntersections[tcount2]->rightPoint;
						tB.leftPoint = trapezoidIntersections[tcount2]->leftPoint;
						tB.rightPoint = trapezoidIntersections[tcount2]->rightPoint;
						tB.top = (*lsIt);
						tB.bottom = trapezoidIntersections[tcount2]->bottom;
						trapezoidalList.push_back(tB);
						std::list<Trapezoid>::iterator trapB = trapezoidalList.end();
						trapB--;
						(*trapA).neighbours[2] = trapezoidIntersections[tcount2]->neighbours[2];
						if((*trapA).neighbours[2])
							(*trapA).neighbours[2]->neighbours[0] = &(*trapA);
						(*trapB).neighbours[0] = previousBottomPosition;
						(*trapB).neighbours[1] = trapezoidIntersections[tcount2]->neighbours[1];
						if((*trapB).neighbours[0])
							(*trapB).neighbours[0]->neighbours[2] = &(*trapB);
						if((*trapB).neighbours[1])
							(*trapB).neighbours[1]->neighbours[3] = &(*trapB);
						(*trapB).neighbours[3] = trapezoidIntersections[tcount2]->neighbours[3];
						if((*trapB).neighbours[3])
							(*trapB).neighbours[3]->neighbours[1] = &(*trapB);

						AG_State *replacement = (*trapezoidIntersections[tcount2]).treeNode;
						replacement->type = AG_YNODE;
						replacement->lineSegment = (*lsIt);
						replacement->left = previousTopPosition->treeNode;
						std::vector<AG_State *>::iterator stateIt;
						int flag = 1;
						for(stateIt = replacement->left->parents.begin();stateIt!=replacement->left->parents.end();stateIt++)
						{
							if(*stateIt == replacement)
							{
								flag = 0;
								break;
							}
						}
						if(flag == 1)
						{
							replacement->left->parents.push_back(replacement);
						}
						replacement->right = new AG_State;
						replacement->right->type = AG_LEAF;
						replacement->right->trapezoid = &(*trapB);
						trapB->treeNode = replacement->right;
						replacement->right->parents.push_back(replacement);
					}
				}
				else
				{
					Trapezoid tA,tB,tC,*trapB,*trapC;
					tA.leftPoint = (*lsIt).end;
					tA.rightPoint = trapezoidIntersections[tcount2]->rightPoint;
					tA.top = trapezoidIntersections[tcount2]->top;
					tA.bottom = trapezoidIntersections[tcount2]->bottom;
					trapezoidalList.push_back(tA);
					std::list<Trapezoid>::iterator trapA = trapezoidalList.end();
					trapA--;
					bool above = false;
					if(Classify_Point_To_LineSegment(trapezoidIntersections[tcount2]->leftPoint,(*lsIt)) == CL_ABOVE)
					{
						above = true;
						tB.leftPoint = trapezoidIntersections[tcount2]->leftPoint;
						tB.rightPoint = (*lsIt).end;
						tB.top = trapezoidIntersections[tcount2]->top;
						tB.bottom = (*lsIt);
						trapezoidalList.push_back(tB);
						std::list<Trapezoid>::iterator trapB_it = trapezoidalList.end();
						trapB_it--;
						trapB = &(*trapB_it);
						trapC = previousBottomPosition;
						(*trapC).rightPoint = (*lsIt).end;
					}
					else
					{
						trapB = previousTopPosition;
						(*trapB).rightPoint = (*lsIt).end;
						tC.leftPoint = trapezoidIntersections[tcount2]->leftPoint;
						tC.rightPoint = (*lsIt).end;
						tC.top = (*lsIt);
						tC.bottom = trapezoidIntersections[tcount2]->bottom;
						trapezoidalList.push_back(tC);
						std::list<Trapezoid>::iterator trapC_it = trapezoidalList.end();
						trapC_it--;
						trapC = &(*trapC_it);
					}
					if(above)
					{
						(*trapA).neighbours[0] = &(*trapB);
						(*trapA).neighbours[1] = &(*trapC);
					}
					else
					{
						(*trapA).neighbours[0] = &(*trapB);
						(*trapA).neighbours[1] = &(*trapC);
					}
					(*trapA).neighbours[2] = trapezoidIntersections[tcount2]->neighbours[2]; 
					(*trapA).neighbours[3] = trapezoidIntersections[tcount2]->neighbours[3]; 
					if((*trapA).neighbours[0]) 
						(*trapA).neighbours[0]->neighbours[2] = &(*trapA); 
					if((*trapA).neighbours[1]) 
						(*trapA).neighbours[1]->neighbours[3] = &(*trapA); 
					if((*trapA).neighbours[2]) 
						(*trapA).neighbours[2]->neighbours[0] = &(*trapA); 
					if((*trapA).neighbours[3]) 
						(*trapA).neighbours[3]->neighbours[1] = &(*trapA);
					if(above)
					{
						(*trapB).neighbours[0] = trapezoidIntersections[tcount2]->neighbours[0]; 
						if((*trapB).neighbours[0]) 
							(*trapB).neighbours[0]->neighbours[2] = &(*trapB); 
						(*trapB).neighbours[1] = previousTopPosition;
						if((*trapB).neighbours[1]) 
							(*trapB).neighbours[1]->neighbours[3] = &(*trapB);
						(*trapB).neighbours[2] = &(*trapA); 
						if((*trapB).neighbours[2]) 
							(*trapB).neighbours[2]->neighbours[0] = &(*trapB);
						(*trapC).neighbours[3] = &(*trapA);
					}
					else
					{
						(*trapB).neighbours[2] = &(*trapA); 
						(*trapC).neighbours[0] = previousBottomPosition;
						if((*trapC).neighbours[0])
							(*trapC).neighbours[0]->neighbours[2] = &(*trapC);
						(*trapC).neighbours[3] = &(*trapA);
						if((*trapC).neighbours[3])
							(*trapC).neighbours[3]->neighbours[1] = &(*trapC);
						(*trapC).neighbours[1] = trapezoidIntersections[tcount2]->neighbours[1];
						if((*trapC).neighbours[1])
							(*trapC).neighbours[1]->neighbours[3] = &(*trapC);
					}
					if(above)
					{
						AG_State *replacement = (*trapezoidIntersections[tcount2]).treeNode;
						replacement->type = AG_XNODE;
						replacement->point = (*lsIt).end;
						replacement->right = new AG_State;
						replacement->right->type = AG_LEAF;
						replacement->right->trapezoid = &(*trapA);
						(*trapA).treeNode = replacement->right;
						replacement->right->parents.push_back(replacement);
						replacement->left = new AG_State;
						replacement->left->type = AG_YNODE;
						replacement->left->lineSegment = (*lsIt);
						replacement->left->parents.push_back(replacement);
						replacement->left->left = new AG_State;
						replacement->left->left->type = AG_LEAF;
						replacement->left->left->trapezoid = &(*trapB);
						(*trapB).treeNode = replacement->left->left;
						replacement->left->left->parents.push_back(replacement->left);
						replacement->left->right = previousBottomPosition->treeNode;
						std::vector<AG_State *>::iterator stateIt;
						int flag = 1;
						for(stateIt = replacement->left->right->parents.begin();stateIt!=replacement->left->right->parents.end();stateIt++)
						{
							if(*stateIt == replacement->left)
							{
								flag = 0;
								break;
							}
						}
						if(flag == 1)
						{
							replacement->left->right->parents.push_back(replacement->left);
						}
					}
					else
					{
						AG_State *replacement = (*trapezoidIntersections[tcount2]).treeNode;
						replacement->type = AG_XNODE;
						replacement->point = (*lsIt).end;
						replacement->right = new AG_State;
						replacement->right->type = AG_LEAF;
						replacement->right->trapezoid = &(*trapA);
						(*trapA).treeNode = replacement->right;
						replacement->right->parents.push_back(replacement);
						replacement->left = new AG_State;
						replacement->left->type = AG_YNODE;
						replacement->left->lineSegment = (*lsIt);
						replacement->left->parents.push_back(replacement);
						replacement->left->right = new AG_State;
						replacement->left->right->type = AG_LEAF;
						replacement->left->right->trapezoid = &(*trapC);
						(*trapC).treeNode = replacement->left->right;
						replacement->left->right->parents.push_back(replacement->left);
						replacement->left->left = previousTopPosition->treeNode;
						std::vector<AG_State *>::iterator stateIt;
						int flag = 1;
						for(stateIt = replacement->left->left->parents.begin();stateIt!=replacement->left->left->parents.end();stateIt++)
						{
							if(*stateIt == replacement->left)
							{
								flag = 0;
								break;
							}
						}
						if(flag == 1)
						{
							replacement->left->left->parents.push_back(replacement->left);
						}
					}
				}
				for(unsigned int k = 0; k < tcount; k++)
					trapezoidalList.remove(*trapezoidIntersections[k]);
			}
		}
	}
}

void MotionPlanning_PointRobot::RemoveUnusedTrapezoids()
{
	std::list<Trapezoid>::iterator tr = trapezoidalList.begin();
	while (tr != trapezoidalList.end())
	{
		if((*tr).top.normal.y < 0 && (*tr).bottom.normal.y > 0)
		{
			unusedTrapezoidalList.push_back((*tr));
			tr = trapezoidalList.erase(tr);
		}
		else
			tr++;
	}
}

void MotionPlanning_PointRobot::AdjustRemainingTrapezoids()
{
	//Adjust the end points of all of the trapezoid's top-bottom line segments.
	std::list<Trapezoid>::iterator tr;
	for(tr = trapezoidalList.begin(); tr != trapezoidalList.end(); tr++)
	{
		float starty,endy,topslope;
		//set the end-points of the top segment
		topslope = (tr->top.end.y - tr->top.start.y)/(tr->top.end.x - tr->top.start.x);//different x(s)
		starty = topslope*(tr->leftPoint.x - tr->top.start.x) + tr->top.start.y;
		endy = topslope*(tr->rightPoint.x - tr->top.start.x) + tr->top.start.y;

		tr->top.start.x = tr->leftPoint.x;
		tr->top.start.y = starty;
		tr->top.end.x = tr->rightPoint.x;
		tr->top.end.y = endy;

        //set the end-points of the bottom segment
		topslope = (tr->bottom.end.y - tr->bottom.start.y)/(tr->bottom.end.x - tr->bottom.start.x);//different x(s)
		starty = topslope*(tr->leftPoint.x - tr->bottom.start.x) + tr->bottom.start.y;
		endy = topslope*(tr->rightPoint.x - tr->bottom.start.x) + tr->bottom.start.y;

		tr->bottom.start.x = tr->leftPoint.x;
		tr->bottom.start.y = starty;
		tr->bottom.end.x = tr->rightPoint.x;
		tr->bottom.end.y = endy;
	}
}

void MotionPlanning_PointRobot::PathBuilding()
{
	path = new Path;
	path->currentState = -1;
	Trapezoid *startTrapezoid;
	std::list<Trapezoid>::iterator tr;
	startTrapezoid = &(*trapezoidalList.begin());
	for(tr = trapezoidalList.begin();tr!=trapezoidalList.end();++tr)
	{
		if((*tr).leftPoint.x < startTrapezoid->leftPoint.x)
		{
			startTrapezoid = &(*tr);
		}
	}
	StatesBuild(startTrapezoid,path->currentState);
	std::vector<State>::iterator stateIt;
	for(int i = 0;i <= path->currentState; ++i)
	{
		std::vector<std::pair<DM_TYPE,float>> DM_row;
		std::vector<std::pair<PATH_TYPE,int>> PATH_row;
		for(int j = 0;j <= path->currentState; ++j)
		{
			if(i==j)
			{
				DM_row.push_back(std::make_pair(DEFINED,0.0f));
				PATH_row.push_back(std::make_pair(ADJACENT,-1));
			}
			else
			{
				DM_row.push_back(std::make_pair(VIRTUALMAX,float(MAX_DISTANCE)));
				PATH_row.push_back(std::make_pair(NOPATH,-1));
			}
		}
		path->DM_MATRIX.push_back(DM_row);
		path->PATH_MATRIX.push_back(PATH_row);
	}
	for(unsigned int i = 0;i < path->states.size(); ++i)
	{
		for(unsigned int j = 0;j < path->states[i].edges.size();++j)
		{
			Point2D p1(path->states[path->states[i].edges[j].from].worldPositionX,path->states[path->states[i].edges[j].from].worldPositionY);
			Point2D p2(path->states[path->states[i].edges[j].to].worldPositionX,path->states[path->states[i].edges[j].to].worldPositionY);
			float weight = distanceBetweenTwoPoints(p1,p2);
			path->DM_MATRIX[path->states[i].edges[j].from][path->states[i].edges[j].to] = std::make_pair(DEFINED,weight);
			path->PATH_MATRIX[path->states[i].edges[j].from][path->states[i].edges[j].to] = std::make_pair(ADJACENT,-1);
		}
	}
	path->FloydAlgorithm();
}

void MotionPlanning_PointRobot::StatesBuild(Trapezoid * next,int previousState)
{
	if(next->neighbourUsed[0] && next->neighbourUsed[1] && 
		next->neighbourUsed[2] && next->neighbourUsed[3])
	{
		Edge e1,e2;
		e1.from = next->stateIndex;
		e1.to = previousState;
		e2.from = previousState;
		e2.to = next->stateIndex;
		path->states[next->stateIndex].edges.push_back(e1);
		path->states[previousState].edges.push_back(e2);
		return;
	}
	if(!next->isVisited)
	{
		(path->currentState)++;
		next->stateIndex = path->currentState;
		State middle;
		middle.worldPositionX = (next->top.start.x + next->top.end.x +
			next->bottom.start.x + next->bottom.end.x)/4;
		middle.worldPositionY = (next->top.start.y + next->top.end.y +
			next->bottom.start.y + next->bottom.end.y)/4;
		path->states.push_back(middle);
		next->isVisited = true;
	}
	if(!next->neighbourUsed[0])
	{
		next->neighbourUsed[0] = true;
		if(next->neighbours[0])
		{
			State between;
			between.worldPositionX = next->top.start.x;
			between.worldPositionY = (next->top.start.y + next->leftPoint.y)/2;
			path->states.push_back(between);
			(path->currentState)++;
			Edge e1,e2;
			e1.from = next->stateIndex;
			e1.to = path->currentState;
			e2.from = path->currentState;
			e2.to = next->stateIndex;
			path->states[next->stateIndex].edges.push_back(e1);
			path->states[path->currentState].edges.push_back(e2);
			next->neighbours[0]->neighbourUsed[2] = true;
			StatesBuild(next->neighbours[0],path->currentState);
		}
	}
	if(!next->neighbourUsed[1])
	{
		next->neighbourUsed[1] = true;
		if(next->neighbours[1])
		{
			State between;
			between.worldPositionX = next->bottom.start.x;
			between.worldPositionY = (next->bottom.start.y + next->leftPoint.y)/2;
			path->states.push_back(between);
			(path->currentState)++;
			Edge e1,e2;
			e1.from = next->stateIndex;
			e1.to = path->currentState;
			e2.from = path->currentState;
			e2.to = next->stateIndex;
			path->states[next->stateIndex].edges.push_back(e1);
			path->states[path->currentState].edges.push_back(e2);
			next->neighbours[1]->neighbourUsed[3] = true;
			StatesBuild(next->neighbours[1],path->currentState);
		}
	}
	if(!next->neighbourUsed[2])
	{
		next->neighbourUsed[2] = true;
		if(next->neighbours[2])
		{
			State between;
			between.worldPositionX = next->top.end.x;
			between.worldPositionY = (next->top.end.y + next->rightPoint.y)/2;
			path->states.push_back(between);
			(path->currentState)++;
			Edge e1,e2;
			e1.from = next->stateIndex;
			e1.to = path->currentState;
			e2.from = path->currentState;
			e2.to = next->stateIndex;
			path->states[next->stateIndex].edges.push_back(e1);
			path->states[path->currentState].edges.push_back(e2);
			next->neighbours[2]->neighbourUsed[0] = true;
			StatesBuild(next->neighbours[2],path->currentState);
		}
	}
	if(!next->neighbourUsed[3])
	{
		next->neighbourUsed[3] = true;
		if(next->neighbours[3])
		{
			State between;
			between.worldPositionX = next->bottom.end.x;
			between.worldPositionY = (next->bottom.end.y + next->rightPoint.y)/2;
			path->states.push_back(between);
			(path->currentState)++;
			Edge e1,e2;
			e1.from = next->stateIndex;
			e1.to = path->currentState;
			e2.from = path->currentState;
			e2.to = next->stateIndex;
			path->states[next->stateIndex].edges.push_back(e1);
			path->states[path->currentState].edges.push_back(e2);
			next->neighbours[3]->neighbourUsed[1] = true;
			StatesBuild(next->neighbours[3],path->currentState);
		}
	}
	if(previousState!=-1)
	{
		Edge e1,e2;
		e1.from = next->stateIndex;
		e1.to = previousState;
		e2.from = previousState;
		e2.to = next->stateIndex;
		path->states[next->stateIndex].edges.push_back(e1);
		path->states[previousState].edges.push_back(e2);
	}
}

Trapezoid MotionPlanning_PointRobot::Calculate_BoundingRectangle()
{
	Trapezoid res;
	float minX=MAX_DISTANCE,maxX=0.0f,minY=MAX_DISTANCE,maxY=0.0f;

	unsigned int sizeLS = segmentList.size();
	for(unsigned int i = 0; i < sizeLS; i++)
	{
		if(segmentList[i].start.x < minX)
			minX = segmentList[i].start.x;
		if(segmentList[i].start.x > maxX)
			maxX = segmentList[i].start.x;

		if(segmentList[i].start.y < minY)
			minY = segmentList[i].start.y;
		if(segmentList[i].start.y > maxY)
			maxY = segmentList[i].start.y;

		if(segmentList[i].end.x < minX)
			minX = segmentList[i].end.x;
		if(segmentList[i].end.x > maxX)
			maxX = segmentList[i].end.x;

		if(segmentList[i].end.y < minY)
			minY = segmentList[i].end.y;
		if(segmentList[i].end.y > maxY)
			maxY = segmentList[i].end.y;
	}
	minX -= BOUNDING_RECT_MARGIN;
	maxX += BOUNDING_RECT_MARGIN;
	minY -= BOUNDING_RECT_MARGIN;
	maxY += BOUNDING_RECT_MARGIN;

	res.leftPoint.x = minX;
	res.leftPoint.y = maxY;
	res.rightPoint.x = maxX;
	res.rightPoint.y = minY;

	res.top.start.x = minX;
	res.top.start.y = minY;
	res.top.end.x = maxX;
	res.top.end.y = minY;
	res.bottom.start.x = minX;
	res.bottom.start.y = maxY;
	res.bottom.end.x = maxX;
	res.bottom.end.y = maxY;

	res.neighbours[0] = NULL;
	res.neighbours[1] = NULL;
	res.neighbours[2] = NULL;
	res.neighbours[3] = NULL;

	res.top.normal.x = 0;
	res.top.normal.y = 1;

	res.bottom.normal.x = 0;
	res.bottom.normal.y = -1;

	return res;
}

float MotionPlanning_PointRobot::distanceBetweenTwoPoints(const Point2D &p1,const Point2D &p2)
{
	return(sqrt(((p2.x-p1.x)*(p2.x-p1.x))+((p2.y-p1.y)*(p2.y-p1.y))));
}

unsigned int MotionPlanning_PointRobot::sizeOftrapezoidalList()
{
	return trapezoidalList.size();
}

unsigned int MotionPlanning_PointRobot::sizeOfunusedTrapezoidalList()
{
	return unusedTrapezoidalList.size();
}

std::list<Trapezoid> MotionPlanning_PointRobot::retrieveTrapezoidalList()
{
	return trapezoidalList;
}

std::list<Trapezoid> MotionPlanning_PointRobot::retrieveUnusedTrapezoidalList()
{
	return unusedTrapezoidalList;
}

