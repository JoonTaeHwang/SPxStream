/*********************************************************************
*
* (c) Copyright 2014 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamCtrlTest class - a class used to
*       simulate a real camera for testing software.
*
* Revision Control:
*   21/01/19 v1.13   AGC	Add pan/tilt/zoom timeouts.
*
* Previous Changes:
*   25/06/18 1.12   AGC	Correctly suppress cppcheck warning.
*   08/03/17 1.11   SP 	Add some more missing overrides.
*   21/02/17 1.10   SP 	Add missing GetLast...() overrides.
*   02/02/17 1.9    AGC	Add options for concurrent PTZ.
*   23/08/16 1.8    AGC	Add functions for testing.
*			Add mutex protection.
*   08/04/16 1.7    AGC	Add new forms of GetSerialName/Baud().
*   10/12/15 1.6    AGC	Add absolute direction disabling.
*   03/11/15 1.5    AGC	Allow retrieval of actual pan/tilt speeds.
*   23/06/15 1.4    AGC	Add direction specific absolute command in progress checks.
*   06/03/15 1.3    AGC	Support for automated testing.
*   14/04/14 1.2    SP 	Support dummy serial port.
*   27/03/14 1.1    SP 	Initial version.
*
**********************************************************************/
#ifndef _SPX_CAM_CTRL_TEST_H
#define _SPX_CAM_CTRL_TEST_H

/*
 * Other headers required.
 */

/* For SPxCriticalSection class. */
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxThreads.h"

/* For base class. */
#include "SPxLibData/SPxCamCtrlBase.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

#define SPX_CAM_TEST_SPEED_MIN	  (0x00)
#define SPX_CAM_TEST_SPEED_MAX	  (0x3F)
#define SPX_CAM_TEST_SPEED_TURBO  (0xFF)
#define SPX_CAM_TEST_SPEED_MIN_F (static_cast<float>(SPX_CAM_TEST_SPEED_MIN))
#define SPX_CAM_TEST_SPEED_MAX_F  (static_cast<float>(SPX_CAM_TEST_SPEED_MAX))
#define SPX_CAM_TEST_SPEED_TURBO_F (static_cast<float>(SPX_CAM_TEST_SPEED_TURBO))

/*
 * Define our class.
 */
class SPxCamCtrlTest : public SPxCamCtrlBase
{
public:
    /* Construction and destruction. */
    SPxCamCtrlTest(void);
    virtual ~SPxCamCtrlTest(void);

    /* Camera control functions. */
    SPxErrorCode Move(int horizSpeed, int vertSpeed, int zoomSpeed=0);
    SPxErrorCode MoveUp(UINT8 speed=SPX_CAM_TEST_SPEED_MAX);
    SPxErrorCode MoveDown(UINT8 speed=SPX_CAM_TEST_SPEED_MAX);
    SPxErrorCode MoveLeft(UINT8 speed=SPX_CAM_TEST_SPEED_MAX);
    SPxErrorCode MoveRight(UINT8 speed=SPX_CAM_TEST_SPEED_MAX);
    SPxErrorCode MoveUpLeft(UINT8 speed=SPX_CAM_TEST_SPEED_MAX) { return MoveUpLeft(speed, speed); }
    SPxErrorCode MoveUpRight(UINT8 speed=SPX_CAM_TEST_SPEED_MAX) { return MoveUpRight(speed, speed); }
    SPxErrorCode MoveDownLeft(UINT8 speed=SPX_CAM_TEST_SPEED_MAX) { return MoveDownLeft(speed, speed); }
    SPxErrorCode MoveDownRight(UINT8 speed=SPX_CAM_TEST_SPEED_MAX) { return MoveDownRight(speed, speed); }
    SPxErrorCode MoveUpLeft(UINT8 hSpeed, UINT8 vSpeed);
    SPxErrorCode MoveUpRight(UINT8 hSpeed, UINT8 vSpeed);
    SPxErrorCode MoveDownLeft(UINT8 hSpeed, UINT8 vSpeed);
    SPxErrorCode MoveDownRight(UINT8 hSpeed, UINT8 vSpeed);
    SPxErrorCode ResetPan(void);
    SPxErrorCode ZoomIn(void);
    SPxErrorCode ZoomIn(UINT8 speed);
    SPxErrorCode ZoomOut(void);
    SPxErrorCode ZoomOut(UINT8 speed);
    SPxErrorCode FocusNear(void);
    SPxErrorCode FocusNear(UINT8 speed);
    SPxErrorCode FocusFar(void);
    SPxErrorCode FocusFar(UINT8 speed);
    SPxErrorCode Stop(void);

    /* Base class functions. */

    /* Settings. */
    virtual SPxErrorCode SetSerialName(const char *name);
    virtual SPxErrorCode GetSerialName(char *name, unsigned int bufLen) const;
    virtual const char *GetSerialName(void) const;
    virtual SPxErrorCode SetSerialBaud(unsigned int baud);
    virtual SPxErrorCode GetSerialBaud(unsigned int *baud) const;
    virtual unsigned int GetSerialBaud(void) const;

    /* Self reference. */
    virtual SPxErrorCode SelfReference(void);

    /* Absolute positioning. */
    virtual int IsAbsCmdInProgress(UINT32 dirMask=SPX_CAMERA_DIR_ALL) const;

    virtual int IsAbsPanSupported(void) const;
    virtual SPxErrorCode SetPan(float panAngleDegs);
    virtual SPxErrorCode GetPan(float *panAngleDegs) const;
    virtual SPxErrorCode GetLastPan(float *panAngleDegs) const;

    virtual int IsAbsTiltSupported(void) const;
    virtual SPxErrorCode SetTilt(float tiltAngleDegs);
    virtual SPxErrorCode GetTilt(float *tiltAngleDegs) const;
    virtual SPxErrorCode GetLastTilt(float *tiltAngleDegs) const;

    virtual int IsAbsZoomSupported(void) const;
    virtual SPxErrorCode SetZoom(float fovDegs);
    virtual SPxErrorCode GetZoom(float *fovDegs) const;
    virtual SPxErrorCode GetLastZoom(float *fovDegs) const;
    virtual SPxErrorCode SetZoomRange(float minFovDegs, float maxFovDegs);
    virtual SPxErrorCode GetZoomRange(float *minFovDegs, float *maxFovDegs) const;
    virtual SPxErrorCode SetMaxZoomPosition(UINT16 /*pos*/) { return SPX_NO_ERROR; }
    virtual SPxErrorCode SetQueriedZoomIsPercentage(int /*state*/) { return SPX_NO_ERROR; }

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

    virtual int IsRelZoomSupported(void) const { return TRUE; }
    virtual SPxErrorCode Zoom(float zoomSpeed, int continuePanTilt=FALSE,
			      double timeoutSecs=0.0);

    virtual int IsRelFocusSupported(void) const { return TRUE; }
    virtual SPxErrorCode Focus(float focusSpeed, int continuePanTilt=FALSE);

    virtual int IsAutoFocusSupported(void) const { return TRUE; }
    virtual SPxErrorCode AutoFocus(int autoFocus);
    virtual int IsAutoFocusEnabled(void) const;

    /* Generic get/set parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    /* Functions for testing. */
    explicit SPxCamCtrlTest(int startThread);
    void StartThread(void);
    void UpdateCamera(void);
    // cppcheck-suppress virtualCallInConstructor
    virtual void Shutdown(void);

private:
    /* Private variables. */
    SPxThread *m_thread;
    mutable SPxCriticalSection m_mutex;
    float m_reqPanDegs;
    float m_panDegs;
    float m_reqTiltDegs;
    float m_tiltDegs;
    float m_reqFovDegs;
    float m_fovDegs;
    float m_minFovDegs;
    float m_maxFovDegs;
    int m_panSpeed;
    int m_tiltSpeed;
    int m_zoomSpeed;
    int m_focusSpeed;
    int m_isAbsCmdInProgress;
    int m_isAutoFocusEnabled;
    const char *m_serialName;
    unsigned int m_serialBaud;

    /* Private functions. */
    void init(int startThread);

    /* Private static functions. */
    static void *ThreadHandlerWrapper(SPxThread *thread);
    virtual void ThreadHandler(SPxThread *thread);

}; /* SPxCamCtrlTest */

#endif /* _SPX_CAM_CTRL_TEST_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
