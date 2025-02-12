/*********************************************************************
*
* (c) Copyright 2017 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxSimPlatform object which is a simulated platform
*	for use with the SPxRadarSimulator radar source.
*
*
* Revision Control:
*   28/06/22 v1.27   AGC	Support date/time nav data output.
*
* Previous Changes:
*   21/06/22 1.26   AGC	Support multiple nav datas.
*   10/02/21 1.25   AGC	Support VBW stern speeds.
*   25/11/20 1.24   AGC	Revert unnecessary ADS-B version message change.
*   19/11/20 1.23   AGC	Support enable/disable ADS-B version messages.
*   22/10/20 1.22   AGC	Support delaying nav data output.
*   19/08/20 1.21   AGC	Add GetCurrTime() function.
*   17/01/20 1.20   REW	Support control of CAT21 versions.
*   20/11/19 1.19   AGC	Support AIS VDO for platform pos.
*   17/10/19 1.18   AGC	Remove unused GetCurrentCourseNoiseDegs().
*   27/09/19 1.17   AGC	Fix naming of set/drift.
*   13/05/19 1.16   AGC	Support per-platform AIS/ADS-B output.
*   29/04/19 1.15   AGC	Use correct case for ADS-B.
*   29/04/19 1.14   AJH	Add support for limiting ADS-B range.
*   28/09/18 1.13   AGC	Support separate rate for pitch/roll messages.
*			Support platform pitch simulation.
*   26/09/18 1.12   AGC	Improve platform roll simulation.
*   30/04/18 1.11   AGC	Use new separate nav data object for network/ROC output.
*   07/02/18 1.10   AGC	Support rate of turn messages.
*   18/01/18 1.9    AGC	Extend AIS support.
*   06/12/17 1.8    AGC	Keep course correct when paused.
*   26/05/17 1.7    AGC	Add position noise.
*			Improve nav data output.
*   16/05/17 1.6    AGC	Use separate nav data objects for I/O and display.
*			Support set and drift.
*   12/04/17 1.5    AGC	Support set/get of noise seed.
*   15/03/17 1.4    AGC	Improve platform positioning on jump-to-time.
*   08/03/17 1.3    AGC	Add platform roll noise.
*   02/02/17 1.2    AGC	Improve nav data replay.
*   31/01/17 1.1    AGC	Initial version.
**********************************************************************/
#ifndef SPX_SIM_PLATFORM_H_
#define SPX_SIM_PLATFORM_H_

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxEventHandler. */
#include "SPxLibUtils/SPxEventHandler.h"

/* For base classes. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibData/SPxJoystickSim.h"

/* For SPxRadarSimHeightAbove. */
#include "SPxLibData/SPxRadarSimulator.h"

/*
 * Types
 */

enum SPxRadarSimPlatformSource
{
    SPX_RADAR_SIM_PLAT_SRC_STATIC = 0,
    SPX_RADAR_SIM_PLAT_SRC_MOTION = 1,
    SPX_RADAR_SIM_PLAT_SRC_NAV_DATA = 2,
    SPX_RADAR_SIM_PLAT_SRC_JOYSTICK = 3
};

enum SPxRadarSimNavDataPlatformSource
{
    SPX_RADAR_SIM_NAV_DATA_PLAT_SRC_NET = 0,
    SPX_RADAR_SIM_NAV_DATA_PLAT_SRC_AIS = 1,
    SPX_RADAR_SIM_NAV_DATA_PLAT_SRC_FILE = 2
};

/* Forward declarations. */
class SPxRadarSimulator;
class SPxSimPlatNav;
class SPxNavData;
class SPxSimMotion;
struct SPxLatLong_tag;
struct SPxSimState;

/* Define our class. */
class SPxSimPlatform : public SPxObj, public SPxAISIface, public SPxJoystickSimIface
{
public:
    /* Public types. */
    typedef SPxErrorCode (*NameChangeHandler)(void);

    /* Public functions. */
    explicit SPxSimPlatform(SPxRadarSimulator *radarSim,
			    SPxNavData *navDataDisplay,
			    SPxNavData *navDataWithHeading=NULL,
			    unsigned int index=0);
    virtual ~SPxSimPlatform(void);

    SPxErrorCode GetRadarSim(SPxRadarSimulator **radarSim);
    SPxErrorCode GetIndex(unsigned int *index) const;
    SPxErrorCode GetCurrTime(double *currTime) const;

    SPxErrorCode SetPlatformName(const char *platformName);
    SPxErrorCode GetPlatformName(char *buffer, unsigned int bufLen) const;

    /* Configuration. */
    SPxErrorCode SetPlatformSource(SPxRadarSimPlatformSource platformSource);
    SPxErrorCode GetPlatformSource(SPxRadarSimPlatformSource *platformSource) const;
    int IsSourceJoystick(void) const;
    SPxErrorCode SetNavDataPlatformSource(SPxRadarSimNavDataPlatformSource navDataPlatformSource);
    SPxErrorCode GetNavDataPlatformSource(SPxRadarSimNavDataPlatformSource *navDataPlatformSource) const;
    SPxErrorCode SetReplayNavData(int replayNavData);
    SPxErrorCode GetReplayNavData(int *replayNavData) const;
    SPxErrorCode SetExtrapolateNavData(int extrapolateNavData);
    int IsExtrapolateNavData(void) const;
    SPxErrorCode SetHeadingOffsetDegs(double headingOffsetDegs);
    SPxErrorCode GetHeadingOffsetDegs(double *headingOffsetDegs) const;
    SPxErrorCode SetPlatformLatLong(const SPxLatLong_tag *latLong);
    SPxErrorCode GetPlatformLatLong(SPxLatLong_tag *latLong) const;
    SPxErrorCode GetInitialPlatformLatLong(SPxLatLong_tag *latLong) const;
    SPxErrorCode SetMotionProfile(SPxSimMotion *motionProfile);
    SPxErrorCode GetMotionProfile(const SPxSimMotion **motionProfile) const;
    SPxErrorCode GetMotionProfile(SPxSimMotion **motionProfile);
    SPxErrorCode SetHeightMetres(double heightMetres);
    SPxErrorCode GetHeightMetres(double *heightMetres) const;
    SPxErrorCode SetHeightAbove(SPxRadarSimHeight heightAbove);
    SPxErrorCode GetHeightAbove(SPxRadarSimHeight *heightAbove) const;
    SPxErrorCode GetHeightAboveSeaMetres(double *heightAboveSeaMetres) const;
    SPxErrorCode SetMagneticVariation(double magVarDegs);
    SPxErrorCode GetMagneticVariation(double *magVarDegs) const;
    SPxErrorCode SetSetDegs(double setDegs);
    SPxErrorCode GetSetDegs(double *setDegs) const;
    SPxErrorCode SetDriftMps(double driftMps);
    SPxErrorCode GetDriftMps(double *driftMps) const;
    SPxErrorCode SetLengthMetres(double lengthMetres);
    SPxErrorCode GetLengthMetres(double *lengthMetres) const;
    SPxErrorCode SetBreadthMetres(double breadthMetres);
    SPxErrorCode GetBreadthMetres(double *breadthMetres) const;

    /* Set/get current state. */
    virtual SPxErrorCode SetCurrentState(const SPxSimState *state);
    virtual SPxErrorCode GetCurrentState(SPxSimState *state) const;
    SPxErrorCode GetCurrentState(SPxSimState *state, int mutexLocked) const;

    /* Configure AIS output. */
    SPxErrorCode SetAISOutputType(SPxMesgIOType_t aisOutputType);
    SPxErrorCode GetAISOutputType(SPxMesgIOType_t *aisOutputType) const;
    SPxErrorCode SetAISOutputUseLWE(int aisOutputUseLWE);
    int IsAISOutputUsingLWE(void) const;

    /* Configure ADS-B output. */
    SPxErrorCode SetAdsbOutputType(SPxMesgIOType_t adsbOutputType);
    SPxErrorCode GetAdsbOutputType(SPxMesgIOType_t *adsbOutputType) const;
    SPxErrorCode SetAdsbOutputVersion(const char *version);
    const char *GetAdsbOutputVersion(void) const;

    /* Configure AIS information. */
    SPxErrorCode SetAISTagBlockMask(UINT32 mask);
    SPxErrorCode GetAISTagBlockMask(UINT32 *mask) const;
    SPxErrorCode SetAISSFI(const char *aisSFI);
    SPxErrorCode GetAISSFI(char *aisSFI, unsigned int bufLen) const;
    SPxErrorCode EnableAISOwnShip(int enable);
    int IsAISOwnShipEnabled(void) const;
    virtual int IsAISEditable(void) const;
    SPxErrorCode SetAISName(const char *aisName);
    SPxErrorCode GetAISName(char *aisName, unsigned int bufLen,
			    int mutexAlreadyLocked = FALSE) const;
    virtual SPxErrorCode SetAISInfo(const SPxAISInfo *aisInfo);
    virtual SPxErrorCode GetAISInfo(SPxAISInfo *aisInfo) const;
    SPxErrorCode GetAISInfo(SPxAISInfo *aisInfo, int mutexAlreadyLocked) const;

    /* Configure max ranges for detecting AIS/ADS-B. */
    SPxErrorCode SetAISMaxTargetRangeMetres(double aisMaxTargetRangeMetres);
    SPxErrorCode GetAISMaxTargetRangeMetres(double *aisMaxTargetRangeMetres) const;
    SPxErrorCode SetAdsbMaxTargetRangeMetres(double adsbMaxTargetRangeMetres);
    SPxErrorCode GetAdsbMaxTargetRangeMetres(double *adsbMaxTargetRangeMetres) const;

    /* Platform noise. */
    SPxErrorCode SetPositionNoiseStdDev(double posNoiseMetres);
    SPxErrorCode GetPositionNoiseStdDev(double *posNoiseMetres) const;
    SPxErrorCode SetCourseNoiseStdDev(double courseNoiseDegs);
    SPxErrorCode GetCourseNoiseStdDev(double *courseNoiseDegs) const;
    SPxErrorCode SetHeadingNoiseStdDev(double headingNoiseDegs);
    SPxErrorCode GetHeadingNoiseStdDev(double *headingNoiseDegs) const;
    SPxErrorCode SetSpeedNoiseStdDev(double speedNoiseMps);
    SPxErrorCode GetSpeedNoiseStdDev(double *speedNoiseMps) const;
    SPxErrorCode GetCurrentHeadingNoiseDegs(double *headingNoiseDegs) const;
    SPxErrorCode SetMaxPitchDegs(double maxPitchDegs);
    SPxErrorCode GetMaxPitchDegs(double *maxPitchDegs) const;
    SPxErrorCode SetMaxRollDegs(double maxRollDegs);
    SPxErrorCode GetMaxRollDegs(double *maxRollDegs) const;
    SPxErrorCode SetPitchPeriodSecs(double pitchPeriodSecs);
    SPxErrorCode GetPitchPeriodSecs(double *pitchPeriodSecs) const;
    SPxErrorCode SetRollPeriodSecs(double rollPeriodSecs);
    SPxErrorCode GetRollPeriodSecs(double *rollPeriodSecs) const;
    SPxErrorCode GetCurrentPitchRollDegs(double radCurSecs,
					 double *pitchDegs,
					 double *rollDegs) const;
    SPxErrorCode SetNoiseSeed(const UINT32 *seeds, unsigned int numSeeds);
    SPxErrorCode GetNoiseSeed(UINT32 *seeds, unsigned int numSeeds) const;

    /* Get internal objects. */
    unsigned int GetNumPlatNav(void) const;
    SPxErrorCode GetPlatNav(SPxSimPlatNav **platNav,
			    unsigned int index=0) const;
    SPxErrorCode GetNavDataDisplay(SPxNavData **navData) const;
    SPxErrorCode GetNavDataWithHeading(SPxNavData **navData) const;
    SPxErrorCode GetAISSenderNet(SPxPacketSender **aisSenderNet);
    SPxErrorCode GetAISSenderSerial(SPxSerialPort **aisSenderSerial);
    SPxErrorCode GetAdsbSender(SPxPacketSender **adsbSender);

    /* Retrieve AIS station object. */
    SPxSimAISStation *GetAISStation(void);
    const SPxSimAISStation *GetAISStation(void) const;

    /* Add/remove name change callbacks. */
    template<typename F, typename O>
    SPxErrorCode AddNameChangeCallback(F fn, O *userArg)
    {
	return m_nameChangeFns.Add(fn, userArg);
    }
    template<typename F, typename O>
    SPxErrorCode RemoveNameChangeCallback(F fn, O *userArg)
    {
	return m_nameChangeFns.Remove(fn, userArg);
    }
    
    /* Set/get parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    explicit SPxSimPlatform(SPxRadarSimulator *radarSim,
			    SPxNavData *navDataDisplay,
			    SPxNavData *navDataWithHeading,
			    unsigned int index,
			    SPxPacketSender *aisSenderNet);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxEventHandler<NameChangeHandler> m_nameChangeFns;
    
    /* Called by SPxRadarSimulator. */
    SPxErrorCode RemoveRadarSim(void);
    void SetNavDataUpdateRequired(void);
    void SetCurrentTimeSecs(double currentTimeSecs);
    SPxErrorCode Update(UINT64 msecs, double speed, double radCurrSecs);
    SPxErrorCode OutputNavData(double speed, double radCurrSecs);
    SPxErrorCode OutputHeading(void);
    SPxErrorCode OutputRateOfTurn(void);
    SPxErrorCode OutputPitchRoll(double radCurrSecs);
    SPxErrorCode OutputDateTime(void);
    SPxErrorCode OutputAISPos(SPxSimAISStation *aisStation);
    SPxErrorCode OutputAISStatic(SPxSimAISStation *aisStation);
    SPxErrorCode OutputAISAssignment(const char *sentences, unsigned int sizeBytes);
    SPxErrorCode OutputAdsb(SPxSimTarget *target);
    SPxErrorCode RemoveMotion(const SPxSimMotion *motion);
    void MotionChanged(const SPxSimMotion *motionProfile);
    const char *GetDesc(void) const;
    void SetFile(const char *filename);
    int WasFromFile(const char *filename);
    SPxErrorCode ApplyDelayed(void);
    SPxErrorCode SetDelayedMotion(const char *delayedMotion);

    /* Call by SPxSimPlatNav. */
    SPxErrorCode AddPlatNav(SPxSimPlatNav *platNav);
    SPxErrorCode RemovePlatNav(SPxSimPlatNav *platNav);

    /* Private functions. */
    void init(void);
    int navDataFn(int mutexLocked);
    SPxErrorCode setPlatformSource(SPxRadarSimPlatformSource platformSource,
				   SPxRadarSimNavDataPlatformSource navDataPlatformSource,
				   int replayNavData);
    SPxErrorCode setMotionProfile(SPxSimMotion *motionProfile);
    SPxErrorCode updatePlatformPos(UINT64 diffMs,
				   double speed,
				   double radCurrSecs,
				   int updateRequired);
    SPxErrorCode getNewPlatformPos(double intervalSecs,
				   SPxSimState *state,
				   int updateRequired,
				   int realUpdate);
    SPxErrorCode outputNavData(double speed, double radCurrSecs);
    SPxErrorCode outputHeading(void);
    SPxErrorCode outputRateOfTurn(void);
    SPxErrorCode outputPitchRoll(double radCurrSecs);
    SPxErrorCode outputDateTime(void);
    UINT32 getReqNavData(UINT32 now) const;
    UINT32 getReqHeading(UINT32 now) const;
    UINT32 getReqRateOfTurn(UINT32 now) const;
    UINT32 getReqPitchRoll(UINT32 now) const;
    UINT32 getReqDateTime(UINT32 now) const;
    SPxErrorCode updateInternalNavData(int withHeading);
    SPxErrorCode getCurrentPitchRollDegs(double radCurrSecs,
					 double *pitchDegs,
					 double *rollDegs) const;
    int isLatLongWithinAISRange(const SPxLatLong_tag *latLong) const;
    int isLatLongWithinAdsbRange(const SPxLatLong_tag *latLong) const;
    SPxErrorCode sendAISTrack(const char *sentences, unsigned int sizeBytes);

    friend class SPxRadarSimulator;
    friend class SPxSimPlatNav;

}; /* SPxSimPlatform. */

#endif /* SPX_SIM_PLATFORM_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/
