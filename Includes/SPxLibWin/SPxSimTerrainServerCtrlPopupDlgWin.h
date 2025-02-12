/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Terrain Database server control pop-up dialog.
*
*
* Revision Control:
*   04/08/20 v1.1    AGC	Initial version.
*
* Previous Changes:
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxSimTerrainServerCtrlDlgWin.h"

/*
 * SPxSimTerrainServerCtrlPopupDlgWin dialog
 */
class SPxSimTerrainServerCtrlPopupDlgWin : public SPxSimTerrainServerCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    SPxSimTerrainServerCtrlPopupDlgWin(CWnd* pParent,
				       SPxTerrainDatabase *tdb);
    virtual ~SPxSimTerrainServerCtrlPopupDlgWin(void);

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */

}; /* class SPxSimTerrainServerCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
