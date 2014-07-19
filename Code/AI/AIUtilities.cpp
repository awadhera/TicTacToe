/******************************************************************************/
/*!
\file AIUtilities.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Utilities file for AI
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#include "AIUtilities.h"

/******************************************************************************/
/*!
generateNrandom - Generate random numbers in range 0 - N-1
\param [out] random
Random array of unsigned integers from 0 - N-1
\param [in] bufferSize
Size of the random array
\return
void
*/
/******************************************************************************/
void generateNrandom(unsigned int random[],unsigned int bufferSize)
{
	static unsigned int randomCounter_1150_1 = 2;
	srand(randomCounter_1150_1);	
	int *checkArr;
	checkArr = new int[bufferSize];
	for(unsigned int i =0;i<bufferSize;++i)
	{
		checkArr[i] = 0;
	}
	int random_integer;
	unsigned int index = 0;
	while(index<bufferSize)
	{
		random_integer = (rand()%bufferSize);
		if(checkArr[random_integer]==0)
		{
			checkArr[random_integer] = 1;
			random[index] = random_integer;
			++index;
		}
	}
	delete [] checkArr;
	++randomCounter_1150_1;
}