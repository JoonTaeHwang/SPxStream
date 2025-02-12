/*********************************************************************
*
* (c) Copyright 2021, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxRemoteCamera class.
*
* Revision Control:
*   16/03/22 v1.9    AGC	Re-apply priority when changing camera index.
*
* Previous Changes:
*   21/06/21 1.8    AJH	Add IsAbsCmdInProgress().
*   21/06/21 1.7    AJH	Add Set/GetDisplayName().
*   21/06/21 1.6    AGC	Add ReleaseControl().
*   17/06/21 1.5    AJH	Install callback for remote connection.
*   26/05/21 1.4    AJH	Add Set/GetFocusMetres().
*   19/05/21 1.3    AJH	Add StartTour() and GetFollowedTrack().
*   11/05/21 1.2    AJH	Add set/get parameter.
*   23/03/21 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_REMOTE_CAMERA_H
#define _SPX_REMOTE_CAMERA_H

/* Library headers. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxCamera.h" /* For various enums. */

/* Forward declarations. */
class SPxUniTrack;

class SPxRemoteCamera : public SPxObj
{
public:
    /* Constructor and destructor. */
    explicit SPxRemoteCamera(SPxRemoteServer *remoteServer);
    virtual ~SPxRemoteCamera(void);

    SPxErrorCode SetCameraIndex(UINT8 camIndex);
    UINT8 GetCameraIndex(void) const;

    int IsSecondarySupported(void);

    /* Check for support within any camera controller. */
    int IsAbsPanSupported(void);
    int IsAbsTiltSupported(void);
    int IsAbsZoomSupported(int isPrimary=TRUE);
    int IsAbsFocusSupported(int isPrimary=TRUE);
    int IsRelPanSupported(void);
    int IsRelTiltSupported(void);
    int IsRelPanTiltSupported(void);
    int IsRelZoomSupported(int isPrimary=TRUE);
    int IsRelFocusSupported(int isPrimary=TRUE);
    int IsAutoFocusSupported(int isPrimary=TRUE);

    /* Location and positioning. */
    SPxErrorCode SetLatLong(double latDegs, double lonDegs);
    SPxErrorCode GetLatLong(double *latDegsRtn, double *lonDegsRtn);
    SPxErrorCode SetAltitudeMetres(double altitudeMetres);
    SPxErrorCode GetAltitudeMetres(double *altitudeMetres);
    SPxErrorCode SetAltitudeAboveGround(int altitudeAboveGround);
    int IsAltitudeAboveGround(void);
    SPxErrorCode SetFixedBearingDegs(double fixedBearingDegs);
    SPxErrorCode GetFixedBearingDegs(double *fixedBearingDegs);
    SPxErrorCode SetFixedElevationDegs(double fixedElevationDegs);
    SPxErrorCode GetFixedElevationDegs(double *fixedElevationDegs);
    SPxErrorCode SetFixedFOVDegs(double fixedFovDegs, int isPrimary=TRUE);
    SPxErrorCode GetFixedFOVDegs(double *fixedFovDegs, int isPrimary=TRUE);
    SPxErrorCode SetBearingCorrectionDegs(double bearingCorrectionDegs);
    SPxErrorCode GetBearingCorrectionDegs(double *bearingCorrectionDegs);
    SPxErrorCode SetElevationCorrectionDegs(double elevationCorrectionDegs);
    SPxErrorCode GetElevationCorrectionDegs(double *elevationCorrectionDegs);
    SPxErrorCode SetLeanCorrectionTiltDegs1(double leanCorrectionTiltDegs1);
    SPxErrorCode GetLeanCorrectionTiltDegs1(double *leanCorrectionTiltDegs1);
    SPxErrorCode SetLeanCorrectionBearingDegs1(double leanCorrectionBearingDegs1);
    SPxErrorCode GetLeanCorrectionBearingDegs1(double *leanCorrectionBearingDegs1);
    SPxErrorCode SetLeanCorrectionTiltDegs2(double leanCorrectionTiltDegs2);
    SPxErrorCode GetLeanCorrectionTiltDegs2(double *leanCorrectionTiltDegs2);
    SPxErrorCode SetLeanCorrectionBearingDegs2(double leanCorrectionBearingDegs2);
    SPxErrorCode GetLeanCorrectionBearingDegs2(double *leanCorrectionBearingDegs2);

    /* Homing. */
    SPxErrorCode SetHomeBearingDegs(double homeBearingDegs);
    SPxErrorCode GetHomeBearingDegs(double *homeBearingDegs);
    SPxErrorCode SetHomeElevationDegs(double homeElevationDegs);
    SPxErrorCode GetHomeElevationDegs(double *homeElevationDegs);
    SPxErrorCode SetHomeFOVDegs(double homeFovDegs, int isPrimary=TRUE);
    SPxErrorCode GetHomeFOVDegs(double *homeFovDegs, int isPrimary=TRUE);
    SPxErrorCode Home(void);
    int IsHome(void);
    SPxErrorCode Stop(void);
    SPxErrorCode StartTour(void);
    int OnTour(void);

    /* Absolute movement, if possible. */
    SPxErrorCode SetBearingDegs(double bearingDegs);
    SPxErrorCode GetBearingDegs(double *bearingDegs);
    SPxErrorCode SetElevationDegs(double elevationDegs);
    SPxErrorCode GetElevationDegs(double *elevationDegs);
    SPxErrorCode SetFOVDegs(double fovDegs, int isPrimary=TRUE);
    SPxErrorCode GetFOVDegs(double *fovDegs, int isPrimary=TRUE);
    SPxErrorCode SetFocusMetres(double rangeMetres, int isPrimary=TRUE);
    SPxErrorCode GetFocusMetres(double *rangeMetres, int isPrimary=TRUE);
    SPxErrorCode SetZoomRange(double minFovDegs, double maxFovDegs, int isPrimary=TRUE);
    SPxErrorCode GetZoomRange(double *minFovDegs, double *maxFovDegs, int isPrimary=TRUE);
    SPxErrorCode SetZoomLUTFilename(const char *filename, int isPrimary=TRUE);
    SPxErrorCode GetZoomLUTFilename(char *buffer, int bufLen, int isPrimary=TRUE);
    SPxErrorCode SetFocusLUTFilename(const char *filename, int isPrimary=TRUE);
    SPxErrorCode GetFocusLUTFilename(char *buffer, int bufLen, int isPrimary=TRUE);
    int IsAbsCmdInProgress(void);

    /* Camera properties. */
    SPxErrorCode SetMaxPanSpeedDegsPerSec(double maxPanSpeedDegsPerSec);
    SPxErrorCode GetMaxPanSpeedDegsPerSec(double *maxPanSpeedDegsPerSec);
    SPxErrorCode SetMaxTiltSpeedDegsPerSec(double maxTiltSpeedDegsPerSec);
    SPxErrorCode GetMaxTiltSpeedDegsPerSec(double *maxTiltSpeedDegsPerSec);
    SPxErrorCode SetMinSmoothSpeedDegsPerSec(double minSmoothSpeedDegsPerSec);
    SPxErrorCode GetMinSmoothSpeedDegsPerSec(double *minSmoothSpeedDegsPerSec);

    /* Relative movement. */
    void Pan(double speed);
    void Tilt(double speed);
    void PanTilt(double panSpeed, double tiltSpeed);
    void Zoom(double speed, int isPrimary=TRUE);
    void Focus(double speed, int isPrimary=TRUE);
    SPxErrorCode SetSpeedZoomDependent(int enable);
    int IsSpeedZoomDependent(void);

    /* Auto focus. */
    void AutoFocus(int autoFocus, int isPrimary=TRUE);
    int IsAutoFocusEnabled(int isPrimary=TRUE);

    /* Priority control. */
    SPxErrorCode GetControlStatus(SPxCameraControlStatus *status);
    SPxErrorCode GetControllerAddress(UINT32 *address);
    SPxErrorCode SetPriority(UINT8 priority);
    UINT8 GetPriority(void);
    SPxErrorCode ReleaseControl(void);

    /* Presets. */
    SPxErrorCode SetPreset(UINT8 id);
    SPxErrorCode SetPreset(const char *preset);
    SPxErrorCode ClearPreset(UINT8 id);
    SPxErrorCode ClearPreset(const char *preset);
    SPxErrorCode GoToPreset(UINT8 id, int isPrimary=TRUE);
    SPxErrorCode GoToPreset(const char *preset, int isPrimary=TRUE);
    SPxErrorCode SetPresetOnCamera(UINT8 id, int isPrimary=TRUE);
    SPxErrorCode SetPresetOnCamera(const char *preset, int isPrimary=TRUE);
    SPxErrorCode ClearPresetOnCamera(UINT8 id, int isPrimary=TRUE);
    SPxErrorCode ClearPresetOnCamera(const char *preset, int isPrimary=TRUE);

    /* Composite positioning and movement. */
    SPxErrorCode SetLookAtMode(SPxCamera::LookAtMode lookAtMode);
    SPxCamera::LookAtMode GetLookAtMode(void);
    SPxErrorCode SetLookAtExtrapolateMS(UINT32 ms);
    UINT32 GetLookAtExtrapolateMS(void);
    SPxErrorCode LookAtLatLong(double latDegs, double longDegs);
    SPxErrorCode LookAtLatLong(double latDegs, double longDegs,
			       double altitudeMetres);
    SPxErrorCode LookAtTrack(const char *trackDesc);
    SPxErrorCode LookAtTrack(const SPxUniTrack *track);
    SPxErrorCode FollowTrack(const char *trackDesc);
    SPxErrorCode FollowTrack(const SPxUniTrack *track);
    const char *GetFollowedTrack(void) const;
    void SetMinVisibleWidthMetres(double widthMetres);
    double GetMinVisibleWidthMetres(void);
    void SetMinVisibleFOVDegs(double fovDegs);
    double GetMinVisibleFOVDegs(void);

    /* Tweaks to apply to LookAtLatLong() position according to user input. */
    SPxErrorCode SetUserExtraPanTiltLimitPercent(double extraLimitPercent);
    double GetUserExtraPanTiltLimitPercent(void);
    SPxErrorCode SetUserExtraZoomLimitPercent(double extraLimitPercent);
    double GetUserExtraZoomLimitPercent(void);
    SPxErrorCode SetUserExtraBearingDegs(double extraDegs);
    SPxErrorCode SetUserExtraTiltDegs(double extraDegs);
    SPxErrorCode SetUserExtraZoomMult(double extraZoom);
    double GetUserExtraBearingDegs(void);
    double GetUserExtraTiltDegs(void);
    double GetUserExtraZoomMult(void);

    /* Navigation data. */
    SPxErrorCode SetLatLongFromNavData(int set);
    int IsLatLongFromNavData(void);
    SPxErrorCode SetBearingFromNavData(int set);
    int IsBearingFromNavData(void);
    SPxErrorCode SetAltitudeFromNavData(int set);
    int IsAltitudeFromNavData(void);

    /* Miscellaneous. */
    SPxErrorCode SetDisplayName(const char *name);
    const char *GetDisplayName(void) const;

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    SPxErrorCode setPriority(UINT8 priority);
    int checkRemoteServer(void) const;
    static void connectionHandlerWrapper(SPxRemoteServer *server, void *obj, int connected);
    void connectionHandler(int connected);
    SPxErrorCode setParam(const char *param, double value);
    SPxErrorCode getParam(const char *param, double *value);

}; /* SPxRemoteCamera */

#endif /* SPX_REMOTE_CAMERA_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
