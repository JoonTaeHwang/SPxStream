/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the P322 client pop-up dialog.
*
*
* Revision Control:
*   20/07/18 v1.1   AGC 	Initial version.
*
* Previous Changes:
*
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxP322ClientDlgWin.h"

/*
 * SPxP322ClientPopupDlgWin dialog
 */
class SPxP322ClientPopupDlgWin : public SPxP322ClientDlgWin
{
public:
    /* Constructor and destructor. */
    SPxP322ClientPopupDlgWin(CWnd* pParent,
			     SPxNetworkClientP322 *obj);
    virtual ~SPxP322ClientPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

};

/*********************************************************************
*
* End of file
*
**********************************************************************/
