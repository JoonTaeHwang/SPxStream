/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamCtrlSkyWall class - a library
*	class for SkyWall camera control.
*
* Revision Control:
*   04/03/22 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/
#ifndef _SPX_CAM_CTRL_SKY_WALL_H
#define _SPX_CAM_CTRL_SKY_WALL_H

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For base class. */
#include "SPxLibData/SPxCamCtrlBase.h"

/*********************************************************************
*
*   Type/constant definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxSkyWallConn;

/*
 * Define our class.
 */
class SPxCamCtrlSkyWall : public SPxCamCtrlBase
{
public:
    /* Construction and destruction. */
    explicit SPxCamCtrlSkyWall(SPxSkyWallConn *conn);
    virtual ~SPxCamCtrlSkyWall(void);

    virtual SPxMesgIOType_t GetMesgType(void) const { return SPX_MESG_IO_TYPE_NETWORK_SERVER; }
    virtual int IsSerialSupported(void) const { return FALSE; }
    virtual int IsNetworkSupported(void) const { return TRUE; }
    virtual SPxErrorCode SetAddress(const char *address, int port=0, const char *ifAddr=NULL);
    virtual SPxErrorCode SetAddress(UINT32 address, int port=0, const char *ifAddr=NULL);
    virtual UINT32 GetAddress(void) const;
    virtual const char *GetAddressString(void) const;
    virtual int GetPort(void) const;
    virtual UINT32 GetIfAddress(void) const;
    virtual const char *GetIfAddressString(void) const;
    virtual int IsTCP(void) const { return TRUE; }

    virtual int IsConnected(void) const;

    virtual int IsSecondarySupported(void) const { return TRUE; }

    virtual int IsAbsPanSupported(void) const { return IsConnected(); }
    virtual SPxErrorCode SetPan(float panAngleDegs);
    virtual SPxErrorCode GetPan(float *panAngleDegs) const;
    virtual SPxErrorCode GetLastPan(float *panAngleDegs) const;
    virtual SPxErrorCode GetPanRange(float *minPanAngleDegs, float *maxPanAngleDegs) const;

    virtual int IsAbsTiltSupported(void) const { return IsConnected(); }
    virtual SPxErrorCode SetTilt(float tiltAngleDegs);
    virtual SPxErrorCode GetTilt(float *tiltAngleDegs) const;
    virtual SPxErrorCode GetLastTilt(float *tiltAngleDegs) const;
    virtual SPxErrorCode GetTiltPos(UINT16 *tiltPos) const;
    virtual SPxErrorCode GetTiltRange(float *minTiltAngleDegs, float *maxTiltAngleDegs) const;

    virtual int IsAbsZoomSupported(void) const { return IsConnected(); }
    virtual SPxErrorCode SetZoom(float fovDegs, int isPrimary);
    virtual SPxErrorCode GetZoom(float *fovDegs, int isPrimary) const;
    virtual SPxErrorCode GetLastZoom(float *fovDegs, int isPrimary) const;
    
    virtual int IsAbsFocusSupported(void) const { return IsConnected(); }
    virtual SPxErrorCode SetFocus(float rangeMetres, int isPrimary);
    virtual SPxErrorCode GetFocus(float *rangeMetres, int isPrimary) const;
    virtual SPxErrorCode GetLastFocus(float *rangeMetres, int isPrimary) const;

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

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

}; /* SPxCamCtrlSkyWall */

#endif /* _SPX_CAM_CTRL_SKY_WALL_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
