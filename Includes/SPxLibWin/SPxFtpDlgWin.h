/*********************************************************************
*
* (c) Copyright 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxFtpDlgWin.h,v $
* ID: $Id: SPxFtpDlgWin.h,v 1.3 2016/03/03 10:59:08 rew Exp $
*
* Purpose:
*	Header for generic SPx FTP dialog.
*
*
* Revision Control:
*   03/03/16 v1.3    AGC	Fix clang warnings.
*
* Previous Changes:
*   24/09/14 1.2    AGC	Add link for forgotten password.
*   04/07/14 1.1    AGC	Initial version.
**********************************************************************/
#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

#include "SPxLibWin/SPxWinEditCtrl.h"

/* Define our class. */
class SPxFtpDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxFtpDlgWin)

public:
    /* Construction and destruction */
    explicit SPxFtpDlgWin(CWnd* pParentWin, const char *defUsername,
	const char *filename=NULL);
    virtual ~SPxFtpDlgWin(void);
    virtual BOOL Create(void);

    CString GetUsername(void) const { return m_username; }
    CString GetPassword(void) const { return m_password; }

    /* Dialog data. */
    enum { IDD = IDD_SPX_FTP_DIALOG };

protected:
    /* Protected functions */
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(void);
    BOOL OnSetCursor(CWnd *pWnd, UINT nHitTest, UINT message);
    HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    CWnd *m_parent;
    CStringA m_filename;
    CString m_username;
    CString m_password;
    SPxWinEditCtrl m_editUsername;
    SPxWinEditCtrl m_editPassword;
    CStatic m_linkPassword;
    HCURSOR m_cursor;
    CFont m_fontUnderline;
    int m_visited;

    void OnUsernameChange(void);
    void OnPasswordChange(void);
    void OnUsernameReturn(NMHDR*, LRESULT*);
    void OnPasswordReturn(NMHDR*, LRESULT*);
    void OnForgotPassword(void);

    using CDialog::Create;

}; /* SPxFtpDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
