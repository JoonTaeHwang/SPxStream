/*********************************************************************
*
* (c) Copyright 2012 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxSimTarget object which represents a simulated
*	target within the SPxRadarSimulator radar source.
*
*
* Revision Control:
*   06/07/22 v1.57   AGC	Support intensity above background noise.
*
* Previous Changes:
*   10/01/22 1.56   AGC	Support ADS-B version and emergency type.
*   26/07/21 1.55   AGC	Support always jumping targets to motion profile.
*   02/03/21 1.54   AGC	Move dynamics to new base class.
*   29/10/20 1.53   AGC	Support P466 info.
*   26/08/20 1.52   AGC	Support IJMS and MIL2525D properties.
*   14/10/19 1.51   AGC	Suppress cppcheck warning.
*   26/09/19 1.50   AGC	Support various Mode 5 options.
*   13/05/19 1.49   AGC	Make GetUniqueID() public.
*   01/05/19 1.48   AGC	Improve On-Ground track flag setting.
*   25/06/18 1.47   AGC	Correctly suppress cppcheck warning.
*   02/02/18 1.46   AGC	Support max static draught configuration.
*   19/01/18 1.45   AGC	Support IFF military emergency.
*   18/01/18 1.44   AGC	Extend AIS support.
*   14/12/17 1.43   AGC	Support various ASTERIX specific options.
*   11/12/17 1.42   AGC	Support IFF confidences.
*   27/09/17 1.41   AGC	Improve name change robustness.
*   07/06/17 1.40   AGC	Support target POD.
*   15/12/16 1.39   AGC	Add support for SART targets.
*   29/11/16 1.38   AGC	Support target description.
*   31/10/16 1.37   AGC	Add TTM input flag.
*   23/08/16 1.36   AGC	Event function prototype updated.
*			Improve ordering of targets.
*   08/04/16 1.35   AGC	Improve efficieny of targets updates.
*			Support IFF values of zero.
*   16/03/16 1.34   AGC	Support extended IFF mode 1.
*   22/10/15 1.33   AGC	Support joystick control.
*   26/08/15 1.32   AGC	Support undo shape change.
*   06/08/15 1.31   AGC	Add const GetTrack() overload.
*   31/07/15 1.30   AGC	Support reporting targets as on ground or fixed.
*   10/07/15 1.29   AGC	Support reporting targets as manually initiated.
*   11/05/15 1.28   AGC	Signal event when target ID changed at end of motion.
*   17/03/15 1.27   AGC	Improve SPI handling.
*   07/11/14 1.26   AGC	Add GetRadarSim() function.
*			Support event handlers.
*   22/10/14 1.25   AGC	Check for changes to additional properties
*			before signalling target event.
*   14/10/14 1.24   AGC	Support IFF SPI and X pulse.
*			Support reporting targets as test targets.
*   01/09/14 1.23   AGC	Improve change detection for input targets.
*   18/07/14 1.22   AGC	Add Racon support.
*   02/06/14 1.21   AGC	Add motion offset height.
*			Use common AIS interface.
*   19/05/14 1.20   AGC	Fix target visibility when hidden due to motion.
*   16/05/14 1.19   AGC	Support symmetric shapes.
*   10/04/14 1.18   AGC	Move aircraft address to Mode-S structure.
*			Update track info when state changed.
*   26/03/14 1.17   AGC	Support reporting targets as simulated.
*			Support parameter control.
*			Support free running targets.
*   22/01/14 1.16   AGC	Support heading offset for targets.
*   21/01/14 1.15   AGC	Support setting IFF information.
*   05/09/13 1.14   AGC	Support passing platform lat/long to SetCurrentState().
*   06/06/13 1.13   AGC	Support multiple simulation files.
*   24/05/13 1.12   AGC	Support multilateration targets.
*   03/05/13 1.11   REW	Remove trailing comma from last change.
*   03/05/13 1.10   AGC	Support ADS-B info.
*   25/04/13 1.9    AGC	Support target templates.
*			Support custom target shapes.
*   28/03/13 1.8    AGC	Support extended AIS info.
*			Pre-calculate radar offsets.
*			Move load config here from SPxRadarSimulator.
*   18/03/13 1.7    AGC	Replace intensity with RCS.
*			Support flags.
*   13/02/13 1.6    AGC	Use percentage for intensity.
*   12/02/13 1.5    AGC	Prevent duplicated target names.
*   25/01/13 1.4    AGC	Add copy constructor.
*   21/12/12 1.3    AGC	Respond to motion change events.
*   09/11/12 1.2    AGC	Support configurable target size.
*			Support static targets.
*			Support AIS output.
*			Support configurable target intensity.
*   26/10/12 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_SIM_TARGET_H
#define _SPX_SIM_TARGET_H

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxErrorCode. */
#include "SPxLibUtils/SPxError.h"

/* For SPxEventHandler. */
#include "SPxLibUtils/SPxEventHandler.h"

/* For SPxAsterixSsrMode. */
#include "SPxLibNet/SPxAsterixEncoder.h"

/* For base classes. */
#include "SPxLibData/SPxSimMovable.h"
#include "SPxLibNet/SPxAISDecoder.h"

/*
 * Types
 */

/* Flags describing the type of target. */
enum SPxSimTargetFlags
{
    SPX_SIM_TARGET_FLAGS_NONE		= 0x00000000,
    SPX_SIM_TARGET_FLAGS_INPUT		= 0x00000001,
    SPX_SIM_TARGET_FLAGS_TEST		= 0x00000002,
    SPX_SIM_TARGET_FLAGS_INPUT_SPX	= 0x00000010,
    SPX_SIM_TARGET_FLAGS_INPUT_ASTERIX	= 0x00000020,
    SPX_SIM_TARGET_FLAGS_INPUT_AIS	= 0x00000040,
    SPX_SIM_TARGET_FLAGS_INPUT_ADSB	= 0x00000080,
    SPX_SIM_TARGET_FLAGS_INPUT_TTM	= 0x00000100
};

/* Set method for determinining target intensity. */
enum SPxSimTargetIntensity
{
    SPX_SIM_TARGET_INTENSITY_RCS		= 0,
    SPX_SIM_TARGET_INTENSITY_ABOVE_BACKGROUND	= 1
};

/* Structure for setting/getting ADSB information. */
struct SPxSimTargetAdsbInfo
{
    unsigned int capabilities;
    unsigned int category;
    unsigned int adsbVersion;
    unsigned int emergencyType;
};

/* Structure for setting/getting Mode-S information. */
struct SPxSimTargetModeSInfo
{
    UINT32 aircraftAddress;
};

static const UINT8 SPX_SIM_TARGET_IFF_MODE_5_FLAGS_SUMMARY	    = 0x01;
static const UINT8 SPX_SIM_TARGET_IFF_MODE_5_FLAGS_PIN		    = 0x02;
static const UINT8 SPX_SIM_TARGET_IFF_MODE_5_FLAGS_NATIONAL_ORIGIN  = 0x04;
static const UINT8 SPX_SIM_TARGET_IFF_MODE_5_FLAGS_POSITION	    = 0x08;
static const UINT8 SPX_SIM_TARGET_IFF_MODE_5_FLAGS_ALTITUDE	    = 0x10;
static const UINT8 SPX_SIM_TARGET_IFF_MODE_5_FLAGS_TIME_OFFSET	    = 0x20;
static const UINT8 SPX_SIM_TARGET_IFF_MODE_5_FLAGS_X_PULSE	    = 0x40;
static const UINT8 SPX_SIM_TARGET_IFF_MODE_5_FLAGS_FIGURE_OF_MERIT  = 0x80;

/* Structure for setting/getting IFF information. */
struct SPxSimTargetIffInfo
{
    UINT8 mode1;
    UINT8 mode1Conf;
    UINT16 mode1Extended;
    UINT16 mode2;
    UINT16 mode2Conf;
    UINT16 mode3A;
    UINT16 mode3AConf;
    UINT16 modeCConf;
    UINT8 flags;
    UINT8 flagsExt;
    UINT8 mode5Flags;
    UINT8 mode5Summary;
    UINT16 mode5PIN;
    UINT16 mode5NationalOrigin;
    double mode5TimeOffsetSecs;
    UINT8 mode5XPulse;
    UINT8 mode5FigureOfMerit;
};

/* Structure for setting/getting Racon information. */
struct SPxSimTargetRaconInfo
{
    char code;
    double onSecs;
    double offSecs;
};

/* Structure for setting/getting SART information. */
struct SPxSimTargetSartInfo
{
    int enabled;
    double pulseUsecs;
};

/* Structure for setting/getting P466 information. */
struct SPxSimTargetP466Info
{
    int isIdentitySet;
    UINT16 systemTrackNum;
    UINT8 identity;
    int isQualitySet;
    UINT8 quality;
    int isM6Set;
    UINT8 m6;
    int isClassificationSet;
    UINT8 classification;
    int isAltitudeSet;
    int isPositionSet;
    int isVelocitySet;
    int isCallsignSet;
};

/* Structure for setting/getting MIL2525D information. */
struct SPxSimTargetMIL2525DInfo
{
    UINT8 identity;
    UINT8 set;
    UINT8 entity;
    UINT8 entityType;
    UINT8 entitySubtype;
    UINT8 modifier1;
    UINT8 modifier2;
};

/* Forward declarations. */
struct SPxAISInfo;
class SPxRadarSimulator;
class SPxSimTargetTemplate;
class SPxSimMotion;
class SPxSimRadar;
class SPxSimAISStation;
struct SPxSimState;
class SPxRadarTrack;
struct SPxLatLong_tag;
struct SPxRadarSimEventParams;

/* Define our class. */
class SPxSimTarget : public SPxSimMovable, public SPxAISIface
{
public:
    /* Public types. */
    typedef SPxErrorCode (*EventHandler)(UINT32 flags);

    /* Public functions. */
    explicit SPxSimTarget(SPxRadarSimulator *radarSim, const char *name=NULL,
	UINT32 flags=SPX_SIM_TARGET_FLAGS_NONE);
    explicit SPxSimTarget(SPxSimTarget *target, const char *name=NULL, int checkName=TRUE);
    virtual ~SPxSimTarget(void);

    /* Target name. */
    SPxErrorCode SetName(const char *name);
    SPxErrorCode GetName(char *buffer, unsigned int bufLen) const;

    /* Is target editable etc? */
    SPxErrorCode SetFlags(UINT32 flags);
    UINT32 GetFlags(int mutexAlreadyLocked=FALSE) const;

    /* Target properties. */
    SPxErrorCode SetTemplate(SPxSimTargetTemplate *tgtTemplate);
    SPxErrorCode GetTemplate(SPxSimTargetTemplate **tgtTemplate) const;
    SPxErrorCode EnablePrimary(int enable);
    int IsPrimary(int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode EnableTrack(int enable);
    int IsTrack(int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode EnableMultilat(int enable);
    int IsMultilat(void) const;
    SPxErrorCode EnableAIS(int enable);
    int IsAIS(int mutexAlreadyLocked=FALSE) const;
    int IsAISEditable(void) const;
    SPxErrorCode EnableADSB(int enable);
    int IsADSB(int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetLengthMetres(double lengthMetres);
    SPxErrorCode GetLengthMetres(double *lengthMetres, int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetBreadthMetres(double breadthMetres);
    SPxErrorCode GetBreadthMetres(double *breadthMetres, int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetMaxStaticDraughtMetres(double maxStaticDraughtMetres);
    SPxErrorCode GetMaxStaticDraughtMetres(double *maxStaticDraughtMetres) const;
    SPxErrorCode SetShape(const SPxPoint *points, unsigned int numPoints, int symmetric=FALSE);
    SPxErrorCode GetShape(SPxPoint *points, unsigned int *pointsLen,
			  int *symmetric=NULL, int useTemplate=TRUE,
			  int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetIntensityType(SPxSimTargetIntensity intensityType);
    SPxErrorCode GetIntensityType(SPxSimTargetIntensity *intensityType,
				  int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetRcs(double rcs);
    SPxErrorCode GetRcs(double *rcs, int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetIntensityAboveBackground(double intensityAboveBackground);
    SPxErrorCode GetIntensityAboveBackground(double *intensityAboveBackground,
					     int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetRegularPoD(double regularPoD);
    SPxErrorCode GetRegularPoD(double *regularPoD,
			       int mutexAlreadyLocked=FALSE) const;

    /* Additional properties. */
    // cppcheck-suppress virtualCallInConstructor
    SPxErrorCode SetAISInfo(const SPxAISInfo *aisInfo);
    virtual SPxErrorCode GetAISInfo(SPxAISInfo *aisInfo) const
    {
	return GetAISInfo(aisInfo, FALSE);
    }
    SPxErrorCode GetAISInfo(SPxAISInfo *aisInfo, int mutexAlreadyLocked) const;
    SPxErrorCode SetAdsbInfo(const SPxSimTargetAdsbInfo *adsbInfo);
    SPxErrorCode GetAdsbInfo(SPxSimTargetAdsbInfo *adsbInfo,
			     int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetModeSInfo(const SPxSimTargetModeSInfo *modeSInfo);
    SPxErrorCode GetModeSInfo(SPxSimTargetModeSInfo *modeSInfo,
			      int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetIffInfo(const SPxSimTargetIffInfo *iffInfo);
    SPxErrorCode GetIffInfo(SPxSimTargetIffInfo *iffInfo,
			    int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode GetSpiRemainingMS(UINT32 *spiRemainingMS) const;
    SPxErrorCode SetRaconInfo(const SPxSimTargetRaconInfo *raconInfo);
    SPxErrorCode GetRaconInfo(SPxSimTargetRaconInfo *raconInfo,
			      int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetSartInfo(const SPxSimTargetSartInfo *sartInfo);
    SPxErrorCode GetSartInfo(SPxSimTargetSartInfo *sartInfo,
			     int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetP466Info(const SPxSimTargetP466Info *p466Info);
    SPxErrorCode GetP466Info(SPxSimTargetP466Info *p466Info,
			     int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetIJMSCode(const char *IJMSCode);
    SPxErrorCode GetIJMSCode(char *buf, unsigned int bufLen,
			     int getEffective=FALSE) const;
    SPxErrorCode SetMIL2525DInfo(const SPxSimTargetMIL2525DInfo *MIL2525DInfo);
    SPxErrorCode GetMIL2525DInfo(SPxSimTargetMIL2525DInfo *MIL2525DInfo,
				 int getEffective=FALSE) const;
    SPxErrorCode SetReportSimulated(int reportSimulated);
    int IsReportSimulated(int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetReportTest(int reportTest);
    int IsReportTest(int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetReportOnGround(int reportOnGround);
    int IsReportOnGround(int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetReportManuallyInitiated(int reportManuallyInitiated);
    int IsReportManuallyInitiated(int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetReportFixed(int reportFixed);
    int IsReportFixed(int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetSsrMode(SPxAsterixSsrMode ssrMode);
    SPxErrorCode GetSsrMode(SPxAsterixSsrMode *ssrMode,
			    int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetSpfData(const unsigned char *spfData, UINT8 spfLen);
    SPxErrorCode GetSpfData(unsigned char *spfData, UINT8 spfLen,
			    int getEffective=FALSE,
			    UINT16 asterixCat=48,
			    int mutexAlreadyLocked=FALSE) const;
    UINT8 GetSpfDataLen(int getEffective=FALSE,
			UINT16 asterixCat=48,
			int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetTrackQualityX(float sdX);
    SPxErrorCode GetTrackQualityX(float *sdX,
				  int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetTrackQualityY(float sdY);
    SPxErrorCode GetTrackQualityY(float *sdY,
				  int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetTrackQualityV(float sdV);
    SPxErrorCode GetTrackQualityV(float *sdV,
				  int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetTrackQualityH(float sdH);
    SPxErrorCode GetTrackQualityH(float *sdH,
				  int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetAcasResAdv(const unsigned char (&acasResAdv)[7]);
    SPxErrorCode GetAcasResAdv(unsigned char (&acseResAdv)[7],
			       int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode SetCommsCapability(UINT16 commsCapability);
    SPxErrorCode GetCommsCapability(UINT16 *commsCapability,
				    int mutexAlreadyLocked=FALSE) const;

    /* Target description. */
    SPxErrorCode SetDescription(const char *description);
    SPxErrorCode GetDescription(char *buffer, unsigned int bufLen) const;

    virtual SPxErrorCode SetMotionProfile(SPxSimMotion *motionProfile,
					  int jumpTo=TRUE);

    /* Track information. */
    SPxRadarTrack *GetTrack(void);
    const SPxRadarTrack *GetTrack(void) const;
    UINT32 GetUniqueID(void) const;

    /* Retrieve AIS station object. */
    SPxSimAISStation *GetAISStation(void);
    const SPxSimAISStation *GetAISStation(void) const;

    /* SPxLock interface. */
    SPxErrorCode Enter(void) const;
    SPxErrorCode Leave(void) const;

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Called by SPxRadarSimulator. */
    SPxErrorCode SetNameInternal(const char *name);
    SPxErrorCode SetTrack(SPxRadarTrack *track);
    SPxErrorCode SetUniqueID(UINT32 uniqueID);
    virtual void StateChanged(void);
    virtual SPxErrorCode Update(double currSecs, double diffSecs, 
				SPxLatLong_tag *initPlatLatLong,
				int changed=FALSE);
    virtual void MotionProfileWrapped(void);
    virtual SPxErrorCode RemoveRadarSim(void);
    virtual void ApplyDesc(const char *buffer);
    virtual const char *GetDesc(void) const;
    virtual SPxErrorCode ApplyDelayed(void);
    SPxErrorCode SignalEvent(UINT32 flags, const SPxRadarSimEventParams *params);
    virtual int CanSetInitialState(void) const;

    /* Private functions. */
    int isAIS(void) const;
    SPxErrorCode getShape(SPxPoint *points, unsigned int *pointsLen,
			  int *symmetric=NULL, int useTemplate=TRUE) const;
    SPxErrorCode setDelayedTemplate(const char *delayedTemplate);
    SPxErrorCode updateTrackInfo(void);
    SPxErrorCode updateAISStation(int changed);
    SPxErrorCode buildP466Spf(unsigned char *buf, unsigned int *bufLenPtr) const;
    SPxErrorCode signalEvent(void);
    SPxErrorCode signalEvent(UINT32 flags,
			     const SPxRadarSimEventParams *params=NULL);

    friend class SPxRadarSimulator;
    friend class SPxSimMotion;

}; /* SPxSimTarget. */

#endif /* _SPX_SIM_TARGET_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
