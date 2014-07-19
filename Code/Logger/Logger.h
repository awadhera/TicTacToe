/******************************************************************************/
/*!
\file Logger.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Interface for Logger class which handles logging to console/file
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef LOGGER_H
#define LOGGER_H

#include<iostream>
#include <GL/glew.h>
#include <GL/wglew.h>

//!  Logger class
/*!
Class for handling logging to console/file
*/

class Logger
{
	static Logger *s_pInstance;		/*!< Pointer to object of Logger which is static */
	FILE *pCout;	/*!< Streamhandler for stdout */
	FILE *pCerr;	/*!< Streamhandler for stderr */
	int flag;		/*!< Flag which indicates if console opening was successful*/
	Logger();
	~Logger();
	Logger(Logger &);
	Logger& operator = (Logger &);
public:
	static void create();
	static void destroy();
	static Logger &Instance();
	void log(const char *message,const char &type,const int &level,const int &line,const char *sourceFile);
	void checkAndReportGLError(const char *message,const char &type,const int &level,const int &line,const char *sourceFile);
};



#endif