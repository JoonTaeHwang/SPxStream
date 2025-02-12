/*********************************************************************
*
* (c) Copyright 2012 - 2013, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxP172RcPopupDlgWin.h,v $
* ID: $Id: SPxP172RcPopupDlgWin.h,v 1.2 2013/10/15 14:59:26 rew Exp $
*
* Purpose:
*	Header for the P172 radar control pop-up dialog.
*
*
* Revision Control:
*   15/10/13 v1.2   SP  	Pass stand-alone flag to base dialog.
*
* Previous Changes:
*   18/10/12 1.1   SP  	Initial version.
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxP172RcDlgWin.h"

/*
 * SPxP172RcPopupDlgWin dialog
 */
class SPxP172RcPopupDlgWin : public SPxP172RcDlgWin
{
public:
    /* Constructor and destructor. */
    SPxP172RcPopupDlgWin(BOOL isStandAlone=TRUE);
    virtual ~SPxP172RcPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */
   
}; /* class SPxP172RcPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
