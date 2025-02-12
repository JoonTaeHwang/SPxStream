/*********************************************************************
*
* (c) Copyright 2012, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxTestGenCtrlPopupDlgWin.h,v $
* ID: $Id: SPxTestGenCtrlPopupDlgWin.h,v 1.1 2012/10/26 14:25:17 rew Exp $
*
* Purpose:
*	Header for the Test Generator control pop-up dialog.
*
*
* Revision Control:
*   26/10/12 v1.1    AGC	Initial version.
*
* Previous Changes:
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxTestGenCtrlDlgWin.h"

/*
 * SPxTestGenCtrlPopupDlgWin dialog
 */
class SPxTestGenCtrlPopupDlgWin : public SPxTestGenCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    SPxTestGenCtrlPopupDlgWin(CWnd* pParent, 
			      SPxTestGenerator *obj);
    virtual ~SPxTestGenCtrlPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */
   
}; /* class SPxTestGenCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
