/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the RF effector control pop-up dialog.
*
*
* Revision Control:
*   31/05/08 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxRFEffectorDlgWin.h"

/*
 * SPxRFEffectorPopupDlgWin dialog
 */
class SPxRFEffectorPopupDlgWin : public SPxRFEffectorDlgWin
{
public:
    /* Constructor and destructor. */
    SPxRFEffectorPopupDlgWin(CWnd* pParent,
			     SPxRFJammer *obj);
    virtual ~SPxRFEffectorPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */
   
}; /* class SPxRFEffectorPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
