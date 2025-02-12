/*********************************************************************
*
* (c) Copyright 2011 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the HPx advanced control child dialog.
*
*
* Revision Control:
*   23/01/18 v1.7    AGC	Support changing object.
*
* Previous Changes:
*   04/05/17 1.6    SP 	Add digital enable checkbox.
*   12/04/17 1.5    SP 	Major rework to support HPx-400 dual streams.
*   03/03/16 1.4    AGC	Fix clang warnings.
*   26/10/12 1.3    AGC	Add forward declaration of SPxHPx100Source.
*   09/12/11 1.2    SP 	Add m_warnPrfRangeTooShort.
*   25/05/11 1.1    SP 	Initial version.
*
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* HPx card header. */
#include "SPxLibData/SPxHPx100Source.h"

/*
 * SPxWinCtrlSrcHPx dialog
 */
class SPxHPxAdvCtrlDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxHPxAdvCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxHPxAdvCtrlDlgWin(CWnd* pParent, SPxHPx100Source *obj);
    virtual ~SPxHPxAdvCtrlDlgWin();
    BOOL Create(int fixedStreamIdx=-1);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SRC_HPX_CTRL_ADV_DIALOG };

    /* Sync functions. */
    void SyncFromObj(void);

    /* Configuration retrieval functions. */
    SPxHPx100Source *GetObj(void) { return m_obj; }
    void SetObj(SPxHPx100Source *obj) { m_obj = obj; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    virtual void OnTimer(UINT_PTR nIDEvent);
    virtual void OnOK(void);
    virtual void OnCancel(void);
    virtual HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    DECLARE_MESSAGE_MAP()

private:

    /* Private types */
    typedef struct
    {
        BOOL warnPrfRangeTooShort;      /* Bad PRF range warning flag */

    } StreamContextAdv_t;

    /*
     * Handler functions.
     */
    afx_msg void OnStream(void);
    afx_msg void OnEnableStats(void);
    afx_msg void OnIntRateSpin(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnAcpVrefSpin(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnArpVrefSpin(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnTrgVrefSpin(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSig1VrefSpin(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSig2VrefSpin(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSig3VrefSpin(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnEnableDigital(void);

    /* Update functions */
    void UpdateStatusDisplay(void);
    void UpdateStatisticsDisplay(void);
    
    /* GUI control variables. */
    int m_streamIdx;        /* Current stream index (must be signed). */
    BOOL m_statsEnable;     /* Enable or disable stats from HPx */
    int m_intRateHz;        /* Interrupt rate in Hz */
    double m_acpVref;       /* ACP single-ended threshold voltage */
    double m_arpVref;       /* ARP single-ended threshold voltage */
    double m_trgVref;       /* Trigger single-ended threshold voltage */
    double m_sig1Vref;      /* SIG1 single-ended threshold voltage */
    double m_sig2Vref;      /* SIG2 single-ended threshold voltage */
    double m_sig3Vref;      /* SIG3 single-ended threshold voltage */
    BOOL m_digEnable;       /* Digital input enabled? */

    /* The parent window */
    CWnd *m_parentWin;

    /* The underlying object. */
    SPxHPx100Source *m_obj;	   /* The object we are controlling */

    /* Used to limit control of dual-stream functions. */
    int m_fixedStreamIdx; /* Fixed stream index or -1 if not fixed. */

    /* Stream specific variables. */
    StreamContextAdv_t m_streams[SPX_HPX400_MAX_NUM_STREAMS];

    using CDialog::Create;

}; /* class SPxHPxAdvCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
