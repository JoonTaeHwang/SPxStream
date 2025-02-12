/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the terrain slice pop-up dialog.
*
* Revision Control:
*   23/12/21 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxTerrainSliceCtrlDlgWin.h"

/*
 * SPxTerrainSliceCtrlPopupDlgWin dialog
 */
class SPxTerrainSliceCtrlPopupDlgWin : public SPxTerrainSliceCtrlDlgWin
{
public:
    /* Constructor and destructor. */
    explicit SPxTerrainSliceCtrlPopupDlgWin(CWnd *parent,
					    SPxTerrainDatabase *terrainDB,
					    SPxBuildingDatabase *buildingDB=NULL);
    virtual ~SPxTerrainSliceCtrlPopupDlgWin(void);

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);

public:
    /* Nothing at present */

}; /* class SPxTerrainSliceCtrlPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
