/*********************************************************************
*
* (c) Copyright 2018 - 2020, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for remote P226 radar control pop-up dialog.
*
*
* Revision Control:
*   20/11/20 v1.3  BTB 	Rename class.
*
* Previous Changes:
*   19/11/20 1.2  BTB 	Make constructor explicit.
*   07/06/18 1.1  BTB 	Initial version.
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxRemoteP226CtrlDlgWin.h"

/*
 * SPxRemoteP226CtrlPopupDlgWin dialog
 */
class SPxRemoteP226CtrlPopupDlgWin : public SPxRemoteP226CtrlDlgWin
{
public:
    /* Constructor and destructor. */
    explicit SPxRemoteP226CtrlPopupDlgWin(SPxRemoteP226Ctrl *p226Conn, CWnd *parentWin = NULL);
    virtual ~SPxRemoteP226CtrlPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */
   
}; /* class SPxRemoteP226CtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
