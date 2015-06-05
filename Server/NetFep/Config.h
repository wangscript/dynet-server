#ifndef FEP_CONFIG_H_
#define FEP_CONFIG_H_

#ifdef DLL_FILE
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif

#define MAX_FEP_SOCKET_CONNECT 1000

#endif