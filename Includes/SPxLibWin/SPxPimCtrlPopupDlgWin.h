/*********************************************************************
*
* (c) Copyright 2015, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxPimCtrlPopupDlgWin.h,v $
* ID: $Id: SPxPimCtrlPopupDlgWin.h,v 1.1 2015/11/06 13:48:24 rew Exp $
*
* Purpose:
*	Header for the PIM control pop-up dialog.
*
*
* Revision Control:
*   06/11/15 v1.1    REW	Initial Version.
*
* Previous Changes:
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxPimCtrlDlgWin.h"

/*
 * SPxPimCtrlPopupDlgWin dialog
 */
class SPxPimCtrlPopupDlgWin : public SPxPimCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    SPxPimCtrlPopupDlgWin(CWnd* pParent, 
                          SPxPIM *obj);
    virtual ~SPxPimCtrlPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */
   
}; /* class SPxPimCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
