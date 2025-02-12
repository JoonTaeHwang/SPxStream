/*********************************************************************
*
* (c) Copyright 2012 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Terrain Database control child dialog.
*
*
* Revision Control:
*   15/11/21 v1.10   AGC	Update for sim terrain changes.
*
* Previous Changes:
*   03/12/18 1.9    AGC	Fix high DPI layout.
*   31/01/17 1.8    AGC	Support units configuration.
*   03/03/16 1.7    AGC	Fix clang warnings.
*   02/06/14 1.6    AGC	Support renaming tab titles.
*   27/01/14 1.5    AGC	Split terrain dialog into server and radar sections.
*   08/03/13 1.4    AGC	Support use within tabbed dialog.
*   25/01/13 1.3    AGC	Add flags for terrain event handler.
*   09/11/12 1.2    AGC	Add obscure terrain option.
*			Add gain/cliff resolution controls.
*			Interface with SPxSimTerrain object.
*   26/10/12 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* Forward declarations. */
class SPxSimTerrain;
class SPxSimRadar;
class SPxTerrainDatabase;
class SPxTabbedItemName;

/*
 * SPxSimTerrainCtrlDlgWin dialog
 */
class SPxSimTerrainCtrlDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxSimTerrainCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxSimTerrainCtrlDlgWin(CWnd* pParent, 
			    SPxTerrainDatabase *tdb,
			    SPxSimTerrain *simTerrain[],
			    SPxSimRadar *simRadar[],
			    unsigned int numSimTerrain);
    virtual ~SPxSimTerrainCtrlDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);
    void SetTabTitleObjects(SPxTabbedItemName *objects[]);
    
    void SetDisplayUnits(SPxUnitsDist_t unitsDist);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_TERRAIN_CTRL_DIALOG };

protected:
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    void OnSize(UINT nType, int cx, int cy);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* The parent window */
    CWnd *m_parentWin;

    /* Private functions. */
    static void syncFnWrapper(void *arg);
    void syncFn(void);
    void resize(void);

    using CDialog::Create;

}; /* class SPxSimTerrainCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
