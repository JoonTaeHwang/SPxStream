/*********************************************************************
*
* (c) Copyright 2011, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxHPxAdvCtrlPopupDlgWin.h,v $
* ID: $Id: SPxHPxAdvCtrlPopupDlgWin.h,v 1.1 2011/05/26 07:56:20 rew Exp $
*
* Purpose:
*	Header for the HPx advanced control pop-up dialog.
*
*
* Revision Control:
*   25/05/11 v1.1  SP	Initial version.
*
* Previous Changes:
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxHPxAdvCtrlDlgWin.h"

/*
 * SPxHPxAdvCtrlPopupDlgWin dialog
 */
class SPxHPxAdvCtrlPopupDlgWin : public SPxHPxAdvCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    SPxHPxAdvCtrlPopupDlgWin(CWnd* pParent, SPxHPx100Source *obj);
    virtual ~SPxHPxAdvCtrlPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */
   
}; /* class SPxHPxAdvCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
