/******************************************************************************/
/*!
\file FileWrapper.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Wrapper for handling file specifics for this project
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef FILEWRAPPER_H
#define FILEWRAPPER_H

#include <sstream>
#include "FileHandler.h"
#include "../Geometry/ObjectUtility.h"

//!  FileWrapper class
/*!
Class for handling file specifics for this project
*/
class FileWrapper
{
	FileHandler *file;	/*!< Pointer to an object of FileHandler */
	void Tokenize(const std::string& str,std::vector<std::string>& tokens,const std::string& delimiters = " ");
	void readTokenToNum(std::string &s,unsigned short &n);
	void readTokenToNum(std::string &s,unsigned char &n);
public:
	unsigned short gridX; /*!< Specifics read from input file for grid */
	unsigned short gridY; /*!< Specifics read from input file for grid */
	unsigned short cubeX; /*!< Specifics read from input file for cube */
	unsigned short cubeY; /*!< Specifics read from input file for cube */
	unsigned short cylStk; /*!< Specifics read from input file for cylinder */
	unsigned short cylSli; /*!< Specifics read from input file for cylinder */
	unsigned short coneStk; /*!< Specifics read from input file for cone */
	unsigned short coneSli; /*!< Specifics read from input file for cone */
	unsigned short sphStk; /*!< Specifics read from input file for sphere */
	unsigned short sphSli; /*!< Specifics read from input file for sphere */
	Color4 grid; /*!< Color structure for grid */
	Color4 cube;  /*!< Color structure for cube */
	Color4 cylinder;  /*!< Color structure for cylinder */
	Color4 cone;  /*!< Color structure for cone */
	Color4 sphere;  /*!< Color structure for sphere */
	FileWrapper(const char *fileName);
	~FileWrapper();
	void readData();
	bool checkData();
};

#endif