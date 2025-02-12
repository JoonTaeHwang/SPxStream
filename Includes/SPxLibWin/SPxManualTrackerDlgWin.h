/*********************************************************************
*
* (c) Copyright 2020 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the manual tracker control child dialog.
*
*
* Revision Control:
*   02/09/22 v1.8    AGC	Supress cppcheck duplicate inherited member warning.
*
* Previous Changes:
*   26/05/22 1.7    BTB	Support specifying type of track report to send.
*   10/03/20 1.6    BTB	Support showing/hiding track lat/long.
*                      	Improve resizing of dialog.
*   04/03/20 1.5    BTB	Don't print speed and couse if not calculating it.
*   03/03/20 1.4    BTB	Move sensor info controls to collapsible child dialog.
*   02/03/20 1.3    BTB	Support configuration of speed/course calculation.
*   02/03/20 1.2    BTB	Fix track count.
*   21/02/20 1.1    BTB	Initial Version.
**********************************************************************/

#pragma once

/* Library headers. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* Include the SPx resource header */
#include "SPxResource.h"

#define SPX_AFXWIN

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* Forward declarations. */
class SPxAsterixEncoder;
class SPxManualTrackerSensorInfoDlgWin;
class SPxPacketSender;
class SPxTableDlgWin;

/*
 * SPxManualTrackListBox class.
 */
class SPxManualTrackListBox : public CListBox
{
public:
    SPxManualTrackListBox(int showSpeedCourse, int showLatLong)
        : m_showSpeedCourse(showSpeedCourse), m_showLatLong(showLatLong)
    { /* Do nothing */; }
    virtual ~SPxManualTrackListBox(void)	{ /* Do nothing */; }
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

private:
    int m_showSpeedCourse;
    int m_showLatLong;
};

/*
 * SPxManualTrackerDlgWin class.
 */
class SPxManualTrackerDlgWin : public CDialog
{
public:
    /* Constructor and destructor. */
    SPxManualTrackerDlgWin(CWnd* pParent,
                           SPxAsterixEncoder *trackEncoder,
                           SPxPacketSender *trackSender,
                           int calcSpeedCourse,
                           int showLatLong,
                           int reportType);
    virtual ~SPxManualTrackerDlgWin();
    BOOL Create(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_MANUAL_TRACKER_DIALOG };

    /* Sync functions. */
    virtual void SyncGUI(void);

protected:
    virtual void DoResizing(void);
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    virtual void OnTimer(UINT_PTR nIDEvent);
    void OnShowWindow(BOOL bShow, UINT nStatus);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    afx_msg void OnButtonCreate(void);
    afx_msg void OnButtonDelete(void);
    afx_msg void OnButtonUpdate(void);
    afx_msg void OnListBoxSelChanged(void);
    LRESULT OnLayoutChangeMessage(WPARAM, LPARAM);
    DECLARE_MESSAGE_MAP()

private:
    /*
     * Private members.
     */
    struct impl;
    // cppcheck-suppress duplInheritedMember
    SPxAutoPtr<impl> m_p;

    /* The parent window */
    CWnd *m_parentWin;

    /* Controls. */
    CStatic m_trackCountLabel;
    CButton m_trackCreateBn;
    CButton m_trackDeleteBn;
    SPxManualTrackListBox m_trackListBox;
    SPxWinSpinCtrl m_trackRangeSpin;
    SPxWinSpinCtrl m_trackBearingSpin;
    SPxWinSpinCtrl m_trackAltitudeSpin;
    CButton m_trackUpdateBn;
    CWnd m_sensorInfoPh;

    /* Sensor info child dialog and table dialog containing it. */
    SPxTableDlgWin *m_tableDlg;
    SPxManualTrackerSensorInfoDlgWin *m_sensorInfoDlg;

    /* Font for list box. */
    CFont m_font;

    /* Information used for sending status messages. */
    UINT32 m_lastStatusTimeMsecs;
    UINT32 m_tracksSinceLastStatus;

    /* Encoder for creating ASTERIX messages. */
    SPxAsterixEncoder *m_trackEncoder;

    /* Packet sender for outputting ASTERIX messages. */
    SPxPacketSender *m_trackSender;

    /* Whether to calculate (and show) track speed and course. */
    int m_calcSpeedCourse;

    /* Whether or not to show track lat/long. */
    int m_showLatLong;

    /* The type of report we are sending. */
    int m_reportType;

    /*
     * Private functions
     */
    void syncTrackListBox(void);
    void syncTrackControls(void);
    void syncTrackCount(void);

    using CDialog::Create;

}; /* class SPxManualTrackerDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
