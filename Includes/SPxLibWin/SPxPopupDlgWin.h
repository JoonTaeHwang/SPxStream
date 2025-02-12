/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for resizable popup dialog that wraps a child dialog.
*
* Revision Control:
*   18/10/21 v1.2    AGC	Prevent Enter from closing dialog.
*
* Previous Changes:
*   07/10/21 1.1    AGC	Initial version.
**********************************************************************/
#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* Define our class. */
class SPxPopupDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxPopupDlgWin)

public:

    /* Construction and destruction */
    explicit SPxPopupDlgWin(void);
    virtual ~SPxPopupDlgWin(void);

    void SetChild(CWnd *childWnd);

protected:
    /* Protected functions */
    virtual void OnOK(void) {};
    virtual void OnCancel(void);
    void OnShowWindow(BOOL bShow, UINT nStatus);
    void OnSize(UINT nType, int cx, int cy);
    void OnGetMinMaxInfo(MINMAXINFO *lpMMI);
    void OnSysCommand(UINT nID, LPARAM lParam);
    DECLARE_MESSAGE_MAP()

private:
    CWnd *m_child;

}; /* SPxPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
