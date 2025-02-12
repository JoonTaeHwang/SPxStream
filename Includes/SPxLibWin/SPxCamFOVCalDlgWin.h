/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for main camera FOV calibration dialog.
*
*
* Revision Control:
*   09/08/19 v1.2   BTB	Keep old LUT filename updated properly.
*
* Previous Changes:
*   07/08/19 1.1   BTB	Initial version.
*
**********************************************************************/

#pragma once

/* Standard headers. */

/* Library headers. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* Include the SPx resource header */
#include "SPxResource.h"

#define SPX_AFXWIN

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/
enum SPxCamFOVCalibrationState_t
{
    SPX_CAM_FOV_CAL_STATE_SELECTION = 0,
    SPX_CAM_FOV_CAL_STATE_STAGES = 1,
    SPX_CAM_FOV_CAL_STATE_PREVIEW = 2
};

/* Forward declarations. */
class SPxCamera;
class SPxCamFOVCalSelDlgWin;
class SPxCamFOVCalStgsDlgWin;
class SPxCamFOVCalPrevDlgWin;
class SPxAVRenderCamFOVCal;

/*
 * SPxCamFOVCalDlgWin dialog
 */
class SPxCamFOVCalDlgWin : public CDialog
{
public:
    /* Handler for closing the top level dialog. */
    typedef void (*DialogCloseFn_t)(void *userArg);	/* User arg. */

    /* Handler for saving the application config. */
    typedef void (*AppSaveFn_t)(void *userArg);	/* User arg. */

    /* Constructor and destructor. */
    explicit SPxCamFOVCalDlgWin(CWnd* parentWin);
    virtual ~SPxCamFOVCalDlgWin(void);
    virtual BOOL Create(void);

    void SetToInitialState(void);

    void AddCamera(SPxCamera *camera);
    void AddRenderer(SPxAVRenderCamFOVCal *renderer);

    void AddNewCalibrationPoint(UINT16 zoomPos, float fovDegs);
    unsigned int GetNumCalibrationPoints(void);
    void DiscardNewCalibration(void);

    void SetOldCalibration(void);
    void SetNewCalibration(void);

    void FinishCalibration(void);

    /* Dialog close handler. */
    void SetDialogCloseHandler(DialogCloseFn_t fn, void *userArg);

    /* Application save handler. */
    void SetApplicationSaveHandler(AppSaveFn_t fn, void *userArg);

    /* Dialog Data */
    enum { IDD = IDD_SPX_CAM_FOV_CALIBRATION_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ }
    virtual void OnCancel(void){ FinishCalibration(); }
    DECLARE_MESSAGE_MAP()

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* The parent window. */
    CWnd *m_parentWin;

    /* Current dialog state. */
    SPxCamFOVCalibrationState_t m_currentState;

    /* Child dialogs. */
    SPxCamFOVCalSelDlgWin *m_selectionDialog;
    SPxCamFOVCalStgsDlgWin *m_stagesDialog;
    SPxCamFOVCalPrevDlgWin *m_previewDialog;

    /* Currently selected camera. */
    SPxCamera *m_selectedCamera;
    /* Primary/secondary controller. */
    int m_primaryController;

    /* Controls. */
    CStatic m_instructionsLabel;
    CButton m_previousBn;
    CButton m_nextBn;
    CButton m_saveBn;
    CButton m_cancelBn;

    /* Dialog Close function. */
    DialogCloseFn_t m_dialogCloseFn;
    void *m_dialogCloseUserArg;

    /* Application Save function. */
    AppSaveFn_t m_appSaveFn;
    void *m_appSaveUserArg;

    /* Private functions. */
    void refreshCameras(void);
    void syncControlsToState(void);
    void enableRenderers(int enable);
    void setCameraOnVideoRenderers(void);
    void showRendererPoints(int showCentrePoint, int showOffsetPoint);
    void updateOldLUTFilename(void);
    void updateNewLUTFilename(void);
    void saveNewCalibration(const char *filename);
    void syncGUIFromTimer(void);
    void waitBeforeSave(void);

    /* GUI callback functions. */
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedPreviousButton(void);
    afx_msg void OnBnClickedNextButton(void);
    afx_msg void OnBnClickedSaveButton(void);
    afx_msg void OnBnClickedCancelButton(void);

    using CDialog::Create;

};

/*********************************************************************
*
* End of file
*
**********************************************************************/
