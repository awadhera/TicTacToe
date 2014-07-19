/******************************************************************************/
/*!
\file FileHandler.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Implementation of FileHandler class
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#include "FileHandler.h"
#include "../Logger/Logger.h"

/******************************************************************************/
/*!
Constructor of FileHandler class
\param fileName
Name of the file to be handled
*/
/******************************************************************************/
FileHandler::FileHandler(const char *fileName)
{
	file.insert(0,fileName);
	/// Set openFlag to false. Indicates whether file is currently opened
	openFlag = false;
}

/******************************************************************************/
/*!
open the file
\param mode
Mode in which file is opened
\return
void
*/
/******************************************************************************/
void FileHandler::open(std::ios_base::openmode mode)
{
	in.open(file,mode);
	if(in.fail())
	{
		(Logger::Instance()).log("Failed to open file",'e',4,__LINE__,__FILE__);
	}
	else
	{
		/// Mark file as opened
		openFlag = true;
	}
}

/******************************************************************************/
/*!
close the file
\return
void
*/
/******************************************************************************/
void FileHandler::close()
{
	/// If file is currently opened - close it
	if(openFlag)
		in.close();
}

/******************************************************************************/
/*!
Destructor for FileHandler class
*/
/******************************************************************************/
FileHandler::~FileHandler()
{
	close();
}

/******************************************************************************/
/*!
Read data from file as bytes
\param NUMBYTES
Number of bytes to be read
\return
std::string - Return the data read as a string
*/
/******************************************************************************/
std::string FileHandler::read(unsigned long &NUMBYTES)
{
	std::string retStr;
	if(openFlag)
	{		
		std::streamsize size;
		char *arr = new char[NUMBYTES];
		/// read NUMBYTES to char array
		in.read(arr,NUMBYTES);
		if(in.fail())
		{
			if(in.eof())
			{
				/// if end of file is reached - read all the bytes till the end
				size = in.gcount();
				NUMBYTES = (unsigned long)size;
			}
			else
			{
				(Logger::Instance()).log("Failed to read data from file",'e',4,__LINE__,__FILE__);
			}
		}
		retStr.insert(0,arr,NUMBYTES);
		delete [] arr;
	}
	else
	{
		(Logger::Instance()).log("File is not opened.Cannot read data",'e',4,__LINE__,__FILE__);
	}
	return retStr;
}

/******************************************************************************/
/*!
Read data from file as strings
\return
std::string - Return the data read as a string
*/
/******************************************************************************/
std::string FileHandler::readLine()
{
	std::string retStr;
	if(openFlag)
	{
		/// Read a line of data from file
		std::getline(in,retStr);
		if(in.fail())
		{
			(Logger::Instance()).log("Failed to read a line from file",'e',4,__LINE__,__FILE__);
		}
	}
	else
	{
		(Logger::Instance()).log("File is not opened.Cannot read data",'e',4,__LINE__,__FILE__);
	}
	return retStr;
}