/*********************************************************************
*
* (c) Copyright 2012 - 2014, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Terrain Database control pop-up dialog.
*
*
* Revision Control:
*   15/11/21 v1.5    AGC	Update for sim terrain changes.
*
* Previous Changes:
*   06/10/21 1.4    AGC	Improve child to popup conversion.
*   27/01/14 1.3    AGC	Split terrain dialog into server and radar sections.
*   09/11/12 1.2    AGC	Interface with SPxSimTerrain object.
*   26/10/12 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxSimTerrainCtrlDlgWin.h"

/*
 * SPxSimTerrainCtrlPopupDlgWin dialog
 */
class SPxSimTerrainCtrlPopupDlgWin : public SPxSimTerrainCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    SPxSimTerrainCtrlPopupDlgWin(CWnd* pParent, 
				 SPxTerrainDatabase *tdb,
				 SPxSimTerrain *simTerrain[],
				 SPxSimRadar *simRadar[],
				 unsigned int numSimTerrain);
    virtual ~SPxSimTerrainCtrlPopupDlgWin(void);

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);

public:
    /* Nothing at present */
   
}; /* class SPxSimTerrainCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
