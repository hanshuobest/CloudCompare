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

#ifndef CC_GRAPHICAL_SEGMENTATION_TOOLS_HEADER
#define CC_GRAPHICAL_SEGMENTATION_TOOLS_HEADER

//Local
#include <ccOverlayDialog.h>

//qCC_db
#include <ccHObject.h>

//Qt
#include <QSet>

//GUI
#include <ui_graphicalSegmentationDlg.h>

class ccPolyline;
class ccPointCloud;
class ccGLWindow;


//! 带折线的图形分割机制
class ccGraphicalSegmentationTool : public ccOverlayDialog, public Ui::GraphicalSegmentationDlg
{
	Q_OBJECT

public:

	//! Default constructor
	explicit ccGraphicalSegmentationTool(QWidget* parent);
	//! Destructor
	virtual ~ccGraphicalSegmentationTool();

	//! Adds an entity (and/or its children) to the 'to be segmented' pool
	/** Warning: some entities may be rejected if they are
		locked, or can't be segmented this way.
		\return whether entity has been added to the pool or not返回是否这个项目被加入到池中
	**/
	bool addEntity(ccHObject* anObject);
	
	//! Returns the number of entites currently in the the 'to be segmented' pool
	//！返回当前在待分割池中实体的数量
	unsigned getNumberOfValidEntities() const;

	//! Get a pointer to the polyline that has been segmented
	//! 获取已被分段的折线的指针
	ccPolyline *getPolyLine()
	{
		return m_segmentationPoly;
	}

	//! Returns the active 'to be segmented' set
	QSet<ccHObject*>& entities()
	{ 
		return m_toSegment;
	}
	//! Returns the active 'to be segmented' set (const version)
	const QSet<ccHObject*>& entities() const 
	{ 
		return m_toSegment;
	}

	//inherited from ccOverlayDialog
	virtual bool linkWith(ccGLWindow* win);
	virtual bool start();
	virtual void stop(bool accepted);

	//! Returns whether hidden parts should be delete after segmentation
	//! 分割后是否隐藏删除的部分
	bool deleteHiddenParts() const 
	{ 
		return m_deleteHiddenParts;
	}

	//! Remove entities from the 'to be segmented' pool
	/** \warning 'unallocateVisibilityArray' will be called on all point clouds
		prior to be removed from the pool.
	**/
	void removeAllEntities(bool unallocateVisibilityArrays);

protected slots:

	void segmentIn();
	void segmentOut();
	void segment(bool);
	void reset();
	void apply();
	void applyAndDelete();
	void cancel();
	void addPointToPolyline(int x, int y);
	void closePolyLine(int x=0, int y=0); //arguments for compatibility with ccGlWindow::rightButtonClicked signal
	void closeRectangle();
	void updatePolyLine(int x, int y, Qt::MouseButtons buttons);
	void pauseSegmentationMode(bool);
	void doSetPolylineSelection();
	void doSetRectangularSelection();
	void doActionUseExistingPolyline();
	void doExportSegmentationPolyline();

	//! To capture overridden shortcuts (pause button, etc.)
	void onShortcutTriggered(int);

protected:

	//! Whether to allow or not to exort the current segmentation polyline
	//! 是否允许导出当前分割折线
	void allowPolylineExport(bool state);

	//! Set of entities to be segmented
	//! 被分割的实体集
	QSet<ccHObject*> m_toSegment;

	//! Whether something has changed or not (for proper 'cancel')
	bool m_somethingHasChanged;

	//! Process states
	enum ProcessStates
	{
		POLYLINE		= 1,
		RECTANGLE		= 2,
		//...			= 4,
		//...			= 8,
		//...			= 16,
		PAUSED			= 32,
		STARTED			= 64,
		RUNNING			= 128,
	};

	//! Current process state
	unsigned m_state;

	//! Segmentation polyline
	ccPolyline* m_segmentationPoly;

	ccPointCloud* m_polyVertices;//分割折线的顶点

	//! Selection mode
	bool m_rectangularSelection;

	//! Whether to delete hidden parts after segmentation
	bool m_deleteHiddenParts;//分割后是否删除隐藏的部分
};

#endif //CC_GRAPHICAL_SEGMENTATION_TOOLS_HEADER
