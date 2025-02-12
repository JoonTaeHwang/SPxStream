/*********************************************************************
*
* (c) Copyright 2020, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the info DB item editing pop-up dialog.
*
*
* Revision Control:
*   02/09/22 v1.3    AGC	Supress cppcheck duplicate inherited member warning.
*
* Previous Changes:
*   07/07/20 1.2    BTB	Move OnOK() and OnCancel() to base class.
*   29/04/20 1.1    BTB	Initial Version.
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxInfoDBItemEditDlgWin.h"

/*
 * SPxInfoDBItemEditPopupDlgWin dialog
 */
class SPxInfoDBItemEditPopupDlgWin : public SPxInfoDBItemEditDlgWin
{
public:
    /* Constructor and destructor. */
    SPxInfoDBItemEditPopupDlgWin(SPxInfoDBNode *dbNode,
                                 const char *itemType,
                                 CWnd *parentWin = NULL);
    virtual ~SPxInfoDBItemEditPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);

private:
    struct impl;
    // cppcheck-suppress duplInheritedMember
    SPxAutoPtr<impl> m_p;

}; /* class SPxInfoDBItemEditPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
