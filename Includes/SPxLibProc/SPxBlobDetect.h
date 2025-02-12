/*********************************************************************
*
* (c) Copyright 2007 - 2019, 2021 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for the blob-detector base class.
*
* Revision Control:
*   29/09/22 v1.39   AGC	Increase size of sensor specific data field.
*				Support specifying sensor specific data length.
*				Support pass through of custom sensor specific data.
*
* Previous Changes:
*   02/03/21 1.38   REW	Add more SPX_BLOB_FLAG fields (test/sim/nogo).
*   09/02/21 1.37   REW	Include doppler stats in blob structure.
*   29/01/21 1.36   REW	Support link with doppler store.
*   25/06/19 1.35   AGC	Add blob<->plot conversion functions.
*   20/06/19 1.34   AGC	Add height to blobs.
*   12/04/18 1.33   REW	Support sector callbacks.
*   22/01/18 1.32   REW	Fix typo in field name in last change.
*   19/01/18 1.31   REW	Add iffExtFlags etc.
*   27/09/17 1.30   REW	Don't use repeated azis to confirm IFF codes.
*   06/09/17 1.29   REW	Don't simply use last IFF code, keep counts.
*			Add iffConfirmedFlags to structure.
*   24/03/16 1.28   REW	Use SPX_PACKET_TRACK_SECONDARY_IFF_FLAGS_...
*			definitions, not SPX_SPANDETECT_IFF_FLAGS_....
*   11/03/16 1.27   REW	Add iff12BitMode1 field to
*			SPxBlobIFFSensorSpecificData_t;
*   09/12/15 1.26   REW	Add SPX_BLOB_SENSORDATA_ID_EXTEND.
*			and SPX_BLOB_SENSORDATA_EXT_P313.
*   17/11/15 1.25   REW	Change name of flag in last change.
*   16/11/15 1.24   REW	Add flags to SPxBlob_t.
*   29/07/14 1.23   REW	Support IFF extraction.
*   13/12/13 1.22   AGC	Report errors if world map inhibition requested,
*			but not possible.
*   04/10/13 1.21   AGC	Simplify headers.
*   23/07/13 1.20   REW	Support antiClockwise flags.
*   11/01/13 1.19   AGC	Support a radar index.
*   08/11/12 1.18   REW	Support limits on plot counts per range bin.
*   10/11/11 1.17   REW	Support sensorSpecificData from PIM.
*   15/11/10 1.16   REW	Add metresFromShoreline as enhanced field.
*   05/11/10 1.15   REW	Add world map support.
*   15/09/10 1.14   REW	Make destructor virtual.
*   31/08/10 1.13   REW	Add density and orientation to SPxBlob_t.
*   12/03/10 1.12   REW	Add sensorSpecificData to SPxBlob_t.
*   16/12/09 1.11   REW	Support StateChanged() from base class.
*   25/06/09 1.10   REW	Support plot report callback functions.
*   05/05/09 1.9    REW	Throttle fragment errors.
*   01/05/09 1.8    REW	Add fragment information for LMM process..
*   10/11/08 1.7    REW	Add radialSpeedMps to SPxBlob_t.
*   03/06/08 1.6    REW	Add numComponents to SPxBlob_t.
*   16/05/08 1.5    REW	Add UpdateConvenienceFields()
*   09/01/08 1.4    REW	Add sizeAzimuthMetres to blob reports.
*   02/01/08 1.3    REW	Add cartesian position to blobs.  Timestamps.
*   10/10/07 1.2    REW	Add aziWidthDegs to ReturnStarted().
*   09/10/07 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_BLOBDETECT_H
#define _SPX_BLOBDETECT_H

/*
 * Other headers required.
 */
/* For SPxCallbackListFn_t. */
#include "SPxLibUtils/SPxCallbackList.h"

/* For SPxTime_t. */
#include "SPxLibUtils/SPxTime.h"

/* We need our base class header. */
#include "SPxLibProc/SPxProcSpanDetect.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Bit-fields for "quality" flags in SPxBlob_t and SPxPacketPlot. */
#define	SPX_BLOB_FLAG_NONQUAL		0x01	/* Doesn't meet size params */
#define	SPX_BLOB_FLAG_TEST		0x02	/* This is a test target */
#define	SPX_BLOB_FLAG_SIMULATED		0x04	/* This is a simulated target */
#define	SPX_BLOB_FLAG_NOGO		0x08	/* Sources says nogo */

/* Bit-fields for enhancedFlags in SPxBlob_t.  Bit set if field is valid. */
#define	SPX_BLOB_ENHANCED_RADIALSPEED		0x00000001
#define	SPX_BLOB_ENHANCED_SENSORSPECIFIC	0x00000002
#define	SPX_BLOB_ENHANCED_SHORELINE		0x00000004
#define	SPX_BLOB_ENHANCED_IFF			0x00000008
#define	SPX_BLOB_ENHANCED_HEIGHT		0x00000010
#define	SPX_BLOB_ENHANCED_DOPPLER		0x00000020

/* Size of array in SPxBlob_t for sensorSpecificData. */
#define	SPX_BLOB_SENSORDATA_SIZE_LEGACY  32
#define	SPX_BLOB_SENSORDATA_SIZE	255

/* Possible values for id field (first byte) in sensorSpecificData. 
 * The EXTEND value means that the ID doesn't fit in the first byte and
 * is therefore in the 3rd/4th byte as a 16-bit number instead.
 */
#define	SPX_BLOB_SENSORDATA_ID_IFF	31		/*  31 = 0x1F */
#define	SPX_BLOB_SENSORDATA_ID_RAW	110		/* 110 = 0x6E */
#define	SPX_BLOB_SENSORDATA_ID_P126	126		/* 126 = 0x7E */
#define	SPX_BLOB_SENSORDATA_ID_P127	127		/* 127 = 0x7F */
#define	SPX_BLOB_SENSORDATA_ID_EXTEND	255		/* 255 = 0xFF */

/* Values for 3rd/4th byte if id is SPX_BLOB_SENSORDATA_ID_EXTEND. */
#define	SPX_BLOB_SENSORDATA_EXT_P313	313		/* 313 = 0x0139 */

/* Number of range bins for limits. */
#define	SPX_BLOB_NUM_RANGE_BINS		8

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

/* Forward-declare the classes we use. */
class SPxCallbackList;
class SPxDopplerStore;
class SPxWorldMap;
class SPxPIM;
class SPxTargetDB;
struct SPxReturnHeader_tag;
struct SPxSpan_tag;
struct SPxPacketPlot_tag;
struct SPxTime_tag;

/*
 * Public type for IFF information stored in sensorSpecificData blob field.
 *
 * Although care is taken to align the fields in this structure, note that
 * the struture itself may start in unaligned locations within other message
 * buffers (e.g. when read via a network stream), so take care.
 *
 * Unknown/unavailable fields should be set to 0.
 *
 * Limit this to SPX_BLOB_SENSORDATA_SIZE bytes (32 at the time of writing).
 */
typedef struct SPxBlobIFFSensorSpecificData_tag {
    /* Word 0. */
    UINT8 id;				/* SPX_BLOB_SENSORDATA_ID_IFF */
    UINT8 reserved0a;			/* Reserved */
    UINT16 reserved0b;			/* Reserved */

    /* Word 1. */
    UINT8 iffFlags;			/* SPX_PACKET_TRACK_SECONDARY_IFF... */
    UINT8 iffMode1;			/* 6-bit Mode 1 code, 0 if not known */
    UINT16 iffMode2;			/* Mode 2 code, or 0 if not known */

    /* Word 2. */
    UINT16 iffMode3A;			/* Mode 3/A code, or 0 if not known */
    UINT16 iffModeC;			/* Mode C code, or 0 if not known */

    /* Word 3. */
    UINT16 iff12BitMode1;		/* 12-bit Mode 1 code, 0 if not known */
    UINT8 iffConfirmedFlags;		/* Similar to iffFlags */
    UINT8 iffConfirmedExtFlags;		/* Similar to iffExtFlags */

    /* Word 4. */
    UINT8 iffExtFlags;			/* SPX_PACKET_TRACK_SECONDARY_IFF_EXT.*/
    UINT8 reserved4a;
    UINT16 reserved4b;

    /* Words 5 to 7. */
    UINT32 reserved5;
    UINT32 reserved6;
    UINT32 reserved7;
} SPxBlobIFFSensorSpecificData_t;

typedef struct SPxBlobRawSensorSpecificData_tag {
    UINT8 id;				/* SPX_BLOB_SENSORDATA_ID_RAW */
    UINT8 lenBytes;			/* Length of raw sensor specific data
					 * following this structure.
					 */

} SPxBlobRawSensorSpecificData_t;


/*
 * Public type for blob fragment information.
 */
typedef struct SPxBlobFragment_tag {
    /* Linked list support */
    struct SPxBlobFragment_tag *next;	/* Next in list, or NULL */

    /* Description of this fragment. */
    UINT32 timestamp;			/* Millisecond timestamp */
    UINT16 azi16Bit;			/* Azimuth index, 0 to 0xFFFF */
    UINT16 rngFirstSample;		/* First sample index */
    UINT16 rngLastSample;		/* Last sample index */
    UINT16 reserved;
} SPxBlobFragment_t;

/*
 * Public type for doppler information.  NB: Subject to change!
 */
typedef struct SPxBlobDopplerData_tag {
    REAL32 speedMean;		/* Average speed for blob, +ve inwards */
    REAL32 speedVariance;	/* Variance of speeds */
} SPxBlobDopplerData_t;


/*
 * Public type for blob information (not the same as that used internally,
 * and not the same as that used on the network, which is SPxPacketPlot).
 */
typedef struct SPxBlob_tag {
    /* Position, polar and cartesian. */
    REAL32 rangeMetres;
    REAL32 azimuthDegrees;
    REAL32 xMetres;
    REAL32 yMetres;

    /* Range/azimuth bounding box. */
    REAL32 startRangeMetres;
    REAL32 endRangeMetres;
    REAL32 startAzimuthDegrees;
    REAL32 endAzimuthDegrees;

    /* Convenience values. */
    REAL32 sizeRangeMetres;
    REAL32 sizeAzimuthDegrees;
    REAL32 sizeAzimuthMetres;

    /* Quality information. */
    UINT32 weight;		/* Number of samples used in blob. */
    UINT32 strength;		/* Sum of values of samples used in blob. */
    REAL32 density;		/* Percentage of bounding box filled. */

    /* Shape-related bounding box. */
    REAL32 orientation;		/* Degrees from north blob is angled at. */
    REAL32 lengthMetres;	/* Approximate length along orientation axis */
    REAL32 widthMetres;		/* Approximate width across orientation axis */

    /* Enhanced information if available from sensor. */
    UINT32 enhancedFlags;	/* See SPX_BLOB_ENHANCED_... flags. */
    REAL32 radialSpeedMps;	/* Positive towards sensor, negative away */
    REAL32 heightMetres;	/* Height of blob position, in metres */
    UINT8 sensorSpecificLen;
    UINT8 sensorSpecificData[SPX_BLOB_SENSORDATA_SIZE];	/* Sensor-specific */
    REAL32 metresFromShoreline;	/* Positive out to sea, negative inland */

    /* Component information. */
    UINT32 numComponents;	/* How many small blobs make this up? */

    /* Fragment/shape information. */
    SPxBlobFragment_t *fragmentList;	/* List of fragments, or NULL */

    /* Time information. */
    SPxTime_t epochTime;

    /* Fields for client use. */
    UINT32 usedCount;		/* E.g. number of tracks using it */

    /* Anti-clockwise fields. */
    UINT8 antiClockwise;	/* Flag to say radar is anti-clockwise */
    UINT8 antiClockwiseCorrected; /* Flag to say correction has been made */

    /* Flags and reserved field. */
    UINT8 flags;		/* Bitmask of SPX_BLOB_FLAG_... flags */
    UINT8 reserved;		/* For padding */

    /* Doppler information, if enhancedFlags & SPX_BLOB_ENHANCED_DOPPLER. */
    SPxBlobDopplerData_t doppler;	/* Doppler stats if available */
} SPxBlob_t;

/*
 * Type for reporting sector info.
 */
typedef struct SPxBlobSector_tag {
    UINT32 sectorNumber;	/* 0 to num sectors */
    UINT16 azi16Bit;		/* Azimuth index, 0 to 0xFFFF */
    UINT8 isIFF;		/* Flag */
    UINT8 reserved;
    UINT32 totalBlobs;		/* Total across all sectors */
} SPxBlobSector_t;


/*
 * Blob-detector class, derived from a span detector.
 */
class SPxBlobDetector :public SPxProSpanDetector
{
public:
    /*
     * Public functions.
     */
    /* Constructor/destructor. */
    SPxBlobDetector(SPxTargetDB *targetDB);
    virtual ~SPxBlobDetector();

    /* Override the base class functions we want. */
    void ReturnStarted(unsigned int azi16Bit,
				SPxReturnHeader_tag *header,
				SPxRunProcess *rp,
				double aziWidthDegs);
    void NewSpan(const SPxSpan_tag *span);
    void ReturnFinished(unsigned int azi16Bit);
    void StateChanged(int nowEnabled);

    /* Callback support.  The first ones are for blobs, then for sectors. */
    SPxErrorCode AddCallback(SPxCallbackListFn_t fn, void *userObj)
    {
	return( (SPxErrorCode)m_callbackList->AddCallback(fn, userObj) );
    }
    SPxErrorCode RemoveCallback(SPxCallbackListFn_t fn, void *userObj)
    {
	return( (SPxErrorCode)m_callbackList->RemoveCallback(fn, userObj) );
    }
    SPxErrorCode AddSectorCallback(SPxCallbackListFn_t fn, void *userObj)
    {
	return( (SPxErrorCode)m_sectorCallbackList->AddCallback(fn, userObj) );
    }
    SPxErrorCode RemoveSectorCallback(SPxCallbackListFn_t fn, void *userObj)
    {
	return( (SPxErrorCode)m_sectorCallbackList->RemoveCallback(fn,userObj));
    }

    /* Access functions for blob fragments. */
    SPxBlobFragment_t *GetBlobFragment(void);
    int ReleaseBlobFragment(SPxBlobFragment_t *fragment)
    {
	/* Add it to the start of our free list. */
	if( !fragment )
	{
	    SPxError(SPX_ERR_USER, SPX_ERR_BAD_ARGUMENT, 0, 0,
			"SPxBlobDetector::ReleaseBlobFragment", "NULL ptr");
	    return(SPX_ERR_BAD_ARGUMENT);
	}
	fragment->next = m_firstFreeFragment;
	m_firstFreeFragment = fragment;
	return(SPX_NO_ERROR);
    }

    /* Access for other info. */
    unsigned int GetCurrentSector(void)	const	{ return(m_currentSector); }

    /* World map linkage. */
    void SetWorldMap(SPxWorldMap *worldMap, unsigned int radarIndex=0);

    /* Doppler store linkage. */
    void SetDopplerStore(SPxDopplerStore *dopplerStore);

    /* Static convenience functions. */
    static void UpdateConvenienceFields(SPxBlob_t *blob,
				int updateCartesianPos = TRUE,
				int updateSize = TRUE);

private:
    /*
     * Private functions.
     */
    struct SPxBlobInternal_tag *newBlobFromSpan(const SPxSpan_t *span);
    void addSpanToBlob(SPxBlobInternal_tag *blob,
				const SPxSpan_t *span);
    void addBlobToBlob(SPxBlobInternal_tag *origBlob,
				const SPxBlobInternal_tag *blobToAdd);

    /* IFF handling. */
    SPxErrorCode initIFFInfo(struct SPxBlobIFFInternal_tag *iff);
    SPxErrorCode updateIFFInfo(struct SPxBlobIFFInternal_tag *iff, UINT16 code);

    /*
     * Private variables.
     */
    /* Debug */
    UINT32 m_debug;

    /* Current return header and run-process etc. */
    SPxReturnHeader_tag *m_currentHeader;
    int m_currentAziIsRepeat;
    SPxRunProcess *m_currentRP;
    SPxPIM *m_currentPIM;
    double m_aziWidthDegs;
    int m_numSamples;		/* Num samples in PIM. */

    /* List anchors */
    SPxBlobInternal_tag *m_firstFreeBlob; /* Start of free list */
    SPxBlobInternal_tag *m_firstOpenBlob; /* Start of open list */
    SPxBlobInternal_tag *m_nextOpenBlob;	/* In-progress search point */
    SPxBlobInternal_tag *m_prevOpenBlob;	/* In-progress search point */

    /* Fragment structures. */
    int m_keepFragmentInfo;			/* Flag to say keep shape */
    SPxBlobFragment_t *m_firstFreeFragment;	/* Start of free list */
    unsigned int m_numFragmentsAllocated;	/* Number ever allocated */
    UINT32 m_timeOfLastFragmentError;		/* Timestamp */

    /* Target database to feed. */
    SPxTargetDB *m_targetDB;		/* Target database to fill. */
    unsigned int m_numSectors;		/* Number of sectors in DB. */
    unsigned int m_currentSector;	/* Sector we are currently filling. */

    /* Range bins for plot limiting. */
    unsigned int m_rangeBinCounts[SPX_BLOB_NUM_RANGE_BINS];

    /* World map to use. */
    SPxWorldMap *m_worldMap;		/* World map, if any */
    unsigned int m_radarIndex;		/* Radar index to use. */
    UINT32 m_lastInhibitErrTime;	/* Time of last world map inhibition error. */

    /* Doppler support. */
    SPxDopplerStore *m_dopplerStore;	/* Link to doppler data */

    /* Callback support. */
    SPxCallbackList *m_callbackList;		/* List of plot callbacks */
    SPxCallbackList *m_sectorCallbackList;	/* List of sector callbacks */
};

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

SPxErrorCode SPxConvertPlotToBlob(const SPxPacketPlot_tag *plot,
				  const SPxTime_tag *timestamp,
				  SPxBlob_t *blob,
				  const UINT8 *sensorSpecificData=NULL,
				  UINT8 sensorSpecificDataLen=0);
SPxErrorCode SPxConvertBlobToPlot(const SPxBlob_t *blob,
				  UINT16 senderID,
				  SPxPacketPlot_tag *plot);

#endif /* _SPX_BLOBDETECT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
