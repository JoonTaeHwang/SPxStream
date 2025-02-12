/*********************************************************************
*
* (c) Copyright 2013, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the SPxROC control pop-up dialog.
*
*
* Revision Control:
*   28/02/22 v1.2    AJH	Support advanced control panel.
*
* Previous Changes:
*   25/07/13 1.1    REW	Initial Version.
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxROCCtrlDlgWin.h"

/*
 * SPxROCCtrlPopupDlgWin dialog
 */
class SPxROCCtrlPopupDlgWin : public SPxROCCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    SPxROCCtrlPopupDlgWin(CWnd* pParent, 
                          SPxROC *obj,
			  BOOL showAdvCtrlBtn=TRUE);
    virtual ~SPxROCCtrlPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */
   
}; /* class SPxROCCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
