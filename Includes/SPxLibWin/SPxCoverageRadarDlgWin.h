/*********************************************************************
*
* (c) Copyright 2018, 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for radar coverage control dialog.
*
*
* Revision Control:
*   12/06/19 v1.4    AGC	Improve units handling.
*
* Previous Changes:
*   18/04/18 1.3    AGC	Move checkbox to SPxCoverageDlgWin.
*   13/04/18 1.2    BTB	Support enabling/disabling per radar.
*   29/01/18 1.1    AGC	Initial version.
*
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibWin/SPxWinSpinCtrl.h"

#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxTiledMapSrcCoverage;
/*
 * SPxCoverageRadarDlgWin dialog
 */
class SPxCoverageRadarDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxCoverageRadarDlgWin)

public:
    /* Constructor and destructor. */
    SPxCoverageRadarDlgWin(CWnd* pParent, SPxTiledMapSrcCoverage *obj,
			   unsigned int radarIndex=0);
    virtual ~SPxCoverageRadarDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);

    void SetDistUnits(SPxUnitsDist_t unitsDist);
    SPxUnitsDist_t GetDistUnits(void) const;

    /* Configuration functions. */
    SPxTiledMapSrcCoverage *GetObj(void) { return m_obj; }

    /* Dialog Data */
    enum { IDD = IDD_SPX_COVERAGE_RADAR_CTRL_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual BOOL PreTranslateMessage(MSG *pMsg);
    virtual void OnOK(void) {}
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */

    /* The parent window */
    CWnd *m_parentWin;

    /* The object we are controlling */
    SPxTiledMapSrcCoverage *m_obj;
    unsigned int m_radarIndex;

    /* Controls. */
    CStatic m_lblRange;
    SPxWinSpinCtrl m_spinRange;
    CStatic m_lblBeamMin;
    CStatic m_lblBeamMax;
    SPxWinSpinCtrl m_spinBeamMin;
    SPxWinSpinCtrl m_spinBeamMax;
    CToolTipCtrl m_toolTip;

    /* Other variables. */
    SPxUnitsDist_t m_unitsDist;

    /* Private functions. */
    void syncValues(void);
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);
    void OnCheckEnable(void);
    double metresToUnits(double metres);
    double unitsToMetres(double units);

    using CDialog::Create;

}; /* class SPxCoverageRadarDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
