/*********************************************************************
*
* (c) Copyright 2015, 2016, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxP226AdvCtrlDlgWin.h,v $
* ID: $Id: SPxP226AdvCtrlDlgWin.h,v 1.2 2016/03/03 10:59:08 rew Exp $
*
* Purpose:
*	Header for the P226 advanced control child dialog.
*
*
* Revision Control:
*   03/03/16 v1.2    AGC	Fix clang warnings.
*
* Previous Changes:
*   05/06/15 1.1    SP 	Initial version.
*
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* Forward declarations. */
class SPxNetworkReceiveP226;

/*
 * SPxWinCtrlSrcP226 dialog
 */
class SPxP226AdvCtrlDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxP226AdvCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxP226AdvCtrlDlgWin(CWnd* pParent, SPxNetworkReceiveP226 *obj);
    virtual ~SPxP226AdvCtrlDlgWin();
    BOOL Create(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SRC_P226_CTRL_ADV_DIALOG };

    /* Sync functions. */
    void SyncFromObj(void);

    /* Configuration retrieval functions. */
    SPxNetworkReceiveP226 *GetObj(void) { return m_obj; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    virtual void OnOK(void);
    virtual void OnCancel(void);
    afx_msg void OnBnClickedSerialNumSetButton();
    afx_msg void OnBnClickedLicKeySetButton();
    DECLARE_MESSAGE_MAP()

private:
    /*
     * Handler functions.
     */

    /* Update functions */
    
    /* GUI control variables. */

    /* The parent window */
    CWnd *m_parentWin;

    /* The underlying object. */
    SPxNetworkReceiveP226 *m_obj;   /* The object we are controlling */

    using CDialog::Create;

}; /* class SPxP226AdvCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
