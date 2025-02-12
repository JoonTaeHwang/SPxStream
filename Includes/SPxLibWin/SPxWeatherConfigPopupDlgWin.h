/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the weather display configuration popup dialog.
*
*
* Revision Control:
*   16/02/21 v1.1    BTB	Initial version.
*
* Previous Changes:
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxWeatherConfigDlgWin.h"

/*
 * SPxWeatherConfigPopupDlgWin dialog
 */
class SPxWeatherConfigPopupDlgWin : public SPxWeatherConfigDlgWin
{
public:
    /* Constructor and destructor. */
    SPxWeatherConfigPopupDlgWin(SPxWeatherDatabase *database, CWnd *parentWin = NULL);
    virtual ~SPxWeatherConfigPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */
   
}; /* class SPxWeatherConfigPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
