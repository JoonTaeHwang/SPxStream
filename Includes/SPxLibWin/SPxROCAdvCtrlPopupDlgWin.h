/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for the ROC advanced control pop-up dialog.
*
*
* Revision Control:
*   25/02/22 v1.1  AJH	Initial version.
*
* Previous Changes:
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxROCAdvCtrlDlgWin.h"

/*
 * SPxROCAdvCtrlPopupDlgWin dialog
 */
class SPxROCAdvCtrlPopupDlgWin : public SPxROCAdvCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    SPxROCAdvCtrlPopupDlgWin(CWnd* pParent, SPxROC *obj);
    virtual ~SPxROCAdvCtrlPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */
   
}; /* class SPxROCAdvCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
