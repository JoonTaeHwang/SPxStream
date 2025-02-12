/*********************************************************************
*
* (c) Copyright 2012 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Simulated Motion control child dialog.
*
*
* Revision Control:
*   29/08/18 v1.21   AGC	Performance improvements with lots of segments.
*
* Previous Changes:
*   12/04/17 1.20   AGC	Support modifying max turn rate.
*   15/03/17 1.19   AGC	Support bearing based motion starting position.
*   31/01/17 1.18   AGC	Support units configuration.
*   23/08/16 1.17   AGC	Event function prototype updated.
*   03/03/16 1.16   AGC	Fix clang warnings.
*   07/11/14 1.15   AGC	Improve responsive with lots of input targets.
*   02/06/14 1.14   AGC	Support child motions.
*   27/01/14 1.13   AGC	Remove set name buttons.
*   30/07/13 1.12   AGC	Use SPxWinButton for buttons with icons.
*   06/06/13 1.11   AGC	Support segment end height.
*   03/05/13 1.10   AGC	Support not syncing segment list.
*   29/04/13 1.9    AGC	Fix VS 64-bit error.
*   26/04/13 1.8    AGC	Support appending segments to existing motions.
*			Support tooltips.
*   25/04/13 1.7    AGC	Support end speed segments.
*			Support height above ground.
*   08/03/13 1.6    AGC	Display max motion turn rate.
*   04/03/13 1.5    AGC	Add buttons for moving segments up and down.
*   25/01/13 1.4    AGC	Disable some buttons while adding a motion.
*			Add index to iteration.
*			Support degrees/second segments.
*			Support new types of segment.
*   23/11/12 1.3    AGC	Improve interaction with motion renderer.
*   09/11/12 1.2    AGC	Update to handle end actions.
*			Initial position now part of motion not segment.
*   26/10/12 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxErrorCode. */
#include "SPxLibUtils/SPxError.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxObjPtr. */
#include "SPxLibUtils/SPxObjPtr.h"

/* For SPxObjPtr<SPxSimMotion>. */
#include "SPxLibData/SPxSimMotion.h"

/* For SPxSimMotionRendererMode enum. */
#include "SPxLibData/SPxSimMotionRenderer.h"

/* For SPxWinEditCtrl. */
#include "SPxLibWin/SPxWinEditCtrl.h"

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* For SPxWinButton. */
#include "SPxLibWin/SPxWinButton.h"

/* For SPxMouseInterface. */
#include "SPxLibUtils/SPxMouseControl.h"

/* Forward declarations. */
class SPxRadarSimulator;
class SPxSimMotionRenderer;
class SPxSimMotion;
class SPxSimSegment;
class SPxSimMotionNewDlgWin;
class SPxSimMotionAddChildDlgWin;
struct SPxRadarSimEventParams;

/*
 * SPxSimMotionCtrlDlgWin dialog
 */
class SPxSimMotionCtrlDlgWin : public CDialog, public SPxMouseInterface
{
    DECLARE_DYNAMIC(SPxSimMotionCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxSimMotionCtrlDlgWin(CWnd* pParent, 
			   SPxRadarSimulator *obj,
			   SPxSimMotionRenderer *renderer=NULL);
    virtual ~SPxSimMotionCtrlDlgWin(void);
    BOOL Create(void);
    void SyncFromObj(int syncMotionList=TRUE, int syncSegList=TRUE);
    SPxErrorCode SetCurrMotion(SPxSimMotion *motion, SPxSimSegment *segment);
    SPxErrorCode CreateMotion(SPxSimMotion *motion, int append=FALSE);
    int CanCreateMotion(void) const;
    void SetAllowCreate(int allowCreate);

    void SetDisplayUnits(SPxUnitsDist_t unitsDist,
			 SPxUnitsSpeed_t unitsSpeed);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_MOTION_CTRL_DIALOG };

    /* Configuration retrieval functions. */
    SPxRadarSimulator *GetObj(void) { return m_obj; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    void OnTimer(UINT_PTR nIDEvent);
    virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
    BOOL OnNeedToolTip(UINT id, NMHDR *pTTTStruct, LRESULT *pResult);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void);
    void OnShowWindow(BOOL bShow, UINT nStatus);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */

    /* The parent window */
    CWnd *m_parentWin;

    /* Our icon. */
    HICON m_hIcon;

    /* The object we are controlling */
    SPxRadarSimulator *m_obj;
    SPxSimMotionRenderer *m_renderer;
    SPxObjPtr<SPxSimMotion> m_currMotion;
    SPxSimSegment *m_currSegment;
    SPxAutoPtr<SPxSimMotionNewDlgWin> m_newMotionDlg;
    SPxAutoPtr<SPxSimMotionAddChildDlgWin> m_addChildDlg;
    SPxUnitsDist_t m_unitsDist;
    SPxUnitsSpeed_t m_unitsSpeed;

    /* Controls. */
    CButton m_btnAdd;
    CButton m_btnDelete;
    CButton m_btnDeleteAll;
    CComboBox m_comboMotions;
    SPxWinEditCtrl m_editName;
    CButton m_radioEndStop;
    CButton m_radioEndHide;
    CButton m_radioEndRestart;
    CButton m_radioEndReverse;
    CStatic m_lblMotionTime;
    CStatic m_lblMotionMaxTurn;
    SPxWinSpinCtrl m_spinMotionMaxTurn;
    CButton m_btnSegAdd;
    SPxWinButton m_btnSegMoveUp;
    SPxWinButton m_btnSegMoveDown;
    CButton m_btnSegDelete;
    CButton m_btnSegAppend;
    CButton m_btnSegMotions;
    CListBox m_listSegments;
    CButton m_radioSegLengthA;
    CButton m_radioSegLengthB;
    SPxWinSpinCtrl m_editSegLengthA;
    SPxWinSpinCtrl m_editSegLengthB;
    CButton m_radioSegLatLong;
    CButton m_radioSegMetres;
    CButton m_radioSegBearing;
    CButton m_checkSegSpeed;
    CButton m_checkSegCourse;
    CButton m_checkSegHeight;
    SPxWinSpinCtrl m_editSegSpeed;
    SPxWinSpinCtrl m_editSegCourse;
    SPxWinSpinCtrl m_editSegHeight;
    CButton m_radioSegHeightSea;
    CButton m_radioSegHeightGround;
    CButton m_checkSegEndHeight;
    SPxWinSpinCtrl m_editSegEndHeight;
    SPxWinSpinCtrl m_editSegPod;
    CButton m_radioSegAccelLinear;
    CButton m_radioSegAccelEndSpeed;
    CButton m_radioSegAccelTurnG;
    CButton m_radioSegAccelTurnDegs;
    SPxWinSpinCtrl m_editSegAccelLinear;
    SPxWinSpinCtrl m_editSegAccelEndSpeed;
    SPxWinSpinCtrl m_editSegAccelTurnG;
    SPxWinSpinCtrl m_editSegAccelTurnDegs;
    CStatic m_lblSegPosA;
    CStatic m_lblSegPosB;
    SPxWinSpinCtrl m_editSegPosA;
    SPxWinSpinCtrl m_editSegPosB;

    /* Other variables. */
    int m_syncRequired;
    int m_syncMotionList;
    int m_syncSegmentList;
    int m_maxSegItemWidth;
    int m_allowCreate;

    /* Private functions */
    static SPxErrorCode addMotionToCombo(unsigned int index, SPxSimMotion *motion, void *userArg);
    static SPxErrorCode addSegmentToList(unsigned int index, SPxSimSegment *segment, void *userArg);
    static SPxErrorCode eventFnWrapper(UINT32 flags, const SPxRadarSimEventParams *params, void *userArg);
    SPxErrorCode eventFn(UINT32 flags);
    SPxErrorCode motionEventFn(UINT32 flags);
    static void setMotionFn(SPxSimMotion *motion, SPxSimSegment *segment, void *userArg);
    static int deleteRendererFn(void *invokingObject, void *userObject, void *arg);
    
    /* Functions called by SPxSimMotionNewDlgWin. */
    void newMotionComplete(void);

    /*
     * Handler functions.
     */
    void OnBnClickedSpxSimMotionBtnAdd(void);
    void OnBnClickedSpxSimMotionBtnDelete(void);
    void OnBnClickedSpxSimMotionBtnDeleteAll(void);
    void OnCbnSelchangeSpxSimMotionCombo(void);
    void OnBnClickedSpxSimMotionSetName(NMHDR*, LRESULT*);
    void OnBnClickedSpxSimMotionBtnEndAction(void);
    void OnBnClickedSpxSimMotionBtnSegInsert(void);
    void OnBnClickedSpxSimMotionBtnSegMoveUp(void);
    void OnBnClickedSpxSimMotionBtnSegMoveDown(void);
    void OnBnClickedSpxSimMotionBtnSegDelete(void);
    void OnBnClickedSpxSimMotionBtnSegAppend(void);
    void OnBnClickedSpxSimMotionBtnSegMotions(void);
    void OnLbnSelchangeSpxSimMotionListSeg(void);
    void OnBnClickedSpxSimMotionRadioStateLl(void);
    void OnBnClickedSpxSimMotionRadioStateM(void);
    void OnBnClickedSpxSimMotionRadioStateBearing(void);
    void OnBnClickedSpxSimMotionCheckSpeed(void);
    void OnBnClickedSpxSimMotionCheckCourse(void);
    void OnBnClickedSpxSimMotionCheckHeight(void);
    void OnBnClickedSpxSimMotionRadioHeightSea(void);
    void OnBnClickedSpxSimMotionRadioHeightGround(void);
    void OnBnClickedSpxSimMotionCheckEndHeight(void);
    void OnBnClickedSpxSimMotionRadioAccelLinear(void);
    void OnBnClickedSpxSimMotionRadioAccelEndSpeed(void);
    void OnBnClickedSpxSimMotionRadioAccelTurnG(void);
    void OnBnClickedSpxSimMotionRadioAccelTurnDegs(void);
    void OnBnClickedSpxSimMotionRadioLengthA(void);
    void OnBnClickedSpxSimMotionRadioLengthB(void);
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);

    friend class SPxSimMotionNewDlgWin;

    using CDialog::Create;

}; /* class SPxSimMotionCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
