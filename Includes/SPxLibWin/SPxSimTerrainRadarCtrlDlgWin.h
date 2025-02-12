/*********************************************************************
*
* (c) Copyright 2014 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Terrain Database Radar control child dialog,
*	this dialog is typically a child of SPxSimTerrainCtrlDlgWin.
*
*
* Revision Control:
*   15/11/21 v1.11   AGC	Update for sim terrain changes.
*
* Previous Changes:
*   04/10/21 1.10   AGC	Support configurable sea level.
*   25/06/18 1.9    AGC	Correctly suppress cppcheck warning.
*   23/01/18 1.8    AGC	Support multiple objects.
*   31/01/17 1.7    AGC	Support units configuration.
*   03/03/16 1.6    AGC	Fix clang warnings.
*   26/11/15 1.5    AGC	Move blind sectors to radar dialog.
*   16/11/15 1.4    AGC	Add multiple blind sector support.
*   10/07/15 1.3    AGC	Support blind sectors.
*   02/06/14 1.2    AGC	Move target obscure setting here.
*   27/01/14 1.1    AGC	Initial version.
**********************************************************************/
#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* For base class. */
#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxSimTerrain;
class SPxSimRadar;

/*
 * SPxSimTerrainRadarCtrlDlgWin dialog
 */
class SPxSimTerrainRadarCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxSimTerrainRadarCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxSimTerrainRadarCtrlDlgWin(CWnd* pParent,
				 unsigned int numObjects,
				 SPxSimTerrain *obj[],
				 SPxSimRadar *radar[]);
    virtual ~SPxSimTerrainRadarCtrlDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_TERRAIN_RADAR_CTRL_DIALOG };
    virtual int GetIDD(void) const { return IDD; }
    virtual int ShowCheck(void) { return TRUE; }
    virtual void Enable(int enable);
    virtual int GetEnable(void);

    void SetDisplayUnits(SPxUnitsDist_t unitsDist);

    /* Configuration retrieval functions. */
    SPxSimTerrain *GetObj(void) { return m_obj; }
    // cppcheck-suppress virtualCallInConstructor
    virtual void SetObjIndex(int index);

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* The parent window */
    CWnd *m_parentWin;
    SPxUnitsDist_t m_unitsDist;

    /* The objects we are controlling */
    SPxSimTerrain *m_obj;
    SPxSimRadar *m_radar;

    /* Controls. */
    CComboBox m_comboMode;
    CButton m_checkObscureTerrain;
    CButton m_checkObscureTargets;
    SPxWinSpinCtrl m_spinGain;
    CStatic m_lblCliffRes;
    SPxWinSpinCtrl m_spinCliffRes;
    CStatic m_lblSeaLevel;
    SPxWinSpinCtrl m_spinSeaLevel;


    /*
     * Handler functions.
     */
    void OnSelChangeComboMode(void);
    void OnCheckObscureTerrain(void);
    void OnCheckObscureTargets(void);
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);

    using CDialog::Create;

}; /* class SPxSimTerrainRadarCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
