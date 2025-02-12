/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamCtrlLRAD class - a class
*	to allow pan/tilt control of an LRAD platform.
*
* Revision Control:
*   03/07/19 v1.6   BTB	Fix issue with wrongly overriding pan/tilt targets.
*
* Previous Changes:
*   25/06/19 1.5   BTB	Improve sending of stop and get position commands.
*   24/06/19 1.4   BTB	Automatically try to reconnect if necessary.
*   21/06/19 1.3   BTB	Fix minimum command interval.
*   07/02/19 1.2   BTB	Handle address change, clip pan/tilt to soft limits.
*   06/02/19 1.1   BTB	Initial Version.
**********************************************************************/
#ifndef _SPX_CAM_CTRL_LRAD_H
#define _SPX_CAM_CTRL_LRAD_H

/*
* Other headers required.
*/

/* For base class. */
#include "SPxLibData/SPxCamCtrlBase.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/*
* Define our class.
*/
class SPxCamCtrlLRAD : public SPxCamCtrlBase
{
public:
    /* Construction and destruction. */
    SPxCamCtrlLRAD(void);
    virtual ~SPxCamCtrlLRAD(void);

    /* Base class functions. */

    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetMesgType(SPxMesgIOType_t mesgType);

    virtual int IsSerialSupported(void) const { return FALSE; }
    virtual int IsNetworkSupported(void) const { return TRUE; }
    virtual SPxErrorCode SetAddress(const char *address, int port = 0, const char *ifAddr=NULL);
    virtual SPxErrorCode SetAddress(UINT32 address, int port = 0, const char *ifAddr=NULL);

    /* Camera control functions. */
    virtual SPxErrorCode Stop(void);

    /* Absolute positioning. */
    virtual int IsAbsCmdInProgress(UINT32 dirMask) const;

    virtual int IsAbsPanSupported(void) const { return TRUE; }
    virtual SPxErrorCode SetPan(float panAngleDegs);
    virtual SPxErrorCode GetPan(float *panAngleDegs) const;
    virtual SPxErrorCode GetLastPan(float *panAngleDegs) const;

    virtual int IsAbsTiltSupported(void) const { return TRUE; }
    virtual SPxErrorCode SetTilt(float tiltAngleDegs);
    virtual SPxErrorCode GetTilt(float *tiltAngleDegs) const;
    virtual SPxErrorCode GetLastTilt(float *tiltAngleDegs) const;

    virtual int IsAbsGetPTZSupported(void) const { return TRUE; }
    virtual SPxErrorCode GetPTZ(float *panAngleDegs,
				float *tiltAngleDegs,
				float *fovDegs) const;

    /* Relative positioning. */
    virtual int IsRelPanSupported(void) const { return TRUE; }
    virtual SPxErrorCode Pan(float panSpeed, float *panSpeedSet=NULL,
			     int continueZoomFocus=FALSE,
			     double timeoutSecs=0.0);

    virtual int IsRelTiltSupported(void) const { return TRUE; }
    virtual SPxErrorCode Tilt(float tiltSpeed, float *tiltSpeedSet=NULL,
			      int continueZoomFocus=FALSE,
			      double timeoutSecs=0.0);

    virtual int IsRelPanTiltSupported(void) const { return TRUE; }
    virtual SPxErrorCode PanTilt(float panSpeed, float tiltSpeed,
				 float *panSpeedSet=NULL, float *tiltSpeedSet=NULL,
				 int continueZoomFocus=FALSE,
				 double timeoutSecs=0.0);

    virtual UINT32 GetLastCmdTime(void) const;

    /* Minimum command interval. */
    virtual UINT32 GetMinCmdIntervalMsecs(void) const { return 20; }

protected:
    // cppcheck-suppress virtualCallInConstructor
    virtual void Shutdown(void);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    void clearSettings(void);
    SPxErrorCode getSoftLimit(UINT8 softLimit) const;
    SPxErrorCode sendStopCmd(void);
    SPxErrorCode sendGetPosCmd(int updateTargetPos = TRUE) const;
    SPxErrorCode sendRecvCommand(int numCmdBytes, int updateTargetPos = TRUE) const;
    SPxErrorCode processResponse(int numBytes, int updateTargetPos) const;

    const char *getErrorString(const UINT8 errorCode) const;
    const char *getCmdNumString(const UINT8 cmdNum) const;

}; /* SPxCamCtrlLRAD */

#endif /* _SPX_CAM_CTRL_LRAD_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
