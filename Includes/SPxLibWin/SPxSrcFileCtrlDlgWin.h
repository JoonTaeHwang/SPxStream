/*********************************************************************
*
* (c) Copyright 2012 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the File Replay control dialog.
*
*
* Revision Control:
*   25/06/18 v1.11   AGC	Correctly suppress cppcheck warnings.
*
* Previous Changes:
*   23/01/18 1.10   AGC	Support multiple objects.
*   05/09/16 1.9    AGC	Add required includes for date/time controls.
*   02/09/16 1.8    AGC	Add date/time controls.
*   03/03/16 1.7    AGC	Fix clang warnings.
*   01/02/16 1.6    AGC	Add step button.
*   02/06/14 1.5    AGC	Improve enabled state of buttons.
*   21/01/14 1.4    AGC	Create tooltips using method that avoids MFC assert.
*   18/11/13 1.3    AGC	Use SPxCompactPath.
*   08/03/13 1.2    AGC	Support use within tabbed dialog.
*   26/10/12 1.1    AGC	Initial Version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include required header for CDateTimeCtrl. */
#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"
#include "afxdtctl.h"

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibWin/SPxWinButton.h"
#include "SPxLibWin/SPxWinSpinCtrl.h"
#include "SPxLibWin/SPxWinTask.h"

/* For base class. */
#include "SPxLibWin/SPxTabbedPopupDlgWin.h"

/* Forward declarations. */
class SPxRadarReplay;

/*
 * SPxSrcFileCtrlDlgWin dialog
 */
class SPxSrcFileCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxSrcFileCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxSrcFileCtrlDlgWin(CWnd* pParent,
			 SPxRadarReplay *obj);
    SPxSrcFileCtrlDlgWin(CWnd* pParent,
			 unsigned int numObjects,
			 SPxRadarReplay *obj[]);
    virtual ~SPxSrcFileCtrlDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);

    // Dialog Data
    enum { IDD = IDD_SPX_SRC_FILE_CTRL_DIALOG };
    virtual int GetIDD(void) const { return IDD; }
    virtual CWnd *GetGroupBox(void) { return &m_mainGroup; }
    virtual int ShowCheck(void) { return TRUE; }
    virtual void Enable(int enable);
    virtual int GetEnable(void);
    void ShowStepButton(int show);

    /* Set functions. */
    void SetObj(SPxRadarReplay *obj)	{ m_obj = obj; }
    // cppcheck-suppress virtualCallInConstructor
    virtual void SetObjIndex(int index);

    /* Sync functions. */
    void UpdateFileTimes(void);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(void);
    void OnOK(void){};
    void OnCancel(void){};
    void OnTimer(UINT_PTR nIDEvent);
    virtual BOOL PreTranslateMessage(MSG *pMsg);
    void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
    DECLARE_MESSAGE_MAP()

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Parent window. */
    CWnd *m_parentWin;

    /* GUI control variables. */
    CStatic m_mainGroup;		/* Main group box */
    CSliderCtrl m_posnSlider;		/* Position slider */
    SPxWinButton m_btnRewind;		/* Rewind button */
    SPxWinButton m_btnPause;		/* Pause button */
    SPxWinButton m_btnStep;		/* Step button */
    SPxWinButton m_btnPlay;		/* Play button */
    CButton m_btnSelectFile;		/* Select file button. */
    CDateTimeCtrl m_dateCtrl;
    CDateTimeCtrl m_timeCtrl;
    SPxWinSpinCtrl m_btnSpeedUp;	/* Speed-up spin button. */
    CButton m_btnAutoRewind;		/* Auto-rewind check box */
    CToolTipCtrl m_toolTip;
    int m_showStepButton;		/* Show step button? */

    SPxWinTask m_winTask;		/* Object to run async tasks */

    /* The underlying object. */
    SPxRadarReplay *m_obj;		/* The object we are controlling */

    /*
     * Handler functions.
     */
    void OnRewind(void);
    void OnPause(void);
    void OnStep(void);
    void OnPlay(void);
    void OnSetAutoRewind(void);
    void OnBrowse(void);
    void OnSpeedUp(NMHDR *pNMHDR, LRESULT *pResult);
    void OnDateTimeChange(NMHDR *pNMHDR, LRESULT *pResult);

    SPxErrorCode step(void);
    SPxErrorCode stepComplete(void);

    using SPxTabbedItemDlg::Create;

}; /* class SPxSrcFileCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
