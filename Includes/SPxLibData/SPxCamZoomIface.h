/*********************************************************************
*
* (c) Copyright 2018, 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamZoomIface class - an interface for
*	basic zoom camera control.
*
* Revision Control:
*   21/01/19 v1.2    AGC	Support zoom timeout.
*
* Previous Changes:
*   10/08/18 1.1    AGC	Add SetZoomRange().
*   02/07/18 1.0    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_CAM_ZOOM_IFACE_H
#define _SPX_CAM_ZOOM_IFACE_H

#include "SPxLibUtils/SPxTypes.h"
#include "SPxLibUtils/SPxError.h"

class SPxCamZoomIface
{
public:
    /* Construction and destruction. */
    SPxCamZoomIface() {}
    virtual ~SPxCamZoomIface() {}

    /* Absolute positioning. */
    virtual int IsAbsZoomCmdInProgress(void) const = 0;

    virtual int IsAbsZoomSupported(void) const = 0;
    virtual SPxErrorCode SetZoom(float fovDegs) = 0;
    virtual SPxErrorCode GetZoom(float *fovDegs) const = 0;
    virtual SPxErrorCode SetZoomRange(float minFovDegs, float maxFovDegs) = 0;
    virtual SPxErrorCode GetZoomRange(float *minFovDegs, float *maxFovDegs) const = 0;

    /* Relative positioning. */
    virtual int IsRelZoomSupported(void) const = 0;
    virtual SPxErrorCode Zoom(float zoomSpeed, int continuePanTilt=FALSE, double timeoutSecs=0.0) = 0;
    virtual SPxErrorCode GetZoomSpeed(float *zoomSpeed) const = 0;

}; /* SPxCamPanTiltIface */

#endif /* _SPX_CAM_ZOOM_IFACE_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
