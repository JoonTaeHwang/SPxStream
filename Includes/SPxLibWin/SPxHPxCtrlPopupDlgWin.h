/*********************************************************************
*
* (c) Copyright 2011, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxHPxCtrlPopupDlgWin.h,v $
* ID: $Id: SPxHPxCtrlPopupDlgWin.h,v 1.2 2011/08/31 11:34:18 rew Exp $
*
* Purpose:
*	Header for the HPx control pop-up dialog.
*
*
* Revision Control:
*   30/08/11 v1.2  SP	 Add extra contructor args.
*
* Previous Changes:
*   12/05/11 1.1  SP	Initial version.
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxHPxCtrlDlgWin.h"

/*
 * SPxHPxCtrlPopupDlgWin dialog
 */
class SPxHPxCtrlPopupDlgWin : public SPxHPxCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    SPxHPxCtrlPopupDlgWin(CWnd* pParent, 
                          SPxHPx100Source *obj, 
                          BOOL showAdvCtrlBtn=TRUE);
    virtual ~SPxHPxCtrlPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */
   
}; /* class SPxHPxCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
