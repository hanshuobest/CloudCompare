//##########################################################################
//#                                                                        #
//#                               CCLIB                                    #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU Library General Public License as       #
//#  published by the Free Software Foundation; version 2 or later of the  #
//#  License.                                                              #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#          COPYRIGHT: EDF R&D / TELECOM ParisTech (ENST-TSI)             #
//#                                                                        #
//##########################################################################

#ifndef GENERIC_CLOUD_HEADER
#define GENERIC_CLOUD_HEADER

//Local
#include "CCGeom.h"
#include "CCConst.h"

namespace CCLib
{

//! A generic 3D point cloud interface for data communication between library and client applications
//! 一种通用的3D点云接口，用于库和客户端应用之间的数据通信，所有点云类的基类，抽象类
class CC_CORE_LIB_API GenericCloud
{

public:

	//! Default constructor
	GenericCloud() {}

	//! Default destructor
	virtual ~GenericCloud() {}

	//! Generic function applied to a point (used by foreach)
	//! 应用于点的通用函数
	typedef void genericPointAction(const CCVector3&, ScalarType&);

	//! Returns the number of points
	/**	Virtual method to request the cloud size
		\return the cloud size
		虚函数，返回点的数量
	**/
	virtual unsigned size() const = 0;

	//! Fast iteration mechanism
	/**	Virtual method to apply a function to the whole cloud
		\param action the function to apply (see GenericCloud::genericPointAction)
		快速迭代机制
	**/
	virtual void forEach(genericPointAction& action) = 0;

	//! Returns the cloud bounding box
	/**	Virtual method to request the cloud bounding box limits
		\param bbMin lower bounding-box limits (Xmin,Ymin,Zmin)
		\param bbMax higher bounding-box limits (Xmax,Ymax,Zmax)
		返回点云的包围盒
	**/
	virtual void getBoundingBox(CCVector3& bbMin, CCVector3& bbMax) = 0;

	//! Returns a given point visibility state (relatively to a sensor for instance)
	/**	Generic method to request a point visibility (should be overloaded if this functionality is required).
		The point visibility is such as defined in Daniel Girardeau-Montaut's PhD manuscript (see Chapter 2, 
		section 2-3-3). In this case, a ground based laser sensor model should be used to determine it.
		This method is called before performing any point-to-cloud comparison. If the result is not
		POINT_VISIBLE, then the comparison won't be performed and the scalar field value associated
		to this point will be this visibility value.
		\param P the 3D point to test
		\return visibility (default: POINT_VISIBLE)
		返回给定点的可见性状态
	**/
	virtual inline unsigned char testVisibility(const CCVector3& P) const 
	{ 
		return POINT_VISIBLE; 
	}

	//! Sets the cloud iterator at the beginning
	/**	Virtual method to handle the cloud global iterator
	 ** 设置点云迭代器在开头
	**/
	virtual void placeIteratorAtBegining() = 0;

	//! Returns the next point (relatively to the global iterator position)
	/**	Virtual method to handle the cloud global iterator.
		Global iterator position should be increased by one each time
		this method is called.
		Warning:
		- the returned object may not be persistent!
		- THIS METHOD MAY NOT BE COMPATIBLE WITH PARALLEL STRATEGIES
		(see the DgmOctree::executeFunctionForAllCellsAtLevel_MT and
		DgmOctree::executeFunctionForAllCellsAtStartingLevel_MT methods).
		\return pointer on next point (or 0 if no more)
		返回下一个点云指针
	**/
	virtual const CCVector3* getNextPoint() = 0;

	//!	Enables the scalar field associated to the cloud
	/** If the scalar field structure is not yet initialized/allocated,
		this method gives the signal for its creation. Otherwise, if possible
		the structure size should be pre-reserved with the same number of
		elements as the point cloud.
		启用与点云相关联的标量字段
	**/
	virtual bool enableScalarField() = 0;

	//! Returns true if the scalar field is enabled, false otherwise
	//  如果标量字段可用返回真，否则返回假
	virtual bool isScalarFieldEnabled() const = 0;

	//! Sets the ith point associated scalar value
	//! 设置与第i个点相关的标量值
	virtual void setPointScalarValue(unsigned pointIndex, ScalarType value) = 0;

	//! Returns the ith point associated scalar value
	//  返回第i个点关联的标量值
	virtual ScalarType getPointScalarValue(unsigned pointIndex) const = 0;
};

}

#endif //GENERIC_CLOUD_HEADER
