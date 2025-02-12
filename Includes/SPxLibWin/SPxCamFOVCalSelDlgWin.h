/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for camera FOV calibration selection dialog.
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

/* Forward declarations. */
class SPxCamera;
class SPxCamFOVCalDlgWin;

/*
 * SPxCamFOVCalSelDlgWin dialog
 */
class SPxCamFOVCalSelDlgWin : public CDialog
{
public:
    /* Constructor and destructor. */
    SPxCamFOVCalSelDlgWin(SPxCamFOVCalDlgWin* parentWin);
    virtual ~SPxCamFOVCalSelDlgWin(void);
    virtual BOOL Create(void);

    void AddCamera(SPxCamera *camera);
    void ClearCameras(void);
    SPxCamera *GetSelectedCamera(void) { return m_selectedCamera; }
    int IsPrimaryController(void) { return m_primaryController; }
    double GetPanPosDegs(void) { return m_panPosDegs; }

    /* Dialog Data */
    enum { IDD = IDD_SPX_CAM_FOV_CALIBRATION_SELECTION_DIALOG };

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
    /* Current camera pan position. */
    double m_panPosDegs;

    /* Controls. */
    CStatic m_cameraLabel;
    CComboBox m_cameraCombo;
    CStatic m_panPosLabel;
    CStatic m_panPos;
    CButton m_primaryRadio;
    CButton m_secondaryRadio;

    void resetCameraCombo(void);
    void syncGUI(void);
    void syncGUIFromTimer(void);
    void updateRadioButtons(void);

    /* GUI callback functions. */
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnCbnSelchangeCameraCombo(void);
    afx_msg void OnBnClickedPrimaryRadio(void);
    afx_msg void OnBnClickedSecondaryRadio(void);

    using CDialog::Create;

};

/*********************************************************************
*
* End of file
*
**********************************************************************/
