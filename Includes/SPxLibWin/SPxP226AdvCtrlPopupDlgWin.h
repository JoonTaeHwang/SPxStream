/*********************************************************************
*
* (c) Copyright 2015, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxP226AdvCtrlPopupDlgWin.h,v $
* ID: $Id: SPxP226AdvCtrlPopupDlgWin.h,v 1.1 2015/06/08 08:12:23 rew Exp $
*
* Purpose:
*	Header for the P226 advanced control pop-up dialog.
*
*
* Revision Control:
*   05/06/15 v1.1  SP 	Initial version.
*
* Previous Changes:
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxP226AdvCtrlDlgWin.h"

/*
 * SPxP226AdvCtrlPopupDlgWin dialog
 */
class SPxP226AdvCtrlPopupDlgWin : public SPxP226AdvCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    SPxP226AdvCtrlPopupDlgWin(CWnd* pParent, SPxNetworkReceiveP226 *obj);
    virtual ~SPxP226AdvCtrlPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */
   
}; /* class SPxP226AdvCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
