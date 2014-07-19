/******************************************************************************/
/*!
\file Logger.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Implementation of Logger class
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#include "Logger.h"

// static member Initialization
Logger *Logger::s_pInstance = 0;

/******************************************************************************/
/*!
Constructor for Logger class
*/
/******************************************************************************/
Logger::Logger() 
{
	/// Create console for Win32 application 
	flag = AllocConsole();
	if(flag)
	{
		/// Attack console to the Win32 application process
		AttachConsole(GetCurrentProcessId());
		/// Provide stream handles for console output and error
		freopen_s(&pCout,"CONOUT$", "w", stdout );
		freopen_s(&pCerr,"CONOUT$", "w", stderr );
	}
}

/******************************************************************************/
/*!
Destructor for Logger class
*/
/******************************************************************************/
Logger::~Logger() 
{
	if(flag)
	{
		/// Close the console streams
		fclose(pCout);
		fclose(pCerr);
		/// Detach from Win32 application and close the console.
		FreeConsole();
	}
}

/******************************************************************************/
/*!
Create static instance of Logger class
\return
void
*/
/******************************************************************************/
void Logger::create() 
{
	s_pInstance = new Logger;
}

/******************************************************************************/
/*!
delete static instance of Logger class
\return
void
*/
/******************************************************************************/
void Logger::destroy() 
{
	delete s_pInstance;
}

/******************************************************************************/
/*!
Return reference to a static instance of type:Logger
\return
Reference to object of type of Logger
*/
/******************************************************************************/
Logger& Logger::Instance()
{
	return *s_pInstance;
}

/******************************************************************************/
/*!
Log Function to output log messages on the console
\param message
Message string of type char *
\param type
Type of messages - Error:e, Warning:w, Info:i
\param level
Level of log message. Higher levels indicate higher severity
\param line
Source code line number where the error occured
\param sourceFile
Source code File name where the error occured
\return
void
*/
/******************************************************************************/
void Logger::log(const char *message,const char &type,const int &level,const int &line,const char *sourceFile)
{
	char *message_type;
	switch(type)
	{
	case 'i':
		message_type = "INFO";
		break;
	case 'w':
		message_type = "WARNING";
		break;
	default:
		message_type = "ERROR";
		break;
	}
	std::cout<<"\n/////////////////////////////////////////////\n";
	std::cout<<sourceFile<<":"<<line<<":"<<message_type<<":"<<"Level"<<" "<<level<<":"<<message;
	std::cout<<"\n/////////////////////////////////////////////\n";
}

/******************************************************************************/
/*!
Check for openGL error and report if any
\param message
Message string of type char *
\param type
Type of messages - Error:e, Warning:w, Info:i
\param level
Level of log message. Higher levels indicate higher severity
\param line
Source code line number where the error occured
\param sourceFile
Source code File name where the error occured
\return
void
*/
/******************************************************************************/
void Logger::checkAndReportGLError(const char *message,const char &type,const int &level,const int &line,const char *sourceFile)
{
	if( glGetError() != GL_NO_ERROR )
	{
		log(message,type,level,line,sourceFile);
	}
}