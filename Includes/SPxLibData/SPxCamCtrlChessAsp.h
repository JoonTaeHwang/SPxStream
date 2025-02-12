/*********************************************************************
*
* (c) Copyright 2013 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxAVCameraControlChessAsp class - an AV library
*	class for Chess ASP camera contorl.
*
* Revision Control:
*   21/01/19 v1.7    AGC	Add pan/tilt timeouts.
*
* Previous Changes:
*   02/02/17 1.6    AGC	Add options for concurrent PTZ.
*   03/11/15 1.5    AGC	Allow retrieval of actual pan/tilt speeds.
*   23/06/15 1.4    AGC	Add direction specific absolute command in progress checks.
*   24/10/13 1.3    AGC	Move to SPx library.
*   28/08/13 1.2    AGC	Improve absolute command in progress logic.
*   14/08/13 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_CAM_CTRL_CHESS_ASP_H
#define _SPX_CAM_CTRL_CHESS_ASP_H

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

/* Forward declarations. */

/*
 * Define our class.
 */
class SPxCamCtrlChessAsp : public SPxCamCtrlBase
{
public:
    /* Construction and destruction. */
    explicit SPxCamCtrlChessAsp(void);
    virtual ~SPxCamCtrlChessAsp(void);

    virtual SPxErrorCode SelfReference(void);

    /* Camera control functions. */
    virtual int IsAbsCmdInProgress(UINT32 dirMask=SPX_CAMERA_DIR_ALL) const;

    virtual int IsAbsPanSupported(void) const { return TRUE; }
    virtual SPxErrorCode SetPan(float panAngleDegs);
    virtual SPxErrorCode GetPan(float *panAngleDegs) const;
    virtual SPxErrorCode GetPanRange(float *minPanAngleDegs, float *maxPanAngleDegs) const;

    virtual int IsAbsTiltSupported(void) const { return TRUE; }
    virtual SPxErrorCode SetTilt(float tiltAngleDegs);
    virtual SPxErrorCode GetTilt(float *tiltAngleDegs) const;
    virtual SPxErrorCode GetTiltRange(float *minTiltAngleDegs, float *maxTiltAngleDegs) const;

    virtual int IsRelPanSupported(void) const { return TRUE; }
    virtual SPxErrorCode Pan(float panSpeed, float *panSpeedSet=NULL,
			     int continueZoomFocus=FALSE,
			     double timeoutSecs=0.0);

    virtual int IsRelTiltSupported(void) const { return TRUE; }
    virtual SPxErrorCode Tilt(float tiltSpeed, float *tiltSpeedSet=NULL,
			      int continueZoomFocus=FALSE,
			      double timeoutSecs=0.0);

private:
    /* Private variables. */
    mutable int m_selfRefInProgress;
    mutable int m_panInProgress;
    mutable int m_tiltInProgress;
    mutable UINT8 m_msgRequired;
    mutable int m_responseReceived;
    int m_selfReferenced;
    float m_panAngleDegs;
    float m_tiltAngleDegs;

    /* Private functions. */
    virtual unsigned int SerialPortFn(const unsigned char *payload, unsigned int payloadBytes);
    SPxErrorCode sendCommand(const unsigned char *command, unsigned int numBytes) const;
    SPxErrorCode waitForResponse(UINT32 timeoutMS) const;
    SPxErrorCode recvCommand(unsigned char *buffer, unsigned int bufLen, UINT32 timeoutMS) const;
    SPxErrorCode recvAck(unsigned char command) const;
    SPxErrorCode recvComplete(unsigned char command, UINT32 timeoutMS=0) const;
    static UINT8 calcChecksum(const unsigned char *msg, unsigned int msgBytes);

}; /* SPxCamCtrlChessAsp */

#endif /* _SPX_CAM_CTRL_CHESS_ASP_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
