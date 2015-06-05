#ifndef _SCENE_SERVER_MSG_HANDLER_H_
#define _SCENE_SERVER_MSG_HANDLER_H_

#include "Config.h"
#include "BaseSingle.h"

class SceneServerProcHandler : public BaseSingle<SceneServerProcHandler>
{
public:
	SceneServerProcHandler();
	~SceneServerProcHandler();
};

#endif


