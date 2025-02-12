/*********************************************************************
*
* (c) Copyright 2013, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Simulator Target Properties control pop-up dialog.
*
*
* Revision Control:
*   06/10/21 v1.3    AGC	Improve child to popup conversion.
*
* Previous Changes:
*   26/08/20 1.2    AGC	Support P466 identity config.
*   28/03/13 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxSimTargetPropCtrlDlgWin.h"

/*
 * SPxSimTargetPropCtrlPopupDlgWin dialog
 */
class SPxSimTargetPropCtrlPopupDlgWin : public SPxSimTargetPropCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    explicit SPxSimTargetPropCtrlPopupDlgWin(CWnd* pParent, 
					     SPxSimTarget *obj,
					     unsigned int projectCode);
    virtual ~SPxSimTargetPropCtrlPopupDlgWin(void);

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);

public:
    /* Nothing at present */
   
}; /* class SPxSimTargetPropCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
