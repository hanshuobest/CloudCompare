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

#ifndef GENERIC_INDEXED_CLOUD_HEADER
#define GENERIC_INDEXED_CLOUD_HEADER

//Local
#include "GenericCloud.h"

namespace CCLib
{

//! A generic 3D point cloud with index-based point access
/** Implements the GenericCloud interface.
	基于索引访问的通用3D点云类，实现GenericCloud的接口，也是抽象类
**/
class CC_CORE_LIB_API GenericIndexedCloud : virtual public GenericCloud
{

public:

	//! Default destructor
	virtual ~GenericIndexedCloud() {}

	//! Returns the ith point
	/**	Virtual method to request a point with a specific index.
		WARNINGS:
		- the returned object may not be persistent!
		- THIS METHOD MAY NOT BE COMPATIBLE WITH PARALLEL STRATEGIES
		(see the DgmOctree::executeFunctionForAllCellsAtLevel_MT and
		DgmOctree::executeFunctionForAllCellsAtStartingLevel_MT methods).
		Consider the other version of getPoint instead or the 
		GenericIndexedCloudPersist class.
		\param index of the requested point (between 0 and the cloud size minus 1)
		\return the requested point (undefined behavior if index is invalid)
		获取指定索引的三维点，纯虚函数
	**/
	virtual const CCVector3* getPoint(unsigned index) = 0;

	//! Returns the ith point
	/**	Virtual method to request a point with a specific index.
		Index must be valid (undefined behavior if index is invalid)
		\param index of the requested point (between 0 and the cloud size minus 1)
		\param P output point
	**/
	virtual void getPoint(unsigned index, CCVector3& P) const = 0;
};

}

#endif //GENERIC_INDEXED_CLOUD_HEADER
