/*********************************************************************
*
* (c) Copyright 2018 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for camera correction camera selection dialog.
*
*
* Revision Control:
*   07/08/19 v1.5    BTB	Add missing forward declaration.
*
* Previous Changes:
*   02/11/18 1.4    AGC	Remove unused camera index.
*   08/10/18 1.3    AGC	Update camera combo from external changes.
*   05/10/18 1.2    AGC	Add SetSelectedCamera().
*   17/08/18 1.1    BTB	Initial version.
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
class SPxCamCorrDlgWin;

/*
 * SPxCamCorrSelDlgWin dialog
 */
class SPxCamCorrSelDlgWin : public CDialog
{
public:
    /* Constructor and destructor. */
    SPxCamCorrSelDlgWin(SPxCamCorrDlgWin* parentWin);
    virtual ~SPxCamCorrSelDlgWin(void);
    virtual BOOL Create(void);

    void AddCamera(SPxCamera *camera);
    void SetSelectedCamera(SPxCamera *camera);
    SPxCamera *GetSelectedCamera(void);

    void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_CAM_CORRECTION_SELECTION_DIALOG };

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
    SPxCamCorrDlgWin *m_parentWin;

    /* Controls. */
    CStatic m_cameraLabel;
    CComboBox m_cameraCombo;
    CStatic m_bearingLabel;
    SPxWinSpinCtrl m_bearingSpin;
    CStatic m_tiltLabel;
    SPxWinSpinCtrl m_tiltSpin;

    /* GUI callback functions. */
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);
    afx_msg void OnCbnSelchangeCameraCombo(void);

    using CDialog::Create;

};

/*********************************************************************
*
* End of file
*
**********************************************************************/
