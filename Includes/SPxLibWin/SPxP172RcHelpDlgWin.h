/*********************************************************************
*
* (c) Copyright 2012 - 2016, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxP172RcHelpDlgWin.h,v $
* ID: $Id: SPxP172RcHelpDlgWin.h,v 1.7 2016/03/03 10:59:08 rew Exp $
*
* Purpose:
*	A pop-up dialog used to show help information.
*
* Revision Control:
*   03/03/16 v1.7    AGC	Fix clang warnings.
*
* Previous Changes:
*   26/01/15 1.6    AGC	Remove unused HICON.
*   01/09/14 1.5    AGC	VS2015 format string changes.
*   26/11/13 1.4    AGC	Remove unused m_parentWin variable.
*   06/06/13 1.3    SP 	Add ReplaceCtrlChars().
*   26/10/12 1.2    SP 	Further development.
*   18/10/12 1.1    SP 	Initial version.
*
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/*
 * Constants.
 */

/* Prefix for all help parameters. */
#define SPX_P172_HELP_PARAM_PREFIX "P172RcHelp"

/*
 * SPxP172RcHelpDlgWin dialog.
 */
class SPxP172RcHelpDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxP172RcHelpDlgWin)

public:
    /* Constructor and destructor. */
    SPxP172RcHelpDlgWin(void);
    virtual ~SPxP172RcHelpDlgWin();
    virtual BOOL Create(CWnd* pParent);
    virtual BOOL IsHelpAvailable(const char *itemName);
    virtual void ShowHelp(const char *itemName);

    /* Dialog Data */
    enum { IDD = IDD_SPX_P172_RC_HELP_DIALOG };

protected:
    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    virtual afx_msg void OnSize(UINT nType, int cWidth, int cHeight);
    virtual afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

private:

    /*
     * Private functions. 
     */

    void ReplaceCtrlChars(char *text);
    void OpenURL(const char *url);

    /* GUI callback functions */
    afx_msg void OnBnClickedWebButton();

    using CDialog::Create;

    /*
     * Private vatiables. 
     */

    /* GUI objects. */
    CWnd *m_textWin;
    
    /* Minimum dialog size. */
    unsigned int m_minWidth;
    unsigned int m_minHeight;

    /* URL for on-line help. */
    CStringA m_url;

}; /* class SPxP172RcHelpDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
