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
//#          COPYRIGHT: EDF R&D / TELECOM ParisTech (ENST-TSI)             #
//#                                                                        #
//##########################################################################

#ifndef GUI_PARAMETERS_HEADER
#define GUI_PARAMETERS_HEADER

//Qt
#include <QString>

//qCC_db
#include <ccColorTypes.h>

/***************************************************
				GUI parameters
***************************************************/

//! This class manages some persistent parameters (mostly for display)
/** Values of persistent parameters are stored by the system
	(either in the registry or in a separate file depending on the OS).
**/
class ccGui
{
public:

	//! GUI parameters
	//  界面参数结构体
	struct ParamStruct
	{
		//! Light diffuse color (RGBA)
		//  光漫射颜色
		ccColor::Rgbaf lightDiffuseColor;
		//! Light ambient color (RGBA)
		ccColor::Rgbaf lightAmbientColor;
		//! Light specular color (RGBA)
		ccColor::Rgbaf lightSpecularColor;
		//! Double sided light
		bool lightDoubleSided;

		//! Default mesh diffuse color (front)
		ccColor::Rgbaf meshFrontDiff;
		//! Default mesh diffuse color (back)
		ccColor::Rgbaf meshBackDiff;
		//! Default mesh specular color
		ccColor::Rgbaf meshSpecular;

		//! Default text color
		ccColor::Rgbub textDefaultCol;
		//! Default 3D points color
		ccColor::Rgbub pointsDefaultCol;
		//! Background color
		ccColor::Rgbub backgroundCol;
		//! Labels background color
		ccColor::Rgbub labelBackgroundCol;
		//! Labels marker color
		ccColor::Rgbub labelMarkerCol;
		//! Bounding-boxes color
		ccColor::Rgbub bbDefaultCol;
		
		//! Use background gradient
		bool drawBackgroundGradient;

		//! Decimate meshes when moved
		bool decimateMeshOnMove;

		//  抽取时最小网格尺寸
		//! Min mesh size for decimation
		unsigned minLoDMeshSize;

		//! Decimate clouds when moved
		//  移动时抽取的点云数量
		bool decimateCloudOnMove;

		//! Min cloud size for decimation
		//  用于抽取的最小点云数量
		unsigned minLoDCloudSize;
		//! Display cross in the middle of the screen
		bool displayCross;

		//! Whether to use VBOs for faster display
		bool useVBOs;

		//! Label marker size
		unsigned labelMarkerSize;

		//! Color scale option: show histogram next to color ramp
		bool colorScaleShowHistogram;
		//! Whether to use shader for color scale display (if available) or not
		bool colorScaleUseShader;
		//! Whether shader for color scale display is available or not
		bool colorScaleShaderSupported;
		//! Color scale ramp width (for display)
		unsigned colorScaleRampWidth;

		//! Default displayed font size
		unsigned defaultFontSize;
		//! Label font size
		unsigned labelFontSize;
		//! Displayed numbers precision
		unsigned displayedNumPrecision;
		//! Labels background opcaity
		unsigned labelOpacity;

		//! Zoom speed (1.0 by default)
		double zoomSpeed;

		//! Octree computation (for picking) behaviors
		//  定义八叉树计算（拣选）行为
		enum ComputeOctreeForPicking { ALWAYS = 0, ASK_USER = 1, NEVER = 2 };

		//! Octree computation (for picking) behavior
		ComputeOctreeForPicking autoComputeOctree;

		//! Whether to draw rounded points (slower) or not
		//  是否绘制圆点
		bool drawRoundedPoints;

		//! Default constructor
		ParamStruct();

		//! Resets parameters to default values
		void reset();

		//! Loads from persistent DB
		void fromPersistentSettings();

		//! Saves to persistent DB
		void toPersistentSettings() const;

		//! Returns whether a given parameter is already defined in persistent settings or not
		/** \param paramName the corresponding attribute name
		**/
		bool isInPersistentSettings(QString paramName) const;
	};

	//! Returns the stored values of each parameter.
	//  返回每个参数存储的值
	static const ParamStruct& Parameters();

	//! Sets GUI parameters
	static void Set(const ParamStruct& params);

	//! Release unique instance (if any)
	static void ReleaseInstance();

protected:

	//! Parameters set
	ParamStruct params;

};

#endif
