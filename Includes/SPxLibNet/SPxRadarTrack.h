/*********************************************************************
*
* (c) Copyright 2011 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxRadarTrack class.
*
* Revision Control:
*   14/06/22 v1.71   REW	Move some utility functions to SPxTrackUtils.
*
* Previous Changes:
*   19/01/22 1.70   BTB	GetFlightPlan() now returns const pointer.
*   09/06/21 1.69   AJH	Add GetHeading().
*   20/05/21 1.68   AGC	Update ASTERIX target type support for Mode 5.
*   23/02/21 1.67   REW	Keep timestamp for previous altitude.
*			Add GetVerticalRate().
*   10/02/21 1.66   AGC	Add GetADSBCategory().
*   25/11/20 1.65   SP 	Add support for trail point altitude.
*			Add GetSecondaryTargetType().
*   30/10/20 1.64   AGC	Return 12-bit info from GetIFFMode1().
*			Return flight level support from GetAltitude().
*			GetAsterixMsg() now overrides base class.
*   29/09/20 1.63   AGC	IsPlot() now overrides base class function.
*   18/09/20 1.62   AGC	Add MMSI/AA/IFF accessor functions from base class.
*   05/08/20 1.61   SP 	Make GetFirstTimestamp() an override.
*   24/03/20 1.60   AGC	Add SPxTrackFormatLabel() function.
*   19/12/19 1.59   BTB	Add Get/SetIsBold().
*   25/06/19 1.58   REW	Add GetIsEmergency() and GetIsSPI().
*   02/05/19 1.57   REW	Avoid compiler warning in last change.
*   02/05/19 1.56   REW	Support base class ground/test/simulated funcs.
*   06/12/18 1.55   REW	Support ageFraction in trail points.
*   30/10/18 1.54   REW	Add GetBestAltitude().
*   27/09/18 1.53   REW	Add IsYoungerThan() utility function.
*   17/09/18 1.52   SP 	Add GetSecondarySensorType().
*   22/08/18 1.51   AGC	Ignore extrapolated updates for previous values.
*   17/07/18 1.50   AGC	Make isDeleted functions consistent with base class.
*   06/07/18 1.49   AGC	Add IsDeleted().
*   02/07/18 1.48   AGC	Support minimum trail dot interval.
*   25/06/18 1.47   AGC	Correctly suppress cppcheck warning.
*   01/05/18 1.46   SP 	GetPrevLatLong() no longer an override.
*   13/04/18 1.45   REW	Add IsPlot().
*   27/03/18 1.44   SP 	Add link to track database.
*   22/03/18 1.43   REW	Make GetLastUpdateTimestamp() const.
*   15/03/18 1.42   SP 	Remove Set/GetTrackName(), now in base class.
*   14/03/18 1.41   AGC	Support optional stats.
*   11/12/17 1.40   AGC	Support storing ASTERIX message.
*   11/12/17 1.39   SP 	Remove GetBestID().
*			Add GetSecondaryID().
*   24/11/17 1.38   SP 	Use SPX_VIRTUAL_OVERRIDE.
*   23/11/17 1.37   SP 	Make some functions virtual.
*   17/11/17 1.36   AGC	Derive from SPxUniTrack.
*   11/09/17 1.35   AGC	Add GetPrevAltitudeMetres().
*   18/07/17 1.34   SP 	Add GetVelocity().
*   31/01/17 1.33   REW	Make some functions const.
*   05/10/16 1.32   REW	Add GetFirstTimestamp().
*   29/09/16 1.31   REW	Add GetRangeBearingFromLatLong().
*   30/11/15 1.30   REW	Add SetTrackName() function.
*   26/11/15 1.29   REW	Add optional duration to highlighting.
*   26/11/15 1.28   REW	Support highlighted tracks.
*   21/09/15 1.27   AGC	Make GetIsSelected() const.
*   15/07/15 1.26   SP 	Keep a copy of previous track update.
*   12/03/15 1.25   REW	Add IsSPI().
*   26/01/15 1.24   REW	Support flight plan links.
*   13/11/14 1.23   REW	Support SetLabelUserField().
*   17/10/14 1.22   REW	Support GetVerticalDirection().
*   02/10/14 1.21   REW	Support label offsets.
*   03/04/14 1.20   REW	Add IsEmergency(). Make some functions const.
*   01/04/14 1.19   REW	Add GetAltitudeMetres().
*   13/12/13 1.18   SP 	Link trail point back to track.
*   04/10/13 1.17   AGC	Simplify headers.
*   14/08/13 1.16   AGC	Support track specific shapes and images.
*   15/07/13 1.15   AGC	Prevent warnings with high VS warning level.
*   24/05/13 1.14   AGC	Add SPxTrackIsSensorMultiple().
*   09/11/12 1.13   SP 	Modify track data pointer comment.
*   30/10/12 1.12   REW	Derive from SPxObj again.
*   26/10/12 1.11   AGC	Add GetMetresFromLatLong() function.
*			Make Update() function public.
*			Do not derive from SPxObj.
*   08/05/12 1.10   SP 	Support measured and filtered lat/long.
*   04/04/12 1.9    SP 	Support user data.
*   29/03/12 1.8    SP 	Rename file as SPxRadarTrack.h.
*                       Rename SPxTrackReport as SPxRadarTrack.
*                       Rename SPxTrackTrailPoint as 
*                       SPxRadarTrackTrailPoint.
*                       Fix version numbers after V1.6.
*   21/12/11 1.7    REW	Add SPxTrackIsSensor...() functions.
*   15/12/11 1.6    REW	Add SPxTrackGetStringForSensorType().
*   25/10/11 1.5    SP 	Add SetDeleted() and Reset().
*   14/10/11 1.4    SP 	Remove reference count functions.
*                       Add Set/GetCanBeDeleted().
*                       Add linked list pointers.
*                       Make some functions protected.
*   27/07/11 1.3    SP 	Rename ClearTrail() to ClearHistory().
*   13/07/11 1.2    SP 	Add GetMetresFromPoint().
*                       Increase default num trails points to 500.
*   07/07/11 1.1    SP 	Initial Version.
**********************************************************************/

#ifndef _SPX_RADAR_TRACK_H
#define _SPX_RADAR_TRACK_H

/*
 * Other headers required.
 */

/* We need SPxSymbol for SPxSymbolShape_t enumeration. */
#include "SPxLibUtils/SPxSymbol.h"

/* We need SPxTime_t structure. */
#include "SPxLibUtils/SPxTime.h"

/* We need SPxPacketTrackExtended structure. */
#include "SPxLibData/SPxPackets.h"

/* We need the base class. */
#include "SPxLibNet/SPxUniTrack.h"

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

/* Forward-declare other classes we use in case headers are in wrong order. */
class SPxRadarTrack;
class SPxTrackDatabase;
class SPxImage;
struct SPxPacketTrackMinimal_tag;
struct SPxPacketTrackNormal_tag;
struct SPxPacketTrackExtended_tag;
class SPxFlightPlan;
class SPxAsterixMsg;
class SPxTrackRendererBase;

/* ASTERIX track colour modes. */
enum SPxRadarTrackSecTargetType_t
{
    SPX_RADAR_TRACK_STT_NONE		= 0,
    SPX_RADAR_TRACK_STT_COAST		= 1,
    SPX_RADAR_TRACK_STT_PSR		= 2,
    SPX_RADAR_TRACK_STT_SSR		= 3,
    SPX_RADAR_TRACK_STT_SSR_COMB	= 4,
    SPX_RADAR_TRACK_STT_ALL_CALL	= 5,
    SPX_RADAR_TRACK_STT_ROLL_CALL	= 6,
    SPX_RADAR_TRACK_STT_ALL_CALL_COMB	= 7,
    SPX_RADAR_TRACK_STT_ROLL_CALL_COMB	= 8,
    SPX_RADAR_TRACK_STT_GHOST		= 9,
    SPX_RADAR_TRACK_STT_TEST		= 10,
    SPX_RADAR_TRACK_STT_SCF		= 11
};

/* Structure used to store a linked list of track tail points */
typedef struct _SPxRadarTrackTrailPoint
{
     /* Previous item in the list or NULL if first entry */
    _SPxRadarTrackTrailPoint *prev;

    /* Next item in the list or NULL if last entry */
    _SPxRadarTrackTrailPoint *next;

    /* Timestamp when first contributing update to this trail point occurred. */
    SPxTime_t firstTimestamp;

    /* Timestamp when this trail point was created */
    SPxTime_t timestamp;

    /* Age as fraction of trail length (updated in IterateTrailPoints()) */
    double ageFraction;		/* 0.0 for newest trail point, 1.0 oldest */

    /* Additional track data preserved for this trail point */
    UINT8 status;              /* Track status (SPX_PACKET_TRACK_STATUS...) */
    UINT8 numCoasts;           /* Number of consecutive coasts */
    SPxRadarTrackSecTargetType_t stt; /* Secondary target type. */

    /* Control flags. */
    int isMeasAvail;           /* TRUE if measured values are available */
    int isLatLongAvail;        /* TRUE if lat/long values are available */
    int isAltAvail;            /* TRUE if altitude is available. */

    /* Filtered range and azimuth (always available) */
    double rangeMetres;        /* Filtered range in metres */
    double azimuthDeg;         /* Filtered azimuth in degrees */

    /* Measured range and azimuth (available if isMeasAvail set) */
    double measRangeMetres;    /* Measured range in metres */
    double measAzimuthDeg;     /* Measured azimuth in degrees */

    /* Filtered lat/long (available if isLatLongAvail set) */
    double latDegs;            /* Filtered latitude in degrees. */
    double longDegs;           /* Filtered Longitude in degrees. */

    /* Measured lat/long (available if isLatLongAvail & isLatLongAvail set) */
    double measLatDegs;        /* Measured latitude in degrees. */
    double measLongDegs;       /* Measured longitude in degrees. */

    /* Altitude (available if isAltitudeAvail set) */
    double altMetres;          /* Best available altitude. */
    int isAltFromFL;           /* Is altitude originally from a flight level. */

    SPxRadarTrack *track;      /* Link back to the radar track. */

} SPxRadarTrackTrailPoint_t;

/*
 * Define our class.
 */
class SPxRadarTrack : public SPxUniTrack
{

/* The database class can access our internals directly. */
friend class SPxTrackDatabase;

public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Note that the constructor and destructor are not public, so the
     * class can only be instantiated by the SPxTrackDatabase.
     */

    /* Parent database. */
    SPxTrackDatabase *GetDatabase(void) const { return m_database; }

    /* Extract current track data */
    const SPxPacketTrackMinimal *GetMinRpt(void) const { return m_pMinRpt; }
    const SPxPacketTrackNormal *GetNormRpt(void) const { return m_pNormRpt; } 
    const SPxPacketTrackExtended *GetExtRpt(void) const { return m_pExtRpt; }
    virtual const SPxTime_t *GetTimestamp(void) const SPX_VIRTUAL_OVERRIDE { return &m_timestamp; }
    virtual const SPxTime_t *GetFirstTimestamp(void) const SPX_VIRTUAL_OVERRIDE { return &m_firstTimestamp;}
    virtual UINT32 GetID(void) const SPX_VIRTUAL_OVERRIDE { return (m_pMinRpt ? m_pMinRpt->id : 0); }
    virtual UINT32 GetSecondaryID(void) const SPX_VIRTUAL_OVERRIDE;
    virtual UINT32 GetSecondarySensorType(void) const SPX_VIRTUAL_OVERRIDE;
    virtual SPxErrorCode GetMMSI(UINT32 *mmsi) const SPX_VIRTUAL_OVERRIDE;
    virtual SPxErrorCode GetAA(UINT32 *aa) const SPX_VIRTUAL_OVERRIDE;
    virtual SPxErrorCode GetIFFMode1(UINT16 *iffMode1Rtn, int *is12BitRtn=NULL) const SPX_VIRTUAL_OVERRIDE;
    virtual SPxErrorCode GetIFFMode2(UINT16 *iffMode2Rtn) const SPX_VIRTUAL_OVERRIDE;
    virtual SPxErrorCode GetIFFMode3A(UINT16 *iffMode3ARtn) const SPX_VIRTUAL_OVERRIDE;
    virtual void GetDefaultTrackName(char *buf, unsigned int bufLen) const SPX_VIRTUAL_OVERRIDE;
    virtual int GetIsDeleted(void) const SPX_VIRTUAL_OVERRIDE;
    double GetRangeMetres(int filteredPos=TRUE) const;
    double GetAzimuthDeg(int filteredPos=TRUE) const;
    virtual SPxErrorCode GetLatLong(double *latDegsRtn,
				    double *longDegsRtn,
				    int filteredPos = TRUE,
				    double *ageSecsRtn = NULL) const SPX_VIRTUAL_OVERRIDE;
    virtual SPxErrorCode GetVelocity(double *speedMpsRtn, 
				     double *courseDegsRtn) const SPX_VIRTUAL_OVERRIDE;
    virtual SPxErrorCode GetHeading(double *headingDegsRtn) const SPX_VIRTUAL_OVERRIDE;
    double GetMetresFromPoint(double xMetres, 
                              double yMetres,
                              int filteredPos=TRUE) const;
    double GetMetresFromLatLong(double latDegs,
				double longDegs,
				int filteredPos=TRUE,
				unsigned int radarIndex=0,
				double labelMetresPerPixel=0.0) const;
    SPxErrorCode GetRangeBearingFromLatLong(double latDegs,
				double longDegs,
				double *rangeMetresPtr,
				double *bearingDegsPtr,
				int filteredPos=TRUE,
				unsigned int radarIndex=0,
				double labelMetresPerPixel=0.0) const;
    SPxErrorCode GetAltitudeMetres(double *altMetresPtr,
				   int allowExtrapolated=TRUE,
				   int *isAltFromFLRtn=NULL) const;
    virtual SPxErrorCode GetAltitude(double *altMetresRtn,
				     double *ageSecsRtn=NULL,
				     int *isAltFromFLRtn=NULL) const SPX_VIRTUAL_OVERRIDE;
    virtual SPxErrorCode GetVerticalRate(double *verticalRateMpsPtr) const SPX_VIRTUAL_OVERRIDE;
    SPxErrorCode GetVerticalDirection(int *verticalDirPtr) const;
    virtual SPxErrorCode GetADSBCategory(unsigned int *categoryRtn) const SPX_VIRTUAL_OVERRIDE;
    int IsEmergency(void) const;
    int IsSPI(void) const;
    virtual int IsPlot(void) const SPX_VIRTUAL_OVERRIDE;
    int IsYoungerThan(int maxAge) const;

    /* Track configuration */
    SPxErrorCode SetUseShape(int useShape)
    {
	m_useShape = useShape;
	return SPX_NO_ERROR;
    }
    int GetUseShape(void) { return m_useShape; }
    SPxErrorCode SetShape(SPxSymbolShape_t shape, SPxImage *image=NULL)
    {
	m_shape = shape;
	m_image = image;
	return SPX_NO_ERROR;
    }
    SPxSymbolShape_t GetShape(void) { return m_shape; }
    SPxImage *GetImage(void) { return m_image; }
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
    void ClearHistory(void);

    /* Offset from track position for label display. */
    SPxErrorCode SetLabelOffsetPixels(int x, int y)
    {
	m_labelOffsetXPixels = x;
	m_labelOffsetYPixels = y;
	return(SPX_NO_ERROR);
    }
    int GetLabelOffsetXPixels(void) const { return(m_labelOffsetXPixels); }
    int GetLabelOffsetYPixels(void) const { return(m_labelOffsetYPixels); }

    /* String added to label by SPxTrackRenderer if %z included in format. */
    SPxErrorCode SetLabelUserField(const char *str);
    const char *GetLabelUserField(void) const;	/* NB: MAY BE NULL */

    /* Read trail points from a track report */
    SPxErrorCode IterateTrailPoints(unsigned int trailLenSecs,
                                    SPxCallbackListFn_t trailFn,
                                    void *userArg);

    /* Set/get flight plan for this track (used by the track database). */
    SPxErrorCode SetFlightPlan(SPxFlightPlan *fp)
    {
	m_flightPlan = fp;
	return(SPX_NO_ERROR);
    }
    const SPxFlightPlan *GetFlightPlan(void) const { return(m_flightPlan); }

    /* Get/clear statistics. */
    double GetDetectionPercent(void) const { return(m_detectPercent); }
    double GetMeanAziDegs(void) const { return(m_meanAziDegs); }
    double GetMeanRangeMetres(void) const { return(m_meanRangeMetres); }
    double GetStdDevAziDegs(void) const { return(m_stdDevAziDegs); }
    double GetStdDevRangeMetres(void) const { return(m_stdDevRangeMetres); }
    void ClearStats(void);

    /* Get associated ASTERIX message (may be NULL). */
    virtual const SPxAsterixMsg *GetAsterixMsg(void) const SPX_VIRTUAL_OVERRIDE;

    /* Wrappers around the above to match the base class interface. */
    virtual int GetOnGround(void) const SPX_VIRTUAL_OVERRIDE;
    virtual int GetIsTest(void) const SPX_VIRTUAL_OVERRIDE;
    virtual int GetIsSimulated(void) const SPX_VIRTUAL_OVERRIDE;
    virtual int GetIsEmergency(void) const SPX_VIRTUAL_OVERRIDE;
    virtual int GetIsSPI(void) const SPX_VIRTUAL_OVERRIDE;

    /* Secondary target type. */
    SPxRadarTrackSecTargetType_t GetSecondaryTargetType(void);

    /*
     * Public static functions.
     */
    static double GetTrailRangeMetres(const SPxRadarTrackTrailPoint_t *trailPt,
                                      int filteredPos=TRUE);

    static double GetTrailAzimuthDeg(const SPxRadarTrackTrailPoint_t *trailPt,
                                     int filteredPos=TRUE);

    static SPxErrorCode GetTrailLatLong(
        const SPxRadarTrackTrailPoint_t *trailPt, 
        int filteredPos,
        double *latDegsRtn,
        double *longDegsRtn);

    static SPxErrorCode GetTrailAltitudeMetres(const SPxRadarTrackTrailPoint_t *trailPt,
                                               double *altMetresRtn,
                                               int *isAltFromFLRtn=NULL);

    static SPxErrorCode GetBestAltitude(double *altMetresPtr,
					const SPxPacketTrackExtended *pExtRpt,
					SPxHeightMode_t mode,
					int *isAltFromFLRtn=NULL);

    /* Functions that can be given to the SPxTrackDatabase::IterateTracks()
     * function to do certain actions on all tracks.
     */
    static int IteratorSelect(void * /*database*/, void * /*arg*/, void *track)
    {
	if (track != NULL) { ((SPxRadarTrack *)track)->SetIsSelected(TRUE); }
	return(SPX_NO_ERROR);
    }
    static int IteratorDeselect(void * /*database*/, void * /*arg*/, void *track)
    {
	if (track != NULL) { ((SPxRadarTrack *)track)->SetIsSelected(FALSE); }
	return(SPX_NO_ERROR);
    }
    static int IteratorClearHistory(void * /*database*/, void * /*arg*/, void *track)
    {
	if(track != NULL) { ((SPxRadarTrack *)track)->ClearHistory(); }
	return(SPX_NO_ERROR);
    }

    /* Update the track report */
    SPxErrorCode Update(const SPxTime_t *timestamp,
                        SPxPacketTrackMinimal *pMinRpt,
                        SPxPacketTrackNormal *pNormRpt,
                        SPxPacketTrackExtended *pExtRpt,
                        const SPxAsterixMsg *pAsterixMsg=NULL);

    /* Extract previous track data */
    const SPxPacketTrackMinimal *GetPrevMinRpt(void) const { return m_pPrevMinRpt; }
    const SPxPacketTrackNormal *GetPrevNormRpt(void) const { return m_pPrevNormRpt; } 
    const SPxPacketTrackExtended *GetPrevExtRpt(void) const { return m_pPrevExtRpt; }
    virtual SPxErrorCode GetPrevLatLong(double *latDegsRtn,
                                        double *longDegsRtn,
                                        int filteredPos = TRUE) const;
    SPxErrorCode GetPrevAltitudeMetres(double *altMetresPtr) const;

protected:

    /*
     * Protected variables.
     */

    /* Object linked list pointers. */
    SPxRadarTrack *m_next;
    SPxRadarTrack *m_prev;

    /*
     * Protected functions.
     */

    /* Constructor and destructor. */
    explicit SPxRadarTrack(unsigned int maxNumTrailPts=500,
			   int enableStats=FALSE,
			   double minTrailIntervalSecs=0.0);
    virtual ~SPxRadarTrack(void);

    /* Track database. */
    void SetDatabase(SPxTrackDatabase *db) { m_database = db; }

    /* Mark track as deleted */
    virtual void setIsDeleted(int isDeleted) SPX_VIRTUAL_OVERRIDE;

    /* Reset track internals */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode Reset(void) SPX_VIRTUAL_OVERRIDE;

    /* Get last update timestamp */
    const SPxTime_t *GetLastUpdateTimestamp(void) const {return &m_timestamp;}

private:

    /*
     * Private variables.
     */

    /* Link back to the track database. */
    SPxTrackDatabase *m_database;

    /* Timestamp when track was updated */
    SPxTime_t m_timestamp;

    /* Timestamp when track was first received*/
    SPxTime_t m_firstTimestamp;

    /* Track information (may be extrapolated).
     * Do not use these pointer internally for
     * accessing track data as the m_pNormRpt and m_pExtRpt pointers 
     * may be set to NULL when Update() in called. Instead use the 
     * public access functions, GetMinRpt(), GetNormRpt() and
     * GetExtRpt().
     */
    SPxPacketTrackMinimal *m_pMinRpt;
    SPxPacketTrackNormal *m_pNormRpt;
    SPxPacketTrackExtended *m_pExtRpt;

    /* Private track data referenced by above pointers. */
    SPxPacketTrackExtended _m_data;

    /* Previous track information (may be extrapolated). */
    SPxPacketTrackMinimal *m_pPrevMinRpt;
    SPxPacketTrackNormal *m_pPrevNormRpt;
    SPxPacketTrackExtended *m_pPrevExtRpt;

    /* Private track data for previous track update. */
    SPxPacketTrackExtended _m_prev_data;

    /* Track information (real, not extrapolated). */
    SPxPacketTrackMinimal *m_pRealMinRpt;
    SPxPacketTrackNormal *m_pRealNormRpt;
    SPxPacketTrackExtended *m_pRealExtRpt;

    /* Private track data for real track update. */
    SPxPacketTrackExtended _m_real_data;

    /* Copy of values from previous updates. */
    int m_isPrevAltitudeSet;
    double m_prevAltitudeMetres;
    SPxTime_t m_prevAltitudeTimestamp;

    /* Max number of trail points to keep */
    unsigned int m_maxNumTrailPts;

    /* Minimum interval between trail points. */
    double m_minTrailIntervalSecs;

    /* Trail history points stored in a linked list */
    SPxRadarTrackTrailPoint_t *m_trailListHead;
    SPxRadarTrackTrailPoint_t *m_trailListTail;

    /* Number of elements in the trail point linked-list */
    unsigned int m_numTrailPts;

    /* Mutex used to protect trail list */
    SPxCriticalSection m_trailListAccess;

    /* Track specific shape. */
    int m_useShape; /* Use track specific shape? */
    SPxSymbolShape_t m_shape;
    SPxImage *m_image;

    /* Track specific colour. */
    int m_useColour; /* Use track specific colour? */
    UINT32 m_colour;

    /* Render track with increased line width. */
    int m_isBold;

    /* Track specific label offset. */
    int m_labelOffsetXPixels;
    int m_labelOffsetYPixels;

    /* Track specific label field (used by %z in renderer). */
    char *m_labelUserField;
    unsigned int m_labelUserFieldAllocatedSize;

    /* Associated flight plan, if any. */
    SPxFlightPlan *m_flightPlan;

    /* Asterix track message, if any. */
    int m_asterixMsgValid;
    SPxAutoPtr<SPxAsterixMsg> m_asterixMsg;

    /* Simple statistics (primarily for static targets). */
    int m_statsEnabled;
    unsigned int m_numUpdates;
    unsigned int m_numCoasts;
    double m_detectPercent;
    double m_meanSinAziDegs;
    double m_meanCosAziDegs;
    double m_meanAziDegs;
    double m_meanRangeMetres;
    double m_sumDiffAziDegs;
    double m_sumDiffRangeMetres;
    double m_stdDevAziDegs;
    double m_stdDevRangeMetres;

    /*
     * Private functions.
     */

    void UpdateTrail(void);
    SPxErrorCode GetLatLongInternal(const SPxPacketTrackMinimal *pMinRpt,
                                    const SPxPacketTrackNormal *pNormRpt,
                                    const SPxPacketTrackExtended *pExtRpt,
                                    double *latDegsRtn,
                                    double *longDegsRtn,
                                    int filteredPos,
                                    double *ageSecsRtn) const;
    
}; /* SPxRadarTrack */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/* Utility functions. */
extern SPxErrorCode SPxTrackFormatLabel(char *buf, int bufLen,
                                        const SPxRadarTrack *track,
                                        const char *formatString,
                                        SPxFormatLatLong_t latLongFormat = SPX_FORMAT_LL_DEGS,
                                        SPxUnitsDist_t unitsDist = SPX_UNITS_DIST_METRES,
                                        SPxUnitsHeight_t unitsHeight = SPX_UNITS_HGT_AUTO,
                                        SPxUnitsSpeed_t unitsSpeed = SPX_UNITS_SPEED_MPS,
                                        double refLatDegs = 0.0,
                                        double refLongDegs = 0.0);

#endif /* SPX_RADAR_TRACK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
