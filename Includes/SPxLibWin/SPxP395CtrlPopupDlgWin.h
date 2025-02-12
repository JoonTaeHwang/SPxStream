/*********************************************************************
 *
 * (c) Copyright 2019, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for the P395 pop-up radar control dialog.
 *
 * Revision Control:
 *   11/06/19 v1.1   SP 	Initial version.
 *
 * Previous Changes:
 *
 **********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxLibWin/SPxP395CtrlDlgWin.h"

/*
 * SPxP395CtrlPopupDlgWin dialog
 */
class SPxP395CtrlPopupDlgWin : public SPxP395CtrlDlgWin
{
public:
    /* Constructor and destructor. */
    SPxP395CtrlPopupDlgWin(CWnd* pParent, BOOL isStandAlone=FALSE);
    virtual ~SPxP395CtrlPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);
};

/*********************************************************************
*
* End of file
*
**********************************************************************/
