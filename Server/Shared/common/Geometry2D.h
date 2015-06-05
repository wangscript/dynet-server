#ifndef __GEOMETRY2D_H_
#define __GEOMETRY2D_H_
#include <limits>  
#include "BaseDefine.h"
#include <math.h>

struct Point2DEx;
struct Point2D
{
	Point2D(int32 nX = 0, int32 nZ = 0) : nX(nX), nZ(nZ) { }
	Point2D(const Point2DEx &src);
	int32 nX;		// x轴正方向向左
	int32 nZ;		// z轴正方向向下
};

struct Point2DEx
{
	float fX;		// x轴正方向向左
	float fZ;		// z轴正方向向下

	Point2DEx() { }
	Point2DEx(const Point2D& src) : fX(src.nX), fZ(src.nZ) { }
};

inline bool operator==(const Point2D &left, const Point2D &right)
{
	return !memcmp(&left, &right, sizeof(Point2D));
}

inline int32 DistanceSq(const Point2D &left, const Point2D &right)
{
	int32 nXSeparation = right.nX - left.nX;
	int32 nZSeparation = right.nZ - left.nZ;

	return nXSeparation * nXSeparation + nZSeparation * nZSeparation;
}
inline int32 DistanceSq(const Point2DEx &left, const Point2DEx &right)
{
	int32 nXSeparation = right.fX - left.fX;
	int32 nZSeparation = right.fZ - left.fZ;

	return nXSeparation * nXSeparation + nZSeparation * nZSeparation;
}
struct Vector2D
{
	int32 nX;		// x轴正方向向左
	int32 nZ;		// z轴正方向向下

	Vector2D() : nX(0), nZ(0) { }
	Vector2D(int32 _nX,int32 _nZ) :nX(_nX),nZ(_nZ) {}
	Vector2D(const Vector2D& src) : nX(src.nX), nZ(src.nZ) { }
	Vector2D(const Point2D& src) : nX(src.nX), nZ(src.nZ) { }
};

struct Vector2DEx
{
	float fX;		// x轴正方向向左
	float fZ;		// z轴正方向向下

	Vector2DEx() { }
	Vector2DEx(const Vector2D& src) : fX(src.nX), fZ(src.nZ) { }

	inline float LengthSq() const
	{
		return (fX * fX + fZ * fZ);
	}

	inline float Length() const
	{
		return sqrt(fX * fX + fZ * fZ);
	}
	
	inline float Dot(const Vector2DEx& v2) const
	{
		return fX * v2.fX + fZ * v2.fZ;
	}

	inline Vector2DEx& Normalize()
	{
		float vector_length = Length();

		if (vector_length > std::numeric_limits<float>::epsilon())
		{
			fX /= vector_length;
			fZ /= vector_length;
		}

		return *this;
	}

	const Vector2DEx& operator=(const Vector2D& rVector2D )
	{
		this->fX = static_cast<float>(rVector2D.nX);
		this->fZ = static_cast<float>(rVector2D.nZ);
		return *this;
	}
};



struct Rectangle2D
{
	Rectangle2D() { }
	Rectangle2D(int32 nTop, int32 nLeft, int32 nBottom, int32 nRight)
		: sTopLeft(nLeft, nTop), sBottomRight(nRight, nBottom) { }

	Point2D sTopLeft;		// 左上角坐标
	Point2D sBottomRight;	// 右下角坐标
};

// 两点之间的向量
inline Vector2DEx operator-(const Point2D &left, const Point2DEx &right)
{
	Vector2DEx result;
	result.fX = left.nX - right.fX;
	result.fZ = left.nZ - right.fZ;

	return result;
}
inline Vector2DEx operator-(const Point2DEx &left, const Point2DEx &right)
{
	Vector2DEx result;
	result.fX = left.fX - right.fX;
	result.fZ = left.fZ - right.fZ;

	return result;
}

// 点平移
inline Point2D operator+(const Point2D &left, const Vector2D &right)
{
	Point2D result;
	result.nX = left.nX + right.nX;
	result.nZ = left.nZ + right.nZ;

	return result;
}
// 点平移重载
inline Point2D operator+(const Point2D &left, const Vector2DEx &right)
{
	Point2D result;
	result.nX = left.nX + right.fX;
	result.nZ = left.nZ + right.fZ;

	return result;
}

inline Point2DEx operator-(const Point2D &left, const Vector2DEx &right)
{
	Point2DEx result;
	result.fX = left.nX - right.fX;
	result.fZ = left.nZ - right.fZ;

	return result;
}


inline Vector2DEx operator*(const Vector2DEx &right,int32 nMultiple)
{
	Vector2DEx result;
	result.fX = right.fZ * nMultiple;
	result.fZ = right.fZ * nMultiple;
	return result;
}

// 矩形平移
inline Rectangle2D operator+(const Rectangle2D &left, const Vector2D &right)
{
	Rectangle2D result;
	result.sTopLeft = left.sTopLeft + right;
	result.sBottomRight = left.sBottomRight + right;

	return result;
}

struct RectangleRange : public Rectangle2D
{
	RectangleRange() {}
	RectangleRange(const Rectangle2D& _sRange)
		: sStandardRange(_sRange)
	{ }

	inline void SetPos(const Point2D& sPosition)
	{
		*static_cast<Rectangle2D*>(this) = sStandardRange + sPosition;
	}

	Rectangle2D		sStandardRange;		// 标准范围
};

// 矩形相交检测
inline bool CheckIntersect(const Rectangle2D& rFirst, const Rectangle2D& rSecond)
{
	return !(rFirst.sTopLeft.nX > rSecond.sBottomRight.nX ||
		rFirst.sTopLeft.nZ < rSecond.sBottomRight.nZ ||
		rFirst.sBottomRight.nX < rSecond.sTopLeft.nX ||
		rFirst.sBottomRight.nZ > rSecond.sTopLeft.nZ);
}

inline bool CheckIntersect(const RectangleRange& rFirst, const Rectangle2D& rSecond)
{
	return !(rFirst.sTopLeft.nX > rSecond.sBottomRight.nX ||
		rFirst.sTopLeft.nZ < rSecond.sBottomRight.nZ ||
		rFirst.sBottomRight.nX < rSecond.sTopLeft.nX ||
		rFirst.sBottomRight.nZ > rSecond.sTopLeft.nZ);
}

inline bool GetIntersect(const Rectangle2D& i_rFirst, const Rectangle2D& i_rSecond, Rectangle2D& o_rResult)
{
	if (CheckIntersect(i_rFirst, i_rSecond))
	{
		o_rResult.sTopLeft.nX = (i_rFirst.sTopLeft.nX > i_rSecond.sTopLeft.nX) ? i_rFirst.sTopLeft.nX : i_rSecond.sTopLeft.nX;
		o_rResult.sTopLeft.nZ = (i_rFirst.sTopLeft.nZ > i_rSecond.sTopLeft.nZ) ? i_rSecond.sTopLeft.nZ : i_rFirst.sTopLeft.nZ;
		o_rResult.sBottomRight.nX = (i_rFirst.sBottomRight.nX > i_rSecond.sBottomRight.nX) ? i_rSecond.sBottomRight.nX : i_rFirst.sBottomRight.nX;
		o_rResult.sBottomRight.nZ = (i_rFirst.sBottomRight.nZ > i_rSecond.sBottomRight.nZ) ? i_rFirst.sBottomRight.nZ : i_rSecond.sBottomRight.nZ;
	
		return true;
	}
	else
	{
		return false;
	}
}

// 点在矩形中检测
inline bool CheckInside(const Point2D& rFirst, const Rectangle2D& rSecond)
{
	return !(rFirst.nX < rSecond.sTopLeft.nX ||
		rFirst.nZ > rSecond.sTopLeft.nZ ||
		rFirst.nX > rSecond.sBottomRight.nX ||
		rFirst.nZ < rSecond.sBottomRight.nZ);
}

inline bool CheckInside(const Point2D& rFirst, const RectangleRange& rSecond)
{
	return !(rFirst.nX < rSecond.sTopLeft.nX ||
		rFirst.nZ > rSecond.sTopLeft.nZ ||
		rFirst.nX > rSecond.sBottomRight.nX ||
		rFirst.nZ < rSecond.sBottomRight.nZ);
}

#endif

