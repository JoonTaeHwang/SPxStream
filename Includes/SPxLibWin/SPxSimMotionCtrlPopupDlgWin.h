/*********************************************************************
*
* (c) Copyright 2012, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Simulater Motion control pop-up dialog.
*
*
* Revision Control:
*   06/10/21 v1.2    AGC	Improve child to popup conversion.
*
* Previous Changes:
*   26/10/12 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxSimMotionCtrlDlgWin.h"

/*
 * SPxSimMotionCtrlPopupDlgWin dialog
 */
class SPxSimMotionCtrlPopupDlgWin : public SPxSimMotionCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    SPxSimMotionCtrlPopupDlgWin(CWnd* pParent, 
			        SPxRadarSimulator *obj,
				SPxSimMotionRenderer *renderer=NULL);
    virtual ~SPxSimMotionCtrlPopupDlgWin(void);

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);

public:
    /* Nothing at present */
   
}; /* class SPxSimMotionCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
