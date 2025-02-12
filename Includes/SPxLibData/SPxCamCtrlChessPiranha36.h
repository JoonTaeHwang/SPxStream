/*********************************************************************
*
* (c) Copyright 2013 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCameraControlChessPiranha36 class - a
*	class for Chess PIRANHA 36 camera contorl.
*
* Revision Control:
*   21/01/19 v1.7    AGC	Add zoom timeout.
*
* Previous Changes:
*   02/02/17 1.6    AGC	Add options for concurrent PTZ.
*   17/02/14 1.5    AGC	Support setting serial port in Create().
*   24/10/13 1.4    AGC	Move to SPx library.
*   28/08/13 1.3    AGC	Do absolute zoom in background thread.
*   23/08/13 1.2    SP 	Add IsAutoFocusEnabled().
*   14/08/13 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_CAM_CTRL_CHESS_PIRANHA_36_H
#define _SPX_CAM_CTRL_CHESS_PIRANHA_36_H

/*
 * Other headers required.
 */

/* For SPxCriticalSection. */
#include "SPxLibUtils/SPxCriticalSection.h"

/* For base class. */
#include "SPxLibData/SPxCamCtrlBase.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxTimer;

/*
 * Define our class.
 */
class SPxCamCtrlChessPiranha36 : public SPxCamCtrlBase
{
public:
    /* Construction and destruction. */
    explicit SPxCamCtrlChessPiranha36(void);
    virtual ~SPxCamCtrlChessPiranha36(void);
    virtual SPxErrorCode Create(const char *serialName=NULL,
				unsigned int serialBaud=2400);

    /* Camera control functions. */
    virtual int IsAbsZoomSupported(void) const { return TRUE; }
    virtual SPxErrorCode SetZoom(float fovDegs);
    virtual SPxErrorCode GetZoom(float *fovDegs) const;

    virtual int IsRelZoomSupported(void) const { return TRUE; }
    virtual SPxErrorCode Zoom(float zoomSpeed, int continuePanTilt=FALSE, double timeoutSecs=0.0);

    virtual int IsRelFocusSupported(void) const { return TRUE; }
    virtual SPxErrorCode Focus(float focusSpeed, int continuePanTilt=FALSE);

    virtual int IsAutoFocusSupported(void) const { return TRUE; }
    virtual SPxErrorCode AutoFocus(int autoFocus);
    virtual int IsAutoFocusEnabled(void) const { return m_autoFocus; }

    SPxErrorCode SetEnableCrossHair(int enable);
    int IsCrossHairEnabled(void) const;

    SPxErrorCode SetEnableIRFilter(int enable);
    int IsIRFilterEnabled(void) const;

private:
    /* Private variables. */
    mutable SPxCriticalSection m_mutex;	/* Mutex protection. */
    SPxAutoPtr<SPxTimer> m_timer;   /* Timer for sending messages at 50Hz. */
    int m_configReceived;	/* Has the initial config been received? */
    float m_zoomSpeed;		/* +ve for zoom in, -ve for zoom out, 0.0 for no zooming. */
    float m_zoomPos;		/* Current zoom position. */
    float m_reqZoomPos;		/* Requested zoom position. */
    int m_reqZoomPosUpdate;	/* Zoom position update required. */
    int m_autoFocus;		/* TRUE for auto-focus, FALSE for manual focus. */
    float m_focusSpeed;		/* +ve for focus far, -ve for focus near, 0.0 for no change of focus. */
    int m_infraRedFilterIn;	/* TRUE for IR filter in, FALSE for IR filter out. */
    int m_autoExposure;		/* TRUE for auto exposure, FALSE for manual exposure. */
    float m_shutterSpeed;	/* Shutter speed in seconds. */
    int m_displayCrossHair;	/* TRUE to display cross-hair, FALSE to hide. */
    float m_irisControl;	/* Iris control (F values). */
    float m_focusRate;		/* Focus rate in seconds. */
    mutable int m_updatePending;/* Is a status update pending? */

    /* Private functions. */
    static unsigned char calcChecksum(const unsigned char *payload,
				      unsigned int payloadBytes);
    void updateZoom(void);
    static void timerFnWrapper(void *userArg);
    void timerFn(void);
    virtual unsigned int SerialPortFn(const unsigned char *payload,
				      unsigned int payloadBytes);

}; /* SPxCamCtrlChessPiranha36 */

#endif /* _SPX_CAM_CTRL_CHESS_PIRANHA_36_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
