/*********************************************************************
*
* (c) Copyright 2015, 2016, 2019 Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the text entry pop-up dialog.
*
*
* Revision Control:
*   09/05/19 v1.4    REW	Reorder headers to fix dependencies.
*
* Previous Changes:
*   09/09/16 1.3    AGC	Support masked password entry.
*   23/12/15 1.2    REW	Add optional dlgPos argument.
*   30/11/15 1.1    REW	Initial Version.
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxTextEntryDlgWin.h"

/* We need the SPxDialogPosition definitions. */
#include "SPxLibWin/SPxWinMfcUtils.h"

/*
 * SPxTextEntryPopupDlgWin dialog
 */
class SPxTextEntryPopupDlgWin : public SPxTextEntryDlgWin
{
public:
    /* Constructor and destructor. */
    explicit SPxTextEntryPopupDlgWin(CWnd* pParent = NULL,
				     int isPassword = FALSE,
				     const char *title = NULL,
				     SPxDialogPosition dlgPos = SPX_DLG_POS_CENTRE_SCREEN);
    virtual ~SPxTextEntryPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

private:
    /* Private variables. */
    char *m_title;			/* Copy of title of window */
    SPxDialogPosition m_dlgPos;		/* Desired position */

}; /* class SPxTextEntryPopupDlgWin */


/*********************************************************************
*
* End of file
*
**********************************************************************/
