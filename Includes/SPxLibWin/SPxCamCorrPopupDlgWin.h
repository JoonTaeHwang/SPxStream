/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for popup version of main camera correction dialog.
*
*
* Revision Control:
*   26/10/18 v1.3    AGC	Support calculating camera height.
*
* Previous Changes:
*   19/10/18 1.2    AGC	Simplify correction point handling.
*   29/08/18 1.1    AGC	Initial version.
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxCamCorrDlgWin.h"

/*
 * SPxCamCorrPopupDlgWin dialog
 */
class SPxCamCorrPopupDlgWin : public SPxCamCorrDlgWin
{
public:
    /* Constructor and destructor. */
    explicit SPxCamCorrPopupDlgWin(CWnd* parentWin,
				   int isCamHeightKnown = TRUE);
    virtual ~SPxCamCorrPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnOK(void) {};
    virtual void OnCancel(void);

private:
    /* Private functions. */
    static void closeDialog(void *userArg);

}; /* class SPxCamCorrPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
