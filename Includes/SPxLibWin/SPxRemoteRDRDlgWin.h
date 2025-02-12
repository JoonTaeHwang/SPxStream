/*********************************************************************
*
* (c) Copyright 2018 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for remote RDR control dialog.
*
*
* Revision Control:
*   20/11/20 v1.15   BTB	Rename class.
*
* Previous Changes:
*   17/07/20 1.14   AGC	Support customising input label.
*   17/07/20 1.13   AGC	Support control of playback speed.
*   09/07/20 1.12   AGC	Improve syncing.
*   06/07/20 1.11   AGC	Support optional client live/replay control.
*			Display message when note/event logged.
*   23/06/20 1.10  BTB	More fixes to control states.
*   23/06/20 1.9   BTB	Revert change to session index variable type.
*   22/06/20 1.8   BTB	Fix to setting goto controls on session change from RDR.
*   19/06/20 1.7   BTB	Set goto controls to the start time of the session on loading it.
*   17/06/20 1.6   BTB	Fix build issues.
*   16/06/20 1.5   BTB	Add replay goto and loop controls.
*   13/03/19 1.4   SP 	Add record and replay channel lists.
*   16/05/18 1.3   BTB 	Override OnOK and OnCancel.
*   26/04/18 1.2   BTB 	Cleanup and build fixes.
*   25/04/18 1.1   BTB 	Initial Version.
*
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#define SPX_AFXWIN
#include "SPxResource.h"

/* Include required header for CDateTimeCtrl. */
#include "afxdtctl.h"

#include "SPxLibNet/SPxRemoteRDR.h"
#include "SPxLibUtils/SPxAutoPtr.h"

/* Forwards declare any classes required. */
class SPxRemoteRDRChanListDlgWin;

/*
 * SPxRemoteRDRDlgWin dialog
 */
class SPxRemoteRDRDlgWin : public CDialog
{
public:

    /* Constructor and destructor. */
    explicit SPxRemoteRDRDlgWin(SPxRemoteRDR *rdrConn, CWnd *parentWin = NULL,
			        int enableInputCtrls=FALSE,
			        const char *inputCtrlsTitle=NULL);
    virtual ~SPxRemoteRDRDlgWin(void);
    virtual BOOL Create(CWnd *parent);

    void Sync(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_REMOTE_RDR_DLG };

protected:

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };

private:
    /* Private variables. */

    struct impl;
    SPxAutoPtr<impl> m_p;

    SPxRemoteRDR *m_rdrConn;

    /* Application input controls. */
    int m_enableInputCtrls;
    char *m_inputCtrlsTitle;
    CStatic m_inputGroup;
    CButton m_inputLiveRadio;
    CButton m_inputReplayRadio;

    /* Status LEDs. */
    CStatic *m_recordLED;
    CStatic *m_recordChannelsLED;
    CStatic *m_replayLED;
    CStatic *m_replayChannelsLED;

    /* Record buttons. */
    CButton *m_recordStartBn;
    CButton *m_recordStopBn;

    /* Replay buttons. */
    CButton *m_replayPlayBn;
    CButton *m_replayPauseBn;
    CButton *m_replayStopBn;
    CComboBox *m_replaySpeedCombo;
    CButton *m_replayLoopBn;

    /* Logging buttons. */
    CStatic *m_logStatus;
    CButton *m_logNoteBn;
    CButton *m_logEventBn;
    UINT32 m_logTime;

    /* Top level edit controls. */
    CWnd *m_versionEdit;
    CWnd *m_projectEdit;

    /* Record edit controls. */
    CWnd *m_recordSessionEdit;
    CWnd *m_recordStartEdit;
    CWnd *m_recordCurrentEdit;
    CWnd *m_recordDurationEdit;
    CWnd *m_recordNoteEdit;
    CWnd *m_recordDataRateEdit;

    /* Replay edit controls. */
    CWnd *m_replayStartEdit;
    CWnd *m_replayCurrentEdit;
    CWnd *m_replayEndEdit;
    CWnd *m_replayDurationEdit;
    CWnd *m_replayDataRateEdit;

    /* Replay Jump To controls. */
    CDateTimeCtrl *m_replayJumptoDatePicker;
    CDateTimeCtrl *m_replayJumptoTimePicker;
    CWnd *m_replayJumptoDateTimeBn;

    /* Event and Session list controls. */
    CComboBox *m_recordEventCombo;
    CComboBox *m_replaySessionCombo;

    /* Channel lists. */
    SPxRemoteRDRChanListDlgWin *m_recordChanList;
    SPxRemoteRDRChanListDlgWin *m_replayChanList;

    /* Button icons. */
    HICON m_recordIcon;
    HICON m_recordGreyIcon;
    HICON m_stopIcon;
    HICON m_playIcon;
    HICON m_playGreyIcon;
    HICON m_pauseIcon;
    HICON m_pauseGreyIcon;

    /* Private functions. */
    void updateFast(void);
    void updateSlow(void);
    void updateInputControls(void);
    void updateRecordControls(void);
    void updateReplayControls(void);
    void updateEventList(void);
    void updateSessionList(void);
    void updateTopLevelControls(void);

    /* GUI callback functions. */
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedInputLiveRadio(void);
    afx_msg void OnBnClickedInputReplayRadio(void);
    afx_msg void OnBnClickedRecStartButton(void);
    afx_msg void OnBnClickedRecStopButton(void);
    afx_msg void OnBnClickedRepPlayButton(void);
    afx_msg void OnBnClickedRepPauseButton(void);
    afx_msg void OnBnClickedRepStopButton(void);
    afx_msg void OnCbnSelchangeSpeedCombo(void);
    afx_msg void OnBnClickedRepLoopCheck(void);
    afx_msg void OnBnClickedLogNoteButton(void);
    afx_msg void OnBnClickedLogEventButton(void);
    afx_msg void OnBnClickedJumptoDateTimeButton(void);
    afx_msg void OnCbnSelchangeEventCombo(void);
    afx_msg void OnCbnSelchangeSessionCombo(void);

    using CDialog::Create;

};

/*********************************************************************
*
* End of file
*
**********************************************************************/