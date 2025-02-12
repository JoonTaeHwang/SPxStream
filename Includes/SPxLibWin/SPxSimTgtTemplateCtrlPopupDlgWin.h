/*********************************************************************
*
* (c) Copyright 2014, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Simulator Target Template control pop-up dialog.
*
*
* Revision Control:
*   06/10/21 v1.2    AGC	Improve child to popup conversion.
*
* Previous Changes:
*   16/05/14 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxSimTgtTemplateCtrlDlgWin.h"

/*
 * SPxSimTgtTemplateCtrlPopupDlgWin dialog
 */
class SPxSimTgtTemplateCtrlPopupDlgWin : public SPxSimTgtTemplateCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    SPxSimTgtTemplateCtrlPopupDlgWin(CWnd* pParent, 
				     SPxRadarSimulator *obj);
    virtual ~SPxSimTgtTemplateCtrlPopupDlgWin(void);

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);

public:
    /* Nothing at present */
   
}; /* class SPxSimTgtTemplateCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
