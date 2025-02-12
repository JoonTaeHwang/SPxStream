/*********************************************************************
 *
 * (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx Video Simulator SPxVidSimMount class header.
 *
 * Revision Control:
 *   27/09/22 v1.30   AGC	Support range finders.
 *
 * Previous Changes:
 *   27/09/22 1.29  AGC	Rename from SPxVidSimCameraMount to SPxVidSimMount.
 *   15/09/22 1.28  SP 	Support setting of camera type.
 *   30/08/22 1.27  SP 	Move Z reference support to SPxVidSimPlatform.
 *   05/08/22 1.26  SP 	Rename functions to differentiate from base class functions.
 *   01/08/22 1.25  AGC	Add GetCamCtrlRecv().
 *   22/07/22 1.24  SP 	Changes to simplify creation and deletion.
 *   21/08/18 1.23  AGC	Fix line endings.
 *   20/08/18 1.22  AGC	Connect track database to SPxCamCtrlRecv.
 *   10/08/18 1.21  AGC	Use SPxCamCtrlRecv to process camera commands.
 *                     	Use SPxCamSim... to simulate camera behaviour.
 *   18/07/18 1.20  SP 	Remove legacy support for master camera.
 *   02/07/18 1.19  AGC	Derive from new interface.
 *                     	Make various functions const.
 *   25/06/18 1.18  SP 	Support changes to base class.
 *   01/02/18 1.17  SP 	Add GetReqTiltDegs() and GetReqPanDegs().
 *   18/10/17 1.16  AGC	Add SetAbsPitch/YawSpeed().
 *   10/10/17 1.15  AGC	Add GetPitch/YawSpeed().
 *   31/10/16 1.14  SP 	Support Z reference mode.
 *   07/11/16 1.13  AJH	Support presets.
 *   07/10/16 1.12  SP 	Move events to base class.
 *   18/05/16 1.11  AGC	Support network camera control.
 *   04/11/15 1.10  SP 	Simulate camera mount acceleration.
 *                     	Rename slave to master.
 *   03/11/15 1.9   AGC	Packet decoder callback data now const.
 *   04/09/15 1.8   SP 	Return pan/tilt as read from master.
 *   26/08/15 1.5   SP 	Add Create().
 *   06/08/15 1.4   SP 	Support remote control.
 *   20/06/14 1.3   AGC	Move C++ headers to source file.
 *   04/10/13 1.2   AGC	Simplify headers.
 *   15/07/13 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_VID_SIM_MOUNT_H
#define _SPX_VID_SIM_MOUNT_H

/* Other headers required. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxVidSimMovable.h"
#include "SPxLibData/SPxCamCtrlBase.h"  /* For SPxCameraType */

/*********************************************************************
 *
 *  Constants
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Macros
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Type definitions
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Class definitions
 *
 *********************************************************************/

/* Forward declare any classes required. */
class SPxVidSimPlatform;
class SPxVidSimScene;
class SPxVidSimCamera;
class SPxVidSimRangeFinder;
class SPxCamPanTiltIface;
class SPxCamPresetIface;
class SPxCamCtrlRecv;
class SPxTrackDatabase;

class SPxVidSimMount : public SPxVidSimMovable
{
    /* Grant SPxVidSimPlatform private access. */
    friend class SPxVidSimPlatform;

public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxVidSimMount(SPxVidSimPlatform *platform, 
                   SPxCameraType camType=SPX_CAMERA_TYPE_NONE);
    virtual ~SPxVidSimMount(void);
    SPxErrorCode Initialise(void);

    /* Child access. */
    unsigned int GetNumCameras(void) const { return GetNumChildren(OBJECT_TYPE_CAMERA); }
    SPxVidSimCamera *GetCamera(unsigned int index);
    unsigned int GetNumRangeFinders(void) const { return GetNumChildren(OBJECT_TYPE_RANGE_FINDER); }
    SPxVidSimRangeFinder *GetRangeFinder(unsigned int index);

    /* Pan/tilt control. */
    SPxErrorCode SetTiltDegs(double degs);
    double GetReqTiltDegs(void) const;
    SPxErrorCode SetPanDegs(double degs);
    double GetReqPanDegs(void) const;

    /* Presets. */
    SPxErrorCode GetPreset(UINT8 *preset);
    SPxErrorCode GetPreset(char *preset, unsigned int bufLen);

    /* Access to interfaces. */
    SPxCamPanTiltIface *GetPanTiltIface(void);
    SPxCamPresetIface *GetPresetIface(void);
    SPxCamCtrlRecv *GetCamCtrlRecv(void);

    /* Misc. */
    SPxCameraType GetCamType(void) { return m_camType; }

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

    /* Event handling. */
    virtual void HandleEvent(const EventInfo_t *eventInfo);

private:

    /*
     * Private variables.
     */

    class SimPanTilt;

    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxTrackDatabase *m_trackDB;
    SPxCameraType m_camType;
    SPxCameraType m_camTypeOverride;

    /*
     * Private functions.
     */

    /*
     * Private static functions.
     */

}; /* SPxVidSimMount */

#endif /* _SPX_VID_SIM_MOUNT_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
