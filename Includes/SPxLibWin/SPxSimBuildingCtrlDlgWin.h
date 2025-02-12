/*********************************************************************
*
* (c) Copyright 2013 - 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxSimBuildingCtrlDlgWin.h,v $
* ID: $Id: SPxSimBuildingCtrlDlgWin.h,v 1.12 2017/10/31 16:11:58 rew Exp $
*
* Purpose:
*	Header for the Simulated Buildings control child dialog.
*
*
* Revision Control:
*   31/10/17 v1.12   AGC	Remove SetCurrentObj() create arg.
*
* Previous Changes:
*   25/10/17 1.11   AGC	Match change to base class SetCurrentObj().
*   31/01/17 1.10   AGC	Support units configuration.
*   03/03/16 1.9    AGC	Fix clang warnings.
*   26/01/15 1.8    AGC	Remove unused HICON.
*   18/07/14 1.7    AGC	Adding points now supported by base class.
*   06/06/14 1.6    AGC	Show building points/labels dialog visible.
*   02/06/14 1.5    AGC	Disable mouse editing when dialog not shown.
*   16/05/14 1.4    AGC	Use SPxAreaEditorMouse.
*   08/05/14 1.3    AGC	Support holes.
*			Support loading shapefiles.
*   27/01/14 1.2    AGC	Remove set name buttons.
*   30/07/13 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxErrorCode. */
#include "SPxLibUtils/SPxError.h"

/* For SPxMouseInterface. */
#include "SPxLibUtils/SPxMouseControl.h"

/* For SPxWinEditCtrl. */
#include "SPxLibWin/SPxWinEditCtrl.h"

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* For base class. */
#include "SPxLibUtils/SPxAreaEditorMouse.h"

/* Forward declarations. */
class SPxBuildingDatabase;
class SPxAreaRenderer;
class SPxViewControl;
class SPxSimBuilding;
class SPxSimBuildingShape;

/*
 * SPxSimBuildingCtrlDlgWin dialog
 */
class SPxSimBuildingCtrlDlgWin : public CDialog, public SPxAreaEditorMouse
{
    DECLARE_DYNAMIC(SPxSimBuildingCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxSimBuildingCtrlDlgWin(CWnd* pParent, 
			     SPxBuildingDatabase *obj,
			     SPxAreaRenderer *renderer=NULL,
			     SPxViewControl *viewCtrl=NULL);
    virtual ~SPxSimBuildingCtrlDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);
    virtual void SetCurrentObj(SPxAreaObj *obj, int selectNext=FALSE);

    void SetDisplayUnits(SPxUnitsDist_t unitsDist);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_BUILDINGS_CTRL_DIALOG };

    /* Configuration retrieval functions. */
    SPxBuildingDatabase *GetObj(void) { return m_obj; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void);
    void OnShowWindow(BOOL bShow, UINT nStatus);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */

    /* The parent window */
    CWnd *m_parentWin;

    /* The object we are controlling */
    SPxBuildingDatabase *m_obj;
    SPxAreaRenderer *m_renderer;
    SPxViewControl *m_viewControl;
    SPxSimBuilding *m_building;
    SPxSimBuildingShape *m_shape;
    SPxUnitsDist_t m_unitsDist;
    int m_syncing;

    /* Controls. */
    CButton m_btnBuildingAdd;
    CButton m_btnBuildingDelete;
    CButton m_btnBuildingDeleteAll;
    CComboBox m_comboBuildings;
    SPxWinEditCtrl m_editBuildingName;
    CButton m_btnLoad;
    CStatic m_lblLoadHeight;
    SPxWinSpinCtrl m_spinLoadHeight;
    CButton m_checkLoadView;
    CButton m_btnShapeAdd;
    CButton m_btnShapeDelete;
    CButton m_btnShapeDeleteAll;
    CComboBox m_comboShapes;
    SPxWinEditCtrl m_editShapeName;
    CButton m_checkHole;
    CStatic m_lblShapeHeight;
    SPxWinSpinCtrl m_spinShapeHeight;
    CListBox m_listPoints;

    /* Private functions */
    void OnBtnClickedBuildingAdd(void);
    void OnBtnClickedBuildingDelete(void);
    void OnBtnClickedBuildingDeleteAll(void);
    void OnCbnSelChangeComboBuildings(void);
    void OnBtnClickedBuildingNameSet(NMHDR*, LRESULT*);
    void OnBtnClickedBuildingLoad(void);
    void OnBtnClickedShapeAdd(void);
    void OnBtnClickedShapeDelete(void);
    void OnBtnClickedShapeDeleteAll(void);
    void OnCbnSelChangeComboShapes(void);
    void OnBtnClickedShapeNameSet(NMHDR*, LRESULT*);
    void OnBtnClickedCheckHole(void);
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);

    using CDialog::Create;

}; /* class SPxSimBuildingCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
