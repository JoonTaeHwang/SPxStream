/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for popup version of main camera FOV calibration dialog.
*
*
* Revision Control:
*   27/08/19 v1.1    BRB	Initial version.
*
* Previous Changes:
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxCamFOVCalDlgWin.h"

/*
 * SPxCamFOVCalPopupDlgWin dialog
 */
class SPxCamFOVCalPopupDlgWin : public SPxCamFOVCalDlgWin
{
public:
    /* Constructor and destructor. */
    explicit SPxCamFOVCalPopupDlgWin(CWnd* parentWin,
				     int isCamHeightKnown = TRUE);
    virtual ~SPxCamFOVCalPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnOK(void) {};
    virtual void OnCancel(void);

private:
    /* Private functions. */
    static void closeDialog(void *userArg);

}; /* class SPxCamFOVCalPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/