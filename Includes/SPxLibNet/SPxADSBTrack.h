/*********************************************************************
*
* (c) Copyright 2011 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxADSBTrack class which keeps information on
*	targets received via ADSB.
*
*
* Revision Control:
*   10/01/22 v1.30   AGC	Support ADS-B version and emergency type.
*
* Previous Changes:
*   04/11/21 1.29   REW	Support Set/GetTrackNumber().
*   10/02/21 1.28   AGC	Add GetADSBCategory().
*   20/11/20 1.27   REW	Implement base class GetIsSPI() and GetIsEmergency().
*   30/10/20 1.26   AGC	Return flight level support from GetAltitude().
*			GetAsterixMsg() now overrides base class.
*   29/09/20 1.25   REW	Add optional forceReset to SetAA().
*   18/09/20 1.24   AGC	Add AA/IFFMode3A accessor functions from base class.
*   04/08/20 1.23   BTB	Make SPxFR24Decoder a friend.
*   04/02/20 1.22   REW	Support barometric and geometric vertical rates.
*			Support internal set function for auto tests.
*   22/10/19 1.21   AGC	Support barometric and geometric altitudes.
*   02/05/19 1.20   REW	Avoid compiler warning in last change.
*   02/05/19 1.19   REW	Support base class ground/test/simulated funcs.
*   17/09/18 1.18   SP 	Add GetSecondarySensorType().
*   25/06/18 1.17   AGC	Correctly suppress cppcheck warning.
*   25/05/18 1.16   REW	Add BuildSPxReport().
*   06/03/18 1.15   AGC	Support storing ASTERIX message with track.
*   11/12/17 1.14   SP 	Rename GetBestID() as GetID().
*                      	Add GetSecondaryID().
*   17/11/17 1.13   AGC	Derive from SPxUniTrack.
*   13/07/17 1.12   SP 	Add Set/GetUserData().
*   31/07/15 1.11   AGC	Add on ground and fixed support.
*   14/10/14 1.10   AGC	Add test bit support.
*   18/07/14 1.9    AGC	Add simulated bit support.
*   04/07/14 1.8    AGC	Add mode 3/A support.
*   21/01/14 1.7    REW	Clear flags if not lost or not deleted.
*   15/10/13 1.6    AGC	Fix warnings from SPxLibAVMfc.
*   04/10/13 1.5    AGC	Simplify headers.
*   03/05/13 1.4    AGC	Support manually creating SPxADSBTrack objects.
*			Add high res flag to altitude.
*			Support configurable number of history points.
*   28/09/12 1.3    REW	Support creation time.
*   02/01/12 1.2    REW	Support vertical rate.
*   23/12/11 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_ADSB_TRACK_H
#define _SPX_ADSB_TRACK_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibNet/SPxUniTrack.h"
#include "SPxLibData/SPxPackets.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Flags for why callbacks are being invoked. */
#define	SPX_ADSB_TRACK_FLAGS_CREATED	0x00000001	/* Created track */
#define	SPX_ADSB_TRACK_FLAGS_LOST	0x00000002	/* Lost track */
#define	SPX_ADSB_TRACK_FLAGS_DELETED	0x00000004	/* Deleted track */
#define	SPX_ADSB_TRACK_FLAGS_POSITION	0x00000010	/* New position */
#define	SPX_ADSB_TRACK_FLAGS_VELOCITY	0x00000020	/* New velocity */
#define	SPX_ADSB_TRACK_FLAGS_ALTITUDE	0x00000040	/* New altitude */
#define	SPX_ADSB_TRACK_FLAGS_VERTICALRATE 0x000080	/* New vertical rate */
#define	SPX_ADSB_TRACK_FLAGS_CAPABILITIES 0x000100	/* New value */
#define	SPX_ADSB_TRACK_FLAGS_CALLSIGN	0x00000200	/* New callsign */
#define SPX_ADSB_TRACK_FLAGS_MODE3A	0x00000400	/* New mode 3/A code */
#define SPX_ADSB_TRACK_FLAGS_SIMULATED	0x00000800	/* Simulated bit change */
#define SPX_ADSB_TRACK_FLAGS_TEST	0x00001000	/* Test bit change */
#define SPX_ADSB_TRACK_FLAGS_ON_GROUND	0x00002000	/* On ground bit change */
#define SPX_ADSB_TRACK_FLAGS_FIXED	0x00004000	/* Fixed transponder bit change */
#define SPX_ADSB_TRACK_FLAGS_EMERG_TYPE	0x00008000	/* Emergency type bit change */
#define SPX_ADSB_TRACK_FLAGS_VERSION	0x00010000	/* Version bit change */
#define SPX_ADSB_TRACK_FLAGS_ES_IN	0x00020000	/* ES receive bit change */
#define SPX_ADSB_TRACK_FLAGS_UAT_IN	0x00040000	/* UAT receive bit change */

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

/* Forward-declare other classes we use in case headers are in wrong order. */
class SPxADSBDecoder;
class SPxAsterixMsg;

/* Define a history point. */
typedef struct SPxADSBTrackHistoryPt_tag {
    /* Timestamp. */
    SPxTime_t timestamp;

    /* Position. */
    double latDegs;		/* Latitude, degrees positive north */
    double longDegs;		/* Longitude, degrees positive east */
} SPxADSBTrackHistoryPt_t;


/*
 * Define our class.
 */
class SPxADSBTrack :public SPxUniTrack
{
/* The decoder classes can access our internals directly. */
friend class SPxADSBDecoder;
friend class SPxFR24Decoder;

public:
    /*
     * Public functions.
     */
    /* Note that the constructor is not public, so the
     * class can only be instantiated by the SPxADSBDecoder.
     */
    static SPxADSBTrack *Create(unsigned int aircraftAddress,
				double latDegs, double longDegs,
				double speedMps, double courseDegs,
				double altitudeMetres=0.0, int isAltitudeValid=FALSE,
				double verticalRateMps=0.0, int isVerticalRateValid=FALSE,
				unsigned int capabilities=0,
				const char *callsign=NULL,
				unsigned int category=0);
    virtual ~SPxADSBTrack(void);

    /*
     * Value access functions.
     */
    UINT32 GetAA(void) const		{ return(m_aa); }
    virtual SPxErrorCode GetAA(UINT32* /*aa*/) const SPX_VIRTUAL_OVERRIDE;
    virtual UINT32 GetID(void) const SPX_VIRTUAL_OVERRIDE { return GetAA(); }
    virtual UINT32 GetSecondaryID(void) const SPX_VIRTUAL_OVERRIDE { return GetAA(); }
    virtual UINT32 GetSecondarySensorType(void) const SPX_VIRTUAL_OVERRIDE { return SPX_PACKET_TRACK_SENSOR_ADSB; }
    SPxADSBDecoder *GetDecoder(void) const { return(m_parentDecoder); }
    SPxErrorCode GetCreationTime(SPxTime_t *timestampPtr,
				double *ageSecsPtr) const;
    int GetTrackNumber(void) const	{ return(m_trackNumber); }

    /* Dynamic values. */
    SPxErrorCode GetPosition(double *latDegsPtr, double *longDegsPtr,
				SPxTime_t *timestampPtr=NULL,
				double *ageSecsPtr=NULL,
				unsigned int expirySecs=0) const;
    virtual SPxErrorCode GetLatLong(double *latDegsPtr, double *longDegsPtr,
				    int useFiltered = TRUE,
				    double *ageSecsPtr = NULL) const SPX_VIRTUAL_OVERRIDE;
    SPxErrorCode GetVelocity(double *speedMpsPtr, double *courseDegsPtr,
				SPxTime_t *timestampPtr,
				double *ageSecsPtr=NULL,
				unsigned int expirySecs=0) const;
    virtual SPxErrorCode GetVelocity(double *speedMpsRtn,
				     double *courseDegsRtn) const SPX_VIRTUAL_OVERRIDE;
    SPxErrorCode GetAltitude(double *altitudeMetresPtr,
				SPxTime_t *timestampPtr,
				double *ageSecsPtr,
				unsigned int expirySecs=0,
				int *isHighRes=NULL,
				int *isAltFromFLRtn=NULL) const;
    SPxErrorCode GetAltitudeGeo(double *altitudeGeoMetresPtr,
				SPxTime_t *timestampPtr,
				double *ageSecsPtr,
				unsigned int expirySecs=0,
				int *isHighRes=NULL) const;
    SPxErrorCode GetAltitudeBaro(double *altitudeBaroMetresPtr,
				 SPxTime_t *timestampPtr,
				 double *ageSecsPtr,
				 unsigned int expirySecs=0,
				 int *isHighRes=NULL) const;
    virtual SPxErrorCode GetAltitude(double *altMetresRtn,
				     double *ageSecsRtn=NULL,
				     int *isAltFromFLRtn=NULL) const SPX_VIRTUAL_OVERRIDE;
    SPxErrorCode GetVerticalRate(double *rateMpsPtr,
				SPxTime_t *timestampPtr,
				double *ageSecsPtr=NULL,
				unsigned int expirySecs=0) const;
    SPxErrorCode GetVerticalRateGeo(double *rateMpsGeoPtr,
				SPxTime_t *timestampPtr,
				double *ageSecsPtr=NULL,
				unsigned int expirySecs=0) const;
    SPxErrorCode GetVerticalRateBaro(double *rateMpsBaroPtr,
				SPxTime_t *timestampPtr,
				double *ageSecsPtr=NULL,
				unsigned int expirySecs=0) const;
    virtual SPxErrorCode GetVerticalRate(double *verticalRateMpsRtn) const SPX_VIRTUAL_OVERRIDE;
    SPxErrorCode GetCapabilities(unsigned int *capabilitiesPtr,
				SPxTime_t *timestampPtr=NULL,
				double *ageSecsPtr=NULL,
				unsigned int expirySecs=0) const;
    static const char *GetStringForCapabilities(unsigned int capabilities);
    const char *GetCallsign(SPxTime_t *timestampPtr=NULL,
				double *ageSecsPtr=NULL) const;
    SPxErrorCode GetCategory(unsigned int *categoryPtr,
				SPxTime_t *timestampPtr=NULL,
				double *ageSecsPtr=NULL,
				unsigned int expirySecs=0) const;
    virtual SPxErrorCode GetADSBCategory(unsigned int *categoryRtn) const SPX_VIRTUAL_OVERRIDE
    {
	return GetCategory(categoryRtn);
    }
    static const char *GetStringForCategory(unsigned int category);

    /* Convenience function to get composite information about a track. */
    SPxErrorCode GetDescriptiveString(char *buf, int bufSize);

    virtual void GetDefaultTrackName(char *buf, unsigned int bufLen) const SPX_VIRTUAL_OVERRIDE;

    /* Get track information in a different format. */
    SPxErrorCode BuildSPxReport(struct SPxPacketTrackExtended_tag *rpt);

    /* History values. */
    unsigned int GetHistory(const SPxADSBTrackHistoryPt_t **bufferPtr,
				double desiredSeconds=0.0,
				unsigned int desiredPoints=0);
    void ClearHistory(void)	{ m_historyPtsNumUsed = 0; }

    /*
     * Functions relating to the status of the track.
     */
    SPxErrorCode SetCanBeDeleted(int canBeDeleted)
    {
	m_canBeDeleted = (canBeDeleted ? TRUE : FALSE);
	if( m_canBeDeleted ) { m_flags |= SPX_ADSB_TRACK_FLAGS_DELETED; }
	else		     { m_flags &= ~SPX_ADSB_TRACK_FLAGS_DELETED; }
	return(SPX_NO_ERROR);
    }
    int GetCanBeDeleted(void) const	{ return(m_canBeDeleted); }
    int GetIsLost(void) const		{ return(m_isLost); }

    /* Additional track information. */
    SPxErrorCode SetMode3A(UINT16 mode3A, const SPxTime_t *timestamp);
    SPxErrorCode GetMode3A(UINT16 *mode3APtr, SPxTime_t *timestampPtr=NULL,
		     double *ageSecsPtr=NULL,
		     unsigned int expirySecs=0) const;
    virtual SPxErrorCode GetIFFMode3A(UINT16 *iffMode3ARtn) const SPX_VIRTUAL_OVERRIDE;
    SPxErrorCode SetSimulated(int simulated, const SPxTime_t *timestamp);
    SPxErrorCode GetSimulated(int *simulatedPtr, SPxTime_t *timestampPtr=NULL,
		     double *ageSecsPtr=NULL,
		     unsigned int expirySecs=0) const;
    SPxErrorCode SetTest(int test, const SPxTime_t *timestamp);
    SPxErrorCode GetTest(int *testPtr, SPxTime_t *timestampPtr=NULL,
		     double *ageSecsPtr=NULL,
		     unsigned int expirySecs=0) const;
    SPxErrorCode SetOnGround(int onGround, const SPxTime_t *timestamp);
    SPxErrorCode GetOnGround(int *onGroundPtr, SPxTime_t *timestampPtr=NULL,
		     double *ageSecsPtr=NULL,
		     unsigned int expirySecs=0) const;
    SPxErrorCode SetFixed(int fixed, const SPxTime_t *timestamp);
    SPxErrorCode GetFixed(int *fixedPtr, SPxTime_t *timestampPtr=NULL,
		     double *ageSecsPtr=NULL,
		     unsigned int expirySecs=0) const;
    SPxErrorCode SetEmergencyType(unsigned int emergencyType, const SPxTime_t *timstamp);
    SPxErrorCode GetEmergencyType(unsigned int *emergencyTypePtr, SPxTime_t *timestampPtr=NULL,
				  double *ageSecsPtr=NULL,
				  unsigned int expirySecs=0) const;
    SPxErrorCode SetVersion(unsigned int version, const SPxTime_t *timstamp);
    SPxErrorCode GetVersion(unsigned int *versionPtr, SPxTime_t *timestampPtr=NULL,
			    double *ageSecsPtr=NULL,
			    unsigned int expirySecs=0) const;
    SPxErrorCode Set1090ESIn(int esIn, const SPxTime_t *timestamp);
    SPxErrorCode Get1090ESIn(int *esInPtr, SPxTime_t *timestampPtr=NULL,
			     double *ageSecsPtr=NULL,
			     unsigned int expirySecs=0) const;
    SPxErrorCode SetUATIn(int uatIn, const SPxTime_t *timestamp);
    SPxErrorCode GetUATIn(int *uatInPtr, SPxTime_t *timestampPtr=NULL,
			  double *ageSecsPtr=NULL,
			  unsigned int expirySecs=0) const;
    virtual const SPxAsterixMsg *GetAsterixMsg(void) const SPX_VIRTUAL_OVERRIDE;

    /* Wrappers around the above to match the base class interface. */
    virtual int GetOnGround(void) const SPX_VIRTUAL_OVERRIDE;
    virtual int GetIsTest(void) const SPX_VIRTUAL_OVERRIDE;
    virtual int GetIsSimulated(void) const SPX_VIRTUAL_OVERRIDE;

    /* Other functions from base class. */
    virtual int GetIsEmergency(void) const SPX_VIRTUAL_OVERRIDE;
    virtual int GetIsSPI(void) const SPX_VIRTUAL_OVERRIDE;

    /* Flags describing what has changed (for callbacks etc.). */
    UINT32 GetFlags(void) const		{ return(m_flags); }
    void ClearFlags(void)		{ m_flags = 0; }

    /* Functions that can be given to the SPxADSBDecoder::IterateTracks()
     * function to do certain actions on all tracks.
     */
    static int IteratorActivate(void * /*decoder*/, void * /*arg*/, void *track)
    {
	if(track != NULL) { ((SPxADSBTrack *)track)->SetIsActivated(TRUE); }
	return(SPX_NO_ERROR);
    }
    static int IteratorDeactivate(void * /*decoder*/, void * /*arg*/, void *track)
    {
	if(track != NULL) { ((SPxADSBTrack *)track)->SetIsActivated(FALSE); }
	return(SPX_NO_ERROR);
    }
    static int IteratorSelect(void * /*decoder*/, void * /*arg*/, void *track)
    {
	if(track != NULL) { ((SPxADSBTrack *)track)->SetIsSelected(TRUE); }
	return(SPX_NO_ERROR);
    }
    static int IteratorDeselect(void * /*decoder*/, void * /*arg*/, void *track)
    {
	if(track != NULL) { ((SPxADSBTrack *)track)->SetIsSelected(FALSE); }
	return(SPX_NO_ERROR);
    }
    static int IteratorClearHistory(void * /*decoder*/, void * /*arg*/, void *track)
    {
	if(track != NULL) { ((SPxADSBTrack *)track)->ClearHistory(); }
	return(SPX_NO_ERROR);
    }

    /* Functions used by automated test code only - do not call normally. */
    void autoTestSetup(double aMetres, unsigned int cat,
			double vRateGeo, double vRateBaro);

protected:
    /*
     * Protected functions, likely to be used by SPxADSBDecoder.
     */
    /* Constructor. */
    SPxADSBTrack(SPxADSBDecoder *parent, unsigned int numHistoryPts=500);

    /* Value setting functions. */
    SPxErrorCode SetAA(UINT32 aa, SPxTime_t *timestamp, int forceReset=FALSE);
    SPxErrorCode SetTrackNumber(int trackNumber);
    SPxErrorCode SetPosition(double latDegs, double longDegs,
				SPxTime_t *timestamp);
    SPxErrorCode SetVelocity(double speedMps, double courseDegs,
				SPxTime_t *timestamp);
    SPxErrorCode SetAltitude(double altitudeMetres,
				int isHighRes,
				int isBarometric,
				SPxTime_t *timestamp);
    SPxErrorCode SetAltitudeDiff(double altitudeDiffFromBaro,
				 SPxTime_t *timestamp);
    SPxErrorCode SetVerticalRate(double rateMps,
				SPxTime_t *timestamp,
				int isBarometric);
    SPxErrorCode SetCapabilities(unsigned int capabilities,
				SPxTime_t *timestamp);
    SPxErrorCode SetCallsign(const char *callsign,
				SPxTime_t *timestamp);
    SPxErrorCode SetCategory(unsigned int category,
				SPxTime_t *timestamp);
    SPxErrorCode SetAsterixMsg(const SPxAsterixMsg *asterixMsg);

    /*
     * Functions relating to the status of the track.
     */
    SPxErrorCode SetIsLost(int isLost)
    {
	m_isLost = (isLost ? TRUE : FALSE);
	if( m_isLost )	{ m_flags |= SPX_ADSB_TRACK_FLAGS_LOST; }
	else		{ m_flags &= ~SPX_ADSB_TRACK_FLAGS_LOST; }
	return(SPX_NO_ERROR);
    }

    /*
     * Protected variables.
     */
    /* Linked list pointer, used by parent. */
    SPxADSBTrack *m_next;		/* Next in list */

private:
    /*
     * Private variables.
     */
    SPxADSBDecoder *m_parentDecoder;	/* Parent decoder */
    UINT32 m_aa;			/* Unique number */
    int m_trackNumber;			/* Optional track number, -1 if none */

    /* Flags for what's changed recently. */
    UINT32 m_flags;			/* SPX_ADSB_TRACK_FLAGS_... */

    /*
     * Dynamic data.
     */
    /* Creation time. */
    SPxTime_t m_creationTime;		/* Time of first report */

    /* Position. */
    double m_latDegs;			/* Latitude, +ve north */
    double m_longDegs;			/* Longitude, +ve east */
    SPxTime_t m_latLongTime;		/* Time of lat/long */

    /* Velocity. */
    double m_speedMps;			/* Speed in metres per second */
    double m_courseDegs;		/* Course in degs clockwise */
    SPxTime_t m_velocityTime;		/* Time of velocity */

    /* Barometric Altitude. */
    double m_altitudeBaroMetres;	/* Barometric altitude in metres */
    int m_isAltitudeBaroHighRes;	/* Is the altitude low or high resolution? */
    SPxTime_t m_altitudeBaroTime;	/* Time of value */

    /* Geometric Altitude. */
    double m_altitudeGeoMetres;		/* Geometric altitude in metres */
    int m_isAltitudeGeoHighRes;		/* Is the altitude low or high resolution? */
    SPxTime_t m_altitudeGeoTime;	/* Time of value */

    /* Altitude difference. */
    double m_altitudeDiffFromBaroMetres;/* (Geometric - barometric) height in metres */
    SPxTime_t m_altitudeDiffTime;	/* Time of value */

    /* Barometric vertical rate. */
    double m_verticalRateBaroMps;	/* Rate in metres/sec. +ve upwards. */
    SPxTime_t m_verticalRateBaroTime;	/* Time of value */

    /* Geometric vertical rate. */
    double m_verticalRateGeoMps;	/* Rate in metres/sec. +ve upwards. */
    SPxTime_t m_verticalRateGeoTime;	/* Time of value */

    /* Capabilities. */
    unsigned int m_capabilities;	/* 0 to 7 */
    SPxTime_t m_capabilitiesTime;	/* Time of value */

    /* Callsign */
    char m_callsign[9];			/* 8 character string */
    SPxTime_t m_callsignTime;		/* Time of value */

    /* Category. */
    unsigned int m_category;		/* Bits 7-4=cateogry, 0-3=value */
    SPxTime_t m_categoryTime;		/* Time of value */

    /* Additional information. */
    UINT16 m_mode3A;			/* IFF mode 3/A */
    SPxTime_t m_mode3ATime;		/* Time of value */
    int m_simulated;			/* Simulated? */
    SPxTime_t m_simulatedTime;		/* Time of value */
    int m_test;				/* Test target? */
    SPxTime_t m_testTime;		/* Time of value */
    int m_onGround;			/* On ground target? */
    SPxTime_t m_onGroundTime;		/* Time of value */
    int m_fixed;			/* Fixed transponder? */
    SPxTime_t m_fixedTime;		/* Time of value */
    unsigned int m_emergencyType;	/* Type of emergency */
    SPxTime_t m_emergencyTypeTime;	/* Time of value */
    unsigned int m_version;		/* MOPS version */
    SPxTime_t m_versionTime;		/* Time of value */
    int m_esIn;				/* ES receive supported? */
    SPxTime_t m_esInTime;		/* Time of value */
    int m_uatIn;			/* UAT receive supported? */
    SPxTime_t m_uatInTime;		/* Time of value */

    /* Asterix message. */
    SPxAutoPtr<SPxAsterixMsg> m_asterixMsg;

    /* Rendering control. */
    int m_canBeDeleted;			/* True if can be deleted */
    int m_isLost;			/* True if lost */

    /* History points. */
    SPxADSBTrackHistoryPt_t *m_historyPts;	/* Buffer for points */
    unsigned int m_historyPtsSize;		/* Size of buffer, in points */
    unsigned int m_historyPtsNumUsed;		/* Number of stored points */

    /*
     * Private functions.
     */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode Reset(void) SPX_VIRTUAL_OVERRIDE { return Reset(NULL); }
    SPxErrorCode Reset(SPxTime_t *timestamp);

}; /* SPxADSBTrack */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_ADSB_TRACK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
