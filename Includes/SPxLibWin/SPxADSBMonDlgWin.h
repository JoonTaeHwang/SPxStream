/*********************************************************************
*
* (c) Copyright 2021, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the ADS-B Monitoring dialog.
*
*
* Revision Control:
*   01/02/22 v1.5    AGC	Scale minimum dialog size for DPI.
*
* Previous Changes:
*   25/02/21 1.4    AJH Set minimum dialog size.
*   22/02/21 1.3    JGB Renamed ADSBMenuType_t to SPxADSBMonDlgSection_t.
*   12/02/21 1.2    JGB Pass Track DB through constructor.
*   11/02/21 1.1    JGB	Added function to disable specific windows.
*   27/01/21 1.0    JGB	Initial Version.
*
* Previous Changes:
*
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibWin/SPxTableDlgWin.h"

/* Forward declarations. */
class SPxADSBDecoder;
class SPxUniTrackDatabase;
class SPxADSBIoDlgWin;
class SPxADSBStatusDlgWin;
class SPxADSBTrackDlgWin;
class SPxADSBHistoryDlgWin;

typedef enum
{
    ADSB_DLG_IO = 0,
    ADSB_DLG_STATUS,
    ADSB_DLG_TRACK,
    ADSB_DLG_HISTORY
} SPxADSBMonDlgSection_t;

/*
 * SPxADSBMonDlgSection_t dialog
 */
class SPxADSBMonDlgWin : public SPxTableDlgWin
{
    DECLARE_DYNAMIC(SPxADSBMonDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxADSBMonDlgWin(CWnd* pParent, SPxADSBDecoder *obj,
                              SPxUniTrackDatabase* db, int showErrors);
    virtual ~SPxADSBMonDlgWin(void);
    virtual BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_ADSB_MON_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    /* Configuration retrieval functions. */
    SPxADSBDecoder *GetObj(void) { return m_obj; }

    /* Child dialog access. */
    SPxADSBIoDlgWin *GetChildIo(void)	{ return(m_ioDlg); }
    SPxADSBStatusDlgWin *GetChildStatus(void) { return(m_statusDlg); }
    SPxADSBTrackDlgWin *GetChildTracks(void) { return(m_trackDlg); }
    SPxADSBHistoryDlgWin *GetChildHistory(void) { return(m_historyDlg); }

    void SetMenuEnabled(SPxADSBMonDlgSection_t window, BOOL enabled = 1);

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    DECLARE_MESSAGE_MAP()

private:
    /*
     * Private variables.
     */
    /* The parent window */
    CWnd *m_parentWin;

    /* The object we are controlling */
    SPxADSBDecoder *m_obj;

    /* Minimum size of the dialog. */
    RECT m_minSize;
    double m_minSizeScale;

    /* Child dialogs. */
    SPxAutoPtr<SPxADSBIoDlgWin> m_ioDlg;
    SPxAutoPtr<SPxADSBStatusDlgWin> m_statusDlg;
    SPxAutoPtr<SPxADSBTrackDlgWin> m_trackDlg;
    SPxAutoPtr<SPxADSBHistoryDlgWin> m_historyDlg;

    /*
     * Private functions
     */

}; /* class SPxADSBMonDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
