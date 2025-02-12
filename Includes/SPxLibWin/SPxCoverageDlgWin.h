/*********************************************************************
*
* (c) Copyright 2016, 2018, 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for radar coverage control dialog.
*
*
* Revision Control:
*   12/06/19 v1.11   AGC	Improve units handling.
*
* Previous Changes:
*   11/06/19 1.10   AGC	Add radar index to units change handler.
*   18/04/18 1.9    AGC	Show enable checkbox when required.
*   02/02/18 1.8    AGC	Improve multi-radar support.
*   16/09/16 1.7    AGC	Add tooltip support.
*   02/09/16 1.6    AGC	Improve traditional distance units.
*   28/07/16 1.5    AGC	Remove num samples/azimuths controls.
*			Use slider to control alpha.
*   14/07/16 1.4    AGC	Support alpha.
*			Renderer/database changes moved to application.
*   08/07/16 1.3    AGC	Continue development.
*   16/06/16 1.2    AGC	Add missing forward declaration.
*   15/06/16 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibUtils/SPxEventHandler.h"

#include "SPxLibWin/SPxTableDlgWin.h"

/* Forward declarations. */
class SPxTiledMapSrcCoverage;
class SPxCoveragePosDlgWin;
class SPxCoverageRadarDlgWin;
class SPxCoverageColourDlgWin;

/*
 * SPxCoverageDlgWin dialog
 */
class SPxCoverageDlgWin : public SPxTableDlgWin
{
    DECLARE_DYNAMIC(SPxCoverageDlgWin)

public:
    typedef void (*UnitsChanged)(SPxFormatLatLong_t latLongFormat,
				 SPxUnitsDist_t distUnits,
				 SPxUnitsHeight_t heightUnits,
				 unsigned int radarIndex);

    /* Constructor and destructor. */
    SPxCoverageDlgWin(CWnd* pParent, SPxTiledMapSrcCoverage *obj,
		      unsigned int radarIndex=0,
		      int showPosCtrls=TRUE);
    virtual ~SPxCoverageDlgWin(void);
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
    virtual int ShowCheck(void) { return TRUE; }
    virtual void Enable(int enable);
    virtual int GetEnable(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_COVERAGE_CTRL_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
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

    /* Child dialogs. */
    SPxAutoPtr<SPxCoveragePosDlgWin> m_posDlg;
    SPxAutoPtr<SPxCoverageRadarDlgWin> m_radarDlg;

    /* Private functions. */
    void handleUnitsChange(SPxFormatLatLong_t latLongFormat,
			   SPxUnitsDist_t distUnits,
			   SPxUnitsHeight_t heightUnits);

    using CDialog::Create;

}; /* class SPxCoverageDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
