/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxCameraRelIface interface.
*
* Revision Control:
*   26/10/20 v1.1    AGC	Initial version.
*
* Previous Changes:
**********************************************************************/
#ifndef _SPX_CAMERA_REL_IFACE_H
#define _SPX_CAMERA_REL_IFACE_H

#include "SPxLibUtils/SPxError.h"

class SPxCameraRelIface
{
public:
    explicit SPxCameraRelIface(void) {}
    virtual ~SPxCameraRelIface(void) {}

    virtual SPxErrorCode Home(void)=0;

    virtual void Pan(double speed, int isTurbo=FALSE, int speedUp=FALSE)=0;
    virtual void Tilt(double speed, int isTurbo=FALSE, int speedUp=FALSE)=0;
    virtual void PanTilt(double panSpeed, double tiltSpeed,
			 int isTurbo=FALSE, int speedUp=FALSE)=0;
    virtual void Zoom(double speed, int continuePanTilt=FALSE, int isPrimary=TRUE)=0;
    virtual void Focus(double speed, int continuePanTilt=FALSE, int isPrimary=TRUE)=0;

    virtual SPxErrorCode GoToPreset(const char *preset, int controller=-1,
				    int isPrimary=TRUE)=0;

}; /* SPxCameraRelIface */

#endif /* _SPX_CAMERA_REL_IFACE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
