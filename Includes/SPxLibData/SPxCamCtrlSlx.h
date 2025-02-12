/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamCtrlSlx class - a library
*	class for Leonardo SLX-III camera control.
*
* Revision Control:
*   31/05/22 v1.4    AGC	Support configurable command timeout.
*
* Previous Changes:
*   27/05/22 1.3    AGC	Add mutex protection to reply buffer.
*   29/04/22 1.2    AGC	Add auto-focus suppport.
*   27/04/22 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_CAM_CTRL_SLX_H
#define _SPX_CAM_CTRL_SLX_H

/* For SPxCriticalSection. */
#include "SPxLibUtils/SPxCriticalSection.h"

/* For SPxEvent. */
#include "SPxLibUtils/SPxEvents.h"

/* For base class. */
#include "SPxLibData/SPxCamCtrlBase.h"

class SPxCamCtrlSlx : public SPxCamCtrlBase
{
public:
    /* Construction and destruction. */
    explicit SPxCamCtrlSlx(void);
    virtual ~SPxCamCtrlSlx(void);

    virtual int IsAbsZoomSupported(void) const { return TRUE; }
    virtual SPxErrorCode SetZoom(float fovDegs);
    virtual SPxErrorCode GetZoom(float *fovDegs) const;
    virtual SPxErrorCode GetLastZoom(float *fovDegs) const;
    virtual SPxErrorCode SetZoomPos(UINT16 zoomPos);
    virtual SPxErrorCode GetZoomPos(UINT16 *zoomPos) const;
    virtual SPxErrorCode SetZoomRange(float minFovDegs, float maxFovDegs);
    virtual SPxErrorCode GetZoomRange(float *minFovDegs, float *maxFovDegs) const;
    virtual SPxErrorCode GetMaxZoomPosition(UINT16 *pos) const;
    virtual SPxErrorCode SetZoomLUTFilename(const char *filename);
    virtual SPxErrorCode GetZoomLUTFilename(char *buffer, int bufLen) const;
    
    virtual int IsAbsFocusSupported(void) const { return TRUE; }
    virtual SPxErrorCode SetFocus(float rangeMetres);
    virtual SPxErrorCode GetFocus(float *rangeMetres) const;
    virtual SPxErrorCode GetLastFocus(float *rangeMetres) const;

    virtual int IsRelZoomSupported(void) const { return TRUE; }
    virtual SPxErrorCode Zoom(float zoomSpeed, int continuePanTilt=FALSE,
			      double timeoutSecs=0.0);

    virtual int IsRelFocusSupported(void) const { return TRUE; }
    virtual SPxErrorCode Focus(float focusSpeed, int continuePanTilt=FALSE);

    virtual int IsAutoFocusSupported(void) const { return TRUE; }
    virtual SPxErrorCode AutoFocus(int autoFocus);
    virtual int IsAutoFocusOneshotSupported(void) const { return TRUE; }
    virtual SPxErrorCode AutoFocusOneshot(void);
    virtual int IsAutoFocusEnabled(void) const;

    virtual UINT32 GetMinCmdIntervalMsecs(void) const { return 20; }

    SPxErrorCode SetAutoFocusOneshotFullRange(int fullRange);
    int IsAutoFocusOneshotFullRange(void) const;

    SPxErrorCode SetCommandTimeout(UINT32 ms);
    UINT32 GetCommandTimeout(void) const;

    /* Generic get/set parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /* Private variables. */
    mutable SPxCriticalSection m_mutex;
    mutable SPxCriticalSection m_msgReplyMutex;
    mutable SPxEvent m_ev;
    mutable char m_msgReply[16];
    float m_minFovDegs;
    float m_maxFovDegs;
    mutable UINT16 m_zoomPos;
    mutable float m_focusRangeMetres;
    int m_autoFocusOneShotFullRange;
    UINT32 m_commandTimeoutMS;

    /* Private functions. */
    virtual unsigned int SerialPortFn(const unsigned char *payload,
				      unsigned int payloadBytes);
    SPxErrorCode sendRecvCommand(const char *cmd,
				 const char *val=NULL,
				 char *replyBuf=NULL,
				 unsigned int replyBufLen=0) const;
    SPxErrorCode setZoomRange(float minFovDegs, float maxFovDegs);
    float getFOVForZoom(UINT16 zoomPos) const;
    UINT16 getZoomForFOV(float fovDegs) const;

}; /* SPxCamCtrlSlx */

#endif /* _SPX_CAM_CTRL_SLX_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
