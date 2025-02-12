/*********************************************************************
*
* (c) Copyright 2014 - 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxCamCtrlDlgWin.h,v $
* ID: $Id: SPxCamCtrlDlgWin.h,v 1.10 2017/04/20 13:16:32 rew Exp $
*
* Purpose:
*	Header for the Camera control child dialog.
*
*
* Revision Control:
*   20/04/17 v1.9    AGC	Add IsSupported callback.
*
* Previous Changes:
*   31/10/16 1.8    AGC	Support slave controllers.
*   03/03/16 1.7    AGC	Fix clang warnings.
*   10/09/15 1.6    AGC	Simplify now that SPxCamera does not block.
*   26/08/15 1.5    AGC	Add absolute focus control.
*   28/05/15 1.4    REW	Add slew-to-cue smoothness control.
*   05/02/15 1.3    AGC	Derive from SPxTabbedItemDlg.
*   20/06/14 1.2    AGC	Add auto focus control.
*			Support disabling controls.
*   17/02/14 1.1    AGC	Initial version.
*
**********************************************************************/

#pragma once

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For CreateCamCtrl function pointer. */
#include "SPxLibWin/SPxCamCtrlAdvDlgWin.h"

/* Include the base class header. */
#include "SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxCamera;

/*
 * SPxCamCtrlDlgWin dialog
 */
class SPxCamCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxCamCtrlDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxCamCtrlDlgWin(CWnd *pParent, SPxCamera *camera,
			      SPxCamCtrlAdvDlgWin::CreateCamCtrl createFn=NULL,
			      void *createUserArg=NULL,
			      SPxCamCtrlAdvDlgWin::IsSupportedCamCtrl isSupportedFn=NULL,
			      void *isSupportedUserArg=NULL,
			      int showSlaveOption=FALSE);
    virtual ~SPxCamCtrlDlgWin(void);
    BOOL Create(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_CAM_CTRL_DIALOG };

    /* Sync functions. */
    virtual int GetIDD(void) const { return IDD; }
    virtual void SyncFromObj(void);
    void SyncControls(void);
    void EnableControls(int enable);
    void UpdateControllers(void);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnTimer(UINT_PTR nIDEvent);
    virtual void OnOK(void) {}
    virtual void OnCancel(void) {}
    DECLARE_MESSAGE_MAP()

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    void OnCheckPos(void);
    void OnCheckAlt(void);
    void OnSpinLat(NMHDR*, LRESULT*);
    void OnSpinLong(NMHDR*, LRESULT*);
    void OnSpinAlt(NMHDR*, LRESULT*);
    void OnSpinBearing(NMHDR*, LRESULT*);
    void OnSpinElevation(NMHDR*, LRESULT*);
    void OnSpinFov(NMHDR*, LRESULT*);
    void OnSpinFovMin(NMHDR*, LRESULT*);
    void OnSpinFovMax(NMHDR*, LRESULT*);
    void OnSpinSlewSmoothness(NMHDR*, LRESULT*);
    void OnSpinFocus(NMHDR*, LRESULT*);
    void OnCheckAutoFocus(void);
    void OnClickCtrl(void);
    static void joyFnWrapper(void *userArg);
    void joyFn(void);
    void syncFromObj(void);
    void syncControls(void);
    SPxErrorCode syncAbs(void);
    SPxErrorCode updateCamSupport(void);
    SPxErrorCode updateCamSupportGui(void);

    using CDialog::Create;

}; /* class SPxCamCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
