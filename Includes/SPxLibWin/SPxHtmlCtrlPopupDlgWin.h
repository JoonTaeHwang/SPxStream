/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for HTML pop-up dialog for displaying a web page.
*
*
* Revision Control:
*   17/10/18 v1.1  BTB	Initial version.
*
* Previous Changes:
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxHtmlCtrlDlgWin.h"

/*
 * SPxCamCtrlPopupDlgWin dialog
 */
class SPxHtmlCtrlPopupDlgWin : public SPxHtmlCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    SPxHtmlCtrlPopupDlgWin(CWnd* pParent, int scrollable = FALSE);
    virtual ~SPxHtmlCtrlPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */
   
}; /* class SPxHtmlCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
