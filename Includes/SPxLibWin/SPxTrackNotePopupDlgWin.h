/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for popup version of track note logging dialog.
*
*
* Revision Control:
*   17/10/18 v1.2    BTB	Fix comment.
*
* Previous Changes:
*   12/10/18 1.1    BTB	Initial version.
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxTrackNoteDlgWin.h"

/*
 * SPxTrackNotePopupDlgWin dialog
 */
class SPxTrackNotePopupDlgWin : public SPxTrackNoteDlgWin
{
public:
    /* Constructor and destructor. */
    SPxTrackNotePopupDlgWin(SPxTrackNoteDb *trackNoteDb, CWnd* parentWin);
    virtual ~SPxTrackNotePopupDlgWin();

    /* General. */
    virtual SPxErrorCode SetTrack(SPxUniTrack *track);

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnOK(void) {};
    virtual void OnCancel(void);

private:

}; /* class SPxTrackNotePopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
