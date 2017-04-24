//##########################################################################
//#                                                                        #
//#                              CLOUDCOMPARE                              #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; version 2 or later of the License.      #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#                    COPYRIGHT: CloudCompare project                     #
//#                                                                        #
//##########################################################################

#ifndef CC_FRUSTUM_HEADER
#define CC_FRUSTUM_HEADER

//CCLib
#include <CCGeom.h>
#include <RayAndBox.h>

//Local
#include "ccGLMatrix.h"

// 平面方程类
class Plane
{
public:

	Plane()
		: normal(0, 0, 1)
		, constCoef(0)
	{}

	virtual ~Plane()
	{}

	void setCoefficients(float a, float b, float c, float d)
	{
		// set the normal vector
		normal = CCVector3f(a, b, c);
		//compute the length of the vector
		float l = normal.norm();
		if (l != 0)
		{
			// normalize the vector
			normal /= l;
			// and divide constCoef as well
			constCoef = d / l;
		}
		else
		{
			constCoef = d;
		}

	}

	// 计算点p到平面的距离
	float distance(const CCVector3f& p) const
	{
		return normal.dot(p) + constCoef;
	}

public: //members

	CCVector3f normal;//法向量
	float constCoef;//常量系数
};

class AABox : public AABB<float>
{
public:

	AABox(const CCVector3f& A, const CCVector3f& B) : AABB<float>(A, B)
	{
	}

	CCVector3f getVertexP(const CCVector3f& normal) const
	{
		return CCVector3f
		(
			corners[normal.x > 0 ? 1 : 0].x,
			corners[normal.y > 0 ? 1 : 0].y,
			corners[normal.z > 0 ? 1 : 0].z
		);
	}
	CCVector3f getVertexN(const CCVector3f& normal) const
	{
		return CCVector3f
		(
			corners[normal.x < 0 ? 1 : 0].x,
			corners[normal.y < 0 ? 1 : 0].y,
			corners[normal.z < 0 ? 1 : 0].z
		);
	}
};

class AACube
{
public:

	AACube()
		: O(0, 0, 0)
		, d(0)
	{}

	AACube(const CCVector3f& origin, float size)
		: O(origin)
		, d(size)
	{
	}

	virtual ~AACube()
	{
	}

	CCVector3f getVertexP(const CCVector3f& normal) const
	{
		return CCVector3f
			(
			normal.x > 0 ? O.x + d : O.x,
			normal.y > 0 ? O.y + d : O.y,
			normal.z > 0 ? O.z + d : O.z
			);
	}
	
	CCVector3f getVertexN(const CCVector3f& normal) const
	{
		return CCVector3f
			(
			normal.x < 0 ? O.x + d : O.x,
			normal.y < 0 ? O.y + d : O.y,
			normal.z < 0 ? O.z + d : O.z
			);
	}

public: //members

	CCVector3f O;
	float d;
};

//平截头体
class Frustum
{
public:

	Frustum()
	{
	}

	Frustum(const ccGLMatrixd& modelViewMat, const ccGLMatrixd& projMat)
	{
		ccGLMatrixd MP = projMat * modelViewMat;
		initfromMPMatrix(MP);
	}

	virtual ~Frustum()
	{
	}

	enum Intersection
	{
		OUTSIDE = 0,
		INTERSECT = 1,
		INSIDE = 2,
	};

public: //Intersection tests

	//交叉测试，计算点是否在平头截体内
	Intersection pointInFrustum(const CCVector3f& p) const
	{
		for (int i = 0; i < 6; i++)
		{
			if (pl[i].distance(p) < 0)
			{
				return OUTSIDE;
			}
		}

		return INSIDE;
	}

	Intersection sphereInFrustum(const CCVector3f &c, float r) const
	{
		Intersection result = INSIDE;

		for (int i = 0; i < 6; i++)
		{
			float distance = pl[i].distance(c);

			if (distance < -r)
				return OUTSIDE;
			else if (distance < r)
				result = INTERSECT;
		}

		return result;
	}

	// 判断轴对齐边界框是否在视锥体中
	Intersection boxInFrustum(const AABox& box) const
	{
		Intersection result = INSIDE;
		for (int i = 0; i < 6; i++)
		{
			if (pl[i].distance(box.getVertexP(pl[i].normal)) < 0)
				return OUTSIDE;
			else if (pl[i].distance(box.getVertexN(pl[i].normal)) < 0)
				result = INTERSECT;
		}

		return result;
	}

	Intersection boxInFrustum(const AACube& cube) const
	{
		Intersection result = INSIDE;
		for (int i = 0; i < 6; i++)
		{
			if (pl[i].distance(cube.getVertexP(pl[i].normal)) < 0)
				return OUTSIDE;
			else if (pl[i].distance(cube.getVertexN(pl[i].normal)) < 0)
				result = INTERSECT;
		}

		return result;
	}

protected: //protected methods

	enum PLANE
	{
		TOP = 0,
		BOTTOM = 1,
		LEFT = 2,
		RIGHT = 3,
		NEARP = 4,
		FARP = 5
	};

	// 计算六个平面方程
	template <typename T> 
	void initfromMPMatrix(const ccGLMatrixTpl<T>& MP)
	{
		const T* m = MP.data();
		pl[NEARP].setCoefficients
			(
			m[3] + m[2],
			m[7] + m[6],
			m[11] + m[10],
			m[15] + m[14]
			);

		pl[FARP].setCoefficients
			(
			m[3] - m[2],
			m[7] - m[6],
			m[11] - m[10],
			m[15] - m[14]
			);

		pl[BOTTOM].setCoefficients
			(
			m[3] + m[1],
			m[7] + m[5],
			m[11] + m[9],
			m[15] + m[13]
			);

		pl[TOP].setCoefficients
			(
			m[3] - m[1],
			m[7] - m[5],
			m[11] - m[9],
			m[15] - m[13]
			);

		pl[LEFT].setCoefficients
			(
			m[3] + m[0],
			m[7] + m[4],
			m[11] + m[8],
			m[15] + m[12]
			);

		pl[RIGHT].setCoefficients
			(
			m[3] - m[0],
			m[7] - m[4],
			m[11] - m[8],
			m[15] - m[12]
			);
	}

protected: 
	//members

	Plane pl[6];//平头椎体的六个平面方程
};

#endif // CC_FRUSTUM_HEADER
