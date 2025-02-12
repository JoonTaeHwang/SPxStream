/*********************************************************************
*
* (c) Copyright 2018 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamCtrlOnvif class - a class
*	to allow controlling a camera with an Onvif interface.
*
* Revision Control:
*   17/06/21 v1.19   AGC	Add PanTiltZoom().
*
* Previous Changes:
*   06/08/20 1.18   AGC	Add SetZoomPos().
*			Retrieve current pos on timer.
*   04/03/20 1.17   AGC	Resend pan/tilt commands when requried.
*   31/07/19 1.16   BTB	Support calling GetZoomPos() for FOV calibration.
*   27/03/19 1.15   AGC	Improve absolute command completion detection.
*   18/02/19 1.14   AGC	Support calling CheckConnect() for testing.
*   12/02/19 1.13   AGC	Improve support when cannot retrieve abs positions.
*   12/02/19 1.12   AGC	Update connection status on background thread.
*   06/02/19 1.11   AGC	Attempt reconnection on failure.
*   21/01/19 1.10   AGC	Support pan/tilt/zoom timeouts.
*   15/01/19 1.9    AGC	Add option to use/ignore space ranges from camera.
*   14/01/19 1.8    AGC	Improve generic pan/tilt space support.
*   13/12/18 1.7    AGC	Use zoom LUT.
*   12/12/18 1.6    AGC	Avoid using base class interfaces.
*   12/12/18 1.5    AGC	Save/use service address.
*   21/09/18 1.4    AGC	Support combined PTZ queries.
*   09/07/18 1.3    AGC	Overload IsConnected().
*   03/07/18 1.2    AGC	Suppress spurious cppcheck warnings.
*   02/07/18 1.1    AGC	Initial version.
**********************************************************************/
#ifndef SPX_CAM_CTRL_ONVIF_H_
#define SPX_CAM_CTRL_ONVIF_H_

/* For base class. */
#include "SPxLibData/SPxCamCtrlBase.h"

/* Forward declarations. */
class SPxOnvifClient;

/*
 * Define our class.
 */
class SPxCamCtrlOnvif : public SPxCamCtrlBase
{
public:
    /* Construction and destruction. */
    SPxCamCtrlOnvif(void);
    virtual ~SPxCamCtrlOnvif(void);
    virtual SPxErrorCode Create(const char *serialName = NULL,
				unsigned int serialBaud = 2400);

    /* Control. */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetMesgType(SPxMesgIOType_t mesgType);
    virtual SPxMesgIOType_t GetMesgType(void) const;
    virtual int IsSerialSupported(void) const { return FALSE; }
    virtual int IsNetworkSupported(void) const { return TRUE; }
    virtual int IsConnected(void) const;

    SPxOnvifClient *GetOnvifClient(void);

    /* Connection settings. */
    SPxErrorCode SetUsername(const char *username);
    SPxErrorCode GetUsername(char *buf, unsigned int bufLen) const;
    SPxErrorCode SetPassword(const char *password);
    SPxErrorCode GetPassword(char *buf, unsigned int bufLen) const;
    SPxErrorCode SetServerID(const char *serverID, unsigned int *numProfiles);
    SPxErrorCode GetServerID(char *buf, unsigned int bufLen) const;
    SPxErrorCode SetServiceAddress(const char *address, unsigned int *numProfiles);
    SPxErrorCode GetServiceAddress(char *buf, unsigned int bufLen) const;
    SPxErrorCode SetProfileID(const char *profileID);
    SPxErrorCode GetProfileID(char *buf, unsigned int bufLen) const;

    /* Camera control functions. */
    virtual SPxErrorCode Stop(void);
    virtual SPxErrorCode GoToPreset(const char *preset);

    /* Base class functions. */

    /* Absolute positioning. */
    virtual int IsAbsCmdInProgress(UINT32 dirMask) const;

    virtual int IsAbsPanSupported(void) const;
    virtual SPxErrorCode SetPan(float panAngleDegs);
    virtual SPxErrorCode GetPan(float *panAngleDegs) const;
    virtual SPxErrorCode GetLastPan(float *panAngleDegs) const;

    virtual int IsAbsTiltSupported(void) const;
    virtual SPxErrorCode SetTilt(float tiltAngleDegs);
    virtual SPxErrorCode GetTilt(float *tiltAngleDegs) const;
    virtual SPxErrorCode GetLastTilt(float *tiltAngleDegs) const;
    virtual SPxErrorCode SetTiltRange(float minTiltDegs, float maxTiltDegs);
    virtual SPxErrorCode GetTiltRange(float *minTiltDegs, float *maxTiltDegs) const;

    virtual int IsAbsZoomSupported(void) const;
    virtual SPxErrorCode SetZoom(float fovDegs);
    virtual SPxErrorCode GetZoom(float *fovDegs) const;
    virtual SPxErrorCode GetLastZoom(float *fovDegs) const;
    virtual SPxErrorCode SetZoomPos(UINT16 zoomPos);
    virtual SPxErrorCode GetZoomPos(UINT16 *zoomPos) const;
    virtual SPxErrorCode SetZoomRange(float minFovDegs, float maxFovDegs);
    virtual SPxErrorCode GetZoomRange(float *minFovDegs, float *maxFovDegs) const;
    virtual SPxErrorCode SetZoomLUTFilename(const char *filename);
    virtual SPxErrorCode GetZoomLUTFilename(char *buffer, int bufLen) const;

    virtual int IsAbsGetPTZSupported(void) const;
    virtual SPxErrorCode GetPTZ(float *panAngleDegs,
				float *tiltAngleDegs,
				float *fovDegs) const;

    virtual int IsAbsFocusSupported(void) const;
    virtual SPxErrorCode SetFocus(float rangeMetres);
    virtual SPxErrorCode GetFocus(float *rangeMetres) const;
    virtual SPxErrorCode GetLastFocus(float *rangeMetres) const;
    virtual SPxErrorCode SetFocusLUTFilename(const char *filename);
    virtual SPxErrorCode GetFocusLUTFilename(char *buffer, int bufLen) const;

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
				 float *panSpeedSet=NULL,
				 float *tiltSpeedSet=NULL,
				 int continueZoomFocus=FALSE,
				 double timeoutSecs=0.0);

    virtual int IsRelPanTiltZoomSupported(void) const;
    virtual SPxErrorCode PanTiltZoom(float panSpeed, float tiltSpeed, float zoomSpeed,
				     float *panSpeedSet=NULL, float *tiltSpeedSet=NULL,
				     double timeoutSecs=0.0);

    virtual int IsRelZoomSupported(void) const;
    virtual SPxErrorCode Zoom(float zoomSpeed, int continuePanTilt, double timeoutSecs);

    virtual int IsRelFocusSupported(void) const;
    virtual SPxErrorCode Focus(float focusSpeed, int continuePanTilt);

    virtual int IsAutoFocusSupported(void) const;
    virtual SPxErrorCode AutoFocus(int autoFocus);
    virtual int IsAutoFocusEnabled(void) const;

    SPxErrorCode SetUseGenRangesFromCam(int useGenRangesFromCam);
    int IsUsingGenRangesFromCam(void) const;

    /* Generic get/set parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);
    virtual SPxErrorCode SetStateFromConfig(void);

protected:
    // cppcheck-suppress virtualCallInConstructor
    virtual void Shutdown(void);

    /* Check connection. */
    virtual void CheckConnect(void);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    void ApplySettings(void);
    void applySettings(int fromTimer=FALSE);
    float getFOVForZoom(float zoomPos) const;
    float getZoomForFOV(float fovDegs) const;
    double getGenTiltPos(double tiltDegs) const;
    double getDegsTiltPos(double tiltGen) const;
    void relZoomUsingAbs(void);
    void relPanTilt(void);
    void updatePos(void);
    SPxErrorCode getPTZ(float *panAngleDegs,
			float *tiltAngleDegs,
			float *fovDegs) const;
    SPxErrorCode getFocus(float *rangeMetres) const;
    SPxErrorCode getZoomPos(float *zoomPos) const;

}; /* SPxCamCtrlOnvif */

#endif /* SPX_CAM_CTRL_ONVIF_H_ */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
