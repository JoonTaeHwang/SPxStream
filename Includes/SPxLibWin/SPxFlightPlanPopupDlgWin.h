/*********************************************************************
*
* (c) Copyright 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxFlightPlanPopupDlgWin.h,v $
* ID: $Id: SPxFlightPlanPopupDlgWin.h,v 1.1 2014/01/15 09:54:22 rew Exp $
*
* Purpose:
*	Header for the SPxFlightPlan control pop-up dialog.
*
*
* Revision Control:
*   15/01/14 v1.1    REW	Initial Version.
*
* Previous Changes:
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxFlightPlanDlgWin.h"

/*
 * SPxFlightPlanPopupDlgWin dialog
 */
class SPxFlightPlanPopupDlgWin : public SPxFlightPlanDlgWin
{
public:
    /* Constructor and destructor. */
    SPxFlightPlanPopupDlgWin(CWnd *pParent);
    virtual ~SPxFlightPlanPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);
}; /* class SPxFlightPlanPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
