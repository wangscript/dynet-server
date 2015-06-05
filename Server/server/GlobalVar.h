#ifndef __GLOBALVAR_H_
#define __GLOBALVAR_H_

#include "ResourceMgr.h"
#include "GameConfig.h"
#include "SceneChannel.h"
#include "SceneMap.h"
#include "Player.h"

#include "../DyNetMysql/DbSession.h"


static PlayerMgr&	g_rPlayerMgr  = *(PlayerMgr::Instance());
static ResourceMgr& g_rResourceMgr = *(ResourceMgr::Instance());
static GameConfig&	g_rGameCfg = *(GameConfig::Instance());

static DbSessionMgr& g_rDbDatabaseMgr = *(DbSessionMgr::Instance());
static SceneChannelManager&	g_rSceneChannelManager = *(SceneChannelManager::Instance());
static SceneMapManager& g_rSceneMapMgr = *(SceneMapManager::Instance());
	

#endif