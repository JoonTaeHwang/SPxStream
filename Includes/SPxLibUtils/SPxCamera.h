/*********************************************************************
*
* (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxCamera class.
*
* Revision Control:
*   27/09/22 v1.140  AGC	Support specifying number of measurements to MeasureRange().
*
* Previous Changes:
*   26/09/22 1.139  AGC	Support adding camera controller with associated index.
*   26/09/22 1.138  AGC	Support laser dazzlers.
*   26/09/22 1.137  AGC	Further change to fix N/C #951
*			- re-apply min/max fov when secondary support detected.
*   26/09/22 1.136  AGC	Suppress spurious cppcheck warning.
*   29/03/22 1.135  AGC	Add connection ID to handle socket re-use.
*   18/02/22 1.134  AGC	Support commands for measured range.
*			Support laser pointers.
*			Support control of operational mode.
*			Support camera position replay.
*   17/12/21 1.133  AGC	Delay reverting to fixed camera positions.
*   09/09/21 1.132  AGC	Part fix for N/C #888 - disable command recording by default.
*   06/09/21 1.131  AGC	Part fix for N/C #888 - throttle position recording for fixed cameras.
*   06/09/21 1.130  AGC	Part fix for N/C #888 - record positions with correct secondary flag.
*   28/06/21 1.129  AGC	Pass from address to controller.
*   21/06/21 1.128  AJH	Add AbsCmdInProgress remote parameter.
*   21/06/21 1.127  AJH	Add DisplayName remote parameter.
*   21/06/21 1.126  AGC	Support releasing camera manager control.
*   17/06/21 1.125  AJH	Support OnTour remote command.
*   17/06/21 1.124  AGC	Support disabling priority before control in derived classes.
*   01/06/21 1.123  AGC	Support combined absolute pan/tilt.
*   26/05/21 1.122  AJH	Support remote set/get absolute focus.
*   19/05/21 1.121  AJH	Add GetFollowedTrack().
*   23/03/21 1.120  AGC	Make remote command names publicly accessible.
*   18/03/21 1.119  BTB	Support set and clear preset directly on camera.
*   17/03/21 1.118  AGC	Suppress cppcheck warning.
*   16/03/21 1.117  AGC	Support track following.
*   26/01/21 1.116  AGC	Support paired mode.
*   23/11/20 1.115  AGC	Add offsets only mode to LookAtLatLong().
*   17/11/20 1.114  AGC	Support not overriding in-progress abs pan/tilt for LookAtLatLong().
*   26/10/20 1.113  AGC	Support saving/clearing preset positions.
*   26/10/20 1.112  AGC	Derive from SPxCameraRelIface.
*   03/09/20 1.111  BTB	Add applyBearingCorrection() and applyElevationCorrection().
*   20/07/20 1.110  AGC	Record camera positions at least once a second.
*   17/07/20 1.109  AGC	Force update when enabling use of device.
*   06/07/20 1.108  AGC	Suppress spurious cppcheck warning.
*   03/07/20 1.107  AGC	Add function to remove specific recording process.
*   22/06/20 1.106  AJH	Add NUC support.
*   03/06/20 1.105  AJH	Fix error in last change.
*   03/06/20 1.104  AJH	Add black body enable.
*   20/05/20 1.103  AJH	Add retrieval of sensor image size.
*   19/05/20 1.102  AJH	Add functions for IRay camera support.
*   14/10/19 1.101  AGC	Suppress cppcheck warning.
*   26/09/19 1.100  AGC	Add MinVisibleFOVDegs parameter.
*   17/09/19 1.99   BTB	Allow setting/getting of expected position error.
*   11/09/19 1.98   BTB	Allow target width to be supplied for LookAtLatLong() calls.
*   21/08/19 1.97   BTB	Add StartTour().
*   07/08/19 1.96   BTB	Support getting zoom pos.
*   31/05/19 1.95   BTB	Add tour functionality.
*   28/05/19 1.94   AGC	First secondary support check is now blocking.
*   23/05/19 1.93   AGC	Run IsSecondarySupported() checks on camera thread.
*   09/05/19 1.92   SP 	Add GetZoomSpeed().
*   29/03/19 1.91   AGC	Handle nav data deletion.
*   21/02/19 1.90   BTB	Support specifying secondary camera FOV.
*   21/02/19 1.89   BTB	Support specifying symbol shape for rendering the camera.
*   12/02/19 1.88   BTB	Support replay specific camera corrections.
*   06/02/19 1.87   BTB	Support calling GetPTZ() on multiple controllers.
*   20/12/18 1.86   BTB	Support recording of min and max FOV.
*   18/12/18 1.85   AGC	Fix loss of primary zoom lut and range when saving.
*   05/12/18 1.84   AGC	Support minimum smooth camera speed.
*   26/11/18 1.83   SP 	Add GetLastControlTime().
*   21/09/18 1.82   AGC	Support combined PTZ queries.
*   31/08/18 1.81   AGC	Add isSecondary flag when recording camera positions.
*   15/08/18 1.80   AGC	Add IsSecondarySupported().
*   04/07/18 1.79   AGC	Use SPxTimer for LookAtLatLongInit().
*   04/07/18 1.78   AGC	Improve secondary command in progress checks.
*   02/07/18 1.77   AGC	Improve command in progress checking.
*   02/07/18 1.76   AGC	Support named presets.
*   14/06/18 1.75   BTB	Separate limits for pan/tilt offset and zoom offset.
*   12/06/18 1.74   SP 	Add isPrimary to Set/GetDisplayName().
*   11/06/18 1.73   AGC	Remove isPrimary from SetUserExtraZoomMult().
*   07/06/18 1.72   AGC	Apply zoom/focus to both sensors for LookAtLatLong().
*   07/06/18 1.71   AGC	Apply auto-focus one-shot after LookAtLatLong().
*   06/06/18 1.70   AJH	Add support for one-shot auto-focus.
*   06/06/18 1.69   AGC	Support limits of user extra offsets.
*   20/04/18 1.68   AGC	Support individual or combined sensor control.
*			Add MeasureRange().
*   18/04/18 1.67   AGC	Get priority from controller when slave.
*   17/04/18 1.66   AGC	Support altitudes above terrain.
*   17/04/18 1.65   AGC	Remove unused min/max fov commands.
*			Make WaitForGets() const.
*   12/04/18 1.64   AGC	Add elevation overload that also takes bearing.
*   14/03/18 1.63   AGC	Support format warnings with clang on Windows.
*   06/03/18 1.62   AGC	Support writeDebug() format warnings.
*   06/10/17 1.61   SP 	Add Set/GetDisplayName().
*   26/09/17 1.60   AGC	Add no-speed extrapolation option.
*   31/07/17 1.59   AGC	Support setting controller max pan/tilt speeds.
*   22/06/17 1.58   AGC	Add max speed query support.
*   06/06/17 1.57   AGC	Support measured range.
*   02/02/17 1.56   AGC	Add options for concurrent PTZ.
*   23/01/17 1.55   AGC	Add WaitForRequest() function for testing.
*   18/01/17 1.54   AGC	Support priority control.
*   19/10/16 1.53   AGC	Add GoToPreset().
*   23/09/16 1.52   AGC	Add SetUseDevice().
*   09/09/16 1.51   AGC	Allow elevation to retrieved at a bearing.
*   23/08/16 1.50   AGC	Support for disabling use of corrections.
*   03/03/16 1.49   AGC	Set/get zoom LUT filename on background thread.
*   04/02/16 1.48   AGC	Add support for retrieving camera settings.
*   22/01/16 1.47   AGC	Support high priority stop.
*   23/12/15 1.46   AGC	Allow high priority commands.
*			Allow retrieval of actual camera speed.
*   10/12/15 1.45   AGC	Add optional user supplied time for error correction.
*   26/11/15 1.44   AGC	Support record/replay of camera speeds.
*   09/11/15 1.43   AGC	Support recording of camera commands.
*			Number camera request enums.
*   03/11/15 1.42   AGC	Support recording of camera positions.
*   22/10/15 1.41   AGC	Support lean correction.
*   14/10/15 1.40   AGC	Improve extrapolation of bearing/elevation.
*   10/09/15 1.39   AGC	Do not pass LUTs to camera control when slave.
*   26/08/15 1.38   AGC	Add focus LUT support.
*			Set focus as part of LookAtLatLong().
*   27/07/15 1.37   REW	Add pixelsPerMetre to GetMetresFromLatLong().
*   24/07/15 1.36   AGC	Support extrapolation for look-at.
*			Support extrapolation for GetBearing/Elevation.
*			Improve mutex locking for zoom range.
*   24/07/15 1.35   REW	Add pixel offset support.
*   02/07/15 1.34   AGC	Add speed mode.
*			Add elevation correction.
*   25/06/15 1.33   AGC	Remove one of the LookAtLatLong() overloads.
*   25/06/15 1.32   AGC	Indicate when LookAtLatLong() has no new data.
**			Add zoom LUT configuration.
*			Add bounding box to LookAtLatLong() interface.
*   24/06/15 1.31   AGC	Add ZoomChange() function for incremental zoom.
*			Handle deletion of camera controllers.
*   23/06/15 1.30   AGC	Add direction specific absolute command in progress
*			checks.
*   12/06/15 1.29   AGC	Do more of LookAtLatLong() in main thread when possible.
*   27/05/15 1.28   REW	Add support for UserExtra PTZ offsets.
*   22/05/15 1.27   AGC	Force blocking first check for absolute movement
*			support. Add WaitForGets() function.
*   21/05/15 1.26   AGC	Call camera controller functions in background thread.
*   18/05/15 1.25   AGC	Rename GetPan/Tilt() to GetPan/TiltSpeed().
*   15/05/15 1.24   AGC	Add control over auto-zooming.
*   11/05/15 1.23   AGC	Interpolate absolute positioning from LookAt...
*   11/05/15 1.22   AGC	Support various Look-At modes.
*   05/05/15 1.21   AGC	Support speeding up camera over time.
*   29/04/15 1.20   AGC	Add mutex protection.
*			Filter errors on position, not speed.
*   22/04/15 1.19   AGC	Add simplified LookAtLatLong().
*			Add GetPan/Tilt().
*   14/04/15 1.18   AGC	User provided filter in smooth movement functions.
*   26/03/15 1.17   AGC	Initial support for smooth camera movement.
*   17/03/15 1.16   AGC	Support camera speed configuration.
*   06/03/15 1.15   AGC	Add fixed camera settings.
*			Add home settings.
*   02/03/15 1.14   AGC	Add bearing correction.
*   30/01/15 1.13   REW	Add LookAtLatLong().
*   07/11/14 1.12   AGC	Improve management of set flags.
*   26/06/14 1.11   AGC	Add IsAbsCmdInProgress().
*   20/06/14 1.10   AGC	Add auto focus functions.
*   17/02/14 1.9    AGC	Expose camera controller functions.
*   26/11/13 1.8    AGC	Add elevation.
*			Return error code from get functions.
*   22/11/13 1.7    AGC	Add altitude.
*   04/11/13 1.6    AGC	Add Set/GetZoomRange() functions.
*   24/10/13 1.5    AGC	Add initial support for camera controls.
*   27/09/13 1.4    SP 	Make constructor public.
*			Support positioning from nav data.
*			Remove SetCameraName().
*   23/08/13 1.3    SP 	Add camera bearing and FOV.
*                       Add SetCameraName().
*   14/08/13 1.2    REW	Include more appropriate library headers.
*   14/08/13 1.1    SP 	Initial version.
**********************************************************************/

#ifndef _SPX_CAMERA_H
#define _SPX_CAMERA_H

/*
 * Other headers required.
 */
#include <cstdio> /* For std::FILE. */

/* Library headers. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxCameraRelIface.h"
#include "SPxLibData/SPxCamCtrlBase.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxSymbol.h"
#include "SPxLibProc/SPxProcStd.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/


/*********************************************************************
*
*   Macros
*
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declare any other classes we need. */
class SPxTerrainDatabase;
class SPxNavData;
class SPxCamCtrlBase;
class SPxCamCtrlReplay;
class SPxCameraTour;
class SPxPIDFilter;
class SPxThread;
class SPxRunProcess;
class SPxRecord;
class SPxUniTrackDatabase;
struct SPxPacketHeaderB_tag;
struct SPxTime_tag;

/* SPxCamera command names (also used by SPxRemoteCamera). */
static const char SPX_CAMERA_ZOOM_RANGE_MIN_CMD[] = "ZoomRangeMin";
static const char SPX_CAMERA_ZOOM_RANGE_MAX_CMD[] = "ZoomRangeMax";
static const char SPX_CAMERA_ZOOM_LUT_CMD[] = "ZoomLUT";
static const char SPX_CAMERA_FOCUS_LUT_CMD[] = "FocusLUT";
static const char SPX_CAMERA_ZOOM_RANGE_MIN_SECONDARY_CMD[] = "ZoomRangeMinSecondary";
static const char SPX_CAMERA_ZOOM_RANGE_MAX_SECONDARY_CMD[] = "ZoomRangeMaxSecondary";
static const char SPX_CAMERA_ZOOM_LUT_SECONDARY_CMD[] = "ZoomLUTSecondary";
static const char SPX_CAMERA_FOCUS_LUT_SECONDARY_CMD[] = "FocusLUTSecondary";
static const char SPX_CAMERA_LAT_LONG_FROM_NAV_DATA_CMD[] = "LatLongFromNavData";
static const char SPX_CAMERA_BEARING_FROM_NAV_DATA_CMD[] = "BearingFromNavData";
static const char SPX_CAMERA_ALTITUDE_FROM_NAV_DATA_CMD[] = "AltitudeFromNavData";
static const char SPX_CAMERA_LAT_LONG_CMD[] = "LatLong";
static const char SPX_CAMERA_OFFSET_PIXELS_CMD[] = "OffsetPixels";
static const char SPX_CAMERA_ALTITUDE_METRES_CMD[] = "AltitudeMetres";
static const char SPX_CAMERA_ALTITUDE_ABOVE_GROUND_CMD[] = "AltitudeAboveGround";
static const char SPX_CAMERA_FIXED_BEARING_DEGS_CMD[] = "FixedBearingDegs";
static const char SPX_CAMERA_FIXED_ELEVATION_DEGS_CMD[] = "FixedElevationDegs";
static const char SPX_CAMERA_FIXED_FOV_DEGS_CMD[] = "FixedFovDegs";
static const char SPX_CAMERA_FIXED_FOV_DEGS_SECONDARY_CMD[] = "FixedFovDegsSecondary";
static const char SPX_CAMERA_BEARING_COR_DEGS_CMD[] = "BearingCorrectionDegs";
static const char SPX_CAMERA_ELEVATION_COR_DEGS_CMD[] = "ElevationCorrectionDegs";
static const char SPX_CAMERA_LEAN_COR_TILT_DEGS_1_CMD[] = "LeanCorrectionTiltDegs1";
static const char SPX_CAMERA_LEAN_COR_BEARING_DEGS_1_CMD[] = "LeanCorrectionBearingDegs1";
static const char SPX_CAMERA_LEAN_COR_TILT_DEGS_2_CMD[] = "LeanCorrectionTiltDegs2";
static const char SPX_CAMERA_LEAN_COR_BEARING_DEGS_2_CMD[] = "LeanCorrectionBearingDegs2";
static const char SPX_CAMERA_REPLAY_BEARING_COR_DEGS_CMD[] = "ReplayBearingCorrectionDegs";
static const char SPX_CAMERA_REPLAY_ELEVATION_COR_DEGS_CMD[] = "ReplayElevationCorrectionDegs";
static const char SPX_CAMERA_REPLAY_LEAN_COR_TILT_DEGS_1_CMD[] = "ReplayLeanCorrectionTiltDegs1";
static const char SPX_CAMERA_REPLAY_LEAN_COR_BEARING_DEGS_1_CMD[] = "ReplayLeanCorrectionBearingDegs1";
static const char SPX_CAMERA_REPLAY_LEAN_COR_TILT_DEGS_2_CMD[] = "ReplayLeanCorrectionTiltDegs2";
static const char SPX_CAMERA_REPLAY_LEAN_COR_BEARING_DEGS_2_CMD[] = "ReplayLeanCorrectionBearingDegs2";
static const char SPX_CAMERA_HOME_BEARING_DEGS_CMD[] = "HomeBearingDegs";
static const char SPX_CAMERA_HOME_ELEVATION_DEGS_CMD[] = "HomeElevationDegs";
static const char SPX_CAMERA_HOME_FOV_DEGS_CMD[] = "HomeFOVDegs";
static const char SPX_CAMERA_HOME_FOV_DEGS_SECONDARY_CMD[] = "HomeFOVDegs";
static const char SPX_CAMERA_MAX_PAN_SPEED_DEGS_PER_SEC_CMD[] = "MaxPanSpeedDegsPerSec";
static const char SPX_CAMERA_MAX_TILT_SPEED_DEGS_PER_SEC_CMD[] = "MaxTiltSpeedDegsPerSec";
static const char SPX_CAMERA_IS_SPEED_ZOOM_DEPENDENT_CMD[] = "IsSpeedZoomDependent";
static const char SPX_CAMERA_MIN_SMOOTH_SPEED_DEGS_PER_SEC_CMD[] = "MinSmoothSpeedDegsPerSec";
static const char SPX_CAMERA_SPEED_MODE_CMD[] = "SpeedMode";
static const char SPX_CAMERA_LOOK_AT_MODE_CMD[] = "LookAtMode";
static const char SPX_CAMERA_LOOK_AT_EXTRAPOLATE_MS_CMD[] = "LookAtExtrapolateMS";
static const char SPX_CAMERA_MIN_VISIBLE_WIDTH_METRES_CMD[] = "MinVisibleWidthMetres";
static const char SPX_CAMERA_MIN_VISIBLE_FOV_DEGS_CMD[] = "MinVisibleFOVDegs";
static const char SPX_CAMERA_USER_EXTRA_PANTILT_LIMIT_PERCENT_CMD[] = "UserExtraPanTiltLimitPercent";
static const char SPX_CAMERA_USER_EXTRA_ZOOM_LIMIT_PERCENT_CMD[] = "UserExtraZoomLimitPercent";
static const char SPX_CAMERA_USER_EXTRA_BEARING_DEGS_CMD[] = "UserExtraBearingDegs";
static const char SPX_CAMERA_USER_EXTRA_TILT_DEGS_CMD[] = "UserExtraTiltDegs";
static const char SPX_CAMERA_USER_EXTRA_ZOOM_MULT_CMD[] = "UserExtraZoomMult";
static const char SPX_CAMERA_PRIORITY_CMD[] = "Priority";
static const char SPX_CAMERA_VERBOSITY_CMD[] = "Verbosity";
static const char SPX_CAMERA_LOG_FILE_CMD[] = "LogFile";
static const char SPX_CAMERA_EXPECTED_POS_ERR_CMD[] = "ExpectedPositionErrorDegs";
static const char SPX_CAMERA_MEASURED_RANGE_SUPPORTED_CMD[] = "MeasuredRangeSupported";
static const char SPX_CAMERA_MEASURE_RANGE_CMD[] = "MeasureRange";
static const char SPX_CAMERA_LASER_POINTER_SUPPORTED_CMD[] = "LaserPointerSupported";
static const char SPX_CAMERA_LASER_POINTER_ARM_CMD[] = "LaserPointerArm";
static const char SPX_CAMERA_LASER_POINTER_ON_CMD[] = "LaserPointerOn";
static const char SPX_CAMERA_LASER_DAZZLER_SUPPORTED_CMD[] = "LaserDazzlerSupported";
static const char SPX_CAMERA_LASER_DAZZLER_ON_CMD[] = "LaserDazzlerOn";
static const char SPX_CAMERA_LASER_DAZZLER_STROBE_CMD[] = "LaserDazzlerStrobe";
static const char SPX_CAMERA_OP_MODE_SUPPORTED_CMD[] = "OpModeSupported";
static const char SPX_CAMERA_OP_MODE_CMD[] = "OpMode";
static const char SPX_CAMERA_PALETTE_CMD[] = "Palette";
static const char SPX_CAMERA_FREEZE_CMD[] = "Freeze";
static const char SPX_CAMERA_BLACKBODY_TEMPERATURE_CMD[] = "BlackBodyTemperature";
static const char SPX_CAMERA_BLACKBODY_POSITION_CMD[] = "BlackBodyPosition";
static const char SPX_CAMERA_BLACKBODY_ENABLED_CMD[] = "BlackBodyEnabled";
static const char SPX_CAMERA_AUTO_FOCUS_ONESHOT_CMD[] = "AutoFocusOneshot";
static const char SPX_CAMERA_NUC_AUTO_CMD[] = "NUCAuto";
static const char SPX_CAMERA_NUC_INTERVAL_CMD[] = "NUCInterval";

static const char SPX_CAMERA_SECONDARY_SUPPORTED_CMD[] = "SecondarySupported";

static const char SPX_CAMERA_ABS_PAN_SUPPORTED_CMD[] = "AbsPanSupported";
static const char SPX_CAMERA_ABS_TILT_SUPPORTED_CMD[] = "AbsTiltSupported";
static const char SPX_CAMERA_ABS_PAN_TILT_SUPPORTED_CMD[] = "AbsPanTiltSupported";
static const char SPX_CAMERA_ABS_ZOOM_SUPPORTED_CMD[] = "AbsZoomSupported";
static const char SPX_CAMERA_ABS_ZOOM_SUPPORTED_SECONDARY_CMD[] = "AbsZoomSupportedSecondary";
static const char SPX_CAMERA_ABS_FOCUS_SUPPORTED_CMD[] = "AbsFocusSupported";
static const char SPX_CAMERA_ABS_FOCUS_SUPPORTED_SECONDARY_CMD[] = "AbsFocusSupportedSecondary";
static const char SPX_CAMERA_REL_PAN_SUPPORTED_CMD[] = "RelPanSupported";
static const char SPX_CAMERA_REL_TILT_SUPPORTED_CMD[] = "RelTiltSupported";
static const char SPX_CAMERA_REL_PANTILT_SUPPORTED_CMD[] = "RelPanTiltSupported";
static const char SPX_CAMERA_REL_ZOOM_SUPPORTED_CMD[] = "RelZoomSupported";
static const char SPX_CAMERA_REL_ZOOM_SUPPORTED_SECONDARY_CMD[] = "RelZoomSupportedSecondary";
static const char SPX_CAMERA_REL_FOCUS_SUPPORTED_CMD[] = "RelFocusSupported";
static const char SPX_CAMERA_REL_FOCUS_SUPPORTED_SECONDARY_CMD[] = "RelFocusSupportedSecondary";
static const char SPX_CAMERA_AUTO_FOCUS_SUPPORTED_CMD[] = "AutoFocusSupported";
static const char SPX_CAMERA_AUTO_FOCUS_SUPPORTED_SECONDARY_CMD[] = "AutoFocusSupportedSecondary";

static const char SPX_CAMERA_CONTROL_STATUS_CMD[] = "ControlStatus";
static const char SPX_CAMERA_CONTROLLER_ADDRESS_CMD[] = "ControllerAddress";
static const char SPX_CAMERA_RELEASE_CONTROL_CMD[] = "ReleaseControl";
static const char SPX_CAMERA_HOME_CMD[] = "Home";
static const char SPX_CAMERA_BEARING_DEGS_CMD[] = "BearingDegs";
static const char SPX_CAMERA_ELEVATION_DEGS_CMD[] = "ElevationDegs";
static const char SPX_CAMERA_FOV_DEGS_CMD[] = "FOVDegs";
static const char SPX_CAMERA_FIELD_OF_VIEW_DEGS_CMD[] = "FieldOfViewDegs";
static const char SPX_CAMERA_FOV_DEGS_SECONDARY_CMD[] = "FOVDegsSecondary";
static const char SPX_CAMERA_FIELD_OF_VIEW_DEGS_SECONDARY_CMD[] = "FieldOfViewDegsSecondary";
static const char SPX_CAMERA_AUTO_FOCUS_CMD[] = "AutoFocus";
static const char SPX_CAMERA_AUTO_FOCUS_ENABLED_CMD[] = "AutoFocusEnabled";
static const char SPX_CAMERA_AUTO_FOCUS_SECONDARY_CMD[] = "AutoFocusSecondary";
static const char SPX_CAMERA_AUTO_FOCUS_ENABLED_SECONDARY_CMD[] = "AutoFocusEnabledSecondary";
static const char SPX_CAMERA_FOCUS_METRES_CMD[] = "FocusMetres";
static const char SPX_CAMERA_FOCUS_METRES_SECONDARY_CMD[] = "FocusMetresSecondary";

static const char SPX_CAMERA_PAN_CMD[] = "Pan";
static const char SPX_CAMERA_TILT_CMD[] = "Tilt";
static const char SPX_CAMERA_PAN_TILT_CMD[] = "PanTilt";
static const char SPX_CAMERA_ZOOM_CMD[] = "Zoom";
static const char SPX_CAMERA_ZOOM_SECONDARY_CMD[] = "ZoomSecondary";
static const char SPX_CAMERA_FOCUS_CMD[] = "Focus";
static const char SPX_CAMERA_FOCUS_SECONDARY_CMD[] = "FocusSecondary";
static const char SPX_CAMERA_STOP_CMD[] = "Stop";
static const char SPX_CAMERA_ABS_CMD_IN_PROGRESS_CMD[] = "AbsCmdInProgress";

static const char SPX_CAMERA_POINT_TO_LAT_LONG_CMD[] = "PointToLatLong";
static const char SPX_CAMERA_POINT_TO_TRACK_CMD[] = "PointToTrack";
static const char SPX_CAMERA_FOLLOW_TRACK_CMD[] = "FollowTrack";

static const char SPX_CAMERA_SET_PRESET_CMD[] = "SetPreset";
static const char SPX_CAMERA_CLEAR_PRESET_CMD[] = "ClearPreset";
static const char SPX_CAMERA_GO_TO_PRESET_CMD[] = "GoToPreset";
static const char SPX_CAMERA_GO_TO_PRESET_SECONDARY_CMD[] = "GoToPresetSecondary";
static const char SPX_CAMERA_SET_PRESET_ON_CAMERA_CMD[] = "SetPresetOnCamera";
static const char SPX_CAMERA_SET_PRESET_ON_CAMERA_SECONDARY_CMD[] = "SetPresetOnCameraSecondary";
static const char SPX_CAMERA_CLEAR_PRESET_ON_CAMERA_CMD[] = "ClearPresetOnCamera";
static const char SPX_CAMERA_CLEAR_PRESET_ON_CAMERA_SECONDARY_CMD[] = "ClearPresetOnCameraSecondary";

static const char SPX_CAMERA_START_TOUR_CMD[] = "StartTour";
static const char SPX_CAMERA_ON_TOUR_CMD[] = "OnTour";

static const char SPX_CAMERA_DISPLAY_NAME_CMD[] = "DisplayName";

static const char SPX_CAMERA_RECORD_COMMANDS_CMD[] = "RecordCommands";

static const char SPX_CAMERA_MIN_REC_POS_INTERVAL_MS_CMD[] = "MinRecPosIntervalMS";

/*
 * Define our class.
 */
class SPxCamera : public SPxObj, public SPxCameraRelIface
{

/* The database class can access our internals directly. */
friend class SPxThreatDatabase;

public:
    /*
     * Public types.
     */
    enum SensorMode
    {
	SENSOR_MODE_PRIMARY = 0,    /* Single Zoom/Focus controller. */
	SENSOR_MODE_SECONDARY = 1,  /* Single Zoom/Focus controller - commands to secondary sensor. */
	SENSOR_MODE_DUAL = 2,	    /* Single Zoom/Focus controller - commands to specified sensor OR
				     * Multiple Zoom/Focus controller - commands sent to specified controller.
				     */
	SENSOR_MODE_PAIRED = 3	    /* Single or multiple zoom/focus controller - sensors synced. */
    };

    enum LookAtMode
    {
	LOOK_AT_MODE_ABSOLUTE = 0,
	LOOK_AT_MODE_MIXED = 1,
	LOOK_AT_MODE_SMOOTH = 2
    };

    enum SpeedMode
    {
	SPEED_MODE_LINEAR = 0,
	SPEED_MODE_QUAD = 1
    };

    enum PosType
    {
	POS_NORMAL = 0,
	POS_EXTRAPOLATE = 1,
	POS_EXTRAPOLATE_NOSPEED = 2
    };

    enum Priority
    {
	PRIORITY_NORMAL = 0,
	PRIORITY_HIGH = 1
    };

    typedef SPxErrorCode (*FileWriter)(SPxRunProcess *rp,
				 UINT16 packetType,
				 const void *payload,
				 unsigned int payloadBytes,
				 UCHAR channelIndex);

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    explicit SPxCamera(SPxTerrainDatabase *terrainDB=NULL);
    virtual ~SPxCamera(void);

    SPxErrorCode SetSensorMode(SensorMode sensorMode);
    SensorMode GetSensorMode(void) const;
    SPxErrorCode SetSlave(int slave) { return SetSensorMode(slave ? SENSOR_MODE_SECONDARY : SENSOR_MODE_PRIMARY); }
    int IsSlave(void) const { return (GetSensorMode() == SENSOR_MODE_SECONDARY); }

    SPxErrorCode SetUseDevice(int useDevice);

    /* Display name. */
    virtual SPxErrorCode SetDisplayName(const char *name, int isPrimary=TRUE);
    virtual SPxErrorCode GetDisplayName(char *buf, unsigned int bufLen, int isPrimary=TRUE) const;

    /* Controllers. */
    SPxErrorCode AddCameraControl(SPxCamCtrlBase *camCtrl, int isPrimary=TRUE, int index=-1);
    SPxErrorCode RemoveCameraControl(SPxCamCtrlBase *camCtrl);
    int HasCameraControl(void) const;
    int HasCameraControlIndices(void) const;
    unsigned int GetNumCameraControl(void) const;
    SPxCamCtrlBase *GetCameraControl(unsigned int index) const;
    SPxCamCtrlBase *GetCameraControlFromIndex(unsigned int index) const;

    virtual int IsSecondarySupported(void) const;

    virtual int IsAbsCmdInProgress(UINT32 dirMask=SPX_CAMERA_DIR_ALL) const;

    /* Check for support within any camera controller. */
    virtual int IsAbsPanSupported(void) const;
    virtual int IsAbsTiltSupported(void) const;
    virtual int IsAbsPanTiltSupported(void) const;
    virtual int IsAbsZoomSupported(int isPrimary=TRUE) const;
    virtual int IsAbsFocusSupported(int isPrimary=TRUE) const;
    virtual int IsRelPanSupported(void) const;
    virtual int IsRelTiltSupported(void) const;
    virtual int IsRelPanTiltSupported(void) const;
    virtual int IsRelZoomSupported(int isPrimary=TRUE) const;
    virtual int IsRelFocusSupported(int isPrimary=TRUE) const;
    virtual int IsAutoFocusSupported(int isPrimary=TRUE) const;
    virtual int IsAutoFocusOneshotSupported(int isPrimary=TRUE) const;
    virtual int IsMeasuredRangeSupported(void) const;
    virtual int IsLaserPointerSupported(void) const;
    virtual int IsLaserDazzlerSupported(void) const;
    virtual int IsOpModeSupported(void) const;

    /* Location and positioning. */
    virtual SPxErrorCode SetLatLong(double latDegs, double lonDegs);
    virtual SPxErrorCode GetLatLong(double *latDegs, double *lonDegs);
    virtual SPxErrorCode SetOffsetPixels(int offsetPixels);
    virtual SPxErrorCode GetOffsetPixels(int *offsetPixelsPtr) const;
    virtual SPxErrorCode SetAltitudeMetres(double altitudeMetres);
    virtual SPxErrorCode GetAltitudeMetres(double *altitudeMetres) const;
    virtual SPxErrorCode SetAltitudeAboveGround(int altitudeAboveGround);
    virtual int IsAltitudeAboveGround(void) const;
    virtual SPxErrorCode GetAltitudeAboveSeaLevel(double *altitudeMetres) const;
    virtual SPxErrorCode SetFixedBearingDegs(double fixedBearingDegs);
    virtual SPxErrorCode GetFixedBearingDegs(double *fixedBearingDegs) const;
    virtual SPxErrorCode SetFixedElevationDegs(double fixedElevationDegs);
    virtual SPxErrorCode GetFixedElevationDegs(double *fixedElevationDegs) const;
    virtual SPxErrorCode SetFixedFOVDegs(double fixedFovDegs, int isPrimary=TRUE);
    virtual SPxErrorCode GetFixedFOVDegs(double *fixedFovDegs, int isPrimary=TRUE) const;
    virtual SPxErrorCode SetBearingCorrectionDegs(double bearingCorrectionDegs);
    virtual SPxErrorCode GetBearingCorrectionDegs(double *bearingCorrectionDegs) const;
    virtual SPxErrorCode SetElevationCorrectionDegs(double elevationCorrectionDegs);
    virtual SPxErrorCode GetElevationCorrectionDegs(double *elevationCorrectionDegs) const;
    virtual SPxErrorCode SetLeanCorrectionTiltDegs1(double leanCorrectionTiltDegs1);
    virtual SPxErrorCode GetLeanCorrectionTiltDegs1(double *leanCorrectionTiltDegs1) const;
    virtual SPxErrorCode SetLeanCorrectionBearingDegs1(double leanCorrectionBearingDegs1);
    virtual SPxErrorCode GetLeanCorrectionBearingDegs1(double *leanCorrectionBearingDegs1) const;
    virtual SPxErrorCode SetLeanCorrectionTiltDegs2(double leanCorrectionTiltDegs2);
    virtual SPxErrorCode GetLeanCorrectionTiltDegs2(double *leanCorrectionTiltDegs2) const;
    virtual SPxErrorCode SetLeanCorrectionBearingDegs2(double leanCorrectionBearingDegs2);
    virtual SPxErrorCode GetLeanCorrectionBearingDegs2(double *leanCorrectionBearingDegs2) const;
    virtual SPxErrorCode SetUseCorrections(int useCorrections);

    /* Homing. */
    virtual SPxErrorCode SetHomeToHere(void);
    virtual SPxErrorCode SetHomeBearingDegs(double homeBearingDegs);
    virtual SPxErrorCode GetHomeBearingDegs(double *homeBearingDegs) const;
    virtual SPxErrorCode SetHomeElevationDegs(double homeElevationDegs);
    virtual SPxErrorCode GetHomeElevationDegs(double *homeElevationDegs) const;
    virtual SPxErrorCode SetHomeFOVDegs(double homeFovDegs, int isPrimary=TRUE);
    virtual SPxErrorCode GetHomeFOVDegs(double *homeFovDegs, int isPrimary=TRUE) const;
    virtual SPxErrorCode Home(void);
    virtual int IsHome(void) const;
    virtual SPxErrorCode Stop(Priority priority=PRIORITY_NORMAL);

    /* Absolute movement, if possible. */
    virtual SPxErrorCode SetBearingDegs(double bearingDegs,
	int applyCorrections=TRUE);
    virtual SPxErrorCode GetBearingDegs(double *bearingDegs,
	PosType posType=POS_NORMAL, Priority priority=PRIORITY_NORMAL) const;
    virtual SPxErrorCode SetElevationDegs(double elevationDegs,
					  int applyCorrections=TRUE);
    virtual SPxErrorCode SetElevationDegs(double elevationDegs, double bearingDegs,
					  int applyCorrections=TRUE);
    virtual SPxErrorCode GetElevationDegs(double *elevationDegs,
	PosType posType=POS_NORMAL, Priority priority=PRIORITY_NORMAL) const;
    virtual SPxErrorCode GetElevationDegs(double *elevationDegs,
	PosType posType, Priority priority, double bearingDegs) const;
    virtual SPxErrorCode SetBearingElevationDegs(double bearingDegs,
						 double elevationDegs);
    virtual SPxErrorCode SetFOVDegs(double fovDegs, int isPrimary=TRUE);
    virtual SPxErrorCode GetFOVDegs(double *fovDegs, int isPrimary=TRUE) const;
    virtual SPxErrorCode GetZoomPos(UINT16 *zoomPos, int isPrimary=TRUE) const;
    virtual SPxErrorCode SetZoomRange(double minFovDegs, double maxFovDegs, int isPrimary=TRUE);
    virtual SPxErrorCode GetZoomRange(double *minFovDegs, double *maxFovDegs, int isPrimary=TRUE) const;
    virtual SPxErrorCode SetZoomLUTFilename(const char *filename, int isPrimary=TRUE);
    virtual SPxErrorCode GetZoomLUTFilename(char *buffer, int bufLen, int isPrimary=TRUE) const;
    virtual SPxErrorCode SetFocus(double rangeMetres, int isPrimary=TRUE);
    virtual SPxErrorCode GetFocus(double *rangeMetres, int isPrimary=TRUE) const;
    virtual SPxErrorCode SetFocusLUTFilename(const char *filename, int isPrimary=TRUE);
    virtual SPxErrorCode GetFocusLUTFilename(char *buffer, int bufLen, int isPrimary=TRUE) const;

    /* Camera properties. */
    virtual SPxErrorCode SetMaxPanSpeedDegsPerSec(double maxPanSpeedDegsPerSec);
    virtual SPxErrorCode GetMaxPanSpeedDegsPerSec(double *maxPanSpeedDegsPerSec) const;
    virtual SPxErrorCode SetMaxTiltSpeedDegsPerSec(double maxTiltSpeedDegsPerSec);
    virtual SPxErrorCode GetMaxTiltSpeedDegsPerSec(double *maxTiltSpeedDegsPerSec) const;
    virtual SPxErrorCode SetMinSmoothSpeedDegsPerSec(double minSmoothSpeedDegsPerSec);
    virtual SPxErrorCode GetMinSmoothSpeedDegsPerSec(double *minSmoothSpeedDegsPerSec) const;
    virtual SPxErrorCode GetSensorSizePixels(unsigned int *width, unsigned int *height) const;

    /* Relative movement. */
    virtual void Pan(double speed, int isTurbo=FALSE, int speedUp=FALSE);
    virtual double GetPanSpeed(int actualSpeed=FALSE, int zDep=FALSE) const;
    virtual void Tilt(double speed, int isTurbo=FALSE, int speedUp=FALSE);
    virtual double GetTiltSpeed(int actualSpeed=FALSE, int zDep=FALSE) const;
    virtual void PanTilt(double panSpeed, double tiltSpeed,
	int isTurbo=FALSE, int speedUp=FALSE)
    { PanTilt(panSpeed, tiltSpeed, isTurbo, speedUp, PRIORITY_NORMAL); }
    virtual void PanTilt(double panSpeed, double tiltSpeed,
	int isTurbo, int speedUp,
	Priority priority, int isZoomDependent=TRUE);
    virtual void Zoom(double speed, int continuePanTilt=FALSE, int isPrimary=TRUE);
    virtual double GetZoomSpeed(int isPrimary=TRUE) const;
    virtual void Focus(double speed, int continuePanTilt=FALSE, int isPrimary=TRUE);
    virtual void ZoomChange(double newProp, int isPrimary=TRUE);
    virtual SPxErrorCode SetSpeedZoomDependent(int enable);
    virtual int IsSpeedZoomDependent(void) const;
    virtual SPxErrorCode SetSpeedMode(SpeedMode mode);
    virtual SpeedMode GetSpeedMode(void) const;

    /* Auto focus. */
    virtual void AutoFocus(int autoFocus, int isPrimary=TRUE);
    virtual int IsAutoFocusEnabled(int isPrimary=TRUE) const;
    virtual void AutoFocusOneshot(int isPrimary=TRUE);

    /* Priority control. */
    virtual SPxErrorCode GetControlStatus(SPxCameraControlStatus *status) const;
    virtual SPxErrorCode GetControllerAddress(UINT32 *address) const;
    virtual SPxErrorCode SetPriority(UINT8 priority);
    virtual UINT8 GetPriority(void) const;
    virtual SPxErrorCode ReleaseControl(void);

    /* Presets. */
    virtual SPxErrorCode SetPreset(UINT8 id);
    virtual SPxErrorCode SetPreset(const char *preset);
    virtual SPxErrorCode ClearPreset(UINT8 id);
    virtual SPxErrorCode ClearPreset(const char *preset);
    virtual SPxErrorCode GoToPreset(UINT8 id, int controller=-1, int isPrimary=TRUE);
    virtual SPxErrorCode GoToPreset(const char *preset, int controller=-1, int isPrimary=TRUE);
    virtual SPxErrorCode SetPresetOnCamera(UINT8 id, int controller=-1, int isPrimary=TRUE);
    virtual SPxErrorCode SetPresetOnCamera(const char *preset, int controller=-1, int isPrimary=TRUE);
    virtual SPxErrorCode ClearPresetOnCamera(UINT8 id, int controller=-1, int isPrimary=TRUE);
    virtual SPxErrorCode ClearPresetOnCamera(const char *preset, int controller=-1, int isPrimary=TRUE);

    /* Measured range. */
    virtual SPxErrorCode MeasureRange(int numMeasurements=-1);
    virtual SPxErrorCode GetMeasuredRange(double *rangeMetres) const;
    virtual SPxErrorCode GetMeasuredRange(double *rangesMetres, unsigned int *numRanges) const;

    /* Laser pointer. */
    virtual SPxErrorCode SetLaserPointerArmed(int arm);
    virtual int IsLaserPointerArmed(void) const;
    virtual SPxErrorCode SetLaserPointerOn(int on);
    virtual int IsLaserPointerOn(void) const;

    /* Laser dazzler. */
    virtual SPxErrorCode SetLaserDazzlerOn(int on);
    virtual int IsLaserDazzlerOn(void) const;
    virtual SPxErrorCode SetLaserDazzlerStrobe(int on);
    virtual int IsLaserDazzlerStrobe(void) const;
    virtual const char *GetLaserDazzlerMessage(void) const;

    /* Query settings. */
    virtual SPxErrorCode GetFirmwareVersion(UINT16 *version) const;
    virtual SPxErrorCode GetCommsMode(SPxCameraCommsMode *mode) const;
    virtual SPxErrorCode GetPanTiltResponseMode(SPxCameraPanTiltResponseMode *mode) const;
    virtual SPxErrorCode GetZoomSpeedMode(SPxCameraZoomSpeedMode *mode) const;

    /* Composite positioning and movement. */
    virtual SPxErrorCode SetLookAtMode(LookAtMode lookAtMode);
    virtual LookAtMode GetLookAtMode(void) const;
    virtual SPxErrorCode SetLookAtExtrapolateMS(UINT32 ms);
    virtual UINT32 GetLookAtExtrapolateMS(void) const;
    virtual SPxErrorCode LookAtLatLong(double latDegs, double longDegs,
				       double altitudeMetres, int isAltitudeAboveGround,
				       int setElevation, int setZoom, int setFocus);
    virtual SPxErrorCode LookAtLatLong(SPxPIDFilter *filter,
				       double latDegs, double longDegs,
				       double altitudeMetres, int isAltitudeAboveGround,
				       double speedMps, double courseDegs,
				       int initial, int repeat,
				       int setElevation, int setZoom, int setFocus,
				       double bndLatDegs1, double bndLongDegs1,
				       double bndLatDegs2, double bndLongDegs2,
				       double bndLatDegs3, double bndLongDegs3,
				       double bndLatDegs4, double bndLongDegs4);
    virtual SPxErrorCode LookAtLatLong(SPxPIDFilter *filter,
				       double latDegs, double longDegs,
				       double altitudeMetres, int isAltitudeAboveGround,
				       double speedMps, double courseDegs,
				       int initial, int repeat,
				       int setElevation, int setZoom, int setFocus,
				       const double *extentLatLongDegs=NULL,
				       int numExtentLatLongDegs=0,
				       double targetWidthMetres=0.0,
				       int overrideInProgressAbs=TRUE,
				       int setOffsetsOnly=FALSE);
    virtual SPxErrorCode LookAtTrack(const char *trackDesc);
    virtual SPxErrorCode FollowTrack(const char *trackDesc);
    virtual const char *GetFollowedTrack(void) const;
    virtual SPxErrorCode CorrectErrPos(SPxPIDFilter *filter,
				       double panErrDegs, double tiltErrDegs,
				       double maxPanSpeed=0.0, double maxTiltSpeed=0.0,
				       const SPxTime_tag *timeEpoch=NULL);
    virtual void SetMinVisibleWidthMetres(double widthMetres) { m_minVisibleWidthMetres = widthMetres; }
    virtual double GetMinVisibleWidthMetres(void) const { return m_minVisibleWidthMetres; }
    virtual void SetMinVisibleFOVDegs(double fovDegs) { m_minVisibleFOVDegs = fovDegs; }
    virtual double GetMinVisibleFOVDegs(void) const { return m_minVisibleFOVDegs; }
    virtual void SetZoomVarianceFactor(double factor) { m_zoomVarianceFactor = factor; }
    virtual double GetZoomVarianceFactor(void) const { return m_zoomVarianceFactor; }
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetTrackDatabase(SPxUniTrackDatabase *trackDatabase);

    /* Tweaks to apply to LookAtLatLong() position according to user input. */
    virtual SPxErrorCode SetUserExtraPanTiltLimitPercent(double extraLimitPercent);
    virtual double GetUserExtraPanTiltLimitPercent(void) const { return(m_userExtraPanTiltLimitPercent); }
    virtual SPxErrorCode SetUserExtraZoomLimitPercent(double extraLimitPercent);
    virtual double GetUserExtraZoomLimitPercent(void) const { return(m_userExtraZoomLimitPercent); }
    virtual SPxErrorCode SetUserExtraBearingDegs(double extraDegs, int isPrimary=TRUE);
    virtual SPxErrorCode SetUserExtraTiltDegs(double extraDegs, int isPrimary=TRUE);
    virtual SPxErrorCode SetUserExtraZoomMult(double extraZoom);
    virtual double GetUserExtraBearingDegs(void) const { return(m_userExtraBearingDegs); }
    virtual double GetUserExtraTiltDegs(void) const { return(m_userExtraTiltDegs); }
    virtual double GetUserExtraZoomMult(void) const { return(m_userExtraZoomMult); }

    /* Lat/long to metres. */
    virtual double GetMetresFromLatLong(double latDegs, double longDegs,
					double pixelsPerMetre = 0.0);

    /* Navigation data. */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetNavData(SPxNavData *navData,
                                    unsigned int cameraIndex=0,
                                    int setLatLong=TRUE,
                                    int setBearing=FALSE,
                                    int setAltitude=FALSE);
    virtual SPxErrorCode SetLatLongFromNavData(int set);
    virtual int IsLatLongFromNavData(void) const;
    virtual SPxErrorCode SetBearingFromNavData(int set);
    virtual int IsBearingFromNavData(void) const;
    virtual SPxErrorCode SetAltitudeFromNavData(int set);
    virtual int IsAltitudeFromNavData(void) const;

    /* User data. */
    virtual void SetUserData(void *ptr) { m_userData = ptr; }
    virtual void *GetUserData(void) const { return m_userData; }

    /* Output to SPx recording process. */
    SPxErrorCode SetRecordCommands(int enable);
    int GetRecordCommands(void) const;
    SPxErrorCode SetMinRecPosIntervalMS(UINT32 minRecPosIntervalMS);
    UINT32 GetMinRecPosIntervalMS(void) const;
    SPxErrorCode AddRecordingProcess(UCHAR channelIndex, SPxRunProcess *recProc, 
				     FileWriter fn=SPxProRecordPacketB);
    SPxErrorCode RemoveRecordingProcess(SPxRunProcess *recProc);
    SPxErrorCode RemoveRecordingProcess(UCHAR channelIndex, SPxRunProcess *recProc,
					FileWriter fn=SPxProRecordPacketB);
    SPxErrorCode AddRecordingObject(UCHAR channelIndex, SPxRecord *recObj);
    SPxErrorCode RemoveRecordingObject(SPxRecord *recObj);
    SPxErrorCode RemoveRecordingObject(UCHAR channelIndex, SPxRecord *recObj);
    SPxErrorCode AddReplaySource(UCHAR channelIndex, SPxPacketDecoder* replaySource);
    SPxErrorCode RemoveReplaySource(SPxPacketDecoder* replaySource);

    /* Timer. */
    virtual SPxErrorCode Timer(void);
    virtual SPxErrorCode WaitForGets(UINT32 timeoutMS) const;
    virtual UINT32 GetLastControlTime(void) const { return m_lastControlTime; }

    /* Symbol for rendering camera. */
    virtual void SetSymbolShape(SPxSymbolShape_t symbolShape) { m_symbolShape = symbolShape; }
    virtual SPxSymbolShape_t GetSymbolShape(void) const { return m_symbolShape; }

    /* Secondary (optional) camera FOV. */
    virtual void SetSecondaryFOVEnabled(int enabled) { m_secFOVEnabled = enabled; }
    virtual int GetSecondaryFOVEnabled(void) { return m_secFOVEnabled; }
    virtual void SetSecondaryFOVWidth(double widthDegs) { m_secFOVWidthDegs = widthDegs; }
    virtual double GetSecondaryFOVWidth(void) { return m_secFOVWidthDegs; }
    virtual void SetSecondaryFOVLength(double lengthMetres) { m_secFOVLengthMetres = lengthMetres; }
    virtual double GetSecondaryFOVLength(void) { return m_secFOVLengthMetres; }

    // cppcheck-suppress virtualCallInConstructor
    virtual void SetTour(SPxCameraTour *tour);
    virtual SPxCameraTour *GetTour(void);
    virtual void StartTour(void);

    virtual SPxErrorCode SetPalette(unsigned int palette);
    virtual unsigned int GetPalette(void) const { return m_palette; }

    virtual SPxErrorCode SetFreeze(int freeze);
    virtual int GetFreeze(void) const { return m_freeze; }

    virtual SPxErrorCode SetBlackBodyTemperature(double temp);
    virtual double GetBlackBodyTemperature(void) const { return m_blackBodyTemperature; }
    virtual SPxErrorCode SetBlackBodyPosition(unsigned int x1, unsigned int y1,
	unsigned int x2, unsigned int y2);
    virtual SPxErrorCode GetBlackBodyPosition(unsigned int *x1, unsigned int *y1,
	unsigned int *x2, unsigned int *y2) const;
    virtual SPxErrorCode SetBlackBodyEnabled(int enabled);
    virtual int GetBlackBodyEnabled(void) const { return m_blackBodyEnabled; }

    virtual SPxErrorCode SetNUCAuto(int enabled);
    virtual int GetNUCAuto(void) const { return m_nucAuto; }
    virtual SPxErrorCode SetNUCInterval(int interval);
    virtual int GetNUCInterval(void) const { return m_nucInterval; }

    virtual SPxErrorCode SetOpMode(int opMode);
    virtual int GetOpMode(void) const;

    /* Generic get/set parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /*
     * Public static functions.
     */
    void SetVerbosity(UINT8 verbosity);
    UINT8 GetVerbosity(void) const;
    void SetLogFile(const char *logFile);
    const char *GetLogFile(void) const;
    void SetExpectedPositionError(double errorDegs);
    double GetExpectedPositionError(void) const;

protected:
    enum RequestType
    {
	REQ_NONE = 0,
	REQ_IS_CMD_IN_PROGRESS = 1,
	REQ_IS_ABS_PAN = 10,
	REQ_IS_ABS_TILT = 11,
	REQ_IS_ABS_ZOOM = 12,
	REQ_IS_ABS_FOCUS = 13,
	REQ_IS_ABS_PTZ = 14,
	REQ_IS_ABS_PAN_TILT = 15,
	REQ_IS_REL_PAN = 20,
	REQ_IS_REL_TILT = 21,
	REQ_IS_REL_PAN_TILT = 22,
	REQ_IS_REL_ZOOM = 23,
	REQ_IS_REL_FOCUS = 24,
	REQ_IS_AUTO_FOCUS = 30,
	REQ_IS_QUERY = 31,
	REQ_IS_MEASURED_RANGE = 32,
	REQ_IS_MAX_SPEEDS = 33,
	REQ_IS_AUTO_FOCUS_ONESHOT = 34,
	REQ_IS_SECONDARY = 35,
	REQ_IS_LASER_POINTER = 36,
	REQ_IS_OP_MODE = 37,
	REQ_IS_LASER_DAZZLER = 38,
	REQ_SET_ABS_PAN = 40,
	REQ_SET_ABS_TILT = 41,
	REQ_SET_ABS_ZOOM = 42,
	REQ_CHANGE_ZOOM = 43,
	REQ_SET_ABS_FOCUS = 44,
	REQ_SET_ABS_PAN_TILT = 45,
	REQ_GET_ABS_PAN = 50,
	REQ_GET_ABS_TILT = 51,
	REQ_GET_ABS_ZOOM = 52,
	REQ_GET_ABS_FOCUS = 53,
	REQ_GET_ABS_PTZ = 54,
	REQ_GET_ZOOM_POS = 55,
	REQ_SET_REL_PAN = 60,
	REQ_SET_REL_TILT = 61,
	REQ_SET_REL_PAN_TILT = 62,
	REQ_SET_REL_ZOOM = 63,
	REQ_SET_REL_FOCUS = 64,
	REQ_SET_MAX_SPEEDS = 65,
	REQ_SET_AUTO_FOCUS = 70,
	REQ_GET_AUTO_FOCUS = 71,
	REQ_AUTO_FOCUS_ONESHOT = 72,
	REQ_SET_PRESET = 74,
	REQ_GO_TO_PRESET = 75,
	REQ_GO_TO_PRESET_STR = 76,
	REQ_SET_PRESET_ON_CAMERA = 77,
	REQ_SET_PRESET_ON_CAMERA_STR = 78,
	REQ_CLEAR_PRESET_ON_CAMERA = 79,
	REQ_CLEAR_PRESET_ON_CAMERA_STR = 80,
	REQ_GET_MEASURED_RANGE = 81,
	REQ_MEASURE_RANGE = 82,
	REQ_SET_ZOOM_RANGE = 83,
	REQ_GET_ZOOM_RANGE = 84,
	REQ_SET_ZOOM_LUT = 85,
	REQ_GET_ZOOM_LUT = 86,
	REQ_GET_FIRMWARE_VERSION = 90,
	REQ_GET_SETTINGS = 91,
	REQ_GET_MAX_SPEEDS = 92,
	REQ_SET_PRIORITY = 95,
	REQ_GET_CONTROL = 96,
	REQ_GET_CONTROLLER_ADDR = 97,
	REQ_GET_PRIORITY = 98,
	REQ_RELEASE_CONTROL = 99,
	REQ_STOP = 100,
	REQ_SET_PALETTE = 101,
	REQ_SET_FREEZE = 102,
	REQ_SET_BLACKBODY_TEMPERATURE = 103,
	REQ_SET_BLACKBODY_POSITION = 104,
	REQ_SET_BLACKBODY_ENABLED = 105,
	REQ_SET_NUC_AUTO = 106,
	REQ_SET_NUC_INTERVAL = 107,
	REQ_SET_LASER_ARM = 110,
	REQ_SET_LASER_ON = 111,
	REQ_GET_LASER_ARM = 112,
	REQ_GET_LASER_ON = 113,
	REQ_SET_OP_MODE = 114,
	REQ_GET_OP_MODE = 115,
	REQ_SET_LASER_DAZZLE = 116,
	REQ_SET_LASER_STROBE = 117,
	REQ_GET_LASER_DAZZLE = 118,
	REQ_GET_LASER_STROBE = 119,
	REQ_GET_DAZZLER_MSG = 120,
	REQ_START_TOUR = 121
    };

    virtual SPxErrorCode WaitForRequest(RequestType reqType, UINT32 timeoutMS);

private:

    struct Request
    {
	RequestType type;
	Priority priority;
	UINT32 timeSecs;
	UINT32 timeUsecs;
	double value1;
	double value2;
	UINT16 value3;
	UINT16 value4;
	char str[1024];
	UINT32 mask;
	int inProgress;
	int isPrimary;
	int forceUseDevice;
	SOCKET_TYPE sock;
	UINT32 fromAddr;
	UINT32 id;
    };

    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;   /* Private structure. */
    SensorMode m_sensorMode;	/* Which sensors is this camera controlling? */
    int m_useDevice;	    /* Send queries to device? */
    void *m_userData;       /* User defined data. */
    SPxTerrainDatabase *m_terrainDB;	/* Terrain database. */
    double m_latDegs;       /* Latitude. */
    double m_lonDegs;       /* Longitude. */
    mutable int m_isLatLongSet;     /* Is that lat/long valid? */
    int m_offsetPixels;	    /* Offset left (-ve) or right (+ve) of mount */
    double m_homeBearingDegs;       /* Home position. */
    double m_homeElevationDegs;
    double m_homeFovDegs[2];
    double m_fixedBearingDegs;      /* Fixed positions for non-movable cameras. */
    double m_fixedElevationDegs;
    double m_fixedFovDegs[2];
    mutable double m_bearingDegs;   /* Bearing. */
    mutable UINT32 m_bearingDegsTime; /* Time the bearing was last received from camera. */
    mutable double m_prevBearingDegs; /* Previous bearing. */
    mutable UINT32 m_prevBearingDegsTime; /* Time of previous bearing received from camera. */
    double m_bearingCorDegs;        /* Bearing correction in degrees. */
    double m_altitudeMetres;        /* Altitude. */
    int m_isAltitudeAboveGround;    /* Is the altitude above ground or sea level? */
    mutable int m_isAltitudeSet;    /* Is the altitude valid? */
    mutable double m_elevationDegs; /* Elevation. */
    mutable UINT32 m_elevationDegsTime;	/* Time the elevation was last received from camera. */
    mutable double m_prevElevationDegs; /* Previous elevation. */
    mutable UINT32 m_prevElevationDegsTime;	/* Time the last elevation was last received from camera. */
    double m_elevationCorDegs;	    /* Elevation correction in degrees. */
    mutable double m_fovDegs[2];    /* Field of view. */
    mutable UINT32 m_fovDegsTime[2];	/* Last time fov was received from camera. */
    mutable UINT16 m_zoomPos[2];    /* Zoom position. */
    mutable SPxErrorCode m_lastZoomPosErr[2];  /* Error code from last zoom pos request. */
    int m_isZoomRangeSet[2];
    mutable double m_minFovDegs[2]; /* Range for field of view. */
    mutable double m_maxFovDegs[2];
    mutable double m_focus[2];	    /* Focus level. */
    mutable UINT32 m_focusTime[2];	/* Last time focus was received from camera. */
    mutable UINT16 m_firmwareVersion;		/* Firmware version */
    mutable SPxCameraCommsMode m_commsMode;
    mutable SPxCameraPanTiltResponseMode m_panTiltResponseMode;
    mutable SPxCameraZoomSpeedMode m_zoomSpeedMode;
    double m_leanCorrectionTiltDegs1; /* Mount lean correction. */
    double m_leanCorrectionBearingDegs1;
    double m_leanCorrectionTiltDegs2;
    double m_leanCorrectionBearingDegs2;
    double m_replayBearingCorDegs;
    double m_replayElevationCorDegs;
    double m_replayLeanCorrectionTiltDegs1;
    double m_replayLeanCorrectionBearingDegs1;
    double m_replayLeanCorrectionTiltDegs2;
    double m_replayLeanCorrectionBearingDegs2;
    int m_useCorrections;
    int m_recordCommands;	    /* Record camera commands? */
    SPxNavData *m_navData;	    /* Associated nav data object. */
    unsigned int m_navDataCameraIndex; /* Camera index. */
    int m_navDataSetLatLong;	    /* Set camera position from nav data? */
    int m_navDataSetBearing;	    /* Set camera bearing from nav data? */
    int m_navDataSetAltitude;	    /* Set camera altitude from nav data? */
    float m_panSpeed;		    /* Currently required pan speed. */
    float m_tiltSpeed;		    /* Currently required tilt speed. */
    float m_panSpeedZDep;	    /* Currently required Z dependent pan speed. */
    float m_tiltSpeedZDep;	    /* Currently required Z dependent tilt speed. */
    float m_panSpeedCurr;	    /* Actual pan speed in use. */
    float m_tiltSpeedCurr;	    /* Actual tilt speed in use. */
    float m_prevPanSpeedCurr;	    /* Previous actual pan speed in use. */
    float m_prevTiltSpeedCurr;	    /* Previous actual tilt speed in use. */
    UINT32 m_panSpeedTime;	    /* Last time pan speed was actually changed. */
    UINT32 m_tiltSpeedTime;	    /* Last time tile speed was actually changed. */
    UINT32 m_panTime;		    /* Last time pan speed manually adjusted. */
    UINT32 m_tiltTime;		    /* Last time tilt speed manually adjusted. */
    int m_speedUp;		    /* Speed up enabled? */
    float m_zoomSpeed[2];	    /* Current zoom speed. */
    float m_focusSpeed[2];	    /* Current focus speed. */
    mutable double m_maxPanSpeedDegsPerSec; /* Max supported pan speed. */
    mutable double m_maxTiltSpeedDegsPerSec; /* Max supported tilt speed. */
    int m_isSpeedZoomDependent;	    /* Zoom dependent speed enabled? */
    double m_minSmoothSpeedDegsPerSec; /* Minimum supported smooth camera speed. */
    double m_panSpeedErr;	    /* Current pan speed error for dithering. */
    double m_tiltSpeedErr;	    /* Current tilt speed error for dithering. */
    SpeedMode m_speedMode;	    /* Speed mode. */
    LookAtMode m_lookAtMode;	    /* Look at mode. */
    float m_panAbsDegsPerSec;	    /* Speed for absolute pan interpolation. */
    UINT32 m_panAbsTime;	    /* Time when last interpolated absolute pan took place. */
    int m_lookAtQueued;		    /* Has a look-at action been queued? */
    int m_lookAtCancel;		    /* Has the look-at action been cancelled. */
    int m_lookAtTime;		    /* Time when last look-at with new data occurred. */
    UINT32 m_lookAtExtrapolateMS;   /* Extrapolate target positions for look-at by specified time. */
    double m_minVisibleWidthMetres; /* Minimum visible width when auto-zooming. */
    double m_minVisibleFOVDegs;	    /* Minimum visible fov when auto-zooming. */
    double m_zoomVarianceFactor;    /* Relationship between variance in pan/tilt measurements and required zoom. */
    double m_userExtraPanTiltLimitPercent; /* Limit for extra bearing/tilt. */
    double m_userExtraZoomLimitPercent; /* Limit for extra zoom. */
    double m_userExtraBearingDegs;  /* Additional amount for LookAt's */
    double m_userExtraTiltDegs;     /* Additional amount for LookAt's */
    double m_userExtraZoomMult;     /* Additional amount for LookAt's */
    mutable int m_isAbsPanCmdInProgress;
    mutable int m_isAbsTiltCmdInProgress;
    mutable int m_isAbsZoomCmdInProgress[2];
    mutable int m_isAbsFocusCmdInProgress[2];
    mutable int m_isAbsPanNoSupportCount;
    mutable int m_isAbsTiltNoSupportCount;
    mutable int m_isAbsPanTiltNoSupportCount;
    mutable int m_isAbsZoomNoSupportCount[2];
    mutable int m_isAbsFocusNoSupportCount[2];
    mutable int m_isAbsPTZNoSupportCount;
    mutable int m_isRelPanSupported;
    mutable int m_isRelTiltSupported;
    mutable int m_isRelPanTiltSupported;
    mutable int m_isRelZoomSupported[2];
    mutable int m_isRelFocusSupported[2];
    mutable int m_isAutoFocusSupported[2];
    mutable int m_isAutoFocusOneshotSupported[2];
    mutable int m_isMeasuredRangeSupported;
    mutable int m_isLaserPointerSupported;
    mutable int m_isLaserDazzlerSupported;
    mutable int m_isOpModeSupported;
    mutable int m_areQueriesSupported;
    mutable int m_areMaxSpeedsSupported;
    mutable int m_isAutoFocusEnabled[2];
    mutable int m_isSecondarySupported;
    int m_haveCamControl;
    int m_haveReplayCamControl;
    int m_stopInQueue;
    mutable int m_absPanChecked;    /* Has absolute support been checked yet? */
    mutable int m_absTiltChecked;
    mutable int m_absPanTiltChecked;
    mutable int m_absZoomChecked[2];
    mutable int m_absFocusChecked[2];
    mutable int m_absPTZChecked;
    mutable int m_queriesChecked;
    mutable int m_measuredRangeChecked;
    mutable int m_laserPointerChecked;
    mutable int m_laserDazzlerChecked;
    mutable int m_opModeChecked;
    mutable int m_secondaryChecked;
    RequestType m_reqInProgress;
    int m_reqIsPrimary;
    mutable UINT8 m_priority;
    mutable SPxCameraControlStatus m_controlStatus;
    mutable UINT32 m_controllerAddr;
    mutable UINT32 m_lastControlTime;
    SPxSymbolShape_t m_symbolShape;
    int m_secFOVEnabled;
    double m_secFOVWidthDegs;
    double m_secFOVLengthMetres;
    SPxCameraTour *m_tour;
    unsigned int m_palette;
    unsigned int m_freeze;
    double m_blackBodyTemperature;
    unsigned int m_blackBodyPositionTopLeft[2];
    unsigned int m_blackBodyPositionBottomRight[2];
    int m_blackBodyEnabled;
    int m_nucAuto;
    int m_nucInterval;
    mutable int m_laserPointerArmed;
    mutable int m_laserPointerOn;
    mutable int m_laserDazzlerOn;
    mutable int m_laserDazzlerStrobe;
    mutable const char *m_laserDazzlerMsg;
    mutable int m_opMode;

    /*
     * Private functions.
     */
    SPxErrorCode lookAtLatLong(SPxPIDFilter *filter,
			       double latDegs, double longDegs,
			       double altitudeMetres,
			       int isAltitudeAboveGround,
			       double speedMps, double courseDegs,
			       int initial, int repeat,
			       int setElevation, int setZoom, int setFocus,
			       const double *extentLatLongDegs,
			       int numExtentLatLongDegs,
			       double targetWidthMetres,
			       int overrideInProgressAbs,
			       int setOffsetsOnly);
    void lookAtLatLongInit(void);

    int getSensorID(int isPrimary, int dual=FALSE) const;
    int getPrimary(int isPrimary, int dual=FALSE) const;
    int isPrimaryValid(int isPrimary) const;
    void panTilt(double panSpeed, double tiltSpeed,
		 int isTurbo=FALSE, int speedUp=FALSE,
		 Priority priority=PRIORITY_NORMAL, int isZoomDependent=TRUE,
		 int continueZoomFocus=FALSE);
    double getTiltSpeed(int actualSpeed, int zDep) const;
    SPxErrorCode stop(Priority priority=PRIORITY_NORMAL);
    int isAbsCmdInProgress(UINT32 dirMask=SPX_CAMERA_DIR_ALL) const;
    int isSecondarySupported(void) const;
    int isAbsPanSupported(void) const;
    int isAbsTiltSupported(void) const;
    int isAbsPanTiltSupported(void) const;
    int isAbsZoomSupported(int isPrimary, int dual=FALSE) const;
    int isAbsFocusSupported(int isPrimary, int dual=FALSE) const;
    int isAbsPTZSupported(void) const;
    int isRelPanSupported(void) const;
    int isRelTiltSupported(void) const;
    int isRelPanTiltSupported(void) const;
    int isRelZoomSupported(int isPrimary) const;
    int isRelFocusSupported(int isPrimary) const;
    int isAutoFocusSupported(int isPrimary) const;
    int isAutoFocusOneshotSupported(int isPrimary, int dual=FALSE) const;
    int isMeasuredRangeSupported(void) const;
    int isLaserPointerSupported(void) const;
    int isLaserDazzlerSupported(void) const;
    int isOpModeSupported(void) const;
    int areQueriesSupported(void) const;
    int areMaxSpeedsSupported(void) const;
    SPxErrorCode getLatLong(double *latDegs, double *lonDegs);
    SPxErrorCode getAltitudeMetres(double *altitudeMetres) const;
    SPxErrorCode setBearingDegs(double bearingDegs, int applyCorrections=TRUE);
    SPxErrorCode getBearingDegs(double *bearingDegs,
				PosType posType=POS_NORMAL, Priority priority=PRIORITY_NORMAL,
				int forceUseDevice=FALSE) const;
    SPxErrorCode setElevationDegs(double elevationDegs,
				  int currentBearing=TRUE, double bearingDegs=0.0,
				  int applyCorrections=TRUE);
    SPxErrorCode getElevationDegs(double *elevationDegs,
				  PosType posType=POS_NORMAL, Priority priority=PRIORITY_NORMAL,
				  int haveBearing=FALSE, double bearingDegs=0.0,
				  int forceUseDevice=FALSE) const;
    SPxErrorCode setBearingElevationDegs(double bearingDegs, double elevationDegs);
    double getLeanDegs(int currentBearing, double bearingDegs) const;
    SPxErrorCode setFOVDegs(double fovDegs, int isPrimary, int dual=FALSE);
    SPxErrorCode getFOVDegs(double *fovDegs, int isPrimary, int dual=FALSE,
			    int forceUseDevice=FALSE) const;
    SPxErrorCode getZoomPos(UINT16 *zoomPos, int isPrimary) const;
    SPxErrorCode setZoomRange(double minFovDegs, double maxFovDegs, int isPrimary);
    SPxErrorCode getZoomRange(double *minFovDegs, double *maxFovDegs, int isPrimary) const;
    SPxErrorCode setFocusMetres(double focus, int isPrimary, int dual=FALSE);
    SPxErrorCode getFocusMetres(double *focus, int isPrimary) const;
    int isAutoFocusEnabled(int isPrimary,int dual=FALSE) const;
    void autoFocusOneshot(int isPrimary, int dual=FALSE);
    void zoomChange(double newProp, int isPrimary=TRUE);
    SPxErrorCode getControlStatus(SPxCameraControlStatus *status) const;
    SPxErrorCode getControllerAddress(UINT32 *address) const;
    SPxErrorCode setPriority(UINT8 priority);
    SPxErrorCode correctErrPos(SPxPIDFilter *filter,
			       double panErrDegs, double tiltErrDegs,
			       int setElevation, int setZoom, int setFocus,
			       double maxPanSpeed=0.0, double maxTiltSpeed=0.0,
			       const SPxTime_tag *timeEpoch=NULL);
    double getFovFromDistance(double distMetres, double bearingDegs,
	const double *extentLatLongDegs, int numExtentLatLongDegs,
	SPxPIDFilter *filter, double targetWidthMetres);
    double applySpeedMode(double speed) const;
    SPxErrorCode setBearingCorrectionDegs(double bearingCorrectionDegs, int liveCorrection);
    SPxErrorCode getBearingCorrectionDegs(double *bearingCorrectionDegs, int liveCorrection) const;
    SPxErrorCode setElevationCorrectionDegs(double elevationCorrectionDegs, int liveCorrection);
    SPxErrorCode getElevationCorrectionDegs(double *elevationCorrectionDegs, int liveCorrection) const;
    SPxErrorCode setLeanCorrectionTiltDegs1(double leanCorrectionTiltDegs1, int liveCorrection);
    SPxErrorCode getLeanCorrectionTiltDegs1(double *leanCorrectionTiltDegs1, int liveCorrection) const;
    SPxErrorCode setLeanCorrectionBearingDegs1(double leanCorrectionBearingDegs1, int liveCorrection);
    SPxErrorCode getLeanCorrectionBearingDegs1(double *leanCorrectionBearingDegs1, int liveCorrection) const;
    SPxErrorCode setLeanCorrectionTiltDegs2(double leanCorrectionTiltDegs2, int liveCorrection);
    SPxErrorCode getLeanCorrectionTiltDegs2(double *leanCorrectionTiltDegs2, int liveCorrection) const;
    SPxErrorCode setLeanCorrectionBearingDegs2(double leanCorrectionBearingDegs2, int liveCorrection);
    SPxErrorCode getLeanCorrectionBearingDegs2(double *leanCorrectionBearingDegs2, int liveCorrection) const;
    double applyBearingCorrection(double bearingDegs) const;
    double applyElevationCorrection(double elevationDegs, int currentBearing, double bearingDegs) const;

    void
#if defined(__GNUC__) || defined(__clang__)
	__attribute__((format(printf, 3, 4)))
#endif
    writeDebug(UINT8 verbosity, const char *format, ...) const;

    static void *threadFnWrapper(SPxThread *thread);
    void *threadFn(SPxThread *thread);
    void request(RequestType reqType, Priority priority=PRIORITY_NORMAL,
		 double value1=0.0, double value2=0.0, const char *str=NULL, UINT32 mask=0,
		 UINT16 value3=0, int isPrimary=TRUE, UINT16 value4=0,
		 int forceUseDevice=FALSE) const;
    int isRequestControlType(RequestType reqType) const;
    void reqIsAbsCmdInProgress(UINT32 dirMask=SPX_CAMERA_DIR_ALL) const;
    void reqIsAbsPanSupported(void) const;
    void reqIsAbsTiltSupported(void) const;
    void reqIsAbsPanTiltSupported(void) const;
    void reqIsAbsZoomSupported(int isPrimary, int dual) const;
    void reqIsAbsFocusSupported(int isPrimary, int dual) const;
    void reqIsAbsPTZSupported(void) const;
    void reqIsRelPanSupported(void) const;
    void reqIsRelTiltSupported(void) const;
    void reqIsRelPanTiltSupported(void) const;
    void reqIsRelZoomSupported(int isPrimary) const;
    void reqIsRelFocusSupported(int isPrimary) const;
    void reqIsAutoFocusSupported(int isPrimary) const;
    void reqIsAutoFocusOneshotSupported(int isPrimary, int dual) const;
    void reqIsMeasuredRangeSupported(void) const;
    void reqIsLaserPointerSupported(void) const;
    void reqIsLaserDazzlerSupported(void) const;
    void reqIsOpModeSupported(void) const;
    void reqAreQueriesSupported(void) const;
    void reqAreMaxSpeedsSupported(void) const;
    int reqIsSecondarySupported(void) const;
    void reqSetPanDegs(double panDegs);
    void reqSetTiltDegs(double tiltDegs);
    void reqSetPanTiltDegs(double panDegs, double tiltDegs);
    void reqSetFOVDegs(double fovDegs, int isPrimary, int dual);
    void reqChangeFOVDegs(double newProp, int isPrimary, int dual);
    void reqSetFocus(double focus, int isPrimary, int dual);
    void reqGetPanDegs(int forceUseDevice) const;
    void reqGetTiltDegs(int forceUseDevice) const;
    void reqGetFOVDegs(int isPrimary, int dual, int forceUseDevice);
    void reqGetFocus(int isPrimary) const;
    void reqGetPTZ(int isPrimary, int dual);
    void reqGetZoomPos(int isPrimary) const;
    void reqPan(double panSpeed);
    void reqTilt(double tiltSpeed);
    void reqPanTilt(double panSpeed, double tiltSpeed, UINT16 continueZoomFocus);
    void reqZoom(double zoomSpeed, UINT16 continuePanTilt, int isPrimary);
    void reqFocus(double focusSpeed, UINT16 continuePanTilt, int isPrimary);
    void reqSetMaxSpeeds(double maxPanSpeedDegsPerSec, double maxTiltSpeedDegsPerSec);
    void reqAutoFocus(int enable, int isPrimary);
    void reqAutoFocusOneshot(int isPrimary, int dual);
    void reqGetAutoFocus(int isPrimary, int dual) const;
    void reqSetPreset(const char *preset);
    void reqGoToPreset(UINT16 id, UINT32 controller, int isPrimary);
    void reqGoToPresetStr(const char *preset, UINT32 controller, int isPrimary);
    int reqGoToSavedPreset(UINT16 id);
    int reqGoToSavedPresetStr(const char *preset);
    void reqSetPresetOnCamera(UINT16 id, UINT32 controller, int isPrimary);
    void reqSetPresetOnCameraStr(const char *preset, UINT32 controller, int isPrimary);
    void reqClearPresetOnCamera(UINT16 id, UINT32 controller, int isPrimary);
    void reqClearPresetOnCameraStr(const char *preset, UINT32 controller, int isPrimary);
    void reqGetMeasuredRange(void);
    void reqMeasureRange(int numMeasurements);
    void reqSetZoomRange(double minFovDegs, double maxFovDegs, int isPrimary);
    void reqGetZoomRange(int isPrimary) const;
    void reqSetZoomLUT(const char *filename, int isPrimary);
    void reqGetZoomLUT(int isPrimary) const;
    void reqGetFirmwareVersion(void) const;
    void reqGetSettings(void) const;
    void reqGetMaxSpeeds(void) const;
    void reqSetPriority(UINT16 priority);
    void reqGetPriority(void) const;
    void reqGetControlStatus(void) const;
    void reqGetControllerAddr(void) const;
    void reqReleaseControl(void);
    void reqStop(void);
    void reqSetPalette(unsigned int palette);
    void reqSetFreeze(int freeze);
    void reqSetBlackBodyTemperature(double temp);
    void reqSetBlackBodyPosition(UINT16 x1, UINT16 y1, UINT16 x2, UINT16 y2);
    void reqSetBlackBodyEnabled(int enabled);
    void reqSetNUCAuto(int enabled);
    void reqSetNUCInterval(int interval);
    void reqSetLaserPointerArmed(int armed);
    void reqGetLaserPointerArmed(void) const;
    void reqSetLaserPointerOn(int on);
    void reqGetLaserPointerOn(void) const;
    void reqSetLaserDazzlerOn(int on);
    void reqGetLaserDazzlerOn(void) const;
    void reqSetLaserDazzlerStrobe(int on);
    void reqGetLaserDazzlerStrobe(void) const;
    void reqGetLaserDazzlerMessage(void) const;
    void reqSetOpMode(int opMode);
    void reqGetOpMode(void) const;
    void checkRecord(void);
    void recordCommand(RequestType reqType, double value1=0.0, double value2=0.0, UINT32 mask=0) const;
    void recordPos(UINT8 direction, REAL32 valueDegs, int isSecondary=FALSE, int throttle=FALSE) const;
    void recordSpeed(UINT8 direction, REAL32 valueDegsPerSec, int isSecondary=FALSE) const;
    void recordLimit(UINT8 direction, REAL32 valueDegs, int isSecondary=FALSE) const;
    void recordValue(UINT8 direction, UINT8 valType, REAL32 value, int isSecondary) const;
    static void camPosFnWrapper(SPxPacketDecoder *pktDecoder, void *userArg,
				UINT32 packetType, struct sockaddr_in *from,
				struct SPxTime_tag *time, const unsigned char *payload,
				unsigned int numBytes);
    void camPosFn(const unsigned char *payload, unsigned int numBytes);
    static int camCtrlDeleted(void *invokingObject, void *userObject, void *arg);
    static int navDataDeleted(void *invokingObject, void *userObject, void *arg);
    static int tourDeleted(void *invokingObject, void *userObject, void *arg);
    static int replaySourceDeleteFn(void *invokingObject,
				    void *userObject,
				    void *arg);
    void sleepUntilReady(void);
    void followTrack(void);

    static int trackDBDeletedFn(void *invokingObject,
				void *userObject,
				void *arg);

    // cppcheck-suppress virtualCallInConstructor
    virtual int sendPriorityBeforeControl(void) const { return TRUE; }

}; /* SPxCamera */

#endif /* SPX_CAMERA_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
