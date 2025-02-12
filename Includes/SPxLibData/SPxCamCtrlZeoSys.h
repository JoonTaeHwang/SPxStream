/*********************************************************************
*
* (c) Copyright 2017 - 2019, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamCtrlZeoSys class - a class
*	to allow controlling ZeoSys cameras.
*
* Revision Control:
*   27/09/22 v1.23   AGC	Fix virtual function hidden warning.
*
* Previous Changes:
*   27/09/22 1.22   AGC	Support specifying number of measurements to MeasureRange().
*   21/01/19 1.21   AGC	Add pan/tilt/zoom timeouts.
*   14/01/19 1.20   AGC	Fix cppcheck warnings.
*   25/09/18 1.19   AGC	Only stop focus when auto-focus disabled.
*   25/09/18 1.18   AGC	Improve daylight auto-focus restore.
*   21/09/18 1.17   AGC	Limit fov to valid range.
*   10/08/18 1.16   AGC	Avoid new hidden function warnings.
*   31/07/18 1.15   AGC	Add IsSecondarySupported().
*   02/07/18 1.14   AGC	Support one-shot auto-focus.
*   02/07/18 1.13   AGC	Avoid hidden function warnings.
*   21/06/18 1.12   AGC	Improve max speed support.
*   26/04/18 1.11   AGC	Force correct message type.
*   26/04/18 1.10   AGC	Support device type configuration.
*   20/04/18 1.9    AGC	Improve laser range finder control.
*   20/06/17 1.8    AGC	Tweak default max speeds.
*   07/06/17 1.7    AGC	Support automatic NUC.
*			Support measured range timeout.
*   06/06/17 1.6    AGC	Support laser range finder.
*   26/05/17 1.5    AGC	Support primary/secondary zoom/focus.
*   16/05/17 1.4    AGC	Change line endings.
*   28/04/17 1.3    AGC	Add preset support.
*   24/04/17 1.2    AGC	Support daylight/thermal cameras using camera address.
*   20/04/17 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef SPX_CAM_CTRL_ZEO_SYS_H_
#define SPX_CAM_CTRL_ZEO_SYS_H_

/* For base class. */
#include "SPxLibData/SPxCamCtrlBase.h"

/* Forward declarations. */
class SPxThread;

class SPxCamCtrlZeoSys : public SPxCamCtrlBase
{
public:
    /* Types. */
    enum Presets
    {
	PRESET_POWER_CAM = 1,
	PRESET_POWER_LRF = 2,
	PRESET_OP_STATUS_PTZ = 3,
	PRESET_OP_STATUS_CAM = 4,
	PRESET_OP_STATUS_LRF = 5,
	PRESET_AUTO_FOCUS = 6,
	PRESET_LENS_CAP = 7,
	PRESET_BW_HOT = 8,
	PRESET_FREEZE = 9,
	PRESET_NUC = 10,
	PRESET_NUC_QUICK = 11,
	PRESET_LRF_MEASURE = 12
    };

    enum DeviceType
    {
	DEVICE_TYPE_NIGHTOWL_M = 0,
	DEVICE_TYPE_NIGHTOWL_Z = 1,
	DEVICE_TYPE_NIGHTOWL_ZM = 2,
	DEVICE_TYPE_SPARROWHAWK = 3
    };

    /* Construction and destruction. */
    SPxCamCtrlZeoSys(void);
    virtual ~SPxCamCtrlZeoSys(void);
    virtual int IsSupported(void) const;

    /* Control. */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetMesgType(SPxMesgIOType_t mesgType);
    virtual int IsSerialSupported(void) const { return FALSE; }
    virtual SPxErrorCode SetAddress(const char *address, int port = 0, const char *ifAddr=NULL);
    virtual SPxErrorCode SetAddress(UINT32 address, int port = 0, const char *ifAddr=NULL);

    /* Device type. */
    SPxErrorCode SetDeviceType(DeviceType deviceType);
    SPxErrorCode GetDeviceType(DeviceType *deviceType) const;

    /* Base class functions. */

    /* General */
    virtual int IsConnected(void) const;
    virtual int IsSecondarySupported(void) const { return TRUE; }
    virtual SPxErrorCode Stop(int isPrimary);
    virtual SPxErrorCode GoToPreset(UINT8 id, int isPrimary);

    /* Self reference. */
    virtual SPxErrorCode SelfReference(void);

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

    virtual int IsAbsZoomSupported(int isPrimary) const;
    virtual SPxErrorCode SetZoom(float fovDegs, int isPrimary);
    virtual SPxErrorCode GetZoom(float *fovDegs, int isPrimary) const;
    virtual SPxErrorCode GetLastZoom(float *fovDegs, int isPrimary) const;
    virtual SPxErrorCode GetZoomRange(float *minFovDegs, float *maxFovDegs, int isPrimary) const;

    virtual int IsAbsFocusSupported(int isPrimary) const;
    virtual SPxErrorCode SetFocus(float rangeMetres, int isPrimary);
    virtual SPxErrorCode GetFocus(float *rangeMetres, int isPrimary) const;
    virtual SPxErrorCode GetLastFocus(float *rangeMetres, int isPrimary) const;

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

    virtual int IsAutoFocusOneshotSupported(int isPrimary) const;
    virtual SPxErrorCode AutoFocusOneshot(int isPrimary);

    /* Speeds. */
    virtual int AreMaxSpeedsSupported(void) const { return TRUE; }
    virtual SPxErrorCode SetMaxPanSpeedDegsPerSec(double maxPanSpeedDegsPerSec);
    virtual SPxErrorCode SetMaxTiltSpeedDegsPerSec(double maxTiltSpeedDegsPerSec);
    virtual SPxErrorCode GetMaxPanTiltSpeedsDegsPerSec(double *maxPanSpeedDegsPerSec,
                                                       double *maxTiltSpeedDegsPerSec) const;

    /* Range measurement. */
    virtual int IsMeasuredRangeSupported(void) const;
    virtual SPxErrorCode MeasureRange(int numMeasurement=-1);
    virtual SPxErrorCode GetMeasuredRange(double *rangeMetres) const;

    /* Timeouts/intervals. */
    SPxErrorCode SetNUCIntervalMsecs(unsigned int msecs);
    unsigned int GetNUCIntervalMsecs(void) const;
    SPxErrorCode SetMeasuredRangeTimeoutMsecs(unsigned int msecs);
    unsigned int GetMeasuredRangeTimeoutMsecs(void) const;

    /* Generic get/set parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    // cppcheck-suppress virtualCallInConstructor
    virtual void Shutdown(void);

private:  
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    static void *updateWrapper(SPxThread *thread);
    void *update(SPxThread *thread);
    void stopStack(void);
    SPxErrorCode startPanTilt(void);
    void stopPanTilt(void);
    SPxErrorCode startCam(int isPrimary);
    void stopCam(int isPrimary);
    SPxErrorCode startLRF(void);
    void stopLRF(void);
    void checkAbsCmdStatus(void);
    void checkNuc(void);
    SPxErrorCode measureRange(void);

    template<typename T, typename FS, typename FC, typename FCA,
	typename StatusT, typename Control>
    SPxErrorCode toggle(const char *desc, T& dev,
			FS& statusFn, FC& controlFn, FCA& controlAsyncFn,
			StatusT statusOn, StatusT statusOff,
			Control controlOn, Control controlOff);

    using SPxCamZoomIface::IsAbsZoomSupported;
    using SPxCamZoomIface::IsRelZoomSupported;
    using SPxCamZoomIface::SetZoom;
    using SPxCamZoomIface::GetZoom;
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
    using SPxCamPresetIface::GoToPreset;
    using SPxCamMeasureRangeIface::GetMeasuredRange;

}; /* SPxCamCtrlZeoSys */

#endif /* SPX_CAM_CTRL_ZEO_SYS_H_ */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
