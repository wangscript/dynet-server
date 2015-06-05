#ifndef SWA_CONFIG_H_
#define SWA_CONFIG_H_


#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/container/set.hpp>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


#include <memory.h>

using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::system;

using namespace std;

#ifdef WIN32
#define SSleep(ms) Sleep(ms)
#else
#define SSleep(ms) usleep(ms)
#endif

#ifdef DLL_FILE
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif

#define S_SAFE_DELETE(a) if( a ){ delete a;a=NULL; }
#define S_SAFE_RELEASE(a) if( a ){ a->Release(); }

#ifdef WIN32
typedef __int64                     int64;
#else
typedef int64_t                     int64;
#endif
typedef unsigned long long          uint64;
typedef wchar_t						wchar;
typedef unsigned char				uchar;
typedef char						int8;
typedef unsigned char				uint8;
typedef short						int16;
typedef unsigned short				uint16;
typedef int							int32;
typedef unsigned int				uint32;
typedef unsigned int				ulong;
typedef float						float32;
typedef double						float64;

#define MAX_WS_SOCKET_CONNECT 100


#endif