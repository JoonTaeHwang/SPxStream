/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for general-purpose remote radar control popup dialog.
*
*
* Revision Control:
*   19/11/20 v1.1    BTB	Initial version.
*
* Previous Changes:
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxRemoteRadarCtrlDlgWin.h"

/*
 * SPxRemoteRadarCtrlPopupDlgWin dialog
 */
class SPxRemoteRadarCtrlPopupDlgWin : public SPxRemoteRadarCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    SPxRemoteRadarCtrlPopupDlgWin(CWnd* pParent, SPxRemoteRadarCtrl *radarConn);
    virtual ~SPxRemoteRadarCtrlPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */
   
}; /* class SPxRemoteRadarCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
