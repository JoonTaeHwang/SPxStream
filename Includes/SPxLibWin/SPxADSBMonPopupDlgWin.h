/*********************************************************************
*
* (c) Copyright 2020, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the ADS-B Monitoring pop-up dialog.
*
* Revision Control:
*   06/10/21 v1.2    AGC	Improve child to popup conversion.
*
* Previous Changes:
*   12/02/21 1.1    JGB Pass Track DB through constructor.
*   28/01/21 1.0    JGB	Initial Version.
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxADSBMonDlgWin.h"

/*
 * SPxADSBMonPopupDlgWin dialog
 */
class SPxADSBMonPopupDlgWin : public SPxADSBMonDlgWin
{
public:
    /* Constructor and destructor. */
    SPxADSBMonPopupDlgWin(CWnd* pParent, SPxADSBDecoder *obj,
			  SPxUniTrackDatabase* db, int showErrors);
    virtual ~SPxADSBMonPopupDlgWin(void);

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);

public:
    /* Nothing at present */
   
}; /* class SPxADSBMonPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
