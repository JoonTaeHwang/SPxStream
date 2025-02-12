/*********************************************************************
*
* (c) Copyright 2011 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamCtrlPelco class - a class
*	to allow controlling a camera with a Pelco-D serial interface.
*
* Revision Control:
*   08/08/22 v1.48   AGC	Support string based presets through camera manager.
*
* Previous Changes:
*   21/06/21 1.47   AGC	Support releasing camera manager control.
*   17/06/21 1.46   AGC	Support disabling camera manager rechecks in derived classes.
*   18/03/21 1.45   BTB	Support isPrimary flag on SetPreset() and ClearPreset().
*   16/03/21 1.44   AGC	Allow override of SerialPortFn().
*   08/01/21 1.43   AGC	Improve camera address response verification.
*   07/01/21 1.42   AGC	Support isPrimary flag for SendRawCommand().
*   07/01/21 1.41   AGC	Implement dual sensor overload of Stop().
*   13/08/20 1.40   AGC	Do not disable auto-focus supported when oneshot preset set.
*   13/08/20 1.39   AGC	Support preset for auto-focus oneshot.
*   06/08/20 1.38   AGC	Support 64 level zoom speed with camera manager.
*   02/04/19 1.37   AGC	Support secondary camera address.
*   01/02/19 1.36   BTB	Add resetCapabilities().
*   21/01/19 1.35   AGC	Add pan/tilt/zoom timeouts.
*   31/07/18 1.34   AGC	Add IsSecondarySupported().
*   25/06/18 1.33   AGC	Correctly suppress cppcheck warning.
*   07/06/18 1.32   AGC	Support auto-focus one-shot.
*   10/05/18 1.31   AGC	Add virtual GetRecvTimeoutMsecs() for testing.
*   18/04/18 1.30   AGC	Add GetPriority().
*   17/04/18 1.29   AGC	Prefer zoom range from camera.
*   12/04/18 1.28   AGC	Support updated use with camera manager.
*   24/04/17 1.27   AGC	Virtualise camera address and control type functions.
*   06/02/17 1.26   AGC	Fix GetLastTilt().
*   02/02/17 1.25   AGC	Add options for concurrent PTZ.
*   23/01/17 1.24   AGC	Modifications for testing.
*   18/01/17 1.23   AGC	Add priority functions.
*   19/10/16 1.22   AGC	GoToPreset() now virtual.
*   23/09/16 1.21   AGC	Add GetLastPan/Tilt/Zoom() functions.
*   24/03/16 1.20   SP 	Add sleep between zoom and focus commands 
*                       as requested by Silent Sentinel.
*   08/02/16 1.19   AGC	Use speed mode from camera.
*   04/02/16 1.18   AGC	Support for retrieving camera settings.
*   03/11/15 1.17   AGC	Allow retrieval of actual pan/tilt speeds.
*   28/05/15 1.16   AGC	Lock mutex for public GetFOVForZoom() and GetZoomForFOV().
*   26/08/15 1.15   REW	Correct version numbers back to 0.1.
*   26/08/15 1.14   SP 	Make GetFOVForZoom() and GetZoomForFOV() public.
*   26/08/15 1.13   AGC	Add focus LUT support.
*   31/07/15 1.12   AGC	Support absolute focus.
*   25/06/15 1.11   AGC	Add GetZoomLUTFilename() function.
*   23/06/15 1.10   AGC	Add direction specific absolute command in progress checks.
*   12/06/15 1.9    AGC	Stop() function now overrides base class implementation.
*			Add command interval function.
*   22/05/15 1.8    AGC	Change turbo speed value to match recent spec.
*   11/05/15 1.7    AGC	Avoid incorrectly updating absolute support flags.
*   07/11/14 1.6    AGC	Support optional signed tilt.
*   02/06/14 1.5    SP 	Add zoom to FOV LUT.
*   17/02/14 1.4    SP 	Remove zoom calibration.
*   27/01/14 1.3    SP 	Support zoom calibration.
*   04/11/13 1.2    AGC	Cache absolute command support.
*   24/10/13 1.1    AGC	Move to SPx library.
*			Add absolute positioning functions.
*   14/08/13 0.5    AGC	Derive from new SPxAVCamCtrlBase class.
*   17/11/11 0.4    AGC	Rename parameter setting functions.
*   14/11/11 0.3    AGC	Use Up/Down etc for movement functions.
*   22/09/11 0.2    AGC	Allow independent horiz/vert speeds.
*			Add SetSerialName(), SetSerialBaud() functions.
*   13/09/11 0.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_CAM_CTRL_PELCO_H
#define _SPX_CAM_CTRL_PELCO_H

/*
 * Other headers required.
 */

/* For base class. */
#include "SPxLibData/SPxCamCtrlBase.h"

/* Commands that can be sent with SendCommand(). */
static const UINT16 SPX_CAM_PELCO_AUTO_SCAN	= 0x0090;
static const UINT16 SPX_CAM_PELCO_MANUAL_SCAN	= 0x0010;
static const UINT16 SPX_CAM_PELCO_CAMERA_ON	= 0x0088;
static const UINT16 SPX_CAM_PELCO_CAMERA_OFF	= 0x0008;
static const UINT16 SPX_CAM_PELCO_IRIS_CLOSE	= 0x0004;
static const UINT16 SPX_CAM_PELCO_IRIS_OPEN	= 0x0002;
static const UINT16 SPX_CAM_PELCO_FOCUS_NEAR	= 0x0001;
static const UINT16 SPX_CAM_PELCO_FOCUS_FAR	= 0x8000;
static const UINT16 SPX_CAM_PELCO_ZOOM_WIDE	= 0x4000;
static const UINT16 SPX_CAM_PELCO_ZOOM_TELE	= 0x2000;
static const UINT16 SPX_CAM_PELCO_MOVE_DOWN	= 0x1000;
static const UINT16 SPX_CAM_PELCO_MOVE_UP	= 0x0800;
static const UINT16 SPX_CAM_PELCO_MOVE_LEFT	= 0x0400;
static const UINT16 SPX_CAM_PELCO_MOVE_RIGHT	= 0x0200;
static const UINT16 SPX_CAM_PELCO_STOP		= 0x0000;

/* Commands that can be sent with SendRawCommand(). */
static const UINT32 SPX_CAM_PELCO_SET_PRESET		= 0x00000300;
static const UINT32 SPX_CAM_PELCO_CLEAR_PRESET		= 0x00000500;
static const UINT32 SPX_CAM_PELCO_GO_TO_PRESET		= 0x00000700;
static const UINT32 SPX_CAM_PELCO_RESET_PAN		= 0x22000700;
static const UINT32 SPX_CAM_PELCO_SET_ZOOM_SPEED	= 0x00002500;
static const UINT32 SPX_CAM_PELCO_SET_FOCUS_SPEED	= 0x00002700;

/* Camera manager part number. */
static const char SPX_CAMERA_MNGR_PART_NUM[] = "SPX-CAMERA-MNGR";
static const char SPX_CAMERA_MNGR_VER_PART_NUM[] = "SPX-CAM-MNGR-V";

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Pelco camera control type now defined in base class.
 * This typedef if for backwards compat with old name.
 */
typedef SPxCameraControlType SPxCamCtrlPelcoType;

static const UINT8 SPX_CAM_PELCO_SPEED_MIN	= 0x00;
static const UINT8 SPX_CAM_PELCO_SPEED_MAX	= 0x3F;
static const UINT8 SPX_CAM_PELCO_SPEED_TURBO	= 0x40;
static const UINT8 SPX_CAM_PELCO_SPEED_MAX_255	= 0xFF;
static const float SPX_CAM_PELCO_SPEED_MIN_F	= static_cast<float>(SPX_CAM_PELCO_SPEED_MIN);
static const float SPX_CAM_PELCO_SPEED_MAX_F	= static_cast<float>(SPX_CAM_PELCO_SPEED_MAX);
static const float SPX_CAM_PELCO_SPEED_TURBO_F	= static_cast<float>(SPX_CAM_PELCO_SPEED_TURBO);
static const float SPX_CAM_PELCO_SPEED_MAX_255_F= static_cast<float>(SPX_CAM_PELCO_SPEED_MAX_255);

class SPxSerialRecv;
struct SPxPelcoDMesg;
struct SPxPelcoPMesg;
class SPxNetTCP;

/*
 * Define our class.
 */
class SPxCamCtrlPelco : public SPxCamCtrlBase
{
public:
    /* Construction and destruction. */
    SPxCamCtrlPelco(void);
    virtual ~SPxCamCtrlPelco(void);

    /* Control. */
    virtual SPxErrorCode SetMesgType(SPxMesgIOType_t mesgType);
    virtual SPxErrorCode SetSerialName(const char *name);
    virtual SPxErrorCode SetSerialBaud(unsigned int baud);
    virtual SPxErrorCode SetAddress(const char *address, int port = 0, const char *ifAddr=NULL);
    virtual SPxErrorCode SetAddress(UINT32 address, int port = 0, const char *ifAddr=NULL);

    /* Settings. */
    virtual SPxErrorCode SetCameraControlType(SPxCameraControlType type);
    virtual SPxErrorCode GetCameraControlType(SPxCameraControlType *type) const;
    virtual SPxErrorCode SetCameraAddress(UINT8 address, int isPrimary=TRUE);
    virtual SPxErrorCode GetCameraAddress(UINT8 *address, int isPrimary=TRUE) const;

    /* Camera control functions. */
    SPxErrorCode Move(int horizSpeed, int vertSpeed, int zoomSpeed=0) { return Move(horizSpeed, vertSpeed, zoomSpeed, TRUE); }
    SPxErrorCode ZoomIn(void) { return ZoomIn(TRUE); }
    SPxErrorCode ZoomIn(UINT8 speed, int continuePanTilt=FALSE) { return ZoomIn(speed, continuePanTilt, TRUE); }
    SPxErrorCode ZoomOut(void) { return ZoomOut(TRUE); }
    SPxErrorCode ZoomOut(UINT8 speed, int continuePanTilt=FALSE) { return ZoomOut(speed, continuePanTilt, TRUE); }
    SPxErrorCode FocusNear(void) { return FocusNear(TRUE); }
    SPxErrorCode FocusNear(UINT8 speed, int continuePanTilt=FALSE) { return FocusNear(speed, continuePanTilt, TRUE); }
    SPxErrorCode FocusFar(void) { return FocusFar(TRUE); }
    SPxErrorCode FocusFar(UINT8 speed, int continuePanTilt=FALSE) { return FocusFar(speed, continuePanTilt, TRUE); }
    SPxErrorCode Move(int horizSpeed, int vertSpeed, int zoomSpeed, int isPrimary);
    SPxErrorCode MoveUp(UINT8 speed=SPX_CAM_PELCO_SPEED_MAX, int continueZoomFocus=FALSE);
    SPxErrorCode MoveDown(UINT8 speed=SPX_CAM_PELCO_SPEED_MAX, int continueZoomFocus=FALSE);
    SPxErrorCode MoveLeft(UINT8 speed=SPX_CAM_PELCO_SPEED_MAX, int continueZoomFocus=FALSE);
    SPxErrorCode MoveRight(UINT8 speed=SPX_CAM_PELCO_SPEED_MAX, int continueZoomFocus=FALSE);
    SPxErrorCode MoveUpLeft(UINT8 speed=SPX_CAM_PELCO_SPEED_MAX, int continueZoomFocus=FALSE) { return MoveUpLeft(speed, speed, continueZoomFocus); }
    SPxErrorCode MoveUpRight(UINT8 speed=SPX_CAM_PELCO_SPEED_MAX, int continueZoomFocus=FALSE) { return MoveUpRight(speed, speed, continueZoomFocus); }
    SPxErrorCode MoveDownLeft(UINT8 speed=SPX_CAM_PELCO_SPEED_MAX, int continueZoomFocus=FALSE) { return MoveDownLeft(speed, speed, continueZoomFocus); }
    SPxErrorCode MoveDownRight(UINT8 speed=SPX_CAM_PELCO_SPEED_MAX, int continueZoomFocus=FALSE) { return MoveDownRight(speed, speed, continueZoomFocus); }
    SPxErrorCode MoveUpLeft(UINT8 hSpeed, UINT8 vSpeed, int continueZoomFocus=FALSE);
    SPxErrorCode MoveUpRight(UINT8 hSpeed, UINT8 vSpeed, int continueZoomFocus=FALSE);
    SPxErrorCode MoveDownLeft(UINT8 hSpeed, UINT8 vSpeed, int continueZoomFocus=FALSE);
    SPxErrorCode MoveDownRight(UINT8 hSpeed, UINT8 vSpeed, int continueZoomFocus=FALSE);
    SPxErrorCode ResetPan(void);
    SPxErrorCode ZoomIn(int isPrimary);
    SPxErrorCode ZoomIn(UINT8 speed, int continuePanTilt, int isPrimary);
    SPxErrorCode ZoomOut(int isPrimary);
    SPxErrorCode ZoomOut(UINT8 speed, int continuePanTilt, int isPrimary);
    SPxErrorCode FocusNear(int isPrimary);
    SPxErrorCode FocusNear(UINT8 speed, int continuePanTilt, int isPrimary);
    SPxErrorCode FocusFar(int isPrimary);
    SPxErrorCode FocusFar(UINT8 speed, int continuePanTilt, int isPrimary);
    virtual SPxErrorCode Stop(void) { return Stop(TRUE); }
    virtual SPxErrorCode Stop(int isPrimary);
    SPxErrorCode ManualScan(void);
    SPxErrorCode AutoScan(void);
    virtual SPxErrorCode SetPreset(UINT8 id) { return SetPreset(id, TRUE); }
    virtual SPxErrorCode SetPreset(UINT8 id, int isPrimary);
    virtual SPxErrorCode ClearPreset(UINT8 id) { return ClearPreset(id, TRUE); }
    virtual SPxErrorCode ClearPreset(UINT8 id, int isPrimary);
    virtual SPxErrorCode GoToPreset(UINT8 id) { return GoToPreset(id, TRUE); }
    virtual SPxErrorCode GoToPreset(UINT8 id, int isPrimary);
    virtual SPxErrorCode GoToPreset(const char *preset) { return GoToPreset(preset, TRUE); }
    virtual SPxErrorCode GoToPreset(const char *preset, int isPrimary);

    SPxErrorCode SendCommand(UINT16 command, UINT8 panSpeed=0, UINT8 tiltSpeed=0) const;
    SPxErrorCode SendRawCommand(UINT32 command, int isPrimary=TRUE) const;

    /* Base class functions. */
    virtual int IsSecondarySupported(void) const;

    /* Self reference. */
    virtual SPxErrorCode SelfReference(void);

    /* Absolute positioning. */
    virtual int IsAbsCmdInProgress(UINT32 dirMask=SPX_CAMERA_DIR_ALL) const { return IsAbsCmdInProgress(dirMask, TRUE); }
    virtual int IsAbsCmdInProgress(UINT32 dirMask, int isPrimary) const;

    virtual int IsAbsPanSupported(void) const;
    virtual SPxErrorCode SetPan(float panAngleDegs);
    virtual SPxErrorCode GetPan(float *panAngleDegs) const;
    virtual SPxErrorCode GetLastPan(float *panAngleDegs) const;

    virtual int IsAbsTiltSupported(void) const;
    virtual SPxErrorCode SetTilt(float tiltAngleDegs);
    virtual SPxErrorCode GetTilt(float *tiltAngleDegs) const;
    virtual SPxErrorCode GetLastTilt(float *tiltAngleDegs) const;
    virtual SPxErrorCode GetTiltPos(UINT16 *tiltPos) const;
    SPxErrorCode SetTiltSigned(int tiltSigned);
    int IsTiltSigned(void) const;

    virtual int IsAbsZoomSupported(void) const { return IsAbsZoomSupported(TRUE); }
    virtual SPxErrorCode SetZoom(float fovDegs) { return SetZoom(fovDegs, TRUE); }
    virtual SPxErrorCode GetZoom(float *fovDegs) const { return GetZoom(fovDegs, TRUE); }
    virtual SPxErrorCode GetLastZoom(float *fovDegs) const { return GetLastZoom(fovDegs, TRUE); }
    virtual SPxErrorCode GetZoomPos(UINT16 *zoomPos) const { return GetZoomPos(zoomPos, TRUE); }
    virtual SPxErrorCode SetZoomRange(float minFovDegs, float maxFovDegs) { return SetZoomRange(minFovDegs, maxFovDegs, TRUE); }
    virtual SPxErrorCode GetZoomRange(float *minFovDegs, float *maxFovDegs) const { return GetZoomRange(minFovDegs, maxFovDegs, TRUE); }
    virtual SPxErrorCode SetMaxZoomPosition(UINT16 pos) { return SetMaxZoomPosition(pos, TRUE); }
    virtual SPxErrorCode GetMaxZoomPosition(UINT16 *pos) const { return GetMaxZoomPosition(pos, TRUE); }
    virtual SPxErrorCode SetQueriedZoomIsPercentage(int state) { return SetQueriedZoomIsPercentage(state, TRUE); }
    virtual int IsQueriedZoomPercentage(void) const { return IsQueriedZoomPercentage(TRUE); }
    virtual SPxErrorCode SetZoomLUTFilename(const char *filename) { return SetZoomLUTFilename(filename, TRUE); }
    virtual SPxErrorCode GetZoomLUTFilename(char *buffer, int bufLen) const { return GetZoomLUTFilename(buffer, bufLen, TRUE); }

    virtual int IsAbsFocusSupported(void) const { return IsAbsFocusSupported(TRUE); }
    virtual SPxErrorCode SetFocus(float rangeMetres) { return SetFocus(rangeMetres, TRUE); }
    virtual SPxErrorCode GetFocus(float *rangeMetres) const { return GetFocus(rangeMetres, TRUE); }
    virtual SPxErrorCode GetLastFocus(float *rangeMetres) const { return GetLastFocus(rangeMetres, TRUE); }
    virtual SPxErrorCode GetFocusPos(UINT16 *focusPos) const { return GetFocusPos(focusPos, TRUE); }
    virtual SPxErrorCode SetFocusLUTFilename(const char *filename) { return SetFocusLUTFilename(filename, TRUE); }
    virtual SPxErrorCode GetFocusLUTFilename(char *buffer, int bufLen) const { return GetFocusLUTFilename(buffer, bufLen, TRUE); }

    virtual int IsAbsZoomSupported(int isPrimary) const;
    virtual SPxErrorCode SetZoom(float fovDegs, int isPrimary);
    virtual SPxErrorCode GetZoom(float *fovDegs, int isPrimary) const;
    virtual SPxErrorCode GetLastZoom(float *fovDegs, int isPrimary) const;
    virtual SPxErrorCode GetZoomPos(UINT16 *zoomPos, int isPrimary) const;
    virtual SPxErrorCode SetZoomRange(float minFovDegs, float maxFovDegs,
				      int isPrimary);
    virtual SPxErrorCode GetZoomRange(float *minFovDegs, float *maxFovDegs,
				      int isPrimary) const;
    virtual SPxErrorCode SetMaxZoomPosition(UINT16 pos, int isPrimary);
    virtual SPxErrorCode GetMaxZoomPosition(UINT16 *pos, int isPrimary) const;
    virtual SPxErrorCode SetQueriedZoomIsPercentage(int state, int isPrimary);
    virtual int IsQueriedZoomPercentage(int isPrimary) const;
    virtual SPxErrorCode SetZoomLUTFilename(const char *filename, int isPrimary);
    virtual SPxErrorCode GetZoomLUTFilename(char *buffer, int bufLen, int isPrimary) const;

    virtual int IsAbsFocusSupported(int isPrimary) const;
    virtual SPxErrorCode SetFocus(float rangeMetres, int isPrimary);
    virtual SPxErrorCode GetFocus(float *rangeMetres, int isPrimary) const;
    virtual SPxErrorCode GetLastFocus(float *rangeMetres, int isPrimary) const;
    SPxErrorCode GetFocusPos(UINT16 *focusPos, int isPrimary) const;
    virtual SPxErrorCode SetFocusLUTFilename(const char *filename, int isPrimary);
    virtual SPxErrorCode GetFocusLUTFilename(char *buffer, int bufLen, int isPrimary) const;

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
    virtual SPxErrorCode Zoom(float zoomSpeed, int continuePanTilt=FALSE, double timeoutSecs=0.0) { return Zoom(zoomSpeed, continuePanTilt, timeoutSecs, TRUE); }
    virtual SPxErrorCode Zoom(float zoomSpeed, int continuePanTilt, double timeoutSecs, int isPrimary);

    virtual int IsRelFocusSupported(void) const { return TRUE; }
    virtual SPxErrorCode Focus(float focusSpeed, int continuePanTilt=FALSE) { return Focus(focusSpeed, continuePanTilt, TRUE); }
    virtual SPxErrorCode Focus(float focusSpeed, int continuePanTilt, int isPrimary);

    virtual int IsAutoFocusSupported(void) const { return TRUE; }
    virtual SPxErrorCode AutoFocus(int autoFocus) { return AutoFocus(autoFocus, TRUE); }
    virtual int IsAutoFocusEnabled(void) const { return IsAutoFocusEnabled(TRUE); }
    virtual SPxErrorCode AutoFocus(int autoFocus, int isPrimary);
    virtual int IsAutoFocusEnabled(int isPrimary) const;
    virtual int IsAutoFocusOneshotSupported(void) const { return IsAutoFocusOneshotSupported(TRUE); }
    virtual SPxErrorCode AutoFocusOneshot(void) { return AutoFocusOneshot(TRUE); }
    virtual int IsAutoFocusOneshotSupported(int isPrimary) const;
    virtual SPxErrorCode AutoFocusOneshot(int isPrimary);
    SPxErrorCode SetAutoFocusOneshotPreset(UINT8 autoFocusOneshotPreset);
    UINT8 GetAutoFocusOneshotPreset(void) const { return m_autoFocusOneshotPreset;  }

    /* Query. */
    virtual int AreQueriesSupported(void) const;
    virtual SPxErrorCode GetFirmwareVersion(UINT16 *version) const;
    virtual SPxErrorCode GetCommsMode(SPxCameraCommsMode *mode) const;
    virtual SPxErrorCode GetPanTiltResponseMode(SPxCameraPanTiltResponseMode *mode) const;
    virtual SPxErrorCode GetZoomSpeedMode(SPxCameraZoomSpeedMode *mode) const;
    virtual SPxErrorCode GetMaxPanTiltSpeedsDegsPerSec(double *maxPanSpeedDegsPerSec,
						       double *maxTiltSpeedDegsPerSec) const;

    /* Priority control. */
    virtual SPxErrorCode GetControlStatus(SPxCameraControlStatus *status) const;
    virtual SPxErrorCode GetControllerAddress(UINT32 *address) const;
    virtual SPxErrorCode SetPriority(UINT8 priority);
    virtual SPxErrorCode GetPriority(UINT8 *priority) const;
    virtual SPxErrorCode ReleaseControl(void);

    /* Minimum command interval. */
    virtual UINT32 GetMinCmdIntervalMsecs(void) const { return 20; }

    /* Conversion functions. */
    float GetFOVForZoom(UINT16 zoomPos) const { return GetFOVForZoom(zoomPos, TRUE); }
    UINT16 GetZoomForFOV(float fovDegs) const { return GetZoomForFOV(fovDegs, TRUE); }
    float GetFOVForZoom(UINT16 zoomPos, int isPrimary) const;
    UINT16 GetZoomForFOV(float fovDegs, int isPrimary) const;
    float GetMetresForFocus(UINT16 focusPos) const { return GetMetresForFocus(focusPos, TRUE); }
    UINT16 GetFocusForMetres(float focusMetres) const { return GetFocusForMetres(focusMetres, TRUE); }
    float GetMetresForFocus(UINT16 focusPos, int isPrimary) const;
    UINT16 GetFocusForMetres(float focusMetres, int isPrimary) const;

    void SetCameraManagerVersion(UINT8 version);

    /* Generic get/set parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    /* Protected functions that may be called by derived classes for testing. */
    explicit SPxCamCtrlPelco(SPxNetTCP *netTCP, UINT8 version);
    // cppcheck-suppress virtualCallInConstructor
    virtual void Shutdown(void);

    virtual UINT32 GetRecvTimeoutMsecs(void) const { return 300; }

    virtual unsigned int SerialPortFn(const unsigned char *payload,
				      unsigned int payloadBytes);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;	/* Private structure - contains STL bits. */

    SPxCameraControlType m_camControlType;	/* Camera control type. */
    UINT8 m_cameraAddress[2];			/* Address of camera (0-255). */
    mutable SPxAtomic<UINT32> m_expCamAddr;	/* Camera address for response. */
    mutable SPxAtomic<UINT32> m_opCode;		/* Op-code for extended responses. */
    mutable SPxAtomic<UINT32> m_opCodeAlt;	/* Alternative op-code for extended responses. */
    mutable UINT8 m_opCheck;			/* Checksum for sent data. */
    mutable UINT32 m_data;			/* Received data. */
    mutable int m_dataIsAlt;			/* Received data is from alternative response. */
    mutable SPxAtomic<UINT32> m_recvBytes;	/* Bytes received while waiting for response. */
    mutable int m_isFovSet[2];			/* Has the field of view been set? */
    mutable int m_isFovFromCam[2];		/* Is field of view range from camera? */
    mutable float m_minFovDegs[2];		/* Minimum field of view. */
    mutable float m_maxFovDegs[2];		/* Maximum field of view. */
    UINT16 m_maxZoomPos[2];                     /* Zoom position at maximum zoom (min FOV). */
    int m_isQueriedZoomPercentage[2];           /* Queried zoom value is a percenage of max zoom. */
    mutable int m_absPanSupported;
    mutable UINT32 m_absPanCheckTime;
    mutable int m_absTiltSupported;
    mutable UINT32 m_absTiltCheckTime;
    mutable int m_absZoomSupported[2];
    mutable UINT32 m_absZoomCheckTime[2];
    mutable int m_absFocusSupported[2];
    mutable UINT32 m_absFocusCheckTime[2];
    mutable int m_autoFocusOneshotSupported[2];
    mutable UINT32 m_autoFocusOneshotCheckTime[2];
    mutable int m_queriesSupported;
    mutable UINT32 m_queriesCheckTime;
    mutable UINT32 m_settingsCheckTime;
    mutable UINT32 m_maxSpeedsCheckTime;
    mutable UINT32 m_zoomRangeCheckTime[2];
    mutable UINT32 m_lastRelCommand;
    mutable UINT16 m_panPos;                    /* Last raw pan position. */
    mutable UINT32 m_panPosCheckTime;           /* Last time pan position was updated. */
    mutable UINT8 m_panSpeed;			/* Current pan speed. */
    mutable UINT16 m_tiltPos;                   /* Last raw tilt position. */
    mutable UINT32 m_tiltPosCheckTime;          /* Last time tilt position was updated. */
    mutable UINT8 m_tiltSpeed;			/* Current tilt speed. */
    mutable UINT16 m_zoomPos[2];                /* Last raw zoom position. */
    mutable UINT32 m_zoomPosCheckTime[2];       /* Last time zoom position was updated. */
    UINT32 m_zoomSpeed[2];			/* Current zoom speed (0-3). */
    UINT32 m_zoomSpeed8[2];			/* Current zoom speed (0-63). */
    mutable UINT16 m_focusPos[2];		/* Last raw focus position. */
    mutable UINT32 m_focusPosCheckTime[2];	/* Last time focus positions was updated. */
    UINT32 m_focusSpeed[2];			/* Current focus speed. */
    mutable UINT32 m_isSecondaryCheckTime;	/* Last time this was checked. */
    mutable int m_isAbsPanCmdInProgress;        /* Absolute command in progress? */
    mutable int m_isAbsTiltCmdInProgress;       /* Absolute command in progress? */
    mutable int m_isAbsZoomCmdInProgress[2];    /* Absolute command in progress? */
    mutable int m_isAbsFocusCmdInProgress[2];	/* Absolute command in progress? */
    mutable UINT32 m_absPanCmdInProgressCheckTime; /* Last time this was checked. */
    mutable UINT32 m_absTiltCmdInProgressCheckTime; /* Last time this was checked. */
    mutable UINT32 m_absZoomCmdInProgressCheckTime[2]; /* Last time this was checked. */
    mutable UINT32 m_absFocusCmdInProgressCheckTime[2];/* Last time this was checked. */
    mutable int m_isAbsPanPosSet;                  /* Are the values below set? */
    mutable int m_isAbsTiltPosSet;                 /* Are the values below set? */
    mutable int m_isAbsZoomPosSet[2];              /* Are the values below set? */
    mutable int m_isAbsFocusPosSet[2];		   /* Are the values below set? */
    mutable UINT16 m_absCmdPanPos;              /* Used to check for pan changes. */
    mutable UINT16 m_absCmdTiltPos;             /* Used to check for tilt changes. */
    mutable UINT16 m_absCmdZoomPos[2];          /* Used to check for zoom changes. */
    mutable UINT16 m_absCmdFocusPos[2];		/* Used to check for focus changes. */
    mutable int m_isAutoFocusEnabled[2];        /* Is auto focus enabled? */
    mutable int m_isSecondarySupported;		/* Is secondary sensor supported? */
    mutable UINT16 m_firmwareVersion;		/* Firmware version */
    mutable SPxCameraCommsMode m_commsMode;
    mutable SPxCameraPanTiltResponseMode m_panTiltResponseMode;
    mutable SPxCameraZoomSpeedMode m_zoomSpeedMode;
    mutable double m_maxPanSpeedDegsPerSec;
    mutable double m_maxTiltSpeedDegsPerSec;
    UINT8 m_priority;
    UINT8 m_autoFocusOneshotPreset;

    /* Private functions. */
    void initialise(void);
    void resetCapabilities(void);
    float getTiltDegs(void) const;
    int isSecondarySupported(void) const;
    int isAbsFocusSupported(int isPrimary, int autoFocusOnly=FALSE) const;
    SPxErrorCode goToPreset(UINT8 id, int isPrimary);
    SPxErrorCode getPan(float *panAngleDegs, int genError, int forceUpdate=FALSE) const;
    SPxErrorCode getTilt(float *tiltAngleDegs, int genError, int forceUpdate=FALSE) const;
    SPxErrorCode getZoom(float *fovDegs, int isPrimary, int genError, int forceUpdate=FALSE) const;
    SPxErrorCode getFocus(float *focus, int isPrimary, int genError, int forceUpdate=FALSE) const;
    SPxErrorCode getFirmwareVersion(UINT16 *firmwareVersion, int genError, int forceUpdate=FALSE) const;
    SPxErrorCode getSettings(void) const;
    SPxErrorCode getMaxSpeeds(void) const;
    SPxErrorCode setZoomRange(float minFovDegs, float maxFovDegs, int isPrimary);
    SPxErrorCode getZoomRange(int isPrimary) const;
    SPxErrorCode setMaxZoomPosition(UINT16 pos, int isPrimary);
    int isQueriedZoomPercentage(int isPrimary) const;
    SPxErrorCode sendCommand(UINT16 command, UINT8 panSpeed,
			     UINT8 tiltSpeed, UINT16 respCode, UINT16 respCodeAlt=0,
			     int isPrimary=TRUE, const char *str=NULL) const;
    SPxErrorCode sendRawCommand(UINT32 command, UINT16 respCode, UINT16 respCodeAlt=0,
				int isPrimary=TRUE, const char *str=NULL) const;
    SPxErrorCode sendRecvRawCommand(UINT32 command, UINT32 *response,
				    UINT16 respCode, UINT16 respCodeAlt=0,
				    int *respIsAlt=NULL, int genError=TRUE,
				    int isPrimary=TRUE) const;
    SPxErrorCode sendDCommand(UINT16 command, UINT8 panSpeed=0, UINT8 tiltSpeed=0,
			      int isPrimary=TRUE, const char *str=NULL) const;
    SPxErrorCode sendPCommand(UINT16 command, UINT8 panSpeed=0, UINT8 tiltSpeed=0,
			      int isPrimary=TRUE) const;
    SPxErrorCode calculateCheckSum(SPxPelcoDMesg *msg) const;
    SPxErrorCode calculateCheckSum(SPxPelcoPMesg *msg) const;
    float getFOVForZoom(UINT16 zoomPos, int isPrimary) const;
    UINT16 getZoomForFOV(float fovDegs, int isPrimary) const;
    UINT8 getPelcoSpeed(float speed, float *speedSet) const;
    UINT32 getCmdSleepMsecs(void) const;
    void checkIfCamManager(void) const;
    virtual int recheckCameraManager(void) const { return TRUE; }

}; /* SPxCamCtrlPelco */

#endif /* _SPX_CAM_CTRL_PELCO_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
