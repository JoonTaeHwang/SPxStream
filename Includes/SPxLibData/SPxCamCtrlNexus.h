/*********************************************************************
*
* (c) Copyright 2017 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamCtrlNexus class - a class
*	to allow controlling a camera with a Nexus.CGI interface.
*
* Revision Control:
*   21/01/19 v1.20   AGC	Add pan/tilt/zoom timeouts.
*
* Previous Changes:
*   10/10/18 1.19   AGC	Support zoom LUT for thermal camera.
*   10/10/18 1.18   AGC	Improve auto-focus support reporting.
*   31/07/18 1.17   AGC	Add IsSecondarySupported().
*   25/06/18 1.16  AGC	Correctly suppress cppcheck warning.
*   07/06/18 1.15  AJH	Add preset for one-shot auto-focus.
*   06/06/18 1.14  AJH	Add support for one-shot auto-focus.
*   31/05/18 1.13  AJH	Add variables for max speeds set from config.
*   15/05/18 1.12  AJH	Support subsystem-on preset.
*   17/04/18 1.11  AGC	Support parameters for zoom range.
*   12/04/18 1.10  AGC	Implement more function overloads.
*   12/04/18 1.9   AJH	Add m_ptInitialized.
*   14/03/18 1.8   AGC	Support format warnings with clang on Windows.
*   06/03/18 1.7   AGC	Support sendCommand() format warnings.
*   28/07/17 1.6   AJH	Fix line endings and comments.
*   26/07/17 1.5   AJH	Implement absolute focus LUT.
*			Make DLTV relative focus via timer.
*   21/07/17 1.4   AJH	Add new internal functions.
*   20/07/17 1.3   AJH	IsAbsZoom/FocusSupported() use isPrimary arg.
*   18/07/17 1.2   AJH	Remove redundant args from prototypes.
*			Remove redundant member variables and timer wrappers.
*   13/07/17 1.1   AJH	Initial version.
**********************************************************************/
#ifndef _SPX_CAM_CTRL_NEXUS_H
#define _SPX_CAM_CTRL_NEXUS_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxTimer.h"

/* For base class. */
#include "SPxLibData/SPxCamCtrlBase.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

class SPxNetTCP;

/*
 * Define our class.
 */
class SPxCamCtrlNexus : public SPxCamCtrlBase
{
public:
    /* Types. */
    typedef enum
    {
	PRESET_AUTO_FOCUS = 1,
	PRESET_FREEZE = 2,
	PRESET_SUBSYSTEM_ON = 3,
	PRESET_AUTO_FOCUS_ONESHOT = 4
    } NexusPresets;

    typedef struct
    {
	int isFovRangeSet;	/* Has the field of view range been set? */
	int isFovRangeFromCam;	/* Does the field of view range come from the camera? */
	float minFovDegs;	/* Minimum field of view. */
	float maxFovDegs;	/* Maximum field of view. */
	float zoomPos;		/* Last zoom position. */
	float zoomPosCmd;	/* Last commanded zoom position. */
	float focusPos;		/* Last focus position. */
	float focusPosCmd;	/* Last commanded focus position. */
	float absCmdZoomPos;	/* Used to check for zoom changes. */
	float absCmdFocusPos;	/* Used to check for focus changes. */
	float zoomSpeed;	/* Current zoom speed. */
	float focusSpeed;	/* Current focus speed. */
    } NexusSensor;

    /* Construction and destruction. */
    SPxCamCtrlNexus(void);
    virtual ~SPxCamCtrlNexus(void);
    virtual SPxErrorCode Create(const char *serialName = NULL,
	unsigned int serialBaud = 2400);

    /* Control. */
    virtual SPxErrorCode SetAddress(const char *address, int port = 0,
	const char *ifAddr=NULL);
    virtual SPxErrorCode SetAddress(UINT32 address, int port = 0,
	const char *ifAddr=NULL);
    virtual int IsSerialSupported(void) const { return FALSE; }
    virtual int IsNetworkSupported(void) const { return TRUE; }
    virtual SPxErrorCode Setup(void);

    /* Camera control functions. */
    virtual SPxErrorCode Stop(void);
    virtual SPxErrorCode GoToPreset(UINT8 id) { return GoToPreset(id, TRUE); }
    virtual SPxErrorCode GoToPreset(UINT8 id, int isPrimary);

    /* Base class functions. */
    virtual int IsSecondarySupported(void) const { return TRUE; }

    /* Absolute positioning. */
    virtual int IsAbsCmdInProgress(UINT32 dirMask) const { return IsAbsCmdInProgress(dirMask, TRUE); }
    virtual int IsAbsCmdInProgress(UINT32 dirMask, int isPrimary) const;

    virtual int IsAbsPanSupported(void) const;
    virtual SPxErrorCode SetPan(float panAngleDegs);
    virtual SPxErrorCode GetPan(float *panAngleDegs) const;
    virtual SPxErrorCode GetLastPan(float *panAngleDegs) const;

    virtual int IsAbsTiltSupported(void) const;
    virtual SPxErrorCode SetTilt(float tiltAngleDegs);
    virtual SPxErrorCode GetTilt(float *tiltAngleDegs) const;
    virtual SPxErrorCode GetLastTilt(float *tiltAngleDegs) const;

    virtual int IsAbsZoomSupported(void) const { return IsAbsZoomSupported(TRUE); }
    virtual SPxErrorCode SetZoom(float fovDegs) { return SetZoom(fovDegs, TRUE); }
    virtual SPxErrorCode GetZoom(float *fovDegs) const { return GetZoom(fovDegs, TRUE); }
    virtual SPxErrorCode GetLastZoom(float *fovDegs) const { return GetLastZoom(fovDegs, TRUE); }
    virtual SPxErrorCode SetZoomRange(float minFovDegs, float maxFovDegs) { return SetZoomRange(minFovDegs, maxFovDegs, TRUE); }
    virtual SPxErrorCode GetZoomRange(float *minFovDegs, float *maxFovDegs) const { return GetZoomRange(minFovDegs, maxFovDegs, TRUE); }
    virtual int IsAbsZoomSupported(int isPrimary) const;
    virtual SPxErrorCode SetZoom(float fovDegs, int isPrimary);
    virtual SPxErrorCode GetZoom(float *fovDegs, int isPrimary) const;
    virtual SPxErrorCode GetLastZoom(float *fovDegs, int isPrimary) const;
    virtual SPxErrorCode SetZoomRange(float minFovDegs, float maxFovDegs,
				      int isPrimary);
    virtual SPxErrorCode GetZoomRange(float *minFovDegs, float *maxFovDegs,
				      int isPrimary) const;
    virtual SPxErrorCode SetZoomLUTFilename(const char *filename, int isPrimary);
    virtual SPxErrorCode GetZoomLUTFilename(char *buffer, int bufLen, int isPrimary) const;

    virtual int IsAbsFocusSupported(void) const { return IsAbsFocusSupported(TRUE); }
    virtual SPxErrorCode SetFocus(float rangeMetres) { return SetFocus(rangeMetres, TRUE); }
    virtual SPxErrorCode GetFocus(float *rangeMetres) const { return GetFocus(rangeMetres, TRUE); }
    virtual SPxErrorCode GetLastFocus(float *rangeMetres) const { return GetLastFocus(rangeMetres, TRUE); }
    virtual SPxErrorCode SetFocusPos(UINT16 focusPos) { return SetFocusPos(focusPos, TRUE); }
    virtual SPxErrorCode GetFocusPos(UINT16 *focusPos) const { return GetFocusPos(focusPos, TRUE); }
    virtual int IsAbsFocusSupported(int isPrimary) const;
    virtual SPxErrorCode SetFocus(float rangeMetres, int isPrimary);
    virtual SPxErrorCode GetFocus(float *rangeMetres, int isPrimary) const;
    virtual SPxErrorCode GetLastFocus(float *rangeMetres, int isPrimary) const;
    virtual SPxErrorCode SetFocusPos(UINT16 focusPos, int isPrimary);
    virtual SPxErrorCode GetFocusPos(UINT16 *focusPos, int isPrimary) const;
    virtual SPxErrorCode SetFocusLUTFilename(const char *filename);
    virtual SPxErrorCode GetFocusLUTFilename(char *buffer, int bufLen) const;

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
				 float *panSpeedSet=NULL,
				 float *tiltSpeedSet=NULL,
				 int continueZoomFocus=FALSE,
				 double timeoutSecs=0.0);

    virtual int IsRelZoomSupported(void) const { return TRUE; }
    virtual SPxErrorCode Zoom(float zoomSpeed, int continuePanTilt, double timeoutSecs) { return Zoom(zoomSpeed, continuePanTilt, timeoutSecs, TRUE); }
    virtual SPxErrorCode Zoom(float zoomSpeed, int continuePanTilt, double timeoutSecs,
			      int isPrimary);

    virtual int IsRelFocusSupported(void) const { return TRUE; }
    virtual SPxErrorCode Focus(float focusSpeed, int continuePanTilt) { return Focus(focusSpeed, continuePanTilt, TRUE); }
    virtual SPxErrorCode Focus(float focusSpeed, int continuePanTilt,
			       int isPrimary);

    virtual int IsAutoFocusSupported(void) const { return IsAutoFocusSupported(TRUE); }
    virtual SPxErrorCode AutoFocus(int autoFocus) { return AutoFocus(autoFocus, TRUE); }
    virtual int IsAutoFocusEnabled(void) const { return IsAutoFocusEnabled(TRUE); }
    virtual int IsAutoFocusSupported(int isPrimary) const;
    virtual SPxErrorCode AutoFocus(int autoFocus, int isPrimary);
    virtual int IsAutoFocusEnabled(int isPrimary) const;
    virtual int IsAutoFocusOneshotSupported(void) const { return TRUE; }
    virtual SPxErrorCode AutoFocusOneshot(void) { return AutoFocusOneshot(TRUE); }
    virtual SPxErrorCode AutoFocusOneshot(int isPrimary);

    /* Queries. */
    virtual int AreMaxSpeedsSupported(void) const { return TRUE; }
    virtual SPxErrorCode SetMaxPanTiltSpeedsDegsPerSec(double maxPanSpeedDegsPerSec,
	double maxTiltSpeedDegsPerSec) const;
    virtual SPxErrorCode GetMaxPanTiltSpeedsDegsPerSec(double *maxPanSpeedDegsPerSec,
						       double *maxTiltSpeedDegsPerSec) const;

    /* Generic get/set parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    /* Protected functions that may be called by derived classes for testing. */
    explicit SPxCamCtrlNexus(SPxNetTCP *netTCP);
    // cppcheck-suppress virtualCallInConstructor
    virtual void Shutdown(void);
    void GetZoomRanges(void) const;

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;	/* Private structure - contains STL bits. */

#define SPX_NEXUS_NUM_SENSOR	    2
#define SPX_NEXUS_SENSOR_PRIMARY    0
#define SPX_NEXUS_SENSOR_SECONDARY  1

    mutable NexusSensor m_sensor[SPX_NEXUS_NUM_SENSOR];
    mutable int m_absPanSupported;		/* Absolute pan supported? */
    mutable UINT32 m_absPanCheckTime;		/* LAst check time for abs pan. */
    mutable int m_absTiltSupported;		/* Absolute tilt supported? */
    mutable UINT32 m_absTiltCheckTime;		/* Last check time for abs tilt. */
    mutable int m_absZoomSupported;		/* Absolute zoom supported? */
    mutable UINT32 m_absZoomCheckTime;		/* Last check time for abs zoom. */
    mutable int m_absFocusSupported;		/* Absolute focus supported? */
    mutable UINT32 m_absFocusCheckTime;		/* Last check time for abs focus. */
    mutable float m_panPos;			/* Last pan position in degrees. */
    mutable float m_panPosCmd;			/* Commanded pan position. */
    mutable float m_panSpeed;			/* Current pan speed. */
    mutable float m_tiltPos;			/* Last tilt position in degrees. */
    mutable float m_tiltPosCmd;			/* Commanded tilt position. */
    mutable float m_tiltSpeed;			/* Current tilt speed. */
    mutable int m_isAbsPanCmdInProgress;        /* Absolute command in progress? */
    mutable int m_isAbsTiltCmdInProgress;       /* Absolute command in progress? */
    mutable int m_isAbsZoomCmdInProgress;       /* Absolute command in progress? */
    mutable int m_isAbsFocusCmdInProgress;	/* Absolute command in progress? */
    mutable UINT32 m_absPanCmdInProgressCheckTime; /* Last time this was checked. */
    mutable UINT32 m_absTiltCmdInProgressCheckTime; /* Last time this was checked. */
    mutable UINT32 m_absZoomCmdInProgressCheckTime; /* Last time this was checked. */
    mutable UINT32 m_absFocusCmdInProgressCheckTime;/* Last time this was checked. */
    mutable int m_isAbsPanPosSet;                  /* Are the values below set? */
    mutable int m_isAbsTiltPosSet;                 /* Are the values below set? */
    mutable int m_isAbsZoomPosSet;                 /* Are the values below set? */
    mutable int m_isAbsFocusPosSet;		   /* Are the values below set? */
    mutable double m_absCmdPanPos;              /* Used to check for pan changes. */
    mutable double m_absCmdTiltPos;             /* Used to check for tilt changes. */
    mutable double m_maxPanSpeedDegsPerSec;	/* Retrieved from camera. */
    mutable double m_maxTiltSpeedDegsPerSec;	/* Retrieved from camera. */
    mutable double m_maxPanSpeedDegsPerSecUser;	/* Set in config file. */
    mutable double m_maxTiltSpeedDegsPerSecUser; /* Set in config file. */
    int m_ptInitialized;			/* Has PT unit been initialized? */
    SPxTimer m_timer;

    /* Private functions. */
    void initialise(void);
    SPxErrorCode getPanTilt(float *panAngleDegs, float *tiltAngleDegs) const;
    SPxErrorCode getZoom(float *fovDegs, int isPrimary) const;
    SPxErrorCode getFocus(float *focus, int isPrimary,
			  int updateDaylightFocus=TRUE) const;
    SPxErrorCode getMaxSpeeds(void) const;
    SPxErrorCode getZoomRanges(void) const;
    SPxErrorCode
#if defined(__GNUC__) || defined(__clang__)
	__attribute__((format(printf, 2, 3)))
#endif
    sendCommand(const char *cmd, ...) const;
    SPxErrorCode setAutofocus(int autofocus, int isPrimary);
    int getAutofocus(int isPrimary) const;
    SPxErrorCode doAutoFocusOneshot(int isPrimary);
    SPxErrorCode setFreeze(int freeze, int isPrimary);
    int getFreeze(int isPrimary) const;
    int getResponseInt(const char *key) const;
    float getResponseFloat(const char *key) const;
    const char *getResponseString(const char *key) const;
    void handleConnectTimer(void);
    void handleZoomTimer(void);
    void updateZoom(void) const;
    void handleFocusTimer(void);
    void updateFocus(void) const;
    SPxErrorCode stopPanTilt(void);
    SPxErrorCode stopZoomFocus(void);

}; /* SPxCamCtrlNexus */

#endif /* _SPX_CAM_CTRL_NEXUS_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
