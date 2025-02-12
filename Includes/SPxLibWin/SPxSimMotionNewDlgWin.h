/*********************************************************************
*
* (c) Copyright 2012 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the dialog to create new Simulated Motion.
*
*
* Revision Control:
*   11/03/22 v1.17   AGC	Use icons for buttons.
*
* Previous Changes:
*   28/08/20 1.16   AGC	Use precompiled header.
*   31/01/17 1.15   AGC	Support units configuration.
*   03/03/16 1.14   AGC	Fix clang warnings.
*   21/09/15 1.13   AGC	Improve choice of when to enable OK button.
*   10/11/14 1.12   AGC	Keep motion name in sync.
*   02/06/14 1.11   AGC	Move motion/segment creation to renderer.
*   27/01/14 1.10   AGC	Remove set name buttons.
*   15/01/14 1.9    AGC	Position spin controls in resource file.
*   04/10/13 1.8    AGC	Simplify headers.
*   26/04/13 1.7    AGC	Support appending segments to existing motions.
*   25/04/13 1.6    AGC	Improve behaviour when switching turn mode.
*			Support closing dialog with X button.
*			Improve undo behaviour.
*   08/03/13 1.5    AGC	Use motion based max turn rate.
*   04/03/13 1.4    AGC	Finish motion on double click.
*			Improve max turn motion creation.
*   25/01/13 1.3    AGC	Improve tidiness on shutdown.
*			Add Undo button.
*			Allow view changes while creating new motion.
*   23/11/12 1.2    AGC	Improve interaction with motion renderer.
*			Support new segment mode - straight with max-g turns.
*   26/10/12 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxMouseControl.h"
#include "SPxLibUtils/SPxObjPtr.h"
#include "SPxLibData/SPxRadarSimulator.h"
#include "SPxLibWin/SPxWinEditCtrl.h"
#include "SPxLibWin/SPxWinSpinCtrl.h"
#include "SPxLibWin/SPxWinButton.h"

/* Forward declarations. */
class SPxSimMotionCtrlDlgWin;
class SPxSimMotionRenderer;
class SPxRadarSimulator;
class SPxSimMotion;
struct SPxLatLong_tag;

/*
 * SPxSimMotionNewDlgWin dialog
 */
class SPxSimMotionNewDlgWin : public CDialog, public SPxMouseInterface
{
    DECLARE_DYNAMIC(SPxSimMotionNewDlgWin)

public:
    /* Constructor and destructor. */
    SPxSimMotionNewDlgWin(SPxSimMotionCtrlDlgWin* pParent, 
			  SPxSimMotion *motion,
			  SPxSimMotionRenderer *renderer,
			  int appendSegments=FALSE);
    virtual ~SPxSimMotionNewDlgWin(void);
    BOOL Create(void);

    void SyncFromObj(void);

    void SetDisplayUnits(SPxUnitsDist_t unitsDist,
			 SPxUnitsSpeed_t unitsSpeed);

    void rendererDeleted(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_MOTION_NEW_DIALOG };

protected:
    virtual void OnOK(void){};
    virtual void OnCancel(void);
    virtual void PostNcDestroy(void);
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    void OnTimer(UINT_PTR nIDEvent);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */

    /* The parent window */
    SPxSimMotionCtrlDlgWin *m_parentWin;

    /* The object we are controlling */
    SPxSimMotionRenderer *m_renderer;
    SPxRadarSimulator *m_radarSim;
    SPxSimMotion *m_motion;
    SPxUnitsDist_t m_unitsDist;
    SPxUnitsSpeed_t m_unitsSpeed;
    int m_appendSegments;
    int m_syncRequired;
    unsigned int m_numSegmentsStart;

    /* Controls. */
    SPxWinEditCtrl m_editName;
    CStatic m_lblInitSpeed;
    CStatic m_lblInitHeight;
    SPxWinSpinCtrl m_spinInitSpeed;
    SPxWinSpinCtrl m_spinInitHeight;
    CButton m_radioMaxG;
    CButton m_radioMaxDegs;
    SPxWinSpinCtrl m_spinMaxG;
    SPxWinSpinCtrl m_spinMaxDegs;
    SPxWinButton m_btnSegMixed;
    SPxWinButton m_btnSegCurve;
    SPxWinButton m_btnSegStraight;
    SPxWinButton m_btnSegStraightMaxTurn;
    CButton m_btnOK;
    CButton m_btnCancel;
    
    /* Private functions */
    void setupButton(SPxWinButton &btn, int iconID, BOOL checked);

    /*
     * Handler functions.
     */
    void OnBnClickedBtnSetName(NMHDR*, LRESULT*);
    void OnSpinSpeed(NMHDR*, LRESULT*);
    void OnSpinHeight(NMHDR*, LRESULT*);
    void OnSpinMaxTurnG(NMHDR*, LRESULT*);
    void OnSpinMaxTurnDegs(NMHDR*, LRESULT*);
    void OnBnClickedBtnMaxTurnG(void);
    void OnBnClickedBtnMaxTurnDegs(void);
    void OnBnClickedBtnSegMixed(void);
    void OnBnClickedBtnSegCurve(void);
    void OnBnClickedBtnSegStraight(void);
    void OnBnClickedBtnSegStraightMaxTurn(void);
    void OnBnClickedBtnOK(void);
    void OnBnClickedBtnCancel(void);
    static SPxErrorCode undoActionsChanged(void *userArg);
    static SPxErrorCode motionFn(unsigned int index, SPxSimMotion *motion, void *userArg);
    void checkLastSeg(void);
    SPxErrorCode eventFn(UINT32 flags);

    using CDialog::Create;

}; /* class SPxSimMotionNewDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
