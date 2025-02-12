/*********************************************************************
*
* (c) Copyright 2011 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxAISTrack class which keeps information on
*	targets received via AIS.
*
*
* Revision Control:
*   01/02/22 v1.30   BTB	Allow configuration of max number of history points.
*
* Previous Changes:
*   07/09/21 1.29   REW	Maintain m_isOwnShip.
*   11/03/21 1.28   AJH	Support track last update time.
*   18/01/21 1.27   REW	Support access to AIS secs for position.
*   30/10/20 1.26   AGC	Return flight level support from GetAltitude().
*   18/09/20 1.25   AGC	Add MMSI accessor function from base class.
*   19/12/19 1.24   BTB	Add Get/SetIsBold().
*   10/12/19 1.23   BTB	Add check for hazardous ship cargo type.
*   29/11/19 1.22   AGC	Support AtoNs.
*   29/04/19 1.21   BTB	Make GetStringForNavStatus() and GetStringForShipCargoType() static.
*   14/01/19 1.20   REW	Support extrapolation.
*   17/09/18 1.19   SP 	Add GetSecondarySensorType().
*   25/06/18 1.18   AGC	Correctly suppress cppcheck warning.
*   02/02/18 1.17   AGC	Support RAIM configuration.
*			Support Class-B flags.
*   18/01/18 1.16   AGC	Support Inland AIS info.
*   11/12/17 1.15   SP 	Rename GetBestID() as GetID().
*                      	Add GetSecondaryID().
*   20/11/17 1.14   AGC	Add virtual override to GetHeading().
*   17/11/17 1.13   AGC	Derive from SPxUniTrack.
*   13/07/17 1.12   SP 	Add Set/GetUserData().
*   16/06/16 1.11   SP 	Correct prototype arg.
*   16/03/16 1.10   AJH	Support set/get for source address/port.
*   27/01/14 1.9    REW	Clear flags if not lost or not deleted.
*   15/10/13 1.8    AGC	Fix warnings from SPxLibAVMfc.
*   04/10/13 1.7    AGC	Simplify headers.
*   14/07/11 1.6    REW	Add ClearHistory() support.
*   13/07/11 1.5    REW	Avoid compiler warnings.
*   05/07/11 1.4    REW	Support history trails.
*			Correct order of args in iterator functions.
*   27/06/11 1.3    REW	Support many more fields etc.
*   28/01/11 1.2    REW	Support heading and dimension.
*   27/01/11 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_AIS_TRACK_H
#define _SPX_AIS_TRACK_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibNet/SPxAISDecoder.h"
#include "SPxLibNet/SPxUniTrack.h"
#include "SPxLibData/SPxPackets.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Flags for why callbacks are being invoked. */
#define	SPX_AIS_TRACK_FLAGS_CREATED		0x00000001	/* Created track */
#define	SPX_AIS_TRACK_FLAGS_LOST		0x00000002	/* Lost track */
#define	SPX_AIS_TRACK_FLAGS_DELETED		0x00000004	/* Deleted track */
#define	SPX_AIS_TRACK_FLAGS_POSITION		0x00000010	/* New position */
#define	SPX_AIS_TRACK_FLAGS_VELOCITY		0x00000020	/* New velocity */
#define	SPX_AIS_TRACK_FLAGS_HEADING		0x00000040	/* New heading */
#define	SPX_AIS_TRACK_FLAGS_ROT			0x00000080	/* New rate of turn */
#define	SPX_AIS_TRACK_FLAGS_NAVSTATUS		0x00000100	/* New nav status */
#define SPX_AIS_TRACK_FLAGS_SPECMANIND		0x00000200	/* New special manoeuvre indicator */
#define SPX_AIS_TRACK_FLAGS_RAIM		0x00000400	/* New RAIM */
#define SPX_AIS_TRACK_FLAGS_CLASSB		0x00000800	/* New Class B flags */
#define SPX_AIS_TRACK_FLAGS_ATON		0x00001000	/* New Aid-to-navigation type */
#define	SPX_AIS_TRACK_FLAGS_STATICDATA		0x00010000	/* New static data */
#define	SPX_AIS_TRACK_FLAGS_INLANDSTATICDATA	0x00020000	/* New inland static data */
#define	SPX_AIS_TRACK_FLAGS_EXTRAPOLATED	0x10000000	/* New extrapolation */

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
class SPxAISDecoder;

/* Define a history point. */
typedef struct SPxAISTrackHistoryPt_tag {
    /* Timestamp. */
    SPxTime_t timestamp;

    /* Position. */
    double latDegs;		/* Latitude, degrees positive north */
    double longDegs;		/* Longitude, degrees positive north */
} SPxAISTrackHistoryPt_t;


/*
 * Define our class.
 */
class SPxAISTrack :public SPxUniTrack
{
/* The decoder class can access our internals directly. */
friend class SPxAISDecoder;

public:
    /*
     * Public functions.
     */
    /* Note that the constructor and destructor are not public, so the
     * class can only be instantiated by the SPxAISDecoder.
     */

    /*
     * Value access functions.
     */
    UINT32 GetMMSI(void) const		{ return(m_mmsi); }
    virtual SPxErrorCode GetMMSI(UINT32 *mmsi) const SPX_VIRTUAL_OVERRIDE;
    SPxAISDecoder *GetDecoder(void) const { return(m_parentDecoder); }

    /* Dynamic values. */
    SPxErrorCode GetPosition(double *latDegsPtr, double *longDegsPtr,
				SPxTime_t *timestampPtr=NULL,
				double *ageSecsPtr=NULL,
				unsigned int expirySecs=0,
				unsigned char *aisSecsOnlyPtr=NULL) const;
    virtual SPxErrorCode GetExtrapolatedPosition(
				double *latDegsPtr, double *longDegsPtr,
				SPxTime_t *timestampPtr=NULL,
				double *ageSecsPtr=NULL,
				unsigned int expirySecs=0) const;
    virtual SPxErrorCode GetLatLong(double *latDegsPtr, double *longDegsPtr,
				    int useFiltered=TRUE,
				    double *ageSecsPtr = NULL) const SPX_VIRTUAL_OVERRIDE;
    SPxErrorCode GetVelocity(double *speedMpsPtr, double *courseDegsPtr,
				SPxTime_t *timestampPtr,
				double *ageSecsPtr=NULL,
				unsigned int expirySecs=0) const;
    virtual SPxErrorCode GetVelocity(double *speedMpsPtr, double *courseDegsPtr) const SPX_VIRTUAL_OVERRIDE;
    SPxErrorCode GetHeading(double *headingDegsPtr,
				SPxTime_t *timestampPtr,
				double *ageSecsPtr=NULL,
				unsigned int expirySecs=0) const;
    virtual SPxErrorCode GetHeading(double *headingDegsPtr) const SPX_VIRTUAL_OVERRIDE;
    SPxErrorCode GetRateOfTurn(double *rotDegsPerMinPtr,
				SPxTime_t *timestampPtr=NULL,
				double *ageSecsPtr=NULL,
				unsigned int expirySecs=0) const;
    SPxErrorCode GetNavStatus(unsigned int *navStatusPtr,
				SPxTime_t *timestampPtr=NULL,
				double *ageSecsPtr=NULL,
				unsigned int expirySecs=0) const;
    static const char *GetStringForNavStatus(unsigned int navStatus);
    SPxErrorCode GetSpecManInd(UINT8 *specManIndPtr,
			       SPxTime_t *timestampPtr=NULL,
			       double *ageSecsPtr=NULL,
			       unsigned int expirySecs=0) const;
    SPxErrorCode GetRAIM(int *raimPtr,
			 SPxTime_t *timestampPtr=NULL,
			 double *ageSecsPtr=NULL,
			 unsigned int expirySecs=0) const;
    SPxErrorCode GetClassBFlags(int *unitFlagPtr,
				int *displayFlagPtr,
				int *dscFlagPtr,
				int *bandFlagPtr,
				int *message22FlagPtr,
				SPxTime_t *timestampPtr=NULL,
				double *ageSecsPtr=NULL,
				unsigned int expirySecs=0) const;
    SPxErrorCode GetAtoNType(UINT8 *atonTypePtr,
			     SPxTime_t *timestampPtr=NULL,
			     double *ageSecsPtr=NULL,
			     unsigned int expirySecs=0) const;
    virtual SPxErrorCode GetAltitude(double *altMetresRtn,
				     double *ageSecsRtn=NULL,
				     int *isAltFromFLRtn=NULL) const SPX_VIRTUAL_OVERRIDE;

    /* Ship-static values. */
    SPxAISStationType GetStationType(SPxTime_t *timestampPtr = NULL,
				     double *ageSecsPtr = NULL) const;
    const char *GetCallsign(SPxTime_t *timestampPtr=NULL,
				double *ageSecsPtr=NULL) const;
    const char *GetVesselName(SPxTime_t *timestampPtr=NULL,
				double *ageSecsPtr=NULL) const;
    const char *GetDestination(SPxTime_t *timestampPtr=NULL,
				double *ageSecsPtr=NULL) const;
    SPxErrorCode GetDimensions(unsigned int *metresToBowPtr,
				unsigned int *metresToSternPtr,
				unsigned int *metresToPortPtr,
				unsigned int *metresToStarboardPtr,
				SPxTime_t *timestampPtr=NULL,
				double *ageSecsPtr=NULL,
				unsigned int expirySecs=0) const;
    SPxErrorCode GetIMONumber(UINT32 *imoNumberPtr,
				SPxTime_t *timestampPtr=NULL,
				double *ageSecsPtr=NULL,
				unsigned int expirySecs=0) const;
    SPxErrorCode GetDraught(double *draughtMetresPtr,
				SPxTime_t *timestampPtr=NULL,
				double *ageSecsPtr=NULL,
				unsigned int expirySecs=0) const;
    SPxErrorCode GetShipCargoType(UINT32 *shipCargoTypePtr,
				SPxTime_t *timestampPtr=NULL,
				double *ageSecsPtr=NULL,
				unsigned int expirySecs=0) const;
    static const char *GetStringForShipCargoType(unsigned int shipCargoType);
    SPxErrorCode GetETA(UINT32 *etaPtr,
				unsigned int *etaMonthPtr,
				unsigned int *etaDayPtr,
				unsigned int *etaHourPtr,
				unsigned int *etaMinPtr,
				SPxTime_t *timestampPtr=NULL,
				double *ageSecsPtr=NULL,
				unsigned int expirySecs=0) const;

    /* Inland-static values. */
    SPxErrorCode GetInlandENI(UINT32 *eniPtr,
			      SPxTime_t *timestampPtr=NULL,
			      double *ageSecsPtr=NULL,
			      unsigned int expirySecs=0) const;
    SPxErrorCode GetInlandDimensions(double *lengthMetresPtr,
				     double *beamMetresPtr,
				     SPxTime_t *timestampPtr=NULL,
				     double *ageSecsPtr=NULL,
				unsigned int expirySecs=0) const;
    SPxErrorCode GetInlandERI(UINT32 *eriPtr,
			      SPxTime_t *timestampPtr=NULL,
			      double *ageSecsPtr=NULL,
			      unsigned int expirySecs=0) const;
    SPxErrorCode GetInlandHazardousCargo(UINT8 *hazardousCargoPtr,
					 SPxTime_t *timestampPtr=NULL,
					 double *ageSecsPtr=NULL,
					 unsigned int expirySecs=0) const;
    SPxErrorCode GetInlandDraught(double *draughtMetresPtr,
			    SPxTime_t *timestampPtr=NULL,
			    double *ageSecsPtr=NULL,
			    unsigned int expirySecs=0) const;

    /* Own ship status. */
    SPxErrorCode SetIsOwnShip(int isOwnShip);
    int GetIsOwnShip(void) const	{ return(m_isOwnShip); }

    /* Convenience function to get composite information about a track. */
    SPxErrorCode GetDescriptiveString(char *buf, int bufSize);

    /* History values. */
    unsigned int GetHistory(const SPxAISTrackHistoryPt_t **bufferPtr,
				double desiredSeconds=0.0,
				unsigned int desiredPoints=0);
    void ClearHistory(void)	{ m_historyPtsNumUsed = 0; }

    /*
     * Functions relating to the status of the track.
     */
    SPxErrorCode SetCanBeDeleted(int canBeDeleted)
    {
	m_canBeDeleted = (canBeDeleted ? TRUE : FALSE);
	if( m_canBeDeleted )	{ m_flags |= SPX_AIS_TRACK_FLAGS_DELETED; }
	else			{ m_flags &= ~SPX_AIS_TRACK_FLAGS_DELETED; }
	return(SPX_NO_ERROR);
    }
    int GetCanBeDeleted(void) const	{ return(m_canBeDeleted); }
    int GetIsLost(void) const		{ return(m_isLost); }

    /* Flags describing what has changed (for callbacks etc.). */
    UINT32 GetFlags(void) const		{ return(m_flags); }
    void ClearFlags(void)		{ m_flags = 0; }

    /* Is the track considered hazardous. */
    int IsHazardous(void) const;

    /* Last update time. */
    SPxErrorCode GetLastUpdateTime(SPxTime_t *timestampPtr=NULL,
				    double *ageSecsPtr=NULL) const;

    /* Functions that can be given to the SPxAISDecoder::IterateTracks()
     * function to do certain actions on all tracks.
     */
    static int IteratorActivate(void * /*decoder*/, void * /*arg*/, void *track)
    {
	if(track != NULL) { ((SPxAISTrack *)track)->SetIsActivated(TRUE); }
	return(SPX_NO_ERROR);
    }
    static int IteratorDeactivate(void * /*decoder*/, void * /*arg*/, void *track)
    {
	if(track != NULL) { ((SPxAISTrack *)track)->SetIsActivated(FALSE); }
	return(SPX_NO_ERROR);
    }
    static int IteratorSelect(void * /*decoder*/, void * /*arg*/, void *track)
    {
	if(track != NULL) { ((SPxAISTrack *)track)->SetIsSelected(TRUE); }
	return(SPX_NO_ERROR);
    }
    static int IteratorDeselect(void * /*decoder*/, void * /*arg*/, void *track)
    {
	if(track != NULL) { ((SPxAISTrack *)track)->SetIsSelected(FALSE); }
	return(SPX_NO_ERROR);
    }
    static int IteratorClearHistory(void * /*decoder*/, void * /*arg*/, void *track)
    {
	if(track != NULL) { ((SPxAISTrack *)track)->ClearHistory(); }
	return(SPX_NO_ERROR);
    }
    void GetSrcAddr(UINT32 *addr, int *port)
    {
	if (addr) { *addr = m_srcAddr; }
	if (port) { *port = m_srcPort; }
    }

    /* SPxUniTrack functions. */
    virtual UINT32 GetID(void) const SPX_VIRTUAL_OVERRIDE { return GetMMSI(); }
    virtual UINT32 GetSecondaryID(void) const SPX_VIRTUAL_OVERRIDE { return GetMMSI(); }
    virtual UINT32 GetSecondarySensorType(void) const SPX_VIRTUAL_OVERRIDE { return SPX_PACKET_TRACK_SENSOR_AIS; }
    virtual void GetDefaultTrackName(char *buf, unsigned int bufLen) const SPX_VIRTUAL_OVERRIDE;
    virtual SPxErrorCode SetUseColour(int useColour) SPX_VIRTUAL_OVERRIDE
    {
        m_useColour = useColour;
        return SPX_NO_ERROR;
    }
    virtual int GetUseColour(void) SPX_VIRTUAL_OVERRIDE { return m_useColour; }
    virtual SPxErrorCode SetColour(UINT32 colour) SPX_VIRTUAL_OVERRIDE
    {
        m_colour = colour;
        return SPX_NO_ERROR;
    }
    virtual UINT32 GetColour(void) SPX_VIRTUAL_OVERRIDE { return m_colour; }
    virtual SPxErrorCode SetIsBold(int isBold) SPX_VIRTUAL_OVERRIDE
    {
        m_isBold = isBold;
        return SPX_NO_ERROR;
    }
    virtual int GetIsBold(void) SPX_VIRTUAL_OVERRIDE { return m_isBold; }

protected:
    /*
     * Protected functions, likely to be used by SPxAISDecoder.
     */
    /* Constructor and destructor. */
    SPxAISTrack(SPxAISDecoder *parent, unsigned int numHistoryPts=500);
    virtual ~SPxAISTrack(void);

    /* Value setting functions. */
    SPxErrorCode SetMMSI(UINT32 mmsi);
    SPxErrorCode SetPosition(double latDegs, double longDegs,
				const SPxTime_t *timestamp,
				unsigned char aisSecsOnly = 60);
    SPxErrorCode SetVelocity(double speedMps, double courseDegs,
				const SPxTime_t *timestamp);
    SPxErrorCode SetHeading(double headingDegs,
				const SPxTime_t *timestamp);
    SPxErrorCode SetRateOfTurn(double degsPerMin,
				const SPxTime_t *timestamp);
    SPxErrorCode SetNavStatus(unsigned int navStatus,
				const SPxTime_t *timestamp);
    SPxErrorCode SetSpecManInd(UINT8 specManInd,
			       const SPxTime_t *timestamp);
    SPxErrorCode SetRAIM(int raim,
			 const SPxTime_t *timestamp);
    SPxErrorCode SetClassBFlags(int unitFlag,
				int displayFlag,
				int dscFlag,
				int bandFlag,
				int message22Flag,
				const SPxTime_t *timestamp);
    SPxErrorCode SetAtoNType(int atonType,
			     const SPxTime_t *timestamp);
    SPxErrorCode SetShipStaticData(SPxAISStationType stationType,
				const char *callsign,
				const char *vesselName,
				const char *destination,
				unsigned int metresToBow,
				unsigned int metresToStern,
				unsigned int metresToPort,
				unsigned int metresToStarboard,
				double draughtMetres,
				UINT32 imoNumber,
				unsigned int shipCargoType,
				UINT32 eta,
				const SPxTime_t *timestamp);
    SPxErrorCode SetInlandStaticData(UINT32 eni,
				     double lengthMetres,
				     double beamMetres,
				     UINT32 eri,
				     UINT8 hazardousCargo,
				     double draughtMetres,
				     const SPxTime_t *timestamp);
    SPxErrorCode SetLastUpdateTime(const SPxTime_t *timestamp);

    /* Extrapolation support. */
    SPxErrorCode Extrapolate(const SPxTime_t *timestamp);

    /*
     * Functions relating to the status of the track.
     */
    SPxErrorCode SetIsLost(int isLost)
    {
	m_isLost = (isLost ? TRUE : FALSE);
	if( m_isLost )	{ m_flags |= SPX_AIS_TRACK_FLAGS_LOST; }
	else		{ m_flags &= ~SPX_AIS_TRACK_FLAGS_LOST; }
	return(SPX_NO_ERROR);
    }

    /*
     * Set/retrieve the track's source.
     */
    void SetSrcAddr(UINT32 addr, int port) { m_srcAddr = addr; m_srcPort = port; }

    /*
     * Protected variables.
     */
    /* Linked list pointer, used by parent. */
    SPxAISTrack *m_next;		/* Next in list */

private:
    /*
     * Private variables.
     */
    SPxAISDecoder *m_parentDecoder;	/* Parent decoder */
    UINT32 m_mmsi;			/* Unique number */
    UINT32 m_srcAddr;			/* The sender network address */
    int m_srcPort;			/* The sender network port */

    /* Flags for what's changed recently. */
    UINT32 m_flags;			/* SPX_AIS_TRACK_FLAGS_... */

    /*
     * Dynamic data.
     */
    /* Position. */
    double m_latDegs;			/* Latitude, +ve north */
    double m_longDegs;			/* Longitude, +ve east */
    SPxTime_t m_latLongTime;		/* Time of lat/long */
    unsigned char m_latLongAisSecsOnly;	/* Seconds component of AIS time */

    /* Extrapolated position. */
    double m_extrapolatedLatDegs;	/* Latitude, +ve north */
    double m_extrapolatedLongDegs;	/* Longitude, +ve east */
    SPxTime_t m_extrapolatedLatLongTime; /* Time of lat/long */

    /* Velocity. */
    double m_speedMps;			/* Speed in metres per second */
    double m_courseDegs;		/* Course in degs clockwise */
    SPxTime_t m_velocityTime;		/* Time of velocity */

    /* Heading. */
    double m_headingDegs;		/* Heading in degs clockwise */
    SPxTime_t m_headingTime;		/* Time of heading */

    /* Rate of turn. */
    double m_rotDegsPerMin;		/* Rate in degs/min clockwise */
    SPxTime_t m_rotTime;		/* Time of rate of turn */

    /* Nav status. */
    unsigned int m_navStatus;		/* 0 to 15 */
    SPxTime_t m_navStatusTime;		/* Time of navStatus */

    /* Special manoeuvre indicator. */
    UINT8 m_specManInd;			/* 0 - 2 */
    SPxTime_t m_specManIndTime;		/* Time of specManInd */

    /* RAIM (Receiver autonomous integrity monitoring) flag. */
    int m_raim;
    SPxTime_t m_raimTime;		/* Time of raim */

    /* Class B flags. */
    int m_classBUnit;
    int m_classBDisplay;
    int m_classBDsc;
    int m_classBBand;
    int m_classBMessage22;
    SPxTime_t m_classBFlagsTime;	/* Time of Class B flags. */

    /* Aid to navigation. */
    UINT8 m_atonType;
    SPxTime_t m_atonTime;		/* Time of AtoN */

    /* Own ship. */
    int m_isOwnShip;			/* TRUE if own ship */

    /* Last update time. */
    SPxTime_t m_lastUpdateTime;

    /*
     * Ship static data.
     */
    SPxAISStationType m_stationType;	/* Type of AIS station */
    char m_callsign[8];			/* 7 character string */
    char m_vesselName[35];		/* 20 character string + 14 extra chars for AtoN names */
    char m_destination[21];		/* 20 character string */
    unsigned int m_metresToBow;		/* Dimension, or 0 if not known */
    unsigned int m_metresToStern;	/* Dimension, or 0 if not known */
    unsigned int m_metresToPort;	/* Dimension, or 0 if not known */
    unsigned int m_metresToStarboard;	/* Dimension, or 0 if not known */
    double m_draughtMetres;		/* Draught in metres */
    UINT32 m_imoNumber;			/* IMO number */
    unsigned int m_shipCargoType;	/* Ship and cargo type */
    UINT32 m_eta;			/* ETA, 0xMMDDHHMM */
    SPxTime_t m_staticDataTime;		/* Time of static data */

    /* Inland static data. */
    UINT32 m_eni;			/* Unique European Vessel ID, or 0 if not known */
    double m_lengthMetres;		/* Dimension, or 0.0 if not known */
    double m_beamMetres;		/* Dimension, or 0.0 if not known */
    UINT32 m_eri;			/* ERI vessel type, or 0 if not known */
    UINT8 m_hazardousCargo;		/* Hazardous cargo level, or 0 if not known */
    double m_inlandDraughtMetres;	/* Inland waterways draught in metres */
    SPxTime_t m_inlandStaticDataTime;	/* Time of inland static data */

    /* Rendering control. */
    int m_canBeDeleted;			/* True if can be deleted */
    int m_isLost;			/* True if lost */

    /* Track specific colour. */
    int m_useColour; /* Use track specific colour? */
    UINT32 m_colour;

    /* Render track with increased line width. */
    int m_isBold;

    /* History points. */
    SPxAISTrackHistoryPt_t *m_historyPts;	/* Buffer for points */
    unsigned int m_historyPtsSize;		/* Size of buffer, in points */
    unsigned int m_historyPtsNumUsed;		/* Number of stored points */

    /*
     * Private functions.
     */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode Reset(void) SPX_VIRTUAL_OVERRIDE;

}; /* SPxAISTrack */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_AIS_TRACK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
