#ifndef __ENTITY_H_
#define __ENTITY_H_

#include <bitset>
#include <assert.h>

#include "Geometry2D.h"
#include "SceneChannel.h"
#include "ObjPool.h"
#include "UnorderedSet.h"
#include "def_entity.h"
#include "def_attribute.h"
#include "def_creature.h"

class SceneRegion;
class SceneMap;

class Entity : public IChannelRegist, public IChannelOperate(ChannelPosition)
{
public:
	Entity(EntityType eEntityType,int32 nResId,bool bListen, int32 nRegistChannelFlags);

	// 是否注册指定频道
	virtual bool IsRegist(ChannelRegistType eType) const
	{
		return (m_nChannelRegistFlag & eType) != 0;
	}

	const uint64 ID() const
	{
		return m_nID;
	}

	const Vector2DEx& Heading() const
	{
		return m_sHeading;
	}

	void SetHeading(const Vector2D& sHeading)
	{
		// 取保正方向为单位向量
		m_sHeading = sHeading;
		m_sHeading.Normalize();
		m_sChannelPosition.nHeadingX = sHeading.nX;
		m_sChannelPosition.nHeadingZ = sHeading.nZ;
	}

	const Point2D& Position() const
	{
		return m_sPosition;
	}

	void SetPosition(const Point2D& sPosition)
	{
		m_sPosition = sPosition;
		m_sChannelPosition.nPositionX = sPosition.nX;
		m_sChannelPosition.nPositionZ = sPosition.nZ;

		m_sInViewRange.sTopLeft.nX		= m_sPosition.nX + m_nInViewRange;
		m_sInViewRange.sTopLeft.nZ		= m_sPosition.nZ - m_nInViewRange;
		m_sInViewRange.sBottomRight.nX	= m_sPosition.nX - m_nInViewRange;
		m_sInViewRange.sBottomRight.nZ	= m_sPosition.nZ + m_nInViewRange;

		m_sOutViewRangle.sTopLeft.nX		= m_sPosition.nX + m_nOutViewRangle;
		m_sOutViewRangle.sTopLeft.nZ		= m_sPosition.nZ - m_nOutViewRangle;
		m_sOutViewRangle.sBottomRight.nX	= m_sPosition.nX - m_nOutViewRangle;
		m_sOutViewRangle.sBottomRight.nZ	= m_sPosition.nZ + m_nOutViewRangle;

	}

	const RectangleRange& InViewRange() const
	{
		return m_sInViewRange;
	}

	const  RectangleRange& OutViewRange() const
	{
		return m_sOutViewRangle;
	}

	bool IsListen() const
	{
		return m_bListen;
	}

	// 属性修改
	bool SetAttribute(AttributeType eAttributeType, const ValueType& vNewValue);
	bool GetAttribute(AttributeType i_eAttributeType, ValueType& o_vValue);

	// 状态切换
	void StatusToggle(CreatureStatus eCreatureStatus);

protected:
	

	// 属性修改
	bool SetAttribute(AttributeType eAttributeType, const ValueType& vNewValue, bool bEnforce);

protected:
	AttributeOffset								m_sAttributeOffset[ATTRIBUTE_MAX];		// 属性偏移信息

private:

	//初始化成员位置偏移
	virtual void InitAttributeOffet();

	// 进入视野范围修改
	void OnInViewChange(const ValueType& vOldValue, const ValueType& vNewValue);

	// 离开视野范围修改
	void OnOutViewChange(const ValueType& vOldValue, const ValueType& vNewValue);

private:
	uint64										m_nID;									// 实体ID	
	Point2D										m_sPosition;							// 位置坐标
	Vector2DEx									m_sHeading;								// 朝向(保证朝向为单位向量)
	int32										m_nInViewRange;							// 进入视野距离
	int32										m_nOutViewRangle;						// 离开视野距离
	RectangleRange								m_sInViewRange;							// 进入视野范围
	RectangleRange								m_sOutViewRangle;						// 离开视野范围

	const bool									m_bListen;								// 是否监听频道
	int64										m_nChannelRegistFlag;					// 注册标志
};

#endif

