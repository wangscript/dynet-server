/*
* 自动扩展对象池(线程不安全)
*
* 作者：bill.allen@163.com
*/

#ifndef __OBJPOOL_H_
#define __OBJPOOL_H_

#include <stack>

#include "BaseDefine.h"
#include "UnorderedSet.h"

typedef UNORDERED_SET<void*> UsingBlocksSet;

// 自动扩展内存池(线程安全)
class MemPool
{
public:
	MemPool(bool bInit, uint64 nSizeBlock, uint64 nNumPerSlice = 1024)
		: m_bInit(bInit), m_nBlockSize(nSizeBlock), m_nBlockNumPerSlice(nNumPerSlice), m_nSliceSize(nSizeBlock * nNumPerSlice)
	{
		// 初始化不申请任何内存
	}

	virtual ~MemPool()
	{
		// 释放申请的内存片
		while (m_staMemSlices.size())
		{
			void* pSlice = m_staMemSlices.top();
			free(pSlice);
			m_staMemSlices.pop();
		}
	}

protected:
	virtual void* Create()
	{
		if (!m_staBlocks.size())
		{
			if (!enlarge())
			{
				return 0;
			}
		}

		void* pNewBlock = m_staBlocks.top();
		m_staBlocks.pop();
		m_setUsingBlocks.insert(pNewBlock);

		return pNewBlock;
	}

	virtual bool Destroy(void* pAllocatedBlock)
	{
		UsingBlocksSet::iterator iter = m_setUsingBlocks.find(pAllocatedBlock);
		if (iter != m_setUsingBlocks.end())
		{
			m_staBlocks.push(pAllocatedBlock);
			m_setUsingBlocks.erase(iter);

			return true;
		}
		else
		{
			return false;
		}
	}

private:

	// 申请内存片并管理内存块
	bool enlarge()
	{
		// 内存片指针
		void* pNewSlice = 0;

		// 申请内存片
		if (m_bInit)
		{// 初始化0
			pNewSlice = calloc(m_nBlockNumPerSlice, m_nBlockSize);
		}
		else
		{// 未初始化
			pNewSlice = malloc(m_nSliceSize);
		}

		if (!pNewSlice)
		{
			return false;
		}

		// 管理内存片
		m_staMemSlices.push(pNewSlice);

		// 管理内存块
		int8* pBlock = reinterpret_cast<int8*>(pNewSlice);
		for (uint64 i = 0; i < m_nBlockNumPerSlice; ++i)
		{
			m_staBlocks.push(pBlock + m_nBlockSize * i);
		}

		return true;
	}

protected:
	UsingBlocksSet		m_setUsingBlocks;		// 使用中的内存块

private:
	const bool			m_bInit;					// 是否初始化申请的内存
	const uint64		m_nBlockSize;				// 内存块大小
	const uint64		m_nBlockNumPerSlice;		// 内存片中内存块数量
	const uint64		m_nSliceSize;				// 内存片大小
	std::stack<void*>	m_staMemSlices;				// 内存片
	std::stack<void*>	m_staBlocks;				// 内存块引用
};

// 对象内存使用状态
struct ObjMemInfo
{
	bool		bUsing;			// 正在使用
	uint64		nMark;			// 内存标记(同一块内存分配给不同对象时标记不同，永不为0,目前为内存使用次数)
};

// 使用管理信息的内存块
class ManageInfoUsingBlocks
{
public:
	// 获取内存使用标记,返回0为获取失败
	template<typename T>
	static uint64 GetMark(T* pAllocatedObj)
	{
		if (HaveManageInfo(pAllocatedObj))
		{
			ObjMemInfo& rInfo = *reinterpret_cast<ObjMemInfo*>(pAllocatedObj + sizeof(T));

			return rInfo.nMark;
		}

		return 0;
	}

	// 检测正在使用中的内存与标记是否一致
	template<typename T>
	static bool CheckMark(T* pAllocatedObj, uint64 nMark)
	{
		if (HaveManageInfo(pAllocatedObj))
		{
			ObjMemInfo& rInfo = *reinterpret_cast<ObjMemInfo*>(pAllocatedObj + sizeof(T));

			return rInfo.bUsing && !(rInfo.nMark ^ nMark);
		}

		return false;
	}

public:
	static UsingBlocksSet		s_setBlocks;	// 使用中含管理信息的内存块

private:
	// 检测内存是否被管理
	static bool HaveManageInfo(void* pAllocatedObj)
	{
		return s_setBlocks.find(pAllocatedObj) != s_setBlocks.end();
	}
};

// 对象池(必须实现CreateObj才可使用)
template<typename T>
class ObjPool : protected MemPool
{
public:
	ObjPool(bool bIsManageInfo = false, uint64 nNumPerSlice = 1024)
		: MemPool(bIsManageInfo, (bIsManageInfo ? (sizeof(T) + sizeof(ObjMemInfo)) : sizeof(T)), nNumPerSlice), m_bManageInfo(bIsManageInfo)
	{
	}

	virtual ~ObjPool()
	{
		// 保证在释放内存片之前,调用正在使用的内存块上对象的析构函数
		for (UsingBlocksSet::iterator iter = m_setUsingBlocks.begin();
			iter != m_setUsingBlocks.end(); ++iter)
		{
			T* pT = reinterpret_cast<T*>(*iter);
			pT->~T();
		}
	}

	// T* CreateObj(...)
	T* CreateObj()
	{
		if (void* pMem = Create())
		{
			return new(pMem)T();
		}
		return 0;
	}

	template<typename PT1>
	T* CreateObj(PT1 param1)
	{
		if (void* pMem = Create())
		{
			return new(pMem)T(param1);
		}
		return 0;
	}

	template<typename PT1, typename PT2>
	T* CreateObj(PT1 param1, PT2 param2)
	{
		if (void* pMem = Create())
		{
			return new(pMem)T(param1, param2);
		}
		return 0;
	}

	template<typename PT1, typename PT2, typename PT3>
	T* CreateObj(PT1 param1, PT2 param2, PT3 param3)
	{
		if (void* pMem = Create())
		{
			return new(pMem)T(param1, param2, param3);
		}
		return 0;
	}

	template<typename PT1, typename PT2, typename PT3, typename PT4>
	T* CreateObj(PT1 param1, PT2 param2, PT3 param3, PT4 param4)
	{
		if (void* pMem = Create())
		{
			return new(pMem)T(param1, param2, param3, param4);
		}
		return 0;
	}

	template<typename PT1, typename PT2, typename PT3, typename PT4, typename PT5>
	T* CreateObj(PT1 param1, PT2 param2, PT3 param3, PT4 param4, PT5 param5)
	{
		if (void* pMem = Create())
		{
			return new(pMem)T(param1, param2, param3, param4, param5);
		}
		return 0;
	}

	template<typename PT1, typename PT2, typename PT3, typename PT4, typename PT5 , typename PT6>
	T* CreateObj(PT1 param1, PT2 param2, PT3 param3, PT4 param4, PT5 param5, PT6 param6)
	{
		if (void* pMem = Create())
		{
			return new(pMem)T(param1, param2, param3, param4, param5,param6);
		}
		return 0;
	}

	template<typename PT1, typename PT2, typename PT3, typename PT4, typename PT5 , typename PT6, typename PT7>
	T* CreateObj(PT1 param1, PT2 param2, PT3 param3, PT4 param4, PT5 param5, PT6 param6, PT7 param7)
	{
		if (void* pMem = Create())
		{
			return new(pMem)T(param1, param2, param3, param4, param5,param6,param7);
		}
		return 0;
	}

	bool DestroyObj(T* pAllocatedObj)
	{
		if (Destroy(pAllocatedObj))
		{
			pAllocatedObj->~T();

			return true;
		}
		else
		{
			return false;
		}
	}

protected:
	virtual void* Create()
	{
		if (uint8* pNewBlock = reinterpret_cast<uint8*>(MemPool::Create()))
		{
			if (m_bManageInfo)
			{
				ObjMemInfo& rInfo = *reinterpret_cast<ObjMemInfo*>(pNewBlock + sizeof(T));
				rInfo.bUsing = true;
				++rInfo.nMark;				// 使用标记永不为0

				ManageInfoUsingBlocks::s_setBlocks.insert(pNewBlock);
			}

			return pNewBlock;
		}
		else
		{
			return 0;
		}
	}

	virtual bool Destroy(void* pAllocatedBlock)
	{
		if (MemPool::Destroy(pAllocatedBlock))
		{
			if (m_bManageInfo)
			{
				UsingBlocksSet::iterator iter = ManageInfoUsingBlocks::s_setBlocks.find(pAllocatedBlock);
				if (iter != ManageInfoUsingBlocks::s_setBlocks.end())
				{
					uint8* pBlock = reinterpret_cast<uint8*>(pAllocatedBlock);
					ObjMemInfo& rInfo = *reinterpret_cast<ObjMemInfo*>(pBlock + sizeof(T));
					rInfo.bUsing = false;

					ManageInfoUsingBlocks::s_setBlocks.erase(iter);
				}
			}

			return true;
		}
		else
		{
			return false;
		}
	}

private:
	bool						m_bManageInfo;				// 是否有内存管理信息
};

// 对象池管理接口(用于不符合谁创建谁销毁的情况)
struct IObjPoolManagement
{
public:
	virtual void recycle() = 0;
};

#endif
