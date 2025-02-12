/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for track log dialog.
*
*
* Revision Control:
*   30/10/18 v1.3    BTB	Move updating to background thread to avoid blocking GUI thread.
*
* Previous Changes:
*   17/10/18 1.2    BTB	Make UpdateLogs() public.
*   17/10/18 1.1    BTB	Initial version.
*
**********************************************************************/

#pragma once

/* Standard headers. */

/* Library headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#define SPX_AFXWIN

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxTrackNoteDb;
class SPxUniTrack;
class SPxHtmlCtrlDlgWin;
class SPxTrackNoteDlgWin;

/*
* SPxTrackLogDlgWin dialog
*/
class SPxTrackLogDlgWin : public CDialog
{
public:

    /* Constructor and destructor. */
    SPxTrackLogDlgWin(SPxTrackNoteDb *trackNoteDb, CWnd* parentWin = NULL);
    virtual ~SPxTrackLogDlgWin(void);
    virtual BOOL Create(void);

    /* General. */
    virtual SPxErrorCode SetTrack(SPxUniTrack *track);
    virtual SPxUniTrack *GetTrack(void) { return m_track; }
    virtual SPxErrorCode ResetTrack(void);
    void UpdateLogs(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_TRACK_LOG_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    void OnSize(UINT nType, int cx, int cy);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ }
    virtual void OnCancel(void);
    DECLARE_MESSAGE_MAP()

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* The parent window. */
    CWnd *m_parentWin;

    /* Track note database object. */
    SPxTrackNoteDb *m_trackNoteDb;

    /* Current track. */
    SPxUniTrack *m_track;

    /* Child dialogs. */
    SPxHtmlCtrlDlgWin *m_trackLogsDlg;
    SPxTrackNoteDlgWin *m_trackNoteDlg;

    /* Private functions. */
    static void handleLog(void *userArg);
    SPxErrorCode doUpdate(void);
    SPxErrorCode doUpdateComplete(void);

    using CDialog::Create;

}; /* class SPxTrackLogDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
