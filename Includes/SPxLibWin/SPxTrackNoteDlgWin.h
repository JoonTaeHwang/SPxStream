/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for track note logging dialog.
*
*
* Revision Control:
*   30/10/18 v1.4    BTB	Move logging to background thread to avoid blocking GUI thread.
*
* Previous Changes:
*   22/10/18 1.3    BTB	Add multi language support.
*   17/10/18 1.2    BTB	Add handler for reporting logging to parent dialog.
*			Minor cleanup.
*   12/10/18 1.1    BTB	Initial version.
*
**********************************************************************/

#pragma once

/* Standard headers. */

/* Library headers. */
#include "SPxLibUtils/SPxAutoPtr.h"

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
class SPxImage;

/*
 * SPxTrackNoteDlgWin dialog
 */
class SPxTrackNoteDlgWin : public CDialog
{
public:
    /* Handler for reporting the logging of a new track note. */
    typedef void (*ReportLogFn_t)(void *userArg);	/* User arg. */

    /* Constructor and destructor. */
    SPxTrackNoteDlgWin(SPxTrackNoteDb *trackNoteDb, CWnd* parentWin = NULL);
    virtual ~SPxTrackNoteDlgWin(void);
    virtual BOOL Create(void);

    /* General. */
    virtual SPxErrorCode SetTrack(SPxUniTrack *track);
    virtual SPxUniTrack *GetTrack(void) { return m_track; }
    virtual SPxErrorCode ResetTrack(void);

    virtual SPxErrorCode SetSnapshot(SPxImage *image);

    /* Report log handler. */
    void SetReportLogHandler(ReportLogFn_t fn, void *userArg);

    /* Dialog Data */
    enum { IDD = IDD_SPX_TRACK_NOTE_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
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

    /* Track to log notes for. */
    SPxUniTrack *m_track;

    /* Image to log with note. */
    SPxImage *m_image;

    /* Controls. */
    CWnd m_noteEdit;
    CButton m_logBn;

    /* Report log function. */
    ReportLogFn_t m_reportLogFn;
    void *m_reportLogUserArg;

    /* Private functions. */
    SPxErrorCode doLogging(void);
    SPxErrorCode doLoggingComplete(void);
    SPxErrorCode logImage(void);
    SPxErrorCode logImageComplete(void);

    /* GUI callback functions. */
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedLogButton(void);

    using CDialog::Create;

}; /* class SPxTrackNoteDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
