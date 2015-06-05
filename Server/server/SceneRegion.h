#ifndef __SCENEREGION_H_
#define __SCENEREGION_H_

#include <vector>
#include <set>

#include "Geometry2D.h"
#include "UnorderedMap.h"
#include "ObjPool.h"

struct IChannelRegist;

class Entity;
class Channel;

class SceneRegion
{
public:
	SceneRegion() {}

	SceneRegion(const Rectangle2D& rRange);

	// 注册本区域频道
	void RegistReceiveChannels(IChannelRegist& rEntity);

	// 取消注册收集信息
	void CancelReceiveChannels(IChannelRegist& rEntity);

	// 增加实体
	void AddEntity(Entity& rEntity);

	// 移除实体
	void DelEntity(Entity& rEntity);

	// 检测是否在区域内
	bool InScope(const Point2D& rPosition);

	// 获得该区域的所有实现
	const std::set<Entity*> GetEntities() ;

private:
	// 添加频道
	void AddChannel(Channel& rChannel);

	// 注册广播者
	void RegistBroadcasterChannels(IChannelRegist& rEntity);

	// 取消广播者
	void CancelBroadcasterChannels(IChannelRegist& rEntity);

private:
	std::vector<Channel*>			m_vecChannels;		// 频道集合
	std::set<Entity*>				m_setEntities;		// 实体集合
	Rectangle2D						m_sScope;			// 区域范围

	friend class SceneRegionManager;
};

class SceneRegionManager
{
	typedef UNORDERED_MAP<Entity*,std::pair<SceneRegion* /*所在区域*/, std::set<SceneRegion*>/*查看区域*/>>	EntitySceneRegionCollectUMapType;
public:
	/*
	* X轴正方向向右,Z轴正方向上
	* @sTopLeftCoordinate:左上角起始坐标
	* @nXAmount:X轴方向Cell个数,@nZAmount:Z轴方向Cell个数
	* @nXCellLength:X轴方向Cell长度,@nZCellLength:Z轴方向Cell长度
	* vecUseableInfo:坐标Cell可用信息,[0][0]为左上角索引
	*/
	SceneRegionManager(Point2D sTopLeftCoordinate, int32 nXAmount, int32 nZAmount, int32 nXCellLength, int32 nZCellLength,
		const std::vector<std::vector<bool>>& vecUseableInfo);

	~SceneRegionManager();

	// 添加场景实体
	void AddSceneEntity(Entity& rEntity);

	// 删除场景实体
	void DelSceneEntity(Entity& rEntity);

	// 更新频道注册、更新区域中的实体集合
	void Update();

	// 获得碰撞区域
	void GetCollisionRegion(const Rectangle2D& i_rSrcRange, std::set<SceneRegion*>& o_setCollisionRegions);

protected:

protected:

private:
	// 获得指定区域
	SceneRegion* GetSceneRegion(int32 nZIndex, int32 nXIndex) const;

	// 获得所在区域
	SceneRegion* GetBelongRegion(const Point2D& rSrcCoordinate);

private:
	Rectangle2D												m_sScope;					// 范围
	const int32												m_nXAmount;					// X方向区域个数
	const int32												m_nZAmount;					// Z方向区域个数
	const int32												m_nXCellLength;				// X轴方向区域长度
	const int32												m_nZCellLength;				// Z轴方向区域长度
	std::vector<std::vector<SceneRegion*>>					m_vecRegions;				// 区域数组
	
	EntitySceneRegionCollectUMapType						m_umapEntityRegions;		// 实体相关区域

	static ObjPool<SceneRegion>								s_cSceneRegionFactory;		// 区域工厂

};

#endif

