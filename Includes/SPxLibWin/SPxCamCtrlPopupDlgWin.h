/*********************************************************************
*
* (c) Copyright 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxCamCtrlPopupDlgWin.h,v $
* ID: $Id: SPxCamCtrlPopupDlgWin.h,v 1.1 2014/02/17 17:30:45 rew Exp $
*
* Purpose:
*	Header for the Camera control pop-up dialog.
*
*
* Revision Control:
*   17/02/14 v1.1  SP	Initial version.
*
* Previous Changes:
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxCamCtrlDlgWin.h"

/*
 * SPxCamCtrlPopupDlgWin dialog
 */
class SPxCamCtrlPopupDlgWin : public SPxCamCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    SPxCamCtrlPopupDlgWin(CWnd* pParent, SPxCamera *camera,
			  SPxCamCtrlAdvDlgWin::CreateCamCtrl createFn=NULL,
			  void *userArg=NULL);
    virtual ~SPxCamCtrlPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */
   
}; /* class SPxCamCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
