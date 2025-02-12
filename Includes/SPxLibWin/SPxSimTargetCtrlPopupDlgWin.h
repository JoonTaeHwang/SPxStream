/*********************************************************************
*
* (c) Copyright 2012 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Simulator Target control pop-up dialog.
*
*
* Revision Control:
*   06/10/21 v1.7    AGC	Improve child to popup conversion.
*
* Previous Changes:
*   26/08/20 1.6    AGC	Support P466 identity config.
*   18/07/14 1.5    AGC	Add target visibility display.
*   18/03/13 1.4    AGC	Support fixed test targets.
*   04/03/13 1.3    AGC	Improve display when using track input.
*   23/11/12 1.2    AGC	Add track renderer interaction.
*   26/10/12 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxSimTargetCtrlDlgWin.h"

/*
 * SPxSimTargetCtrlPopupDlgWin dialog
 */
class SPxSimTargetCtrlPopupDlgWin : public SPxSimTargetCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    explicit SPxSimTargetCtrlPopupDlgWin(CWnd* pParent, 
					 SPxRadarSimServer *server,
					 SPxTrackRenderer *renderer=NULL,
					 unsigned int projectCode=0);
    virtual ~SPxSimTargetCtrlPopupDlgWin(void);

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);

public:
    /* Nothing at present */
   
}; /* class SPxSimTargetCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
