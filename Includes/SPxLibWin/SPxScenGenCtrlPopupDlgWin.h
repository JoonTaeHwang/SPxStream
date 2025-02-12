/*********************************************************************
*
* (c) Copyright 2012, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxScenGenCtrlPopupDlgWin.h,v $
* ID: $Id: SPxScenGenCtrlPopupDlgWin.h,v 1.1 2012/04/02 09:18:23 rew Exp $
*
* Purpose:
*	Header for the Scenario Generator control pop-up dialog.
*
*
* Revision Control:
*   29/03/12 v1.1    AGC	Initial version.
*
* Previous Changes:
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxScenGenCtrlDlgWin.h"

/*
 * SPxScenGenCtrlPopupDlgWin dialog
 */
class SPxScenGenCtrlPopupDlgWin : public SPxScenGenCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    SPxScenGenCtrlPopupDlgWin(CWnd* pParent, 
			      SPxScenarioGenerator *obj,
			      int showNetCtrls=TRUE);
    virtual ~SPxScenGenCtrlPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */
   
}; /* class SPxScenGenCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
