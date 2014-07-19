/******************************************************************************/
/*!
\file FileHandler.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Handles opening,closing and reading from a file
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include<fstream>
#include<iostream>
#include<string>
#include<vector>

//!  FileHandler class
/*!
Class for handling file opening,closing and reading
*/

class FileHandler
{
	std::ifstream in; /*!< input file stream */
	std::string file; /*!< string for holding file name*/
	bool openFlag;	  /*!< Flag to check if file is opened*/
public:
	FileHandler(const char *fileName);
	~FileHandler();
	void open(std::ios_base::openmode mode);
	void close();
	std::string read(unsigned long &NUMBYTES);
	std::string readLine();
};


#endif