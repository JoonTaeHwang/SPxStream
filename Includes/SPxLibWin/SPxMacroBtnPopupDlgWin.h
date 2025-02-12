/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for popup version of macro button dialog.
*
*
* Revision Control:
*   03/04/20 v1.1    AGC	Initial version.
*
* Previous Changes:
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxMacroBtnDlgWin.h"

/*
 * SPxMacroBtnPopupDlgWin dialog
 */
class SPxMacroBtnPopupDlgWin : public SPxMacroBtnDlgWin
{
public:
    /* Constructor and destructor. */
    explicit SPxMacroBtnPopupDlgWin(CWnd* parentWin,
                                    const char *nameFormat);
    virtual ~SPxMacroBtnPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnOK(void) {};
    virtual void OnCancel(void);

}; /* class SPxMacroBtnPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
