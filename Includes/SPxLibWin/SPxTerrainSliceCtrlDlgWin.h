/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the terrain slice child dialog.
*
*
* Revision Control:
*   24/12/21 v1.3    AGC	Fix Create() override warning.
*
* Previous Changes:
*   23/12/21 1.2    AGC	Add full beam angle support.
*   23/12/21 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */
#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxError.h"

/* Include the SPx resource header */
#include "SPxResource.h"

/* Forward declarations. */
class SPxTerrainDatabase;
class SPxBuildingDatabase;
struct SPxLatLong_tag;
class SPxBitmapWin;

/*
 * SPxTerrainSliceCtrlDlgWin dialog
 */
class SPxTerrainSliceCtrlDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxTerrainSliceCtrlDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxTerrainSliceCtrlDlgWin(CWnd *parent,
				       SPxTerrainDatabase *terrainDB,
				       SPxBuildingDatabase *buildingDB=NULL);
    virtual ~SPxTerrainSliceCtrlDlgWin(void);
    virtual BOOL Create(void);

    SPxErrorCode SetUseEarthCurvature(int useEarthCurvature);

    SPxErrorCode SetRadarName(const char *radarName);
    SPxErrorCode SetRadarPos(const SPxLatLong_tag *latLong, double heightMetres=0.0);
    SPxErrorCode SetRadarStartRange(double startRangeMetres);
    SPxErrorCode SetRadarEndRange(double endRangeMetres);
    SPxErrorCode SetRadarBeamAngles(double minBeamAngle, double maxBeamAngle,
				    double fullMinBeamAngle, double fullMaxBeamAngle);

    SPxErrorCode SetShowTarget(int showTarget);
    SPxErrorCode SetTargetName(const char *targetName);
    SPxErrorCode SetTargetPos(const SPxLatLong_tag *latLong, double heightMetres=0.0);

    /* Dialog Data */
    enum { IDD = IDD_SPX_TERRAIN_SLICE_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(void);
    BOOL OnEraseBkgnd(CDC *pDC);
    void OnTimer(UINT_PTR nIDEvent);
    void OnShowWindow(BOOL bShow, UINT nStatus);
    void OnPaint(void);
    virtual void OnOK(void) {}
    virtual void OnCancel(void) {}
    DECLARE_MESSAGE_MAP()

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    void draw(void);

    using CDialog::Create;

}; /* class SPxTerrainSliceCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
