/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for camera FOV calibration stages dialog.
*
*
* Revision Control:
*   07/08/19 v1.1   BTB	Initial version.
*
* Previous Changes:
*
**********************************************************************/

#pragma once

/* Standard headers. */

/* Library headers. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* Include the SPx resource header */
#include "SPxResource.h"

#define SPX_AFXWIN

#include "SPxLibWin/SPxWinSpinCtrl.h"

/* Forward declarations. */
class SPxCamera;
class SPxCamFOVCalDlgWin;

/*
 * SPxCamFOVCalStgsDlgWin dialog
 */
class SPxCamFOVCalStgsDlgWin : public CDialog
{
public:
    /* Constructor and destructor. */
    SPxCamFOVCalStgsDlgWin(SPxCamFOVCalDlgWin* parentWin);
    virtual ~SPxCamFOVCalStgsDlgWin(void);
    virtual BOOL Create(void);

    void SetSelectedCamera(SPxCamera *camera,
			   int primaryController,
			   double centrePanPosDegs);

    /* Dialog Data */
    enum { IDD = IDD_SPX_CAM_FOV_CALIBRATION_STAGES_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ }
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ }
    DECLARE_MESSAGE_MAP()

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* The parent window. */
    SPxCamFOVCalDlgWin *m_parentWin;

    /* Currently selected camera. */
    SPxCamera *m_selectedCamera;
    /* Primary/secondary controller. */
    int m_primaryController;
    /* Pan position of the camera centred on chosen object. */
    double m_centrePanPosDegs;
    /* Current zoom position and calculated FOV. */
    UINT16 m_currCamZoomPos;
    double m_currCamFOVDegs;

    /* Controls. */
    CStatic m_fovLabel;
    SPxWinSpinCtrl m_fovSpin;
    CStatic m_fovUnitsLabel;
    CButton m_storeBn;
    CStatic m_fovsStoredLabel;

    void syncGUI(void);
    void syncGUIFromTimer(void);

    /* GUI callback functions. */
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);
    afx_msg void OnBnClickedStoreButton(void);

    using CDialog::Create;

};

/*********************************************************************
*
* End of file
*
**********************************************************************/
