/*********************************************************************
*
* (c) Copyright 2012 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxRadarSimulator object which is a radar source
*	that simulates radars and targets.
*
*
* Revision Control:
*   10/08/22 v1.80   AGC	Use speed/course including motion profile offset.
*
* Previous Changes:
*   28/06/22 1.79   AGC	Support date/time nav data output.
*   21/06/22 1.78   AGC	Support multiple nav datas per platform.
*   23/12/21 1.77   AGC	Add GetRadar().
*   28/10/21 1.76   AGC	Support extended heartbeats.
*   26/07/21 1.75   AGC	Support always jumping targets to motion profile.
*   26/07/21 1.74   AGC	Fix N/C #880 - initial view may be wrong.
*   27/05/21 1.73   AGC	Add SAC/SIC control for multilat/weather.
*   11/03/21 1.72   AGC	Add weather object remote control.
*   02/03/21 1.71   AGC	Support weather.
*   18/05/20 1.70   AGC	Add linear acceleration to SPxSimState.
*   17/01/20 1.69   REW	SendTrack() supports sending CAT247 version msg.
*   14/10/19 1.68   AGC	Suppress cppcheck warning.
*   28/05/19 1.67   AGC	Add GetTargetTemplate() using index.
*   13/05/19 1.66   AGC	Support per-platform AIS/ADS-B output.
*   29/04/19 1.65   AGC	Use correct case for ADS-B.
*   29/04/19 1.64   AJH	Add support for limiting ADS-B range.
*   09/04/19 1.63   AGC	Support max simulation time.
*   02/04/19 1.62   AGC	Add option to include AIS tag blocks.
*   28/09/18 1.61   AGC	Support separate rate for pitch/roll messages.
*   07/06/18 1.60   AGC	Improve start-up performance.
*   07/02/18 1.59   AGC	Support platform rate of turn messages.
*   18/01/18 1.58   AGC	Extend AIS support.
*   11/01/18 1.57   AGC	Use 16-bit azi for IsAuxInBlindSector().
*   23/10/17 1.56   AGC	Call radar event handlers on platform event.
*   16/10/17 1.55   AGC	Allow using fake time for testing.
*   27/09/17 1.54   AGC	Improve target name change robustness.
*   26/05/17 1.53   AGC	Separate course/heading output rate.
*   15/03/17 1.52   AGC	Support bearing based motion starting position.
*   08/03/17 1.51   AGC	Mutex improvements.
*   02/02/17 1.50   AGC	Add SPX_RADAR_SIM_EVENT_PLATFORM_MOTION.
*   31/01/17 1.49   AGC	Support multiple platforms.
*   11/01/17 1.48   AGC	Support platform noise.
*   15/12/16 1.47   AGC	Add option to clear aux video in blind sectors. 
*   25/08/16 1.46   AGC	Fix simulation loading with improved ordering.
*   23/08/16 1.45   AGC	Improve ordering of named items.
*			Mutex improvements.
*   08/07/16 1.44   AGC	Fix possible deadlock on shutdown.
*   15/06/16 1.43   AGC	Add option to disable nav data extrapolation.
*   11/01/16 1.42   AGC	Support MTI video.
*   14/12/15 1.41   AGC	Rearrange auxiliary source input.
*   16/11/15 1.40   AGC	Rename AIS functions for consistency.
*   09/11/15 1.39   AGC	Reorganise AIS functions for testing.
*   03/11/15 1.38   AGC	Add maximum AIS range.
*   22/10/15 1.37   AGC	Use interface for joystick control.
*   01/10/15 1.36   AGC	Support joystick control of platform.
*   21/09/15 1.35   AGC	Improve platform update logic.
*   10/09/15 1.34   AGC	Avoid fixed test targets requesting sim file save.
*   04/09/15 1.33   AGC	Output video simulator track deletion reports.
*   27/07/15 1.32   AGC	Add support for video simulator messages.
*   11/05/15 1.31   AGC	Add target ID event.
*   17/03/15 1.30   AGC	Support status byte reporting.
*   07/01/15 1.29   AGC	Support AIS serial output.
*   07/11/14 1.28   AGC	Support associated object for event handler.
*			New target/motion create/delete events.
*   14/10/14 1.27   AGC	Output multilateration deletion report when target deleted.
*   24/09/14 1.26   AGC	Improve accuracy of dynamic load control.
*   15/09/14 1.25   AGC	Use member functions with SPxTimer.
*   01/09/14 1.24   AGC	Improve change detection for input targets.
*   20/08/14 1.23   AGC	Support configurable nav data output rate.
*   20/06/14 1.22   AGC	Always update start lat/long for metres based motions.
*   02/06/14 1.21   AGC	Support AIS own-ship messages.
*   16/05/14 1.20   AGC	Add IterateRadars().
*   08/05/14 1.19   AGC	Support for detecting changes to simulation.
*   10/04/14 1.18   AGC	Reorganise some functions for testing.
*   26/03/14 1.17   AGC	Add TargetNameFromMode3A command.
*   22/01/14 1.16   AGC	Support heading offset and many more NMEA messages.
*   21/01/14 1.15   AGC	Move SPx and Asterix track distribution to SPxSimRadar.
*   17/12/13 1.14   AGC	Override Enable() to reset last update times.
*   30/07/13 1.13   AGC	Add building support.
*   14/06/13 1.12   AGC	Add oldSpeedMps to SPxSimState.
*   06/06/13 1.11   AGC	Support multiple simulation files.
*   24/05/13 1.10   AGC	Support Multilateration track output.
*			Support inclusion of fix char in generated nav data.
*   03/05/13 1.9    AGC	Support separate Asterix output.
*			Support ADS-B output.
*   25/04/13 1.8    AGC	Support target templates.
*			Support height above ground in state.
*   28/03/13 1.7    AGC	Add SetStandby().
*   18/03/13 1.6    AGC	Make GetCurrentState() public.
*			Support fixed test targets.
*   12/02/13 1.5    AGC	Add lateral acc to SPxSimState.
*   25/01/13 1.4    AGC	Support jumping to simulation time.
*			Platform position now defined here.
*			Support raw AIS output.
*   21/12/12 1.3    AGC	Add Set/GetTrackReportExtFlags().
*   09/11/12 1.2    AGC	Support rewinding simulation.
*			Basic AIS output support.
*			Configurable update intervals.
*   26/10/12 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_RADAR_SIMULATOR_H
#define _SPX_RADAR_SIMULATOR_H

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxEventHandler. */
#include "SPxLibUtils/SPxEventHandler.h"

/* For SPxLatLong_t. */
#include "SPxLibUtils/SPxViewControl.h"

/* For SPxMesgIOType. */
#include "SPxLibNet/SPxDecoderRepeater.h"

/* For base classes. */
#include "SPxLibData/SPxRadarSource.h"
#include "SPxLibNet/SPxAISDecoder.h"

/*
 * Types
 */

/* Forward declarations. */
class SPxAreaObj;
class SPxAsterixEncoder;
class SPxBuildingDatabase;
class SPxExtHeartbeatSender;
class SPxJoystickSim;
class SPxNavData;
class SPxPacketSender;
struct SPxReturnHeader_tag;
class SPxRIB;
class SPxSerialPort;
class SPxSimAISInput;
class SPxSimAISStation;
class SPxSimMotion;
class SPxSimPlatform;
class SPxSimRadar;
class SPxSimTarget;
class SPxSimTargetTemplate;
class SPxSimWeather;
class SPxTerrainDatabase;
class SPxTrackDatabase;
class SPxWeatherDatabase;

/* Height above option. */
enum SPxRadarSimHeight
{
    SPX_RADAR_SIM_HEIGHT_ABOVE_SEA = 0,
    SPX_RADAR_SIM_HEIGHT_ABOVE_GROUND = 1
};

enum SPxRadarSimPos
{
    SPX_RADAR_SIM_POS_LAT_LONG = 0,
    SPX_RADAR_SIM_POS_METRES = 1,
    SPX_RADAR_SIM_POS_BEARING = 2
};

/* Radar Simulator event enumeration. */
enum SPxRadarSimEvent
{
    SPX_RADAR_SIM_EVENT_MOTION		= 0x00000001,
    SPX_RADAR_SIM_EVENT_SEGMENT		= 0x00000002,
    SPX_RADAR_SIM_EVENT_RADAR		= 0x00000004,
    SPX_RADAR_SIM_EVENT_TARGET		= 0x00000008,
    SPX_RADAR_SIM_EVENT_TGT_TEMPLATE	= 0x00000010,
    SPX_RADAR_SIM_EVENT_PLATFORM	= 0x00000020,
    SPX_RADAR_SIM_EVENT_BUILDING	= 0x00000040,
    SPX_RADAR_SIM_EVENT_TARGET_INPUT	= 0x00000080,
    SPX_RADAR_SIM_EVENT_TARGET_TEST	= 0x00000100,
    SPX_RADAR_SIM_EVENT_TARGET_CREATE	= 0x00000200,
    SPX_RADAR_SIM_EVENT_TARGET_DELETE	= 0x00000400,
    SPX_RADAR_SIM_EVENT_MOTION_CREATE	= 0x00000800,
    SPX_RADAR_SIM_EVENT_MOTION_DELETE	= 0x00001000,
    SPX_RADAR_SIM_EVENT_MOTION_NAME	= 0x00002000,
    SPX_RADAR_SIM_EVENT_TARGET_ID	= 0x00004000,
    SPX_RADAR_SIM_EVENT_MOVABLE_JOYSTICK = 0x00008000,
    SPX_RADAR_SIM_EVENT_TARGET_NAME	= 0x00010000,
    SPX_RADAR_SIM_EVENT_TGT_TEMPLATE_NAME = 0x00020000,
    SPX_RADAR_SIM_EVENT_NAV_DATA	= 0x00040000,
    SPX_RADAR_SIM_EVENT_PLATFORM_SRC    = 0x00080000,
    SPX_RADAR_SIM_EVENT_PLATFORM_NAME	= 0x00100000,
    SPX_RADAR_SIM_EVENT_PLATFORM_MOTION = 0x00200000,
    SPX_RADAR_SIM_EVENT_MOVABLE		= 0x00400000,
    SPX_RADAR_SIM_EVENT_WEATHER		= 0x00800000,
    SPX_RADAR_SIM_EVENT_WEATHER_CREATE	= 0x01000000,
    SPX_RADAR_SIM_EVENT_WEATHER_DELETE	= 0x02000000,
    SPX_RADAR_SIM_EVENT_WEATHER_NAME	= 0x04000000
};

/* Radar Simulator status byte values. */
enum SPxRadarSimStatus
{
    SPX_RADAR_SIM_STATUS_OK		= 0x00,
    SPX_RADAR_SIM_STATUS_INVALID_RANGE	= 0x01
};

/* Structure describing the state of an object (position, speed, course). */
struct SPxSimState
{
    int isPosSet;	    /* Is the position set? */
    SPxRadarSimPos posType; /* Type of position. */
    SPxLatLong_t latLong;   /* Lat/long position. */
    double xMetres;	    /* X position in metres. */
    double yMetres;	    /* Y position in metres. */
    double rangeMetres;	    /* Range in metres. */
    double bearingDegs;	    /* Bearing clockwise from North in degrees. */
    int isSpeedSet;	    /* Is the speed set? */
    double speedMps;	    /* Speed in metres per second. */
    double oldSpeedMps;	    /* Old speed in metres per second. */
    double speedWithOffsetMps;  /* Speed with adjustment due to motion profile offsets. */
    int isCourseSet;	    /* Is the course set? */
    double courseDegs;	    /* Course in degrees, clockwise from North. */
    double courseWithOffsetDegs; /* Course with adjustment due to motion profile offsets. */
    int isHeightSet;	    /* Is the height set? */
    SPxRadarSimHeight heightAbove;  /* Is the height above sea or ground level? */
    double heightAboveMetres;	/* Height in metres above ground/sea level. */
    double heightMetres;    /* Height in metres above sea level. */
    double accMetresPerSec2; /* Linear acceleration in m/s^2. */
    double accDegsPerSec;   /* Degrees per second acceleration (+ve clockwise). */
    double verticalRateMps; /* Vertical rate in metres per second (+ve up). */
};

struct SPxRadarSimEventParams
{
    UINT32 id;
    SPxSimTarget *target;
    SPxSimTargetTemplate *targetTemplate;
    SPxSimMotion *motion;
    SPxSimPlatform *platform;
    SPxSimWeather *weather;
    char name[512];
    int isLoading;
};

/* Callback types. */
typedef SPxErrorCode (*SPxRadarSimPlatformFn_t)(unsigned int index, SPxSimPlatform *platform, void *userArg);
typedef SPxErrorCode (*SPxRadarSimRadarFn_t)(unsigned int index, SPxSimRadar *radar, void *userArg);
typedef SPxErrorCode (*SPxRadarSimTargetFn_t)(unsigned int index, SPxSimTarget *target, void *userArg);
typedef SPxErrorCode (*SPxRadarSimTargetTemplateFn_t)(unsigned int index, SPxSimTargetTemplate *tgtTemplate, void *userArg);
typedef SPxErrorCode (*SPxRadarSimMotionFn_t)(unsigned int index, SPxSimMotion *motion, void *userArg);
typedef SPxErrorCode (*SPxRadarSimWeatherFn_t)(unsigned int index, SPxSimWeather *weather, void *userArg);

/* Define our class. */
class SPxRadarSimulator : public SPxRadarSource
{
public:
    /* Public types. */
    typedef SPxErrorCode (*EventHandler)(UINT32 flags, const SPxRadarSimEventParams *params);

    /* Public functions. */
    explicit SPxRadarSimulator(SPxTerrainDatabase *tdb=NULL,
			       SPxBuildingDatabase *bdb=NULL,
			       SPxWeatherDatabase *wdb = NULL,
			       SPxJoystickSim *joystickSim=NULL,
			       SPxExtHeartbeatSender *extHbSender=NULL);
    virtual ~SPxRadarSimulator(void);
    virtual void Enable(int state);

    SPxErrorCode SetStandby(int standby);
    int IsStandby(void) const;
    SPxErrorCode SetSpeed(double speed);
    SPxErrorCode GetSpeed(double *speed) const;
    SPxErrorCode Rewind(void);
    SPxErrorCode SetCurrentTimeSecs(double currentTimeSecs);
    SPxErrorCode GetCurrentTimeSecs(double *currentTimeSecs) const;
    SPxErrorCode SetMaxTimeSecs(double maxTimeSecs);
    SPxErrorCode GetMaxTimeSecs(double *maxTimeSecs) const;
    SPxErrorCode GetInitialPlatformLatLong(SPxLatLong_tag *latLong) const;

    /* Update rates. */
    SPxErrorCode SetRadarUpdateIntervalMS(unsigned int ms);
    SPxErrorCode GetRadarUpdateIntervalMS(unsigned int *ms) const;
    SPxErrorCode SetTargetUpdateIntervalMS(unsigned int ms);
    SPxErrorCode GetTargetUpdateIntervalMS(unsigned int *ms) const;
    SPxErrorCode SetMultilatIntervalMS(unsigned int ms);
    SPxErrorCode GetMultilatIntervalMS(unsigned int *ms) const;
    SPxErrorCode SetAdsbIntervalMS(unsigned int ms);
    SPxErrorCode GetAdsbIntervalMS(unsigned int *ms) const;
    SPxErrorCode SetVidSimIntervalMS(unsigned int ms);
    SPxErrorCode GetVidSimIntervalMS(unsigned int *ms) const;
    SPxErrorCode SetWeatherIntervalMS(unsigned int ms);
    SPxErrorCode GetWeatherIntervalMS(unsigned int *ms) const;

    /* Configure target info output. */
    SPxErrorCode EnableMultilatOutput(int enable);
    int IsMultilatOutputEnabled(void) const;
    int IsAISOutputEnabled(void) const;
    SPxErrorCode EnableVidSimOutput(int enable);
    int IsVidSimOutputEnabled(void) const;
    SPxErrorCode EnableWeatherOutput(int enable);
    int IsWeatherOutputEnabled(void) const;
    SPxErrorCode SetMultilatSAC(UINT8 sac);
    UINT8 GetMultilatSAC(void) const;
    SPxErrorCode SetMultilatSIC(UINT8 sic);
    UINT8 GetMultilatSIC(void) const;
    SPxErrorCode SetWeatherOutputType(UINT8 weatherOutputType);
    SPxErrorCode GetWeatherOutputType(UINT8 *weatherOutputType) const;
    SPxErrorCode SetWeatherPolarSpacingDegs(double weatherPolarSpacingDegs);
    SPxErrorCode GetWeatherPolarSpacingDegs(double *weatherPolarSpacingDegs) const;
    SPxErrorCode SetWeatherCartesianSpacingMetres(double weatherCartesianSpacingMetres);
    SPxErrorCode GetWeatherCartesianSpacingMetres(double *weatherCartesianSpacingMetres) const;
    SPxErrorCode SetWeatherSAC(UINT8 sac);
    UINT8 GetWeatherSAC(void) const;
    SPxErrorCode SetWeatherSIC(UINT8 sic);
    UINT8 GetWeatherSIC(void) const;

    /* Configure fixed test targets. */
    SPxErrorCode EnableTestTargets(int enable);
    int AreTestTargetsEnabled(void) const;
    SPxErrorCode SetTestTargetsAziDegs(double testTargetsAziDegs);
    SPxErrorCode GetTestTargetsAziDegs(double *testTargetsAziDegs) const;

    /* Jump to motion. */
    SPxErrorCode SetAlwaysJumpToMotion(int enable);
    int IsAlwaysJumpToMotion(void) const;

    /* Iterate over the radar simulator radars/targets/motions/templates. */
    unsigned int GetNumPlatforms(void) const;
    SPxSimPlatform *GetPlatform(unsigned int index);
    SPxErrorCode IteratePlatforms(SPxRadarSimPlatformFn_t fn, void *userArg);
    SPxSimRadar *GetRadar(unsigned int index);
    SPxErrorCode IterateRadars(SPxRadarSimRadarFn_t fn, void *userArg);
    unsigned int GetNumRadars(void) const;
    SPxSimTarget *GetTarget(unsigned int index, int isMutexAlreadyLocked=FALSE);
    SPxSimTarget *GetTarget(const char *name, UINT32 flags=0, int isMutexAlreadyLocked=FALSE);
    SPxErrorCode GetTargetNameFromMode3A(UINT16 mode3A, char *nameBuf, unsigned int bufLen) const;
    SPxErrorCode GetSelectedTargetName(char *nameBuf, unsigned int bufLen) const;
    SPxErrorCode IterateTargets(SPxRadarSimTargetFn_t fn, void *userArg);
    unsigned int GetNumTargets(void) const;
    SPxSimTargetTemplate *GetTargetTemplate(unsigned int index);
    SPxSimTargetTemplate *GetTargetTemplate(const char *name, int mutexAlreadyLocked=FALSE);
    SPxErrorCode IterateTargetTemplates(SPxRadarSimTargetTemplateFn_t fn, void *userArg);
    unsigned int GetNumTargetTemplates(void) const;
    SPxSimMotion *GetMotion(unsigned int index);
    SPxSimMotion *GetMotion(const char *name,
			    int mutexAlreadyLocked=FALSE);
    SPxErrorCode IterateMotions(SPxRadarSimMotionFn_t fn, void *userArg);
    unsigned int GetNumMotions(void) const;
    unsigned int GetNumWeather(void) const;
    SPxSimWeather *GetWeather(unsigned int index);
    SPxSimWeather *GetWeather(const char *name, int mutexAlreadyLocked=FALSE);
    SPxErrorCode IterateWeather(SPxRadarSimWeatherFn_t fn, void *userArg);
    SPxErrorCode GetWeatherAreaObj(SPxAreaObj **areaObj);

    /* Load/save a simulation. */
    SPxErrorCode Load(const char *filename, int reset=TRUE);
    SPxErrorCode Save(const char *filename=NULL);
    const char *GetShortPath(void) const;
    const char *GetFullPath(void) const;
    int HasChanges(void) const;

    /* Get internal objects. */
    SPxErrorCode GetJoystick(SPxJoystickSim **joystick, int mutexAlreadyLocked=FALSE);
    SPxErrorCode GetTrackDatabase(SPxTrackDatabase **trackDatabase);
    SPxErrorCode GetAISInput(SPxSimAISInput **aisInput);
    SPxErrorCode GetMultilatSender(SPxPacketSender **multilatSender);
    SPxErrorCode GetWeatherSender(SPxPacketSender **weatherSender);
    SPxErrorCode GetTerrainDatabase(SPxTerrainDatabase **tdb) const;
    SPxErrorCode GetBuildingDatabase(SPxBuildingDatabase **bdb) const;
    SPxErrorCode GetExtHeartbeatSender(SPxExtHeartbeatSender **extHbSender) const;

    /* Add/remove event callbacks. */
    template<typename F, typename O>
    SPxErrorCode AddEventCallback(F fn, O *userArg)
    {
	return m_eventFns.Add(fn, userArg);
    }
    template<typename F, typename O>
    SPxErrorCode RemoveEventCallback(F fn, O *userArg)
    {
	return m_eventFns.Remove(fn, userArg);
    }

    /* Override base class status. */
    virtual UINT8 GetStatusByte(void);

    virtual void UpdateRadar(UINT64 diffMs);

    /* Standard parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    explicit SPxRadarSimulator(SPxTerrainDatabase *tdb,
			       SPxBuildingDatabase *bdb,
			       SPxWeatherDatabase *wdb,
			       SPxJoystickSim *joystickSim,
			       SPxAISDecoder *aisInput);
    void InstallTimers(void);

    // cppcheck-suppress virtualCallInConstructor
    virtual void Shutdown(void);

    virtual void TargetFn(void);
    virtual void RadarFn(void);
    virtual void NavDataOutputFn(void);
    virtual void HeadingOutputFn(void);
    virtual void RateOfTurnOutputFn(void);
    virtual void PitchRollOutputFn(void);
    virtual void DateTimeOutputFn(void);
    virtual void EventFn(void);
    virtual void AISAsyncFn(void);

    virtual void UpdateTargets(UINT64 diffMs);
    virtual void UpdateAIS(void);
    virtual void OutputNavData(void);
    virtual void UpdateEvents(void);

    virtual UINT32 GetTickerTime(void) const;
    virtual void GetEpochTime(SPxTime_tag *now) const;

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    struct Event;
    SPxEventHandler<EventHandler> m_eventFns;

    /* Called by SPxSimRadarServer. */
    SPxErrorCode UpdateRadarAux(SPxSimRadar *radar, SPxReturnHeader_tag *header);
    double GetRadialTargetSpeedMps(const SPxSimRadar *radar,
	double tgtBearingRads, double tgtSpeedMps, double tgtCourseDegs) const;
    int IsAuxInBlindSector(SPxSimRadar *radar, UINT16 azimuth) const;

    /* Called by SPxSimPlatform. */
    SPxErrorCode AddPlatform(SPxSimPlatform *platform);
    SPxErrorCode RemovePlatform(SPxSimPlatform *platform);

    /* Called by SPxSimRadar. */
    SPxErrorCode AddRadar(SPxSimRadar *radar);
    SPxErrorCode RemoveRadar(SPxSimRadar *radar);
    SPxErrorCode SendTrack(SPxPacketSender *pktSender,
			   UINT32 packetType,
			   SPxSerialPort *serialPort,
			   const unsigned char *payload,
			   unsigned int payloadSizeBytes,
			   SPxMesgIOType_t outputType=SPX_MESG_IO_TYPE_NETWORK,
			   SPxAsterixEncoder *asterixVersEncoder=NULL);

    /* Called by SPxSimMotion. */
    SPxErrorCode AddMotion(SPxSimMotion *motion, const char *name);
    SPxErrorCode RemoveMotion(SPxSimMotion *motion);
    unsigned int GetNumMotionsCreated(void) const;

    /* Called by SPxSimTarget. */
    SPxErrorCode AddTarget(SPxSimTarget *target, const char *name, int checkName=TRUE);
    SPxErrorCode RemoveTarget(SPxSimTarget *target);
    unsigned int GetNumTargetsCreated(void) const;
    SPxErrorCode SetTargetName(SPxSimTarget *target, const char *newName);

    /* Called by SPxSimTargetTemplate. */
    SPxErrorCode AddTargetTemplate(SPxSimTargetTemplate *tgtTemplate, const char *name);
    SPxErrorCode RemoveTargetTemplate(SPxSimTargetTemplate *tgtTemplate);
    unsigned int GetNumTargetTemplatesCreated(void) const;

    /* Called by SPxSimWeather. */
    SPxErrorCode AddWeather(SPxSimWeather *weather, const char *name, int checkName=TRUE);
    SPxErrorCode RemoveWeather(SPxSimWeather *weather);
    unsigned int GetNumWeatherCreated(void) const;
    SPxErrorCode SetWeatherName(SPxSimWeather *weather, const char *newName);

    /* Call by SPxSimAISStation and others.
     * These are the only supported methods that may be called by
     * SPxSimPlatform, SPxSimRadar, SPxSimMotion, SPxSimTarget and SPxSimTargetTemplate,
     * outside of their constructors and destructors, with their mutex locked,
     * except for SendTrack().
     * This is required to avoid deadlocks.
     */
    SPxErrorCode OutputAISPos(SPxSimAISStation *aisStation);
    SPxErrorCode OutputAISStatic(SPxSimAISStation *aisStation);
    SPxErrorCode OutputAISAssignment(const char *sentences, unsigned int sizeBytes);
    SPxErrorCode SignalEvent(UINT32 flags, const SPxRadarSimEventParams *params = NULL);

    /* Other private functions. */
    void init(int installTimers, SPxAISDecoder *aisDecoder);
    SPxErrorCode loadSim(const char *filename);
    const char *getFullPathInternal(void) const;
    SPxErrorCode setCurrentTimeSecs(double currentTimeSecs);
    SPxErrorCode updateMetresMotions(void);
    void updateTargets(UINT64 diffMs);
    void updateRadar(UINT64 diffMs);
    void updateWeather(void);
    void updateEvents(void);
    void updateAIS(void);
    void updateAISStatic(void);
    void outputNavData(double speed);
    void outputHeading(void);
    void outputRateOfTurn(void);
    void outputPitchRoll(void);
    void outputDateTime(void);
    static SPxErrorCode buildingFnWrapper(double minLatDegs, double maxLatDegs,
					  double minLongDegs, double maxLongDegs,
					  int crossZeroLong, void *userArg);
    SPxErrorCode buildingFn(void);
    static void multilatFnWrapper(void *userArg);
    static void adsbFnWrapper(void *userArg);
    static void vidSimFnWrapper(void *userArg);
    static void weatherFnWrapper(void *userArg);
    static SPxErrorCode outputMultilatFnWrapper(unsigned int index, SPxSimTarget *target, void *userArg);
    static SPxErrorCode outputAISPosFnWrapper(unsigned int index, SPxSimTarget *target, void *userArg);
    static SPxErrorCode outputAISStaticFnWrapper(unsigned int index, SPxSimTarget *target, void *userArg);
    static SPxErrorCode outputAdsbFnWrapper(unsigned int index, SPxSimTarget *target, void *userArg);
    static SPxErrorCode outputVidSimFnWrapper(unsigned int index, SPxSimTarget *target, void *userArg);
    SPxErrorCode outputVidSimDeletion(UINT32 id);
    SPxErrorCode outputAISPos(SPxSimAISStation *aisStation);
    SPxErrorCode outputAISStatic(SPxSimAISStation *aisStation);
    SPxErrorCode outputMultilatStatus(void);
    SPxErrorCode outputMultilat(SPxSimTarget *target, int deleted = FALSE);
    SPxErrorCode outputAdsb(SPxSimTarget *target);
    SPxErrorCode outputVidSim(SPxSimTarget *target);
    SPxErrorCode outputWeather(void);
    SPxErrorCode setTargetName(SPxSimTarget *target, const char *newName, int checkName);
    SPxErrorCode setWeatherName(SPxSimWeather *weather, const char *newName, int checkName);
    SPxErrorCode iterateTargets(SPxRadarSimTargetFn_t fn, void *userArg);
    SPxErrorCode iterateWeather(SPxRadarSimWeatherFn_t fn, void *userArg);
    SPxErrorCode signalEvent(UINT32 flags, const SPxRadarSimEventParams *params = NULL);
    SPxErrorCode removeMotion(SPxSimMotion *motion);
    SPxErrorCode removeTarget(SPxSimTarget *target);
    SPxErrorCode removeWeather(SPxSimWeather *weather);
    SPxErrorCode removeTargetTemplate(SPxSimTargetTemplate *tgtTemplate);

    friend class SPxRadarSimServer;
    friend class SPxSimPlatform;
    friend class SPxSimPlatNav;
    friend class SPxSimRadar;
    friend class SPxSimMotion;
    friend class SPxSimTarget;
    friend class SPxSimTargetTemplate;
    friend class SPxSimWeather;
    friend class SPxSimMovable;
    friend class SPxSimAISStation;

}; /* SPxRadarSimulator */

#endif /* _SPX_RADAR_SIMULATOR_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
