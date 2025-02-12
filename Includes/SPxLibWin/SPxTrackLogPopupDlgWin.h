/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for popup version of track log dialog.
*
*
* Revision Control:
*   17/10/18 v1.1    BTB	Initial version.
*
* Previous Changes:
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxTrackLogDlgWin.h"

/*
 * SPxTrackLogPopupDlgWin dialog
 */
class SPxTrackLogPopupDlgWin : public SPxTrackLogDlgWin
{
public:
    /* Constructor and destructor. */
    SPxTrackLogPopupDlgWin(SPxTrackNoteDb *trackNoteDb, CWnd* parentWin);
    virtual ~SPxTrackLogPopupDlgWin();

    /* General. */
    virtual SPxErrorCode SetTrack(SPxUniTrack *track);

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnOK(void) {};
    virtual void OnCancel(void);

private:

}; /* class SPxTrackLogPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
