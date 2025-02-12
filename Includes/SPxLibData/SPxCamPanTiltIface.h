/*********************************************************************
*
* (c) Copyright 2018, 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamPanTiltIface class - an interface for
*	basic pan/tilt camera control.
*
* Revision Control:
*   21/01/19 v1.1    AGC	Add pan/tilt timeouts.
*
* Previous Changes:
*   02/07/18 1.0    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_CAM_PAN_TILT_IFACE_H
#define _SPX_CAM_PAN_TILT_IFACE_H

#include "SPxLibUtils/SPxTypes.h"
#include "SPxLibUtils/SPxError.h"

class SPxCamPanTiltIface
{
public:
    /* Construction and destruction. */
    SPxCamPanTiltIface() {}
    virtual ~SPxCamPanTiltIface() {}

    /* Absolute positioning. */
    virtual int IsAbsPanCmdInProgress(void) const = 0;
    virtual int IsAbsTiltCmdInProgress(void) const = 0;

    virtual int IsAbsPanSupported(void) const = 0;
    virtual SPxErrorCode SetPan(float panAngleDegs) = 0;
    virtual SPxErrorCode GetPan(float *panAngleDegs) const = 0;
    virtual SPxErrorCode GetPanRange(float *minPanAngleDegs, float *maxPanAngleDegs) const = 0;

    virtual int IsAbsTiltSupported(void) const = 0;
    virtual SPxErrorCode SetTilt(float tiltAngleDegs) = 0;
    virtual SPxErrorCode GetTilt(float *tiltAngleDegs) const = 0;
    virtual SPxErrorCode GetTiltRange(float *minTiltAngleDegs, float *maxTiltAngleDegs) const = 0;

    /* Relative positioning. */
    virtual int IsRelPanSupported(void) const = 0;
    virtual SPxErrorCode Pan(float panSpeed, float *panSpeedSet=NULL,
			     int continueZoomFocus=FALSE,
			     double timeoutSecs=0.0) = 0;
    virtual SPxErrorCode GetPanSpeed(float *panSpeed) const = 0;

    virtual int IsRelTiltSupported(void) const = 0;
    virtual SPxErrorCode Tilt(float tiltSpeed, float *tiltSpeedSet=NULL,
			      int continueZoomFocus=FALSE,
			      double timeoutSecs=0.0) = 0;
    virtual SPxErrorCode GetTiltSpeed(float *tiltSpeed) const = 0;

    virtual int IsRelPanTiltSupported(void) const = 0;
    virtual SPxErrorCode PanTilt(float panSpeed, float tiltSpeed,
				 float *panSpeedSet=NULL, float *tiltSpeedSet=NULL,
				 int continueZoomFocus=FALSE,
				 double timeoutSecs=0.0) = 0;

    virtual int AreMaxSpeedsSupported(void) const = 0;
    virtual SPxErrorCode GetMaxPanSpeedDegsPerSec(double *maxPanSpeedDegsPerSec) const = 0;
    virtual SPxErrorCode GetMaxTiltSpeedDegsPerSec(double *maxTiltSpeedDegsPerSec) const = 0;

}; /* SPxCamPanTiltIface */

#endif /* _SPX_CAM_PAN_TILT_IFACE_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
