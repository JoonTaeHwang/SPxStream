/*********************************************************************
*
* (c) Copyright 2013 - 2014, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the tiled map control pop-up dialog.
*
*
* Revision Control:
*   06/10/21 v1.3    AGC	Improve child to popup conversion.
*
* Previous Changes:
*   15/01/14 1.2    AGC	Support any number of custom sources.
*   04/11/13 1.1    SP 	Initial ersion.
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxTiledMapDlgWin.h"

/*
 * SPxTiledMapPopupDlgWin dialog
 */
class SPxTiledMapPopupDlgWin : public SPxTiledMapDlgWin
{
public:
    /* Constructor and destructor. */
    SPxTiledMapPopupDlgWin(CWnd *parentWin,
                           SPxTiledMapDatabase *tdb,
                           SPxViewControl *viewControl);
    virtual ~SPxTiledMapPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);

public:
    /* Nothing at present */
   
}; /* class SPxTiledMapPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
