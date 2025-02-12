/*********************************************************************
*
* (c) Copyright 2015, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxP172EmuPopupDlgWin.h,v $
* ID: $Id: SPxP172EmuPopupDlgWin.h,v 1.1 2015/11/26 11:41:32 rew Exp $
*
* Purpose:
*	Header for the P172 radar emulator pop-up dialog.
*
*
* Revision Control:
*   17/11/15 v1.1   AJH 	Initial version.
*
* Previous Changes:
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxP172EmuDlgWin.h"

/*
 * SPxP172EmuPopupDlgWin dialog
 */
class SPxP172EmuPopupDlgWin : public SPxP172EmuDlgWin
{
public:
    /* Constructor and destructor. */
    SPxP172EmuPopupDlgWin(CWnd* pParent,
			  SPxP172Emulator *obj[],
			  unsigned int numRadar,
			  BOOL isStandAlone = TRUE);
    virtual ~SPxP172EmuPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */
   
}; /* class SPxP172EmuPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
