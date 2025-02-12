/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for popup version of track filters dialog.
*
*
* Revision Control:
*   18/09/20 v1.1    AGC	Initial version.
*
* Previous Changes:
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxTrackFiltersDlgWin.h"

/*
 * SPxTrackFiltersPopupDlgWin dialog
 */
class SPxTrackFiltersPopupDlgWin : public SPxTrackFiltersDlgWin
{
public:
    /* Constructor and destructor. */
    explicit SPxTrackFiltersPopupDlgWin(SPxUniTrackDatabase *trackDB,
                                        SPxTrackGroupEditPopupDlgWin *trackGroupEditDlg);
    virtual ~SPxTrackFiltersPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnOK(void) {};
    virtual void OnCancel(void);

}; /* class SPxTrackFiltersPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
