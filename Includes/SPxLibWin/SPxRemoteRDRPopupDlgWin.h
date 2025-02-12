/*********************************************************************
*
* (c) Copyright 2018 - 2020, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for remote RDR control pop-up dialog.
*
*
* Revision Control:
*   20/11/20 v1.2    BTB	Rename class.
*
* Previous Changes:
*   07/06/18 1.1    BTB	Initial version.
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxRemoteRDRDlgWin.h"

/*
 * SPxRemoteRDRPopupDlgWin dialog
 */
class SPxRemoteRDRPopupDlgWin : public SPxRemoteRDRDlgWin
{
public:
    /* Constructor and destructor. */
    SPxRemoteRDRPopupDlgWin(SPxRemoteRDR *rdrConn, CWnd *parentWin = NULL);
    virtual ~SPxRemoteRDRPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */
   
}; /* class SPxRemoteRDRPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
