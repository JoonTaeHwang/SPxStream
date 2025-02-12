/*********************************************************************
*
* (c) Copyright 2012 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Simulated Radar control pop-up dialog.
*
*
* Revision Control:
*   23/01/18 v1.5    AGC	Support multiple objects.
*
* Previous Changes:
*   31/01/17 1.4    AGC	Support multiple platforms.
*   11/12/14 1.3    AGC	Allow any SPxScDestDisplay.
*   20/06/14 1.2    AGC	Clear display when radar disabled.
*   26/10/12 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxSimRadarCtrlDlgWin.h"

/*
 * SPxSimRadarCtrlPopupDlgWin dialog
 */
class SPxSimRadarCtrlPopupDlgWin : public SPxSimRadarCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    SPxSimRadarCtrlPopupDlgWin(CWnd* pParent,
			       unsigned int numObjects,
			       SPxSimRadar *obj[],
			       SPxScSourceLocal *sc[],
			       SPxScDestDisplay *dest[]);
    virtual ~SPxSimRadarCtrlPopupDlgWin(void);

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */
   
}; /* class SPxSimRadarCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
