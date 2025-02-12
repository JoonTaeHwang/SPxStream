/*********************************************************************
*
* (c) Copyright 2018, 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for main camera correction dialog.
*
*
* Revision Control:
*   07/08/19 v1.11   BTB	Minor tidy up.
*
* Previous Changes:
*   19/06/19 1.10   AGC	Support use of Direct2D renderers.
*   02/11/18 1.9    AGC	Simplify camera correction.
*   26/10/18 1.8    AGC	Support calculating camera height.
*   22/10/18 1.7    AGC	Fix warnings.
*   19/10/18 1.6    AGC	Simplify correction point handling.
*   05/10/18 1.5    AGC	Add SetSelectedCamera().
*   29/08/18 1.4    BTB	Code cleanup.
*   24/08/18 1.3    BTB	Change namespace name to avoid future conflicts.
*   23/08/18 1.2    BTB	Allow setting of points based on horizon.
*			Handle closing dialog properly.
*			Various other minor improvements.
*   17/08/18 1.1    BTB	Initial version.
*
**********************************************************************/

#pragma once

/* Standard headers. */

/* Library headers. */
#include "SPxLibUtils/SPxCamCorrDefs.h"
#include "SPxLibUtils/SPxAutoPtr.h"

/* Include the SPx resource header */
#include "SPxResource.h"

#define SPX_AFXWIN

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/
enum SPxCamCorrectionState_t
{
    SPX_CAM_CORR_STATE_CAM_SELECT = 0,
    SPX_CAM_CORR_STATE_POINT_SELECT = 1,
    SPX_CAM_CORR_STATE_ADJUST = 2,
    SPX_CAM_CORR_STATE_PREVIEW = 3
};

/* Forward declarations. */
class SPxCamera;
class SPxCamCorrSelDlgWin;
class SPxCamCorrPointDlgWin;
class SPxCamCorrAdjDlgWin;
class SPxCamCorrPrevDlgWin;
class SPxCamCorrRenderer;
class SPxAVRenderCamCorr;

/*
 * SPxCamCorrDlgWin dialog
 */
class SPxCamCorrDlgWin : public CDialog
{
public:
    /* Handler for closing the top level dialog. */
    typedef void (*DialogCloseFn_t)(void *userArg);	/* User arg. */

    /* Constructor and destructor. */
    explicit SPxCamCorrDlgWin(CWnd* parentWin, int isCamHeightCorrect=TRUE);
    virtual ~SPxCamCorrDlgWin(void);
    virtual BOOL Create(void);

    SPxCamCorrectionState_t GetCurrentState(void) { return m_currentState; }
    unsigned int GetCurrentIndex(void) { return m_currentIndex; }
    void SetToInitialState(void);

    void AddCamera(SPxCamera *camera);
    void AddPPIRenderer(SPxCamCorrRenderer *ppiRdr);
    void AddVideoRenderer(SPxAVRenderCamCorr *videoRdr);

    void SetPointHeight(double heightMetres);
    void SetPointLatLong(double latDegs, double longDegs);
    void SetPointRangeBearing(double rangeMetres, double bearingDegs);
    void SetPointHorizon(double bearingDegs, double tiltDegs);
    void SetPointTilt(double tiltDegs);

    SPxErrorCode GetVideoPointBearingTilt(int xPos, int yPos,
					  unsigned int widthPixels,
					  unsigned int heightPixels,
					  double *bearingDegs,
					  double *tiltDegs) const;
    int IsPPIPointSettable(void) const;
    int IsVideoPointSettable(void) const;
    int IsVideoHorizonSettable(void) const;
    int IsFixedBearingAndHeight(void) const;

    void SetSelectedCamera(SPxCamera *camera);
    SPxCamera *GetSelectedCamera(void){ return m_selectedCamera; }

    void SetOldCorrection(void);
    void SetNewCorrection(void);

    void DiscardCorrection(void);

    /* Dialog close handler. */
    void SetDialogCloseHandler(DialogCloseFn_t fn, void *userArg);

    /* Dialog Data */
    enum { IDD = IDD_SPX_CAM_CORRECTION_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ }
    virtual void OnCancel(void){ DiscardCorrection(); }
    DECLARE_MESSAGE_MAP()

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* The parent window. */
    CWnd *m_parentWin;

    /* Current dialog state. */
    int m_isCamHeightCorrect;
    unsigned int m_numPoints;
    SPxCamCorrectionState_t m_currentState;
    unsigned int m_currentIndex;

    /* Child dialogs. */
    SPxCamCorrSelDlgWin *m_selectionDialog;
    SPxCamCorrPointDlgWin *m_pointDialog;
    SPxCamCorrAdjDlgWin *m_adjustmentDialog;
    SPxCamCorrPrevDlgWin *m_previewDialog;

    /* Currently selected camera. */
    SPxCamera *m_selectedCamera;

    /* Old camera settings. */
    double m_oldLeanBearingDegs1;
    double m_oldLeanTiltDegs1;
    double m_oldLeanBearingDegs2;
    double m_oldLeanTiltDegs2;
    double m_oldCameraHeight;

    /* Current and new sets of correction points. */
    SPxCamCorrectionNS::CorrectionPoint m_points[SPX_MAX_CAM_CORRECTION_POINTS];

    /* Controls. */
    CStatic m_instructionsLabel;
    CButton m_previousBn;
    CButton m_nextBn;
    CButton m_finishBn;
    CButton m_cancelBn;

    /* Dialog Close function. */
    DialogCloseFn_t m_dialogCloseFn;
    void *m_dialogCloseUserArg;

    /* Private functions. */
    void SetPointLatLong(unsigned int point, double latDegs, double longDegs);
    void SetPointHorizon(double bearingDegs);
    void SetPointActualTilt(void);
    void SyncControlsToState(void);
    void EnablePPIRenderers(int enable);
    void EnableVideoRenderers(int enable);
    void SetCameraOnVideoRenderers(void);
    void SaveCorrection(void);
    void SetNoCorrection(void);
    void FinishCorrection(void);

    /* GUI callback functions. */
    afx_msg void OnBnClickedPreviousButton(void);
    afx_msg void OnBnClickedNextButton(void);
    afx_msg void OnBnClickedFinishButton(void);
    afx_msg void OnBnClickedCancelButton(void);

    using CDialog::Create;

};

/*********************************************************************
*
* End of file
*
**********************************************************************/
