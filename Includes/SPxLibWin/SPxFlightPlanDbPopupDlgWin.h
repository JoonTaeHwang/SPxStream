/*********************************************************************
*
* (c) Copyright 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxFlightPlanDbPopupDlgWin.h,v $
* ID: $Id: SPxFlightPlanDbPopupDlgWin.h,v 1.1 2014/01/15 09:54:22 rew Exp $
*
* Purpose:
*	Header for the SPxFlightPlanDb control pop-up dialog.
*
*
* Revision Control:
*   15/01/14 v1.1    REW	Initial Version.
*
* Previous Changes:
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxFlightPlanDbDlgWin.h"

/*
 * SPxFlightPlanDbPopupDlgWin dialog
 */
class SPxFlightPlanDbPopupDlgWin : public SPxFlightPlanDbDlgWin
{
public:
    /* Constructor and destructor. */
    SPxFlightPlanDbPopupDlgWin(CWnd *pParent, SPxFlightPlanDb *obj);
    virtual ~SPxFlightPlanDbPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);
}; /* class SPxFlightPlanDbPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
