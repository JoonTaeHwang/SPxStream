/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for camera correction point selection dialog.
*
*
* Revision Control:
*   26/10/18 v1.6    AGC	Further simplification.
*
* Previous Changes:
*   19/10/18 1.5    AGC	Simplify correction point handling.
*   29/08/18 1.4    BTB	Remove unnecessary member variable.
*   24/08/18 1.3    BTB	Change namespace name to avoid future conflicts.
*   23/08/18 1.2    BTB	Allow setting of points based on horizon.
*   17/08/18 1.1    BTB	Initial version.
*
**********************************************************************/

#pragma once

/* Standard headers. */

/* Library headers. */
#include "SPxLibUtils/SPxCamCorrDefs.h"
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* Include the SPx resource header */
#include "SPxResource.h"

#define SPX_AFXWIN

/* Forward declarations. */
class SPxCamCorrDlgWin;

/*
 * SPxCamCorrPointDlgWin dialog
 */
class SPxCamCorrPointDlgWin : public CDialog
{
public:
    /* Constructor and destructor. */
    SPxCamCorrPointDlgWin(SPxCamCorrDlgWin* parentWin);
    virtual ~SPxCamCorrPointDlgWin(void);
    virtual BOOL Create(void);

    void SetPoint(SPxCamCorrectionNS::CorrectionPoint *point,
		  int fixBearingAndHeight);

    /* Dialog Data */
    enum { IDD = IDD_SPX_CAM_CORRECTION_POINT_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ }
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ }
    DECLARE_MESSAGE_MAP()

private:
    /* The parent window. */
    SPxCamCorrDlgWin *m_parentWin;

    /* Correction point being set */
    int m_isBearingAndHeightFixed;
    SPxCamCorrectionNS::CorrectionPoint m_point;

    /* Controls. */
    CButton m_horizonModeBn;
    CStatic m_latLabel;
    SPxWinSpinCtrl m_latSpin;
    CStatic m_longLabel;
    SPxWinSpinCtrl m_longSpin;
    CStatic m_heightLabel;
    SPxWinSpinCtrl m_heightSpin;
    CStatic m_rangeLabel;
    SPxWinSpinCtrl m_rangeSpin;
    CStatic m_bearingLabel;
    SPxWinSpinCtrl m_bearingSpin;

    /* Private functions. */
    void UpdateControls(void);

    /* GUI callback functions. */
    void OnHorizonMode(void);
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);

    using CDialog::Create;

};

/*********************************************************************
*
* End of file
*
**********************************************************************/
