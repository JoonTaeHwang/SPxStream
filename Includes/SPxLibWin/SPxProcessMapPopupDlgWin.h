/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the SPxProcessMap control pop-up dialog.
*
*
* Revision Control:
*   07/02/18 v1.1    REW	Initial Version.
*
* Previous Changes:
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxProcessMapDlgWin.h"

/*
 * SPxProcessMapPopupDlgWin dialog
 */
class SPxProcessMapPopupDlgWin : public SPxProcessMapDlgWin
{
public:
    /* Constructor and destructor. */
    SPxProcessMapPopupDlgWin(CWnd *pParent);
    virtual ~SPxProcessMapPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);
}; /* class SPxProcessMapPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
