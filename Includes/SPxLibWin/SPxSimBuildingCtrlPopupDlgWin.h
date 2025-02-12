/*********************************************************************
*
* (c) Copyright 2013, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Simulator Buildings control pop-up dialog.
*
*
* Revision Control:
*   06/10/21 v1.3    AGC	Improve child to popup conversion.
*
* Previous Changes:
*   16/05/14 1.2    AGC	Use SPxAreaEditorMouse.
*   30/07/13 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxSimBuildingCtrlDlgWin.h"

/*
 * SPxSimBuildingCtrlPopupDlgWin dialog
 */
class SPxSimBuildingCtrlPopupDlgWin : public SPxSimBuildingCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    SPxSimBuildingCtrlPopupDlgWin(CWnd* pParent, 
				  SPxBuildingDatabase *obj,
				  SPxAreaRenderer *renderer=NULL,
				  SPxViewControl *viewCtrl=NULL);
    virtual ~SPxSimBuildingCtrlPopupDlgWin(void);

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);

public:
    /* Nothing at present */
   
}; /* class SPxSimBuildingCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
