/*********************************************************************
*
* (c) Copyright 2012, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxSrcFileCtrlPopupDlgWin.h,v $
* ID: $Id: SPxSrcFileCtrlPopupDlgWin.h,v 1.1 2012/10/26 13:53:28 rew Exp $
*
* Purpose:
*	Header for the Radar Replay control pop-up dialog.
*
*
* Revision Control:
*   26/10/12 v1.1    AGC	Initial version.
*
* Previous Changes:
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxSrcFileCtrlDlgWin.h"

/*
 * SPxSrcFileCtrlPopupDlgWin dialog
 */
class SPxSrcFileCtrlPopupDlgWin : public SPxSrcFileCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    SPxSrcFileCtrlPopupDlgWin(CWnd* pParent, 
			      SPxRadarReplay *obj);
    virtual ~SPxSrcFileCtrlPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */
   
}; /* class SPxSrcFileCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
