/*********************************************************************
*
* (c) Copyright 2018, 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamSimPanTilt class - a class
*	for simulating the pan/tilt behaviour of a camera.
*
* Revision Control:
*   12/04/19 v1.3    AGC	Support setting pan/tilt range.
*
* Previous Changes:
*   21/01/19 1.2    AGC	Support pan/tilt timeouts.
*   10/08/18 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_CAM_SIM_PAN_TILT_H
#define _SPX_CAM_SIM_PAN_TILT_H

/* For base classes. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibData/SPxCamPanTiltIface.h"

/* Forward declarations. */

class SPxCamSimPanTilt : public SPxObj, public SPxCamPanTiltIface
{
public:
    SPxCamSimPanTilt(void);
    virtual ~SPxCamSimPanTilt(void);

    /* Absolute positioning */
    virtual int IsAbsPanCmdInProgress(void) const;
    virtual int IsAbsTiltCmdInProgress(void) const;

    virtual SPxErrorCode SetAbsPanSupported(int supported);
    virtual int IsAbsPanSupported(void) const;
    virtual SPxErrorCode SetPan(float panAngleDegs);
    virtual SPxErrorCode GetPan(float *panAngleDegs) const;
    virtual SPxErrorCode SetPanRange(float minPanAngleDegs, float maxPanAngleDegs);
    virtual SPxErrorCode GetPanRange(float *minPanAngleDegs, float *maxPanAngleDegs) const;
    virtual SPxErrorCode GetLastReqPan(float *panAngleDegs) const;

    virtual SPxErrorCode SetAbsTiltSupported(int supported);
    virtual int IsAbsTiltSupported(void) const;
    virtual SPxErrorCode SetTilt(float tiltAngleDegs);
    virtual SPxErrorCode GetTilt(float *tiltAngleDegs) const;
    virtual SPxErrorCode SetTiltRange(float minTiltAngleDegs, float maxTiltAngleDegs);
    virtual SPxErrorCode GetTiltRange(float *minTiltAngleDegs, float *maxTiltAngleDegs) const;
    virtual SPxErrorCode GetLastReqTilt(float *tiltAngleDegs) const;

    /* Relative positioning. */
    virtual SPxErrorCode SetRelPanSupported(int supported);
    virtual int IsRelPanSupported(void) const;
    virtual SPxErrorCode Pan(float panSpeed, float *panSpeedSet = NULL,
			     int continueZoomFocus = FALSE,
			     double timeoutSecs = 0.0);
    virtual SPxErrorCode GetPanSpeed(float *panSpeed) const;

    virtual SPxErrorCode SetRelTiltSupported(int supported);
    virtual int IsRelTiltSupported(void) const;
    virtual SPxErrorCode Tilt(float tiltSpeed, float *tiltSpeedSet = NULL,
			      int continueZoomFocus = FALSE,
			      double timeoutSecs = 0.0);
    virtual SPxErrorCode GetTiltSpeed(float *tiltSpeed) const;

    virtual SPxErrorCode SetRelPanTiltSupported(int supported);
    virtual int IsRelPanTiltSupported(void) const;
    virtual SPxErrorCode PanTilt(float panSpeed, float tiltSpeed,
				 float *panSpeedSet = NULL, float *tiltSpeedSet = NULL,
				 int continueZoomFocus = FALSE,
				 double timeoutSecs = 0.0);

    /* Speeds. */
    virtual int AreMaxSpeedsSupported(void) const { return TRUE; }
    virtual SPxErrorCode SetMaxPanSpeedDegsPerSec(double maxPanSpeedDegsPerSec);
    virtual SPxErrorCode SetMaxTiltSpeedDegsPerSec(double maxTiltSpeedDegsPerSec);
    virtual SPxErrorCode GetMaxPanSpeedDegsPerSec(double *maxPanSpeedDegsPerSec) const;
    virtual SPxErrorCode GetMaxTiltSpeedDegsPerSec(double *maxTiltSpeedDegsPerSec) const;

    /* Accelerations. */
    virtual SPxErrorCode SetPanAccelDegsPerSecSq(double panAccelDegsPerSecSq);
    virtual SPxErrorCode SetTiltAccelDegsPerSecSq(double tiltAccelDegsPerSecSq);
    virtual SPxErrorCode GetPanAccelDegsPerSecSq(double *panAccelDegsPerSecSq) const;
    virtual SPxErrorCode GetTiltAccelDegsPerSecSq(double *tiltAccelDegsPerSecSq) const;

protected:
    virtual void Update(void);
    void Update(float panDegs, float tiltDegs,
		float *newPanDegs, float *newTiltDegs);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    void update(float panDegs, float tiltDegs,
		float *newPanDegs, float *newTiltDegs);

}; /* SPxCamSimPanTilt */

#endif /* _SPX_CAM_SIM_PAN_TILT_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
