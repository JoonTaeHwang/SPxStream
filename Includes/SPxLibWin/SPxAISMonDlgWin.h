/*********************************************************************
*
* (c) Copyright 2020-2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the AIS Monitoring dialog.
*
*
* Revision Control:
*   01/02/22 v1.13   AGC	Scale minimum dialog size for DPI.
*
* Previous Changes:
*   25/02/21 1.12   AJH Set minimum dialog size.
*   22/02/21 1.11   JGB Renamed AISMenuType_t to SPxAISMonDlgSection_t.
*   11/02/21 1.10   JGB Added function to disable specific windows.
*   11/02/21 1.9    AJH	Add db param to constructor.
*   18/09/20 1.8    AJH	Move track table to separate dialog.
*   14/09/20 1.7    AJH	Add track table context menu.
*   11/09/20 1.6    AJH	Pass double click to main application.
*   10/09/20 1.5    AJH	Support action on track selection in table.
*   09/09/20 1.4    AJH	Use either static or nav data for radar position.
*			Use auto ptr again.
*   08/09/20 1.3    AJH	Track database not auto ptr.
*   08/09/20 1.2    AJH	Add track table and history.
*   04/09/20 1.1    AJH	Initial Version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibWin/SPxTableDlgWin.h"

/* Forward declarations. */
class SPxAISDecoder;
class SPxUniTrackDatabase;
class SPxAISIoDlgWin;
class SPxAISStatusDlgWin;
class SPxAISTrackDlgWin;
class SPxAISHistoryDlgWin;

typedef enum
{
    AIS_DLG_IO = 0,
    AIS_DLG_STATUS,
    AIS_DLG_TRACK,
    AIS_DLG_HISTORY
} SPxAISMonDlgSection_t;

/*
 * SPxAISMonDlgSection_t dialog
 */
class SPxAISMonDlgWin : public SPxTableDlgWin
{
    DECLARE_DYNAMIC(SPxAISMonDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxAISMonDlgWin(CWnd* pParent, SPxAISDecoder *obj,
			     SPxUniTrackDatabase *db, int showErrors);
    virtual ~SPxAISMonDlgWin(void);
    virtual BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_AIS_MON_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    /* Configuration retrieval functions. */
    SPxAISDecoder *GetObj(void) { return m_obj; }

    /* Child dialog access. */
    SPxAISIoDlgWin *GetChildIo(void)	{ return(m_ioDlg); }
    SPxAISStatusDlgWin *GetChildStatus(void) { return(m_statusDlg); }
    SPxAISTrackDlgWin *GetChildTracks(void) { return(m_trackDlg); }
    SPxAISHistoryDlgWin *GetChildHistory(void) { return(m_historyDlg); }

    void SetMenuEnabled(SPxAISMonDlgSection_t window, BOOL enabled = 1);

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    void OnDpiChangedAfterParent(void);
    void OnTimer(UINT_PTR nIDEvent);
    DECLARE_MESSAGE_MAP()

private:
    /*
     * Private variables.
     */
    /* The parent window */
    CWnd *m_parentWin;

    /* The object we are controlling */
    SPxAISDecoder *m_obj;

    /* Minimum size of the dialog. */
    RECT m_minSize;
    double m_minSizeScale;

    /* Child dialogs. */
    SPxAutoPtr<SPxAISIoDlgWin> m_ioDlg;
    SPxAutoPtr<SPxAISStatusDlgWin> m_statusDlg;
    SPxAutoPtr<SPxAISTrackDlgWin> m_trackDlg;
    SPxAutoPtr<SPxAISHistoryDlgWin> m_historyDlg;

    /*
     * Private functions
     */

}; /* class SPxAISMonDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
