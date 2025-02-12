/*********************************************************************
*
* (c) Copyright 2020-2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the AIS Monitoring pop-up dialog.
*
* Revision Control:
*   06/10/21 v1.3    AGC	Improve child to popup conversion.
*
* Previous Changes:
*   11/02/21 1.2    AJH	Add parameter to constructor.
*   04/09/20 1.1    AJH	Initial Version.
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxAISMonDlgWin.h"

/*
 * SPxAISMonPopupDlgWin dialog
 */
class SPxAISMonPopupDlgWin : public SPxAISMonDlgWin
{
public:
    /* Constructor and destructor. */
    SPxAISMonPopupDlgWin(CWnd* pParent, SPxAISDecoder *obj,
			 SPxUniTrackDatabase *db, int showErrors);
    virtual ~SPxAISMonPopupDlgWin(void);

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);

public:
    /* Nothing at present */
   
}; /* class SPxAISMonPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
