/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for radar coverage radar control dialog.
*
*
* Revision Control:
*   12/06/19 v1.2    AGC	Improve units handling.
*
* Previous Changes:
*   25/01/18 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibUtils/SPxEventHandler.h"
#include "SPxLibWin/SPxWinSpinCtrl.h"

#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxTiledMapSrcCoverage;

/*
 * SPxCoverageDlgWin dialog
 */
class SPxCoveragePosDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxCoveragePosDlgWin)

public:
    typedef void (*UnitsChanged)(SPxFormatLatLong_t latLongFormat,
				 SPxUnitsDist_t distUnits,
				 SPxUnitsHeight_t heightUnits);

    /* Constructor and destructor. */
    SPxCoveragePosDlgWin(CWnd* pParent, SPxTiledMapSrcCoverage *obj,
			 unsigned int radarIndex=0);
    virtual ~SPxCoveragePosDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);

    void SetFormatLatLong(SPxFormatLatLong_t latLongFormat);
    SPxFormatLatLong_t GetFormatLatLong(void) const;
    void SetDistUnits(SPxUnitsDist_t unitsDist);
    SPxUnitsDist_t GetDistUnits(void) const;


    template<typename F, typename O>
    SPxErrorCode AddUnitsHandler(F fn, O *obj)
    {
	return m_unitsFn.Add(fn, obj);
    }
    template<typename F, typename O>
    SPxErrorCode RemoveUnitsHandler(F fn, O *obj)
    {
	return m_unitsFn.Remove(fn, obj);
    }

    /* Configuration functions. */
    SPxTiledMapSrcCoverage *GetObj(void) { return m_obj; }

    /* Dialog Data */
    enum { IDD = IDD_SPX_COVERAGE_POS_CTRL_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual BOOL PreTranslateMessage(MSG *pMsg);
    virtual void OnOK(void) {}
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    SPxEventHandler<UnitsChanged> m_unitsFn;

    /* The parent window */
    CWnd *m_parentWin;

    /* The object we are controlling */
    SPxTiledMapSrcCoverage *m_obj;
    unsigned int m_radarIndex;

    /* Controls. */
    CButton m_radioPosDMS;
    CButton m_radioPosDecimal;
    SPxWinSpinCtrl m_spinDegLat;
    SPxWinSpinCtrl m_spinDegLong;
    CStatic m_lblDegLat;
    CStatic m_lblDegLong;
    SPxWinSpinCtrl m_spinMinLat;
    SPxWinSpinCtrl m_spinMinLong;
    CStatic m_lblMinLat;
    CStatic m_lblMinLong;
    SPxWinSpinCtrl m_spinSecLat;
    SPxWinSpinCtrl m_spinSecLong;
    CStatic m_lblSecLat;
    CStatic m_lblSecLong;
    CButton m_radioPosN;
    CButton m_radioPosS;
    CButton m_radioPosE;
    CButton m_radioPosW;
    SPxWinSpinCtrl m_spinDecLat;
    SPxWinSpinCtrl m_spinDecLong;
    CStatic m_lblDecLat;
    CStatic m_lblDecLong;
    CButton m_radioUnitsMetric;
    CButton m_radioUnitsTrad;
    CStatic m_lblHeight;
    SPxWinSpinCtrl m_spinHeight;
    CToolTipCtrl m_toolTip;

    /* Other variables. */
    SPxFormatLatLong_t m_latLongFormat;
    SPxUnitsDist_t m_unitsDist;
    SPxUnitsHeight_t m_unitsHeight;

    /* Private functions. */
    void syncValues(void);
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);
    void OnRadioPos(void);
    void OnRadioPosDMS(void);
    void OnRadioUnits(void);
    void getPosDMS(double *latDegs, double *longDegs);
    double metresToUnits(double metres, int isHeight);
    double unitsToMetres(double units, int isHeight);

    using CDialog::Create;

}; /* class SPxCoveragePosDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
