#ifndef SS_CONFIG_H_
#define SS_CONFIG_H_

#ifdef DLL_FILE
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif



#endif