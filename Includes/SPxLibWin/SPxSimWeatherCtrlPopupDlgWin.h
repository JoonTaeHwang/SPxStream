/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Simulator Weather control pop-up dialog.
*
*
* Revision Control:
*   06/10/21 v1.2    AGC	Improve child to popup conversion.
*
* Previous Changes:
*   02/03/21 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxSimWeatherCtrlDlgWin.h"

/*
 * SPxSimWeatherCtrlPopupDlgWin dialog
 */
class SPxSimWeatherCtrlPopupDlgWin : public SPxSimWeatherCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    explicit SPxSimWeatherCtrlPopupDlgWin(CWnd* pParent,
					  SPxRadarSimServer *server,
					  SPxWeatherRenderer *renderer=NULL,
					  SPxAreaEditorMouse *editor=NULL);
    virtual ~SPxSimWeatherCtrlPopupDlgWin(void);

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);

public:
    /* Nothing at present */
   
}; /* class SPxSimWeatherCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
