/*********************************************************************
*
* (c) Copyright 2016, 2021 Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the joystick control pop-up dialog.
*
*
* Revision Control:
*   06/10/21 v1.2    AGC	Improve child to popup conversion.
*
* Previous Changes:
*   13/10/16 1.1 AGC	Initial version.
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxJoystickWin.h"

/*
 * SPxAISPopupDlgWin dialog
 */
class SPxJoystickPopupWin : public SPxJoystickWin
{
public:
    /* Constructor and destructor. */
    explicit SPxJoystickPopupWin(BOOL touchMode=FALSE);
    virtual ~SPxJoystickPopupWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);

public:
    /* Nothing at present */
   
}; /* class SPxJoystickPopupWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
