/*********************************************************************
*
* (c) Copyright 2018, 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamSimZoomFocus class - a class
*	for simulating the zoom/focus behaviour of a camera.
*
* Revision Control:
*   21/01/19 v1.3    AGC	Support zoom timeout.
*
* Previous Changes:
*   13/08/18 1.2    AGC	Enhance relative focus support.
*   10/08/18 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_CAM_SIM_ZOOM_FOCUS_H
#define _SPX_CAM_SIM_ZOOM_FOCUS_H

/* For base classes. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibData/SPxCamZoomIface.h"
#include "SPxLibData/SPxCamFocusIface.h"

/* Forward declarations. */

class SPxCamSimZoomFocus : public SPxObj, public SPxCamZoomIface, public SPxCamFocusIface
{
public:
    SPxCamSimZoomFocus(void);
    virtual ~SPxCamSimZoomFocus(void);

    /* Absolute zoom positioning. */
    virtual int IsAbsZoomCmdInProgress(void) const;

    virtual SPxErrorCode SetAbsZoomSupported(int supported);
    virtual int IsAbsZoomSupported(void) const;
    virtual SPxErrorCode SetZoom(float fovDegs);
    virtual SPxErrorCode GetZoom(float *fovDegs) const;
    virtual SPxErrorCode SetZoomRange(float minFovDegs, float maxFovDegs);
    virtual SPxErrorCode GetZoomRange(float *minFovDegs, float *maxFovDegs) const;

    /* Relative zoom positioning. */
    virtual SPxErrorCode SetRelZoomSupported(int supported);
    virtual int IsRelZoomSupported(void) const;
    virtual SPxErrorCode Zoom(float zoomSpeed, int continuePanTilt = FALSE, double timeoutSecs = 0.0);
    virtual SPxErrorCode GetZoomSpeed(float *zoomSpeed) const;

    virtual SPxErrorCode SetMaxZoomSpeedDegsPerSec(float maxZoomSpeedDegsPerSec);
    virtual SPxErrorCode GetMaxZoomSpeedDegsPerSec(float *maxZoomSpeedDegsPerSec) const;

    /* Absolute focus positioning. */
    virtual int IsAbsFocusCmdInProgress(void) const;

    virtual SPxErrorCode SetAbsFocusSupported(int supported);
    virtual int IsAbsFocusSupported(void) const;
    virtual SPxErrorCode SetFocus(float rangeMetres);
    virtual SPxErrorCode GetFocus(float *rangeMetres) const;
    virtual SPxErrorCode SetFocusRangeMetres(float minRangeMetres, float maxRangeMetres);
    virtual SPxErrorCode GetFocusRangeMetres(float *minRangeMetres, float *maxRangeMetres) const;
    virtual SPxErrorCode SetFocusPos(UINT16 focusPos);
    virtual SPxErrorCode GetFocusPos(UINT16 *focusPos) const;
    virtual SPxErrorCode GetFocusRange(UINT16 *minFocusPos, UINT16 *maxFocusPos) const;

    /* Relative focus positioning. */
    virtual SPxErrorCode SetRelFocusSupported(int supported);
    virtual int IsRelFocusSupported(void) const;
    virtual SPxErrorCode Focus(float focusSpeed, int continuePanTilt = FALSE);

    virtual SPxErrorCode SetAutoFocusSupported(int supported);
    virtual int IsAutoFocusSupported(void) const;
    virtual SPxErrorCode AutoFocus(int autoFocus);
    virtual int IsAutoFocusEnabled(void) const;

    virtual SPxErrorCode SetMaxFocusSpeedMetresPerSec(float maxFocusSpeedMetresPerSec);
    virtual SPxErrorCode GetMaxFocusSpeedMetresPerSec(float *maxFocusSpeedMetresPerSec) const;

protected:
    virtual void Update(void);

    int UpdateZoom(void);
    int UpdateFocus(void);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    int updateZoom(void);
    int updateFocus(void);

    float getFOVForZoom(UINT16 zoomPos) const;
    UINT16 getZoomForFOV(float fovDegs) const;

}; /* SPxCamSimZoomFocus */

#endif /* _SPX_CAM_SIM_ZOOM_FOCUS_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
