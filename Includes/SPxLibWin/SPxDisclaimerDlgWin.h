/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for generic SPx disclaimer dialog.
*
*
* Revision Control:
*   15/05/20 v1.1    REW	Initial Version.
*
* Previous Changes:
**********************************************************************/
#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* Forward declarations. */

/* Define our class. */
class SPxDisclaimerDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxDisclaimerDlgWin)

public:
    /* Construction and destruction */
    SPxDisclaimerDlgWin(CWnd* pParentWin, const char *text);
    virtual ~SPxDisclaimerDlgWin(void);
    virtual BOOL Create(void);

    /* Dialog data. */
    enum { IDD = IDD_SPX_DISCLAIMER_DIALOG };

protected:
    /* Protected functions */
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(void);
    void OnSize(UINT nType, int cx, int cy);
    void OnLink(NMHDR *nmhdr, LRESULT *lresult);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */

    using CDialog::Create;

}; /* SPxDisclaimerDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
