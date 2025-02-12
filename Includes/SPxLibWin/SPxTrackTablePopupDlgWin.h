/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the reusable track table pop-up dialog.
*
* Revision Control:
*   05/08/20 v1.5    SP 	Support unified track database.
*
* Previous Changes:
*   30/04/20 1.4    AGC	Set num columns based on parameters.
*   03/04/20 1.3    AGC Support multiple columns.
*   24/03/20 1.2    AGC Convert to reusable library dialog.
*   23/03/20 1.1    AGC	Initial version.
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxTrackTableDlgWin.h"

/* Forward declarations. */
class SPxThreatDatabase;
class SPxUniTrackDatabase;

/*
 * SPxTrackTablePopupDlgWin dialog
 */
class SPxTrackTablePopupDlgWin : public SPxTrackTableDlgWin
{
public:
    /* Constructor and destructor. */
    explicit SPxTrackTablePopupDlgWin(SPxThreatDatabase *threatDB);
    explicit SPxTrackTablePopupDlgWin(SPxUniTrackDatabase *uniTrackDB);
    virtual ~SPxTrackTablePopupDlgWin(void);

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */

}; /* class SPxTrackTablePopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
