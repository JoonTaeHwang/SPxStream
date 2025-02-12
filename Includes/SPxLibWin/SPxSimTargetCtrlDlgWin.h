/*********************************************************************
*
* (c) Copyright 2012 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Simulated Target control child dialog.
*
*
* Revision Control:
*   09/09/22 v1.33   AGC	Improve behaviour when dialog hidden.
*
* Previous Changes:
*   06/07/22 1.32   AGC	Support intensity above background noise.
*   02/03/21 1.31   AGC	Fix typos.
*   26/08/20 1.30   AGC	Support P466 identity config.
*   26/05/17 1.29   AGC	Add dialog retrieval functions.
*   31/01/17 1.28   AGC	Support units configuration.
*   19/12/16 1.27   AGC	Improve behaviour on target deletion.
*   13/10/16 1.26   AGC	Add on-screen joystick button.
*   23/08/16 1.25   AGC	Event function prototype updated.
*   03/03/16 1.24   AGC	Fix clang warnings.
*   22/10/15 1.23   AGC	Support joystick control of targets.
*   26/01/15 1.22   AGC	Remove unused HICON.
*   21/11/14 1.21   AGC	Use new SetBitmap() function for buttons.
*   07/11/14 1.20   AGC	Update radar sim event handler.
*   18/07/14 1.19   AGC	Add target visibility display.
*   02/06/14 1.18   AGC	Support motion height offset.
*			Remove fixed test targets checkbox.
*			Improve PPI target following.
*   16/05/14 1.17   AGC	Support displaying new template dialog.
*   08/05/14 1.16   AGC	Support following selected target.
*   26/03/14 1.15   AGC	Support free-running targets.
*   27/01/14 1.14   AGC	Remove set name buttons.
*   22/01/14 1.13   AGC	Support heading offset for targets.
*   24/05/13 1.12   AGC	Support multilateration targets.
*   03/05/13 1.11   AGC	Support tooltips.
*			Support ADS-B.
*   25/04/13 1.10   AGC	Support height above ground.
*			Support configurable target shapes.
*			Support target templates.
*   08/04/13 1.9    AGC	Use SPxWinButton for configure properties button.
*   28/03/13 1.8    AGC	Add button to configure extended target info.
*   18/03/13 1.7    AGC	Set non-ASCII labels correctly.
*			Support fixed test targets.
*   04/03/13 1.6    AGC	Improve display when using track input.
*   12/02/13 1.5    AGC	Fix motion bitmap display on XP.
*   25/01/13 1.4    AGC	Add index to iteration.
*			Add show motion dialog button.
*   23/11/12 1.3    AGC	Add track renderer interaction.
*   09/11/12 1.2    AGC	Rename spin controls.
*			Support separate AIS/tracks options.
*   26/10/12 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxErrorCode. */
#include "SPxLibUtils/SPxError.h"

/* For SPxObjPtr. */
#include "SPxLibUtils/SPxObjPtr.h"

/* For SPxSimTarget. */
#include "SPxLibData/SPxSimTarget.h"

/* For SPxWinButton. */
#include "SPxLibWin/SPxWinButton.h"

/* For SPxWinEditCtrl. */
#include "SPxLibWin/SPxWinEditCtrl.h"

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* For SPxSimTargetPropCtrlPopupDlgWin. */
#include "SPxLibWin/SPxSimTargetPropCtrlPopupDlgWin.h"

/* For SPxSimTargetShapeCtrlPopupDlgWin. */
#include "SPxLibWin/SPxSimTargetShapeCtrlPopupDlgWin.h"

/* Forward declarations. */
class SPxRadarSimServer;
class SPxRadarSimulator;
class SPxSimRadar;
class SPxSimMotion;
class SPxSimTarget;
class SPxSimTargetTemplate;
class SPxTrackRenderer;
class SPxViewControl;
class SPxRadarTrack;
class SPxSimJoystickCtrlDlgWin;
class SPxSimTargetVisCtrlDlgWin;

/*
 * SPxSimTargetCtrlDlgWin dialog
 */
class SPxSimTargetCtrlDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxSimTargetCtrlDlgWin)

public:
    typedef void (*ShowMotion)(SPxSimMotion *motion, void *userArg);
    typedef void (*ShowTemplate)(SPxSimTargetTemplate *tgtTemplate, void *userArg);
    typedef void (*ShowJoystick)(void *userArg);

    /* Constructor and destructor. */
    explicit SPxSimTargetCtrlDlgWin(CWnd* pParent, 
				    SPxRadarSimServer *server,
				    SPxTrackRenderer *renderer=NULL,
				    unsigned int projectCode=0);
    virtual ~SPxSimTargetCtrlDlgWin(void);
    BOOL Create(void);
    void SyncFromObj(int updateAll=TRUE);
    SPxErrorCode SetCurrTarget(SPxSimTarget *target);
    void AddTarget(void);
    int IsRepaintDue(void);

    void SetShowMotionFn(ShowMotion fn, void *userArg);
    void SetShowTemplateFn(ShowTemplate fn, void *userArg);
    void SetShowJoystickFn(ShowJoystick fn, void *userArg);
    void SetCfgJoystickFn(ShowJoystick fn, void *userArg);
    
    void SetDisplayUnits(SPxUnitsDist_t unitsDist,
			 SPxUnitsSpeed_t unitsSpeed);

    CWnd *GetPropDlg(void);
    CWnd *GetShapeDlg(void);
    CWnd *GetVisDlg(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_TARGET_CTRL_DIALOG };

    /* Configuration retrieval functions. */
    SPxRadarSimulator *GetObj(void) { return m_obj; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    void OnTimer(UINT_PTR nIDEvent);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    void OnShowWindow(BOOL bShow, UINT nStatus);
    virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
    BOOL OnNeedToolTip(UINT id, NMHDR *pTTTStruct, LRESULT *pResult);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */

    /* The parent window */
    CWnd *m_parentWin;

    /* Callback variables. */
    ShowMotion m_showMotionFn;
    void *m_showMotionUserArg;
    ShowTemplate m_showTemplateFn;
    void *m_showTemplateUserArg;
    ShowJoystick m_showJoystickFn;
    void *m_showJoystickUserArg;
    ShowJoystick m_cfgJoystickFn;
    void *m_cfgJoystickUserArg;

    /* Units */
    SPxUnitsDist_t m_unitsDist;
    SPxUnitsSpeed_t m_unitsSpeed;

    /* The object we are controlling */
    SPxRadarSimServer *m_server;
    SPxRadarSimulator *m_obj;
    SPxObjPtr<SPxSimTarget> m_currTarget;
    SPxTrackRenderer *m_renderer;

    /* Controls. */
    CButton m_btnAdd;
    CButton m_btnDelete;
    CButton m_btnDeleteAll;
    CComboBox m_comboTargets;
    SPxWinEditCtrl m_editName;
    SPxWinSpinCtrl m_spinLat;
    SPxWinSpinCtrl m_spinLong;
    SPxWinSpinCtrl m_spinHeight;
    CButton m_radioHeightSea;
    CButton m_radioHeightGround;
    CComboBox m_comboDynSource;
    SPxWinSpinCtrl m_spinSpeed;
    SPxWinSpinCtrl m_spinCourse;
    CStatic m_lblHeadingOffset;
    SPxWinSpinCtrl m_spinHeadingOffset;
    CComboBox m_comboMotions;
    SPxWinButton m_btnShowMotion;
    CStatic m_staticTime;
    SPxWinSpinCtrl m_spinMotionOffTime;
    SPxWinSpinCtrl m_spinMotionOffX;
    SPxWinSpinCtrl m_spinMotionOffY;
    SPxWinSpinCtrl m_spinMotionOffH;
    SPxWinButton m_btnShowJoystick;
    SPxWinButton m_btnCfgJoystick;
    CComboBox m_comboTemplates;
    SPxWinButton m_btnShowTemplate;
    CButton m_checkPrimary;
    CButton m_checkTrack;
    CButton m_checkMultilat;
    CButton m_checkAis;
    CButton m_checkAdsb;
    SPxWinButton m_btnCfgProp;
    SPxWinSpinCtrl m_spinLength;
    SPxWinSpinCtrl m_spinBreadth;
    SPxWinSpinCtrl m_spinRcs;
    SPxWinSpinCtrl m_spinAboveBackground;
    SPxWinButton m_btnCfgShape;
    CStatic m_lblLat;
    CStatic m_lblLong;
    CStatic m_lblHeight;
    CStatic m_lblSpeed;
    CStatic m_lblCourse;
    CStatic m_lblOffsetX;
    CStatic m_lblOffsetY;
    CStatic m_lblOffsetH;
    CStatic m_lblLength;
    CStatic m_lblBreadth;
    CButton m_radioRcs;
    CButton m_radioAboveBackground;
    CButton m_checkFollow;
    CStatic m_lblVis;
    SPxWinButton m_btnShowVis;
    CBrush m_brushOK;
    CBrush m_brushErr;
    CBrush m_brushInvalid;

    /* Other variables. */
    SPxSimTargetPropCtrlPopupDlgWin m_tgtPropDlg;
    SPxSimTargetShapeCtrlPopupDlgWin<SPxSimTarget> m_tgtShapeDlg;
    SPxAutoPtr<SPxSimTargetVisCtrlDlgWin> m_tgtVisDlg;
    SPxAtomic<int> m_repaintDue;
    SPxAtomic<int> m_syncRequired;
    double m_prevCentreLatDegs;
    double m_prevCentreLongDegs;
    double m_prevWidthMetres;
    int m_viewChangeFromTimer;
    int m_removeCallback;

    /* Private functions */
    void followTarget(int setZoom);
    static SPxErrorCode addTargetToCombo(unsigned int index, SPxSimTarget *target, void *userArg);
    static SPxErrorCode addTemplateToCombo(unsigned int index, SPxSimTargetTemplate *tgtTemplate, void *userArg);
    static SPxErrorCode addMotionToCombo(unsigned int index, SPxSimMotion *motion, void *userArg);
    static SPxErrorCode eventFnWrapper(UINT32 flags, const SPxRadarSimEventParams *params, void *userArg);
    SPxErrorCode eventFn(UINT32 flags);
    static void trackSelectFn(SPxRadarTrack *track, void *userArg);
    static int deleteRendererFn(void *invokingObject, void *userObject, void *arg);
    static int viewChangeFnWrapper(void *invokingObject, void *userObject, void *arg);
    void viewChangeFn(SPxViewControl *viewCtrl, UINT32 flags);

    /*
     * Handler functions.
     */
    void OnBnClickedSpxSimTargetBtnDelete(void);
    void OnBnClickedSpxSimTargetBtnDeleteAll(void);
    void OnCbnSelchangeSpxSimTargetCombo(void);
    void OnBnClickedSpxSimTargetSetName(NMHDR*, LRESULT*);
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);
    void OnBnClickedSpxSimTargetBtnShowVis(void);
    void OnBnClickedSpxSimTargetRadioHeightSea(void);
    void OnBnClickedSpxSimTargetRadioHeightGround(void);
    void OnBnClickedSpxSimTargetCheckFollow(void);
    void OnCbnSelchangeSpxSimTargetComboDynSource(void);
    void OnCbnSelchangeSpxSimTargetComboMotion(void);
    void OnBnClickedSpxSimTargetBtnShowMotion(void);
    void OnBnClickedSpxSimTargetBtnShowJoystick(void);
    void OnBnClickedSpxSimTargetBtnCfgJoystick(void);
    void OnCbnSelchangeSpxSimTargetComboTemplate(void);
    void OnBnClickedSpxSimTargetBtnShowTemplate(void);
    void OnBnClickedSpxSimTargetCheckPrimary(void);
    void OnBnClickedSpxSimTargetCheckTrack(void);
    void OnBnClickedSpxSimTargetCheckMultilat(void);
    void OnBnClickedSpxSimTargetCheckAis(void);
    void OnBnClickedSpxSimTargetCheckAdsb(void);
    void OnBnClickedSpxSimTargetCheckRcs(void);
    void OnBnClickedSpxSimTargetCheckAboveBkgnd(void);
    void OnBnClickedSpxSimTargetBtnCfgProp(void);
    void OnBnClickedSpxSimTargetBtnCfgShape(void);

    using CDialog::Create;

}; /* class SPxSimTargetCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
