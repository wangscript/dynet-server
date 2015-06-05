//
//  logManager.cpp
//  test
//
//  Created by fox on 13-1-9.
//
//

#include "LogManager.h"

boost::mutex gMutex;

void  Logfox( int32 level , const char* str , ... )
{
	boost::mutex::scoped_lock lock( gMutex );    
    va_list va;
    static char sstr[255];
    static char sbuf[255];
    memset( sstr , 0 , 255 );
    memset( sbuf , 0 , 255 );
    
    va_start( va , str );
    vsprintf( sstr , str , va );
    va_end(va);
	switch(level)
	{
	case LOG_LEVEL_NORMAL:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY |FOREGROUND_GREEN);
	break;
	case LOG_LEVEL_INFO:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY);
	break;
	case LOG_LEVEL_WARRING:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY |FOREGROUND_RED | FOREGROUND_GREEN);
	break;
	case LOG_LEVEL_ERROR:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY |FOREGROUND_RED);
	break;
	default:
	break;
	}
    printf( "LOG:%s \n", sstr );
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY);
    lock.unlock();
}



