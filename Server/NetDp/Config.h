#ifndef DP_CONFIG_H_
#define DP_CONFIG_H_

#ifdef DLL_FILE
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif

#define MAX_SERVER_CONNECTED 32

#endif