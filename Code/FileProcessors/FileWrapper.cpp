/******************************************************************************/
/*!
\file FileWrapper.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Implementation of FileWrapper class
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#include "FileWrapper.h"

/******************************************************************************/
/*!
Tokenize is used to seperate the input string to different strings based on delimiters
\param [in] str
String to be tokenized
\param [out] tokens
Reference to the string of tokens
\param delimiters
Reference to the delimiters
\return
void
*/
/******************************************************************************/
void FileWrapper::Tokenize(const std::string& str,std::vector<std::string>& tokens,const std::string& delimiters)
{
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	std::string::size_type pos = str.find_first_of(delimiters, lastPos);
	while(std::string::npos != pos || std::string::npos != lastPos)
	{
		/// Repeatedly find the substrings based on delimiters and push it to tokens string
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		lastPos = str.find_first_not_of(delimiters, pos);
		pos = str.find_first_of(delimiters, lastPos);
	}	
}

/******************************************************************************/
/*!
readTokenToNum uses stringstream to read varying data from the token
\param [in] s
Input token
\param [out] n
unsigned short data which is output of the function
\return
void
*/
/******************************************************************************/
void FileWrapper::readTokenToNum(std::string &s,unsigned short &n)
{
	std::istringstream is;
	is.str(s);
	is>>n;
}

/******************************************************************************/
/*!
readTokenToNum uses stringstream to read varying data from the token
\param [in] s
Input token
\param [out] n
unsigned char data which is output of the function
\return
void
*/
/******************************************************************************/
void FileWrapper::readTokenToNum(std::string &s,unsigned char &n)
{
	unsigned int num;
	std::istringstream is;
	is.str(s);
	is>>num;
	n = (char)num;
}

/******************************************************************************/
/*!
Constructor for FileWrapper
\param [in] fileName
Name of the input file
*/
/******************************************************************************/
FileWrapper::FileWrapper(const char *fileName)
{
	file = new FileHandler(fileName);
	file->open(std::ios::in);
}

/******************************************************************************/
/*!
Destructor for FileWrapper
*/
/******************************************************************************/
FileWrapper::~FileWrapper()
{
	file->close();
	delete file;
}

/******************************************************************************/
/*!
read data specifics for this project from input file
\return
void
*/
/******************************************************************************/
void FileWrapper::readData()
{
	std::string str;
	std::vector<std::string> tokens;
	/// Read Grid related data
	str = file->readLine();
	Tokenize(str, tokens, "|=,");
	readTokenToNum(tokens[2],gridX);
	readTokenToNum(tokens[4],gridY);
	readTokenToNum(tokens[6],grid.r);
	readTokenToNum(tokens[7],grid.g);
	readTokenToNum(tokens[8],grid.b);
	tokens.clear();
	/// Read Cube related data
	str = file->readLine();
	Tokenize(str, tokens, "|=,");
	readTokenToNum(tokens[2],cubeX);
	readTokenToNum(tokens[4],cube.r);
	readTokenToNum(tokens[5],cube.g);
	readTokenToNum(tokens[6],cube.b);
	tokens.clear();
	/// Read Cylinder related data
	str = file->readLine();
	Tokenize(str, tokens, "|=,");
	readTokenToNum(tokens[2],cylStk);
	readTokenToNum(tokens[4],cylSli);
	readTokenToNum(tokens[6],cylinder.r);
	readTokenToNum(tokens[7],cylinder.g);
	readTokenToNum(tokens[8],cylinder.b);
	tokens.clear();
	/// Read Cone related data
	str = file->readLine();
	Tokenize(str, tokens, "|=,");
	readTokenToNum(tokens[2],coneStk);
	readTokenToNum(tokens[4],coneSli);
	readTokenToNum(tokens[6],cone.r);
	readTokenToNum(tokens[7],cone.g);
	readTokenToNum(tokens[8],cone.b);
	tokens.clear();
	/// Read Sphere related data
	str = file->readLine();
	Tokenize(str, tokens, "|=,");
	readTokenToNum(tokens[2],sphStk);
	readTokenToNum(tokens[4],sphSli);
	readTokenToNum(tokens[6],sphere.r);
	readTokenToNum(tokens[7],sphere.g);
	readTokenToNum(tokens[8],sphere.b);
}

/******************************************************************************/
/*!
Check Data validates data specifics for this project from input file
\return
bool - Indicates validation status
*/
/******************************************************************************/
bool FileWrapper::checkData()
{
	if((gridX<1)||(gridY<1)||(cubeX<1)||(cylStk<1)||(cylSli<3)||(coneStk<2)||(coneSli<3)||(sphStk<2)||(sphSli<3))
		return false;
	else 
		return true;
}