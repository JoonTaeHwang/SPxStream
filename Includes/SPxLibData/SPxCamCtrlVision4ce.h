/*********************************************************************
*
* (c) Copyright 2017 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamCtrlVision4ce class - a class
*	to allow controlling cameras via Vision4ce video tracker.
*
* Revision Control:
*   08/10/21 v1.18   AGC	Support sensor specific presets.
*
* Previous Changes:
*   01/06/21 1.17   AGC	Support combined absolute pan/tilt.
*   21/01/19 1.16   AGC	Add pan/tilt/zoom timeouts.
*   14/08/18 1.15   AGC	Add IsSecondarySupported() overload.
*   13/08/18 1.14   AGC	Support secondary focus LUT.
*   10/08/18 1.13   AGC	Avoid new hidden function warnings.
*   02/07/18 1.12   AGC	Avoid hidden function warnings.
*   25/06/18 1.11   AGC	Correctly suppress cppcheck warning.
*   30/05/18 1.10   AGC	Support optionally saving address parameters.
*   14/05/18 1.9    AGC	Override GetMesgType().
*   26/09/17 1.8    AGC	Separate primary/secondary zoom range.
*   31/08/17 1.7    AGC	Fix line endings.
*   29/08/17 1.6    AGC	Support Set/GetZoomRange().
*   31/07/17 1.5    AGC	Support SPxCamera setting max pan/tilt speeds.
*   28/07/17 1.4    AGC	Support presets.
*			Support absolute focus.
*   07/07/17 1.3    AGC	Implement functions to retrieve net address.
*   26/06/17 1.2    AGC	Avoid unused parameter warning.
*   22/06/17 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef SPX_CAM_CTRL_VISION4CE_H_
#define SPX_CAM_CTRL_VISION4CE_H_

/* For base class. */
#include "SPxLibData/SPxCamCtrlBase.h"

/* Forward declarations. */
class SPxThread;
class SPxVision4ce;

class SPxCamCtrlVision4ce : public SPxCamCtrlBase
{
public:
    /* Construction and destruction. */
    explicit SPxCamCtrlVision4ce(SPxVision4ce *vision4ce,
				 int saveParams=FALSE);
    virtual ~SPxCamCtrlVision4ce(void);

    /* Control. */
    virtual int IsConnected(void) const;

    /* Connection set through SPxVision4ce object. */
    virtual int IsSerialSupported(void) const { return FALSE; }
    virtual int IsNetworkSupported(void) const { return TRUE; }
    virtual SPxMesgIOType_t GetMesgType(void) const;

    /* Base class functions. */
    virtual SPxErrorCode SetAddress(const char *address, int port = 0, const char *ifAddr = NULL);
    virtual SPxErrorCode SetAddress(UINT32 address, int port = 0, const char *ifAddr = NULL);
    virtual UINT32 GetAddress(void) const;
    virtual const char *GetAddressString(void) const;
    virtual int GetPort(void) const;
    virtual UINT32 GetIfAddress(void) const;
    virtual const char *GetIfAddressString(void) const;

    /* General */
    virtual SPxErrorCode Stop(int isPrimary);

    /* Absolute positioning. */
    virtual int IsAbsCmdInProgress(UINT32 dirMask, int isPrimary) const;

    virtual int IsAbsPanSupported(void) const;
    virtual SPxErrorCode SetPan(float panAngleDegs);
    virtual SPxErrorCode GetPan(float *panAngleDegs) const;
    virtual SPxErrorCode GetLastPan(float *panAngleDegs) const;

    virtual int IsAbsTiltSupported(void) const;
    virtual SPxErrorCode SetTilt(float tiltAngleDegs);
    virtual SPxErrorCode GetTilt(float *tiltAngleDegs) const;
    virtual SPxErrorCode GetLastTilt(float *tiltAngleDegs) const;

    virtual int IsAbsPanTiltSupported(void) const;
    virtual SPxErrorCode SetPanTilt(float panAngleDegs, float tiltAngleDegs);

    virtual int IsAbsZoomSupported(int isPrimary) const;
    virtual SPxErrorCode SetZoom(float fovDegs, int isPrimary);
    virtual SPxErrorCode GetZoom(float *fovDegs, int isPrimary) const;
    virtual SPxErrorCode GetLastZoom(float *fovDegs, int isPrimary) const;
    virtual SPxErrorCode SetZoomRange(float minFovDegs, float maxFovDegs, int isPrimary);
    virtual SPxErrorCode GetZoomRange(float *minFovDegs, float *maxFovDegs, int isPrimary) const;

    virtual int IsAbsFocusSupported(int isPrimary) const;
    virtual SPxErrorCode SetFocus(float rangeMetres, int isPrimary);
    virtual SPxErrorCode GetFocus(float *rangeMetres, int isPrimary) const;
    virtual SPxErrorCode GetLastFocus(float *rangeMetres, int isPrimary) const;
    virtual SPxErrorCode SetFocusLUTFilename(const char *filename, int isPrimary);
    virtual SPxErrorCode GetFocusLUTFilename(char *buffer, int bufLen, int isPrimary) const;

    /* Absolute zoom/focus functions that support primary/secondary. */
    virtual int IsSecondarySupported(void) const { return TRUE; }

    /* Relative positioning. */
    virtual int IsRelPanSupported(void) const;
    virtual SPxErrorCode Pan(float panSpeed, float *panSpeedSet=NULL,
			     int continueZoomFocus=FALSE,
			     double timeoutSecs=0.0);

    virtual int IsRelTiltSupported(void) const;
    virtual SPxErrorCode Tilt(float tiltSpeed, float *tiltSpeedSet=NULL,
			      int continueZoomFocus=FALSE,
			      double timeoutSecs=0.0);

    virtual int IsRelPanTiltSupported(void) const;
    virtual SPxErrorCode PanTilt(float panSpeed, float tiltSpeed,
				 float *panSpeedSet=NULL, float *tiltSpeedSet=NULL,
				 int continueZoomFocus=FALSE,
				 double timeoutSecs=0.0);

    virtual int IsRelZoomSupported(int isPrimary) const;
    virtual SPxErrorCode Zoom(float zoomSpeed, int continuePanTilt,
			      double timeoutSecs, int isPrimary);

    virtual int IsRelFocusSupported(int isPrimary) const;
    virtual SPxErrorCode Focus(float focusSpeed, int continuePanTilt, int isPrimary);

    virtual int IsAutoFocusSupported(int isPrimary) const;
    virtual SPxErrorCode AutoFocus(int autoFocus, int isPrimary);
    virtual int IsAutoFocusEnabled(int isPrimary) const;

    virtual SPxErrorCode GoToPreset(UINT8 id) { return GoToPreset(id, TRUE); }
    virtual SPxErrorCode GoToPreset(UINT8 id, int isPrimary);

    virtual SPxErrorCode SetMaxPanSpeedDegsPerSec(double maxPanSpeedDegsPerSec);
    virtual SPxErrorCode SetMaxTiltSpeedDegsPerSec(double maxTiltSpeedDegsPerSec);

    virtual UINT32 GetMinCmdIntervalMsecs(void) const { return 20; }

protected:
    // cppcheck-suppress virtualCallInConstructor
    virtual void Shutdown(void);

private:  
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    void checkAbsCmdInProgress(void) const;
    int isDirConnected(void) const;
    int isCamConnected(UINT8 index) const;

    using SPxCamZoomIface::IsAbsZoomSupported;
    using SPxCamZoomIface::IsRelZoomSupported;
    using SPxCamZoomIface::SetZoom;
    using SPxCamZoomIface::GetZoom;
    using SPxCamZoomIface::SetZoomRange;
    using SPxCamZoomIface::GetZoomRange;
    using SPxCamZoomIface::Zoom;
    using SPxCamFocusIface::IsAbsFocusSupported;
    using SPxCamFocusIface::IsRelFocusSupported;
    using SPxCamFocusIface::IsAutoFocusSupported;
    using SPxCamFocusIface::Focus;
    using SPxCamFocusIface::SetFocus;
    using SPxCamFocusIface::GetFocus;
    using SPxCamFocusIface::AutoFocus;
    using SPxCamFocusIface::IsAutoFocusEnabled;

}; /* SPxCamCtrlVision4ce */

#endif /* SPX_CAM_CTRL_VISION4CE_H_ */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
