#ifndef __SCENEMAP_H_
#define __SCENEMAP_H_

#include "SceneRegion.h"

#include <vector>
#include <set>
#include <typedef.h>
#include "UnorderedMap.h"
#include "CommonDefine.h"

//地图信息
class SceneMap
{
public:

	SceneMap(uint32 nMapId,Point2D sTopLeftCoordinate, int32 nXAmount, int32 nZAmount, int32 nXCellLength, int32 nZCellLength,
		const std::vector<std::vector<bool>>& vecUseableInfo);

	~SceneMap(void);

	// 对区域管理器进行更新
	void Update();

	// 获得与rEntity相交的区域的entitys
	void GetNeighborsEntity(Entity& rEntity ,std::vector<Entity*>& o_setEnities);

	// 获得地图ID
	uint32 MapID()
	{
		return m_nMapId;
	}

private:

	// 添加地图实体
	void AddMapEntity(Entity& rEntity);

	// 删除地图实体
	void DelMapEntity(Entity& rEntity);

private:

	uint32					m_nMapId;			// 地图ID

	SceneRegionManager		m_oRegionManager;	//管理对象
	std::set<Entity*>		m_setEntities;		//实体对象

	friend class			SceneMapManager;

};

// 角色地图信息
struct CharacterMapInfo
{
	uint32 nLandMapId;		// 主地图ID
	uint32 nInstanceMapId;  // 副本地图ID
};

//地图管理类
class SceneMapManager : public BaseSingle<SceneMapManager>
{
		typedef std::map<uint32,SceneMap*>			SceneMapMapType;
		typedef UNORDERED_MAP<Entity*,SceneMap*>	EntityUMapType;
public:

	SceneMapManager();
	~SceneMapManager();

	// 增加地图信息
	void	AddSceneMap(uint32 nMapId,Point2D sTopLeftCoordinate, int32 nXAmount, int32 nZAmount, int32 nXCellLength, int32 nZCellLength,const std::vector<std::vector<bool>>& vecUseableInfo);

	//获得地图信息
	SceneMap* GetSceneMap(uint32 nMapId) ;

	//获得所在的地图
	SceneMap* GetEntitySceneMap(Entity& rEntity);

	//地图中添加一个实体
	bool AddSceneEnity(uint32 nMapId, Entity& rEntity);

	//删除一个地图的实体
	bool DelSceneEnity(Entity& rEntity);

	// 地图更新
	void Update();

	// 获得所在地图ID
	uint32 GetOnMapId(Entity& entity);

private:

	SceneMapMapType								m_mapSceneMaps;					// 地图场景信息
	UNORDERED_MAP<Entity*,SceneMap*>			m_umapEntities;					// 实体所在地图
	static ObjPool<SceneMap>					s_cSceneMapFactory;				// 地图工厂

};

#endif

