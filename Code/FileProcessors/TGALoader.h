/******************************************************************************/
/*!
\file TGALoader.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Contains struct holding data of a TGA image
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef TGALOADER_H
#define TGALOADER_H

#include<fstream>
#include<iostream>
#include <GL/glew.h>

enum TGAcolorType { RGB, RGBA };
struct TGA
{
	TGA()
	{
		data = (char *)0;
		width = 0;
		height = 0;
		pixelSizeInBytes = 0;
	}
	TGAcolorType cType;	/*!< Color Type info of TGA file */
	int TexID;	/*!< ID given */
	char *data;	/*!< Data of the TGA image */
	int width;	/*!< Width of the TGA image */
	int height;	/*!< Height of the TGA image */
	int pixelSizeInBytes;	/*!< Size of each texel in image */
};

bool TGALoad(TGA &tgaObj,char *imageFile);

#endif