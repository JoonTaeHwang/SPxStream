/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for camera offset manager for relative control of a camera
 *   while potential slew-to-cue active.
 *
 * Revision Control:
 *  20/11/20 v1.4    AGC	Separate offset settings for pan/tilt/zoom.
 *
 * Previous Changes:
 *  18/11/20 1.3    AGC	Support callback when offsets change.
 *  11/11/20 1.2    AGC	Support using a second SPxCamera object for offsets.
 *  26/10/20 1.1    AGC	Initial version.
 **********************************************************************/
#ifndef _SPX_CAMERA_OFFSET_MNGR_H
#define _SPX_CAMERA_OFFSET_MNGR_H

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxCameraRelIface.h"

/* Forward declarations. */
class SPxCamera;

class SPxCameraOffsetMngr : public SPxObj, public SPxCameraRelIface
{
public:
    typedef void (*OffsetFn)(void *userArg);

    explicit SPxCameraOffsetMngr(void);
    virtual ~SPxCameraOffsetMngr(void);

    SPxErrorCode SetCamera(SPxCamera *camera, SPxCamera *offsetCamera=NULL);
    SPxCamera *GetCamera(void) const;

    SPxErrorCode SetOffsetCallback(OffsetFn fn, void *userArg);

    SPxErrorCode SetApplyPanOffset(int applyPanOffset);
    int IsApplyingPanOffset(void) const;
    SPxErrorCode SetApplyTiltOffset(int applyTiltOffset);
    int IsApplyingTiltOffset(void) const;
    SPxErrorCode SetApplyZoomOffset(int applyZoomOffset);
    int IsApplyingZoomOffset(void) const;

    /* SPxCameraRelIface interface. */
    virtual SPxErrorCode Home(void) SPX_VIRTUAL_OVERRIDE;
    virtual void Pan(double speed, int isTurbo=FALSE,
		     int speedUp=FALSE) SPX_VIRTUAL_OVERRIDE;
    virtual void Tilt(double speed, int isTurbo=FALSE,
		      int speedUp=FALSE) SPX_VIRTUAL_OVERRIDE;
    virtual void PanTilt(double panSpeed, double tiltSpeed,
			 int isTurbo=FALSE, int speedUp=FALSE) SPX_VIRTUAL_OVERRIDE;
    virtual void Zoom(double speed, int continuePanTilt=FALSE,
		      int isPrimary=TRUE) SPX_VIRTUAL_OVERRIDE;
    virtual void Focus(double speed, int continuePanTilt=FALSE,
		       int isPrimary=TRUE) SPX_VIRTUAL_OVERRIDE;
    virtual SPxErrorCode GoToPreset(const char *preset, int controller=-1,
				    int isPrimary=TRUE) SPX_VIRTUAL_OVERRIDE;

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    void updateOffsetsFromTimer(void);
    void updateOffsets(void);
    static int cameraDeletedWrapper(void *invokingObject,
				    void *userObject,
				    void *arg);
    int cameraDeleted(SPxCamera *camera);
    static int offsetCameraDeletedWrapper(void *invokingObject,
					  void *userObject,
					  void *arg);
    int offsetCameraDeleted(SPxCamera *camera);

}; /* SPxCameraRelIface */

#endif /* _SPX_CAMERA_OFFSET_MNGR_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
