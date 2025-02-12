/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for remote HPx control popup dialog.
*
*
* Revision Control:
*   27/11/20 v1.1    BTB	Initial version.
*
* Previous Changes:
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxRemoteHPxCtrlDlgWin.h"

/*
 * SPxRemoteHPxCtrlPopupDlgWin dialog
 */
class SPxRemoteHPxCtrlPopupDlgWin : public SPxRemoteHPxCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    SPxRemoteHPxCtrlPopupDlgWin(CWnd* pParent, SPxRemoteHPxCtrl *hpxConn);
    virtual ~SPxRemoteHPxCtrlPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */
   
}; /* class SPxRemoteHPxCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
