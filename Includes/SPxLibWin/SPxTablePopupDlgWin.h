/*********************************************************************
*
* (c) Copyright 2013 - 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxTablePopupDlgWin.h,v $
* ID: $Id: SPxTablePopupDlgWin.h,v 1.1 2015/11/26 11:06:18 rew Exp $
*
* Purpose:
*	Header for a re-usable popup dialog that lays out child 
*	dialogs in a table.
*
*
* Revision Control:
*   26/11/15 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/
#pragma once

/* Include the base class header file */
#include "SPxTableDlgWin.h"

/*
 * SPxTablePopupDlgWin dialog
 */
class SPxTablePopupDlgWin : public SPxTableDlgWin
{
public:
    /* Constructor and destructor. */
    explicit SPxTablePopupDlgWin(CWnd* pParent);
    virtual ~SPxTablePopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */
   
}; /* class SPxTablePopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
