//
//  LogManager.h
//  test
//
//  Created by fox on 13-1-9.
//
//

#ifndef SWALLOW_LOG_MANAGER_H_
#define SWALLOW_LOG_MANAGER_H_

#include "BaseDefine.h"

enum ELogLevel
{
	LOG_LEVEL_NORMAL = 0,
	LOG_LEVEL_INFO,
	LOG_LEVEL_WARRING,
    LOG_LEVEL_ERROR,
};
    
void Logfox( int32 level , const char* str , ... );
    
#ifdef WIN32
#define FLOG_NORMAL( fmt , ... ) Logfox( LOG_LEVEL_NORMAL , fmt , __VA_ARGS__ );
#define FLOG_INFO( fmt , ... ) Logfox( LOG_LEVEL_INFO , fmt , __VA_ARGS__ );
#define FLOG_WARRING( fmt , ... ) Logfox( LOG_LEVEL_WARRING , fmt , __VA_ARGS__ );
#define FLOG_ERROR( fmt , ... ) Logfox( LOG_LEVEL_ERROR , fmt , __VA_ARGS__ );
#else
#define FLOG_NORMAL( fmt , a... ) Logfox( LOG_LEVEL_NORMAL , fmt , ##a );
#define FLOG_INFO( fmt , a... ) Logfox( LOG_LEVEL_INFO , fmt , ##a );
#define FLOG_WARRING( fmt , a... ) Logfox( LOG_LEVEL_WARRING , fmt , ##a );
#define FLOG_ERROR( fmt , a... ) Logfox( LOG_LEVEL_ERROR , fmt , ##a );
#endif


#endif
