#ifndef RESOURCEMGR_H_
#define RESOURCEMGR_H_


#include "BaseDefine.h"
#include "BaseSingle.h"


class ResourceMgr : public BaseSingle<ResourceMgr>
{
public:
	ResourceMgr(){}
	~ResourceMgr(){}

	void	LoadAllRes(string resourePath)
	{

		//gPetCfg->LoadRes((resourePath + "pet.xml").c_str());

		//gPetLevelCfg->LoadRes((resourePath + "pet_level.xml").c_str());

		//gGoodsCfg->LoadRes((resourePath + "goods.xml").c_str());

		//gCharLevelCfg->LoadRes((resourePath + "character_level.xml").c_str());			

		//gSkillConfig->LoadRes((resourePath + "skill.xml").c_str());

		FLOG_INFO( "all config Inited." );

	}

private:

};


#define gResourceMgr  ResourceMgr::Instance()

#endif





