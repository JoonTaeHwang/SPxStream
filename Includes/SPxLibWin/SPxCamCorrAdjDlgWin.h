/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for camera correction adjustment dialog.
*
*
* Revision Control:
*   19/10/18 v1.4    AGC	Simplify correction point handling.
*
* Previous Changes:
*   04/09/18 1.3    BTB	Change tilt offset from spin to label.
*   24/08/18 1.2    BTB	Change namespace name to avoid future conflicts.
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
 * SPxCamCorrAdjDlgWin dialog
 */
class SPxCamCorrAdjDlgWin : public CDialog
{
public:
    /* Constructor and destructor. */
    SPxCamCorrAdjDlgWin(SPxCamCorrDlgWin* parentWin);
    virtual ~SPxCamCorrAdjDlgWin(void);
    virtual BOOL Create(void);

    void SetPoint(const SPxCamCorrectionNS::CorrectionPoint *point);

    /* Dialog Data */
    enum { IDD = IDD_SPX_CAM_CORRECTION_ADJUST_DIALOG };

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
    SPxCamCorrectionNS::CorrectionPoint m_point;

    /* Controls. */
    CStatic m_tiltLabel;
    SPxWinSpinCtrl m_tiltSpin;
    CStatic m_tiltOffsetLabel;
    CStatic m_tiltOffset;

    /* Private functions. */
    void UpdateControls(void);

    /* GUI callback functions. */
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);

    using CDialog::Create;

};

/*********************************************************************
*
* End of file
*
**********************************************************************/
