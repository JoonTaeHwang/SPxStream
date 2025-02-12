/*********************************************************************
*
* (c) Copyright 2015, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Simulated Target Name control child dialog.
*
*
* Revision Control:
*   23/12/21 v1.3    AGC	Derive from re-usable class.
*
* Previous Changes:
*   06/08/15 1.2    AGC	Only show terrain/buildings when enabled.
*   31/07/15 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */
#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibWin/SPxTerrainSliceCtrlDlgWin.h"

/* Include the SPx resource header */
#include "SPxResource.h"

/* Forward declarations. */
class SPxRadarSimulator;
class SPxBitmapWin;

/*
 * SPxSimTargetTerrainCtrlDlgWin dialog
 */
class SPxSimTargetTerrainCtrlDlgWin : public SPxTerrainSliceCtrlDlgWin
{
    DECLARE_DYNAMIC(SPxSimTargetTerrainCtrlDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxSimTargetTerrainCtrlDlgWin(CWnd *parent,
					   SPxRadarSimulator *radarSim);
    virtual ~SPxSimTargetTerrainCtrlDlgWin(void);

    void SetObj(SPxSimTarget *target);
    SPxSimTarget *GetObj(void) const;

    void SetRadarIndex(unsigned int radarIndex);
    unsigned int GetRadarIndex(void) const;

protected:
    virtual BOOL OnInitDialog(void);
    void OnTimer(UINT_PTR nIDEvent);
    DECLARE_MESSAGE_MAP()

private:
    SPxRadarSimulator *m_radarSim;
    SPxSimTarget *m_target;
    unsigned int m_radarIndex;

    void update(void);

}; /* class SPxSimTargetTerrainCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
