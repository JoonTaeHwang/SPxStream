/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for popup version of dialog for controlling and
*	editing camera tours.
*
*
* Revision Control:
*   03/07/20 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/
#pragma once

/* Include the base class header file */
#include "SPxCamTourDlgWin.h"

class SPxCamTourPopupDlgWin : public SPxCamTourDlgWin
{
public:
    /* Constructor and destructor. */
    SPxCamTourPopupDlgWin(void);
    virtual ~SPxCamTourPopupDlgWin(void);

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */

};

/*********************************************************************
*
* End of file
*
**********************************************************************/
