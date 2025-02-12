/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamCtrlFlirE class - a library
*	class for FLIR PTU E-Series camera control.
*
*	This controller only supports Pan and Tilt control.
*
* Revision Control:
*   09/09/22 v1.6    AGC	Support user configurable pan/tilt speed limits.
*
* Previous Changes:
*   31/05/22 1.5    AGC	Support configurable command timeout.
*   20/05/22 1.4    AGC	Account for step mode.
*   20/05/22 1.3    AGC	Suppress spurious cppcheck warning.
*   19/05/22 1.2    AGC	Fix absolute command completion detection.
*   18/05/22 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_CAM_CTRL_FLIR_E_H
#define _SPX_CAM_CTRL_FLIR_E_H

/* For SPxCriticalSection. */
#include "SPxLibUtils/SPxCriticalSection.h"

/* For SPxEvent. */
#include "SPxLibUtils/SPxEvents.h"

/* For base class. */
#include "SPxLibData/SPxCamCtrlBase.h"

class SPxCamCtrlFlirE : public SPxCamCtrlBase
{
public:
    /* Construction and destruction. */
    explicit SPxCamCtrlFlirE(void);
    virtual ~SPxCamCtrlFlirE(void);
    
    virtual int IsAbsCmdInProgress(UINT32 dirMask=SPX_CAMERA_DIR_ALL) const;
    
    virtual int IsAbsPanSupported(void) const { return TRUE; }
    virtual SPxErrorCode SetPan(float panAngleDegs);
    virtual SPxErrorCode GetPan(float *panAngleDegs) const;
    virtual SPxErrorCode GetLastPan(float *panAngleDegs) const;

    virtual int IsAbsTiltSupported(void) const { return TRUE; }
    virtual SPxErrorCode SetTilt(float tiltAngleDegs);
    virtual SPxErrorCode GetTilt(float *tiltAngleDegs) const;
    virtual SPxErrorCode GetLastTilt(float *tiltAngleDegs) const;

    virtual int IsRelPanSupported(void) const { return TRUE; }
    virtual SPxErrorCode Pan(float panSpeed, float *panSpeedSet=NULL,
			     int continueZoomFocus=FALSE,
			     double timeoutSecs=0.0);

    virtual int IsRelTiltSupported(void) const { return TRUE; }
    virtual SPxErrorCode Tilt(float tiltSpeed, float *tiltSpeedSet=NULL,
			      int continueZoomFocus=FALSE,
			      double timeoutSecs=0.0);
    
    virtual int AreMaxSpeedsSupported(void) const { return TRUE; }
    virtual SPxErrorCode GetMaxPanTiltSpeedsDegsPerSec(double *maxPanSpeedDegsPerSec,
						       double *maxTiltSpeedDegsPerSec) const;

    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode Stop(void);
    virtual UINT32 GetMinCmdIntervalMsecs(void) const { return 20; }

    virtual SPxErrorCode GoToPreset(UINT8 id);
    virtual SPxErrorCode SetPreset(UINT8 id);
    virtual SPxErrorCode ClearPreset(UINT8 id);

    SPxErrorCode SetCommandTimeout(UINT32 ms);
    UINT32 GetCommandTimeout(void) const;

    SPxErrorCode SetUserMaxPanSpeed(float maxPanSpeedPosPerSec);
    float GetUserMaxPanSpeed(void) const;
    SPxErrorCode SetUserMaxTiltSpeed(float maxTiltSpeedPosPerSec);
    float GetUserMaxTiltSpeed(void) const;

    /* Generic get/set parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /* Private variables. */
    mutable SPxCriticalSection m_mutex;
    mutable SPxEvent m_ev;
    mutable char m_msgReply[256];
    mutable float m_msgReplyVal;
    mutable int m_resValid;
    mutable float m_panSecsPerPos;
    mutable float m_tiltSecsPerPos;
    mutable float m_panResAdj;
    mutable float m_tiltResAdj;
    mutable float m_minPanSpeed;
    mutable float m_maxPanSpeed;
    float m_userMaxPanSpeed;
    mutable float m_minTiltSpeed;
    mutable float m_maxTiltSpeed;
    float m_userMaxTiltSpeed;
    mutable float m_lastPanPos;
    mutable float m_lastTiltPos;
    mutable int m_isAbsPanInProgress;
    mutable int m_isAbsTiltInProgress;
    float m_reqPanPos;
    UINT32 m_reqPanTime;
    float m_reqTiltPos;
    UINT32 m_reqTiltTime;
    UINT32 m_commandTimeoutMS;

    /* Private functions. */
    virtual unsigned int SerialPortFn(const unsigned char *payload,
				      unsigned int payloadBytes);
    SPxErrorCode sendRecvCommand(const char *cmd,
				 const char *val=NULL) const;
    SPxErrorCode checkRes(void) const;
    float panAngleToPos(float panAngleDegs) const;
    float panPosToAngle(float panPos) const;
    float tiltAngleToPos(float tiltAngleDegs) const;
    float tiltPosToAngle(float tiltPos) const;
    float panSpeedToPos(float panSpeed) const;
    float panPosToSpeed(float panPos) const;
    float tiltSpeedToPos(float tiltSpeed) const;
    float tiltPosToSpeed(float tiltPos) const;

}; /* SPxCamCtrlSlx */

#endif /* _SPX_CAM_CTRL_FLIR_E_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
