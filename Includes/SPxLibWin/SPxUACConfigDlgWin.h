/*********************************************************************
*
* (c) Copyright 2018 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for UAC configuration dialog.
*
*
* Revision Control:
*   20/11/20 v1.3   BTB 	Rename class.
*
* Previous Changes:
*   26/06/18 1.2   BTB 	Add multi-language support.
*   31/05/18 1.1   BTB 	Initial Version.
*
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#define SPX_AFXWIN
#include "SPxResource.h"

/* For SPxRemoteRDR */
#include "SPxLibUtils/SPxAccessControl.h"

/*
 * SPxUACConfigDlgWin dialog
 */
class SPxUACConfigDlgWin : public CDialog
{
public:

    /* Constructor and destructor. */
    SPxUACConfigDlgWin(SPxAccessControl *accessControl, CWnd *parentWin = NULL);
    virtual ~SPxUACConfigDlgWin(void);
    virtual BOOL Create(CWnd *parent);

    /* Dialog Data */
    enum { IDD = IDD_SPX_UAC_CONFIG_DIALOG };

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

    SPxAccessControl *m_accessControl;
    int dlgIsAdmin;

    CButton *m_adminBn;
    CButton *m_passwdBn;

    /* Private functions. */
    void UpdateControls(void);

    /* GUI callback functions. */
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnOnBnClickedAdminButton(void);
    afx_msg void OnOnBnClickedPasswordButton(void);

    using CDialog::Create;

};

/*********************************************************************
*
* End of file
*
**********************************************************************/
