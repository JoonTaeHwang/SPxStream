/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the ROC advanced control child dialog.
*
*
* Revision Control:
*   14/03/22 v1.2    AJH	Support open drain and inverted outputs.
*
* Previous Changes:
*   25/02/22 1.1    AJH	Initial version.
*
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* HPx card header. */
#include "SPxLibData/SPxROC.h"

/*
 * SPxROCAdvCtrlDlgWin dialog
 */
class SPxROCAdvCtrlDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxROCAdvCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxROCAdvCtrlDlgWin(CWnd* pParent, SPxROC *obj);
    virtual ~SPxROCAdvCtrlDlgWin();
    BOOL Create(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_ROC_CTRL_ADV_DIALOG };

    /* Sync functions. */
    void SyncFromObj(void);

    /* Configuration retrieval functions. */
    SPxROC *GetObj(void) { return m_obj; }
    void SetObj(SPxROC *obj) { m_obj = obj; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    virtual void OnTimer(UINT_PTR nIDEvent);
    virtual void OnOK(void);
    virtual void OnCancel(void);
    virtual HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    DECLARE_MESSAGE_MAP()

private:

    /*
     * Handler functions.
     */
    afx_msg void OnTrgVoltsSpin(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnAcpVoltsSpin(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnArpVoltsSpin(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnGPIOVoltsSpin(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnTRG(void);
    afx_msg void OnACP(void);
    afx_msg void OnARP(void);
    afx_msg void OnSHM(void);

    /* Update functions */
    void UpdateStatisticsDisplay(void);

    /* GUI control variables. */
    double m_trgVolts;       /* Trigger voltage */
    double m_acpVolts;       /* ACP voltage */
    double m_arpVolts;       /* ARP voltage */
    double m_GPIOVolts;      /* GPIO voltage */
    unsigned int m_trg;         /* TRG selection */
    unsigned int m_acp;         /* ACP selection */
    unsigned int m_arp;         /* ARP selection */
    unsigned int m_shm;         /* SHM selection */
    BOOL m_trgInv;              /* TRG inverted? */
    BOOL m_acpInv;              /* ACP inverted? */
    BOOL m_arpInv;              /* ARP inverted? */
    BOOL m_shmInv;              /* SHM inverted? */

    /* The parent window */
    CWnd *m_parentWin;

    /* The underlying object. */
    SPxROC *m_obj;	   /* The object we are controlling */

    using CDialog::Create;

}; /* class SPxROCAdvCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
