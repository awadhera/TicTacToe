/******************************************************************************/
/*!
\file TGALoader.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Conatins functions for loading TGA files
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#include "TGALoader.h"

/******************************************************************************/
/*!
Reads the TGA data and loads it into object of type TGA
\param [out] tgaObj
Reference to TGA object
\param [in] imageFile
char pointer pointing to TGA filename
\return
void
*/
/******************************************************************************/
bool TGALoad(TGA &tgaObj,char *imageFile)
{
	std::ifstream in;
	in.open(imageFile,std::ios::in);
	if(in.fail())
	{
		return false;
	}
	char *imageType = new char[3];
	in.read(imageType,3);
	in.seekg (12, std::ios::cur);
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
	delete [] imageType;
	delete [] imageInfo;
	in.close();
	return true;
}

