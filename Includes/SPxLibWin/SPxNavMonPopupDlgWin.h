/*********************************************************************
*
* (c) Copyright 2018, 2019, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Nav Data Monitoring pop-up dialog.
*
* Revision Control:
*   06/10/21 v1.3    AGC	Improve child to popup conversion.
*
* Previous Changes:
*   01/02/19 1.2    AGC	Fix N/C #661 - add option to show errors.
*   05/12/18 1.1    REW	Initial Version.
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxNavMonDlgWin.h"

/*
 * SPxNavMonPopupDlgWin dialog
 */
class SPxNavMonPopupDlgWin : public SPxNavMonDlgWin
{
public:
    /* Constructor and destructor. */
    SPxNavMonPopupDlgWin(CWnd* pParent, SPxNavData *obj,
			 int showErrors);
    virtual ~SPxNavMonPopupDlgWin(void);

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);

public:
    /* Nothing at present */
   
}; /* class SPxNavMonPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
