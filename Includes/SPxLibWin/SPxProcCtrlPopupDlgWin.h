/*********************************************************************
*
* (c) Copyright 2015, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the process control pop-up dialog.
*
*
* Revision Control:
*   06/10/21 v1.2    AGC	Improve child to popup conversion.
*
* Previous Changes:
*   06/03/15 1.1    AGC	Initial version.
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxProcCtrlDlgWin.h"

/* Forward declarations. */
class SPxProcParamIface;

/*
 * SPxProcCtrlPopupDlgWin dialog
 */
class SPxProcCtrlPopupDlgWin : public SPxProcCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    SPxProcCtrlPopupDlgWin(CWnd* pParent, 
			   SPxProcParamIface *obj,
			   const char *title=NULL);
    virtual ~SPxProcCtrlPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);

}; /* class SPxProcCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
