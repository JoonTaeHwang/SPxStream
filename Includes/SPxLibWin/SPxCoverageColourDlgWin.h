/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for radar coverage control dialog.
*
*
* Revision Control:
*   26/06/18 v1.2    BTB	Add multi-language support.
*
* Previous Changes:
*   25/01/18 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibWin/SPxWinEditCtrl.h"

#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxTiledMapSrcCoverage;
/*
 * SPxCoverageColourDlgWin dialog
 */
class SPxCoverageColourDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxCoverageColourDlgWin)

public:
    /* Constructor and destructor. */
    SPxCoverageColourDlgWin(CWnd* pParent, SPxTiledMapSrcCoverage *obj);
    virtual ~SPxCoverageColourDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);

    void SetDistUnits(SPxUnitsDist_t unitsDist);
    void SetHeightUnits(SPxUnitsHeight_t unitsHeight);

    /* Configuration functions. */
    SPxTiledMapSrcCoverage *GetObj(void) { return m_obj; }

    /* Dialog Data */
    enum { IDD = IDD_SPX_COVERAGE_COLOUR_CTRL_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual BOOL PreTranslateMessage(MSG *pMsg);
    virtual void OnOK(void) {}
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* The parent window */
    CWnd *m_parentWin;

    /* The object we are controlling */
    SPxTiledMapSrcCoverage *m_obj;

    /* Controls. */
    CButton m_checkEarth;
    CStatic m_lblVisHeights;
    SPxWinEditCtrl m_editVisHeights;
    CButton m_radioHeightSea;
    CButton m_radioHeightGround;
    CSliderCtrl m_sliderAlpha;
    CToolTipCtrl m_toolTip;
    CStatic m_lblHeightAbove;
    CStatic m_lblTransparency;

    /* Other variables. */
    SPxUnitsDist_t m_unitsDist;
    SPxUnitsHeight_t m_unitsHeight;

    /* Private functions. */
    void syncValues(void);
    void OnToggleEarthCurvature(void);
    void OnHeightColourChange(UINT id);
    void OnEditHeights(NMHDR *, LRESULT *);
    void OnRadioHeight(void);
    void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
    double metresToUnits(double metres, int isHeight);
    double unitsToMetres(double units, int isHeight);

    using CDialog::Create;

}; /* class SPxCoverageColourDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
