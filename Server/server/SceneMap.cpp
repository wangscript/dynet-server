#include "Server_PCH.h"
#include "SceneMap.h"
#include "Entity.h"

using namespace std;

SceneMap::SceneMap(uint32 nMapId,Point2D sTopLeftCoordinate, int32 nXAmount, int32 nZAmount, int32 nXCellLength, int32 nZCellLength,const vector<vector<bool>>& vecUseableInfo)
	:m_nMapId(nMapId),m_oRegionManager(sTopLeftCoordinate,nXAmount,nZAmount,nXCellLength,nZCellLength,vecUseableInfo)
{

}

SceneMap::~SceneMap(void)
{

}

// 添加地图实体
void SceneMap::AddMapEntity(Entity& rEntity)
{
	if(m_setEntities.find(&rEntity) != m_setEntities.end())
		return;
	m_setEntities.insert(&rEntity);
	m_oRegionManager.AddSceneEntity(rEntity);
}

// 删除地图实体
void SceneMap::DelMapEntity(Entity& rEntity)
{
	m_setEntities.erase(&rEntity);
	// 也将删除地图中的实体
	m_oRegionManager.DelSceneEntity(rEntity);
}

// 获得与rEntity相交的区域的entitys
void SceneMap::GetNeighborsEntity(Entity& rEntity ,std::vector<Entity*>& o_setEnities)
{
	set<SceneRegion*> o_setCollisionRegions;
	m_oRegionManager.GetCollisionRegion(rEntity.OutViewRange(), o_setCollisionRegions);
	for (set<SceneRegion*>::iterator it = o_setCollisionRegions.begin(); it != o_setCollisionRegions.end(); ++it)
	{
		const std::set<Entity*> setEntities = (*it)->GetEntities();
		for (set<Entity*>::const_iterator it2 = setEntities.begin(); it2 !=setEntities.end();++it2  )
		{
			o_setEnities.push_back(*it2);
		}	
	}
}

void SceneMap::Update()
{
	m_oRegionManager.Update();
}

//---------------------------------------------------------------------------------------

ObjPool<SceneMap> SceneMapManager::s_cSceneMapFactory(false,5);

SceneMapManager::SceneMapManager()
{
}

SceneMapManager::~SceneMapManager()
{
}

void	SceneMapManager::AddSceneMap(uint32 nMapId,Point2D sTopLeftCoordinate, int32 nXAmount, int32 nZAmount, int32 nXCellLength, int32 nZCellLength,
					 const vector<vector<bool>>& vecUseableInfo)
{
	if(m_mapSceneMaps.find(nMapId) != m_mapSceneMaps.end())
		return;

	if(SceneMap* pSceneMap = s_cSceneMapFactory.CreateObj(nMapId,sTopLeftCoordinate,nXAmount,nZAmount,nXCellLength,nZCellLength,vecUseableInfo))
	{
		m_mapSceneMaps.insert(make_pair(nMapId,pSceneMap));
	}
}

SceneMap* SceneMapManager::GetSceneMap(uint32 nMapId) 
{
	SceneMapMapType::iterator it  = m_mapSceneMaps.find(nMapId);
	if(it == m_mapSceneMaps.end())
		return NULL;
	return it->second;
}

SceneMap* SceneMapManager::GetEntitySceneMap(Entity& rEntity)
{
	EntityUMapType::iterator it = m_umapEntities.find(&rEntity);
	if(it == m_umapEntities.end())
		return NULL;
	return it->second;
}

//地图中添加一个实体
bool SceneMapManager::AddSceneEnity(uint32 nMapId, Entity& rEntity)
{

	SceneMapMapType::iterator it = m_mapSceneMaps.find(nMapId);
	if(it == m_mapSceneMaps.end())
		return false;

	(it->second)->AddMapEntity(rEntity);
	m_umapEntities.insert(make_pair(&rEntity,it->second));
	return true;

}

//删除一个地图的实体
bool SceneMapManager::DelSceneEnity(Entity& rEntity)
{

	EntityUMapType::iterator it = m_umapEntities.find(&rEntity);
	if(it == m_umapEntities.end())
		return false;

	it->second->DelMapEntity(rEntity);
	m_umapEntities.erase(&rEntity);
	return true;

}

// 地图更新
void SceneMapManager::Update()
{
	for (SceneMapMapType::iterator it = m_mapSceneMaps.begin(); it != m_mapSceneMaps.end(); ++it)
	{
		it->second->Update();
	}
}

// 获得所在地图ID
uint32 SceneMapManager::GetOnMapId(Entity& entity)
{
	if(SceneMap* pSceneMap = GetEntitySceneMap(entity))
	{
		return pSceneMap->MapID();
	}
	return 0;
}