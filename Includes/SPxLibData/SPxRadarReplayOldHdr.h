/*********************************************************************
*
* (c) Copyright 2007, 2013, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxRadarReplayOldHdr.h,v $
* ID: $Id: SPxRadarReplayOldHdr.h,v 1.7 2013/10/04 15:31:08 rew Exp $
*
* Purpose:
*   Header for Simple Radar Replay.
*
* Revision Control:
*   04/10/13 v1.7    AGC	Simplify headers.
*
* Previous Changes:
*   25/04/13 1.6    AGC	Use new SPxCompress class.
*   11/12/07 1.5    REW	Renamed SPxRadarReplayOldHdr.
*   01/08/07 1.4    REW	Move Raw derived class to separate file.
*			Add support for encoded files.
*   16/07/07 1.3    REW	Add note about MAGIC1 being 0x53505831.
*   16/07/07 1.2    REW	Add SPxRadarReplayHeaderV1_t. Remove unused
*			m_setupComplete, m_writeFailed, m_endCode.
*			Create per-object line buffer etc.
*   17/06/07 1.1    DGJ	Initial Version
**********************************************************************/

#ifndef _SPX_RADARREPLAYOLDHDR_H
#define _SPX_RADARREPLAYOLDHDR_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types, events, errors etc. */
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxError.h"

/* We need the base class header. */
#include "SPxLibData/SPxRadarSource.h"

/* We also need SPxRIB definitions. */
#include "SPxLibData/SPxRib.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*
 * Magic numbers for file replay.  "SPX1" is 0x53505831 and is used in
 * the main file header.  "SPX2" is 0x53505832 and can be (but doesn't
 * have to be) used as the endOfDataMarker.
 */
#define	SPX_RADAR_REPLAY_MAGIC1	(('S'<<24) | ('P'<<16) | ('X'<<8) | '1')
#define	SPX_RADAR_REPLAY_MAGIC2	(('S'<<24) | ('P'<<16) | ('X'<<8) | '2')

/*
 * Maximum number of range samples supported.
 */
#define SPX_REPLAY_MAX_RANGE_SAMPLES	65536

/* Reason codes for a user event handler being called. */
enum 
{
    SPX_REPLAY_EOF = 0,
    SPX_REPLAY_BAD_AZIMUTH = 1,
    SPX_REPLAY_BAD_DATA = 2,
    SPX_REPLAY_BAD_EOD = 3,
    SPX_REPLAY_BAD_MARKER = 4,
    SPX_REPLAY_BAD_NUMSAMPLES = 5,
    SPX_REPLAY_BAD_PAYLOAD = 6,
    SPX_REPLAY_BAD_READ = 7
};

/* Optional flags for SPxRadarReplayRaw */
#define SPX_REPLAY_AUTO_RESTART (1L)
#define SPX_REPLAY_LITTLE_ENDIAN (1L << 1)

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

/* Forward declarations. */
class SPxCompress;
class SPxThread;

/*
 * Define the header at the start of old recording files.
 */
typedef struct SPxRadarReplayHeaderV1_tag {
    UINT32 magic;		/* Magic number, SPX_RADAR_REPLAY_MAGIC1 */
    UINT16 headerSizeBytes;	/* Size of header, in bytes, this one 32 */
    UINT8 headerVersion;	/* Version of header, this one 1 */
    UINT8 fileFormat;		/* Format of data, SPX_RIB_PACKING_... */
    UINT16 numSamples;		/* Number of range samples per return */
    UINT16 numAzimuths;		/* Number of azimuths in 360 degrees */
    UINT32 endOfDataMarker;	/* Marker that denotes end of return (or 0) */
    UINT16 usecsBetweenReturns;	/* Default replay rate */
    UINT16 reserved1;		/* Reserved - write as zero */
    REAL32 startRange;		/* Range of first sample in each return */
    REAL32 endRange;		/* Range of last sample in each return */
    UINT32 reserved2;		/* Reserved - write as zero */
} SPxRadarReplayHeaderV1_t;

/*
 * Define the header at the start of each spoke.
 */
typedef struct SPxRadarReplayReturnInfo_tag {
    UINT32 marker;		/* Same as endOfDataMarker */
    UINT16 headerSizeBytes;	/* Size of this header */
    UINT16 reserved;		/* Reserved field, write as zero */
    UINT16 azimuth;		/* Azimuth in range 0 to numAzimuths-1 */
    UINT16 numSamples;		/* Number of samples in this spoke */
    UINT32 payloadSize;		/* Size of encoded data for this spoke */
} SPxRadarReplayReturnInfo_t;


/*
 * Define the base replay class.
 */
class SPxRadarReplayOldHdr :public SPxRadarSource
{
public:
    /*
     * Public functions.
     */
    /* Constructor/destructor */
    SPxRadarReplayOldHdr(SPxRIB *buffer);
    virtual ~SPxRadarReplayOldHdr();

    /* Creation function. */
    virtual int Create(char *fileName);

    /* Information retrieval. */
    SPxRIB *GetBuffer(void)		{ return m_rib; }
    int IsPaused(void)			{ return m_isPaused; }

    /* Define the basic controls. */
    int SetFileName(char *fileName);
    virtual void Pause(void)		{ m_isPaused = TRUE; }
    virtual void Play(void)		{ m_isPaused = FALSE; }
    virtual void Rewind(void);

    /* Speed of replay */
    void SetSpeedupFactor(double factor)
    {
	if( factor > 0.0 )
	{
	    m_speedUpFactor = factor;
	}
    }
    double GetSpeedupFactor(void)	{ return m_speedUpFactor; }

    /* Flag control (may also be done via other methods for some classes) */
    void SetFlags(int flags)		{ m_flags = flags; }
    int GetFlags(void)			{ return(m_flags); }

    /* Thread function (needs to be public for SPxThread to use). */
    virtual void *ThreadFunc(SPxThread *thread);

    /* Function to actually decode the data read from a file. */
    virtual SPxErrorCode DecodeReturn(unsigned char *encodedData,
				unsigned int numEncodedBytes,
				unsigned char *buffer,
				unsigned int bufferSizeBytes,
				unsigned int encodeFormat);

    /* Event handling. */
    void SetEventHandler(void (*fn)(SPxRadarReplayOldHdr *obj, int code))
    {
	m_eventHandler = fn;
    }

protected:
    /*
     * Protected variables.
     */
    /* Control state. */
    int m_createOK;		/* 1 if Create() was successful */
    int m_flags;		/* General-purpose flag. */
    int m_isPaused;		/* Set to TRUE if replay is paused */

    /* Timing information */
    double m_speedUpFactor;	/* Factor to speed-up (>1) or slow-down (<1) */

    /* Radar input buffer to feed. */
    SPxRIB *m_rib;

    /* File information. */
    char *m_fileName;			/* Filename for replay. */
    FILE *m_fileHandle;			/* I/O handle */
    int m_fileHeaderRequired;		/* Do we expect a header? */
    unsigned int m_fileHeaderSizeBytes;	/* Size of header if present */
    SPxRadarReplayHeaderV1_t m_fileHeader;	/* File header, if present */
    int m_byteSwapped;			/* Is file swapped from native? */

    /* Access control. */
    SPxCriticalSection m_access;

    /* Resolution of data. */
    int m_rangeSamples;			/* Number of samples per return. */
    int m_azimuthsPerScan;		/* Number of azimuths in 360 degs */

    /* Range of data. */
    double m_rangeOfFirstSample;	/* World range of first sample */
    double m_rangeOfLastSample;		/* World range of last sample */

    /* Replay rate. */
    UINT16 m_timeBetweenReturns;	/* Time between returns in usecs */

    /* Marker between each data block. */
    UINT32 m_endOfDataMarker;		/* Or zero if shouldn't be checked */

    /* User-installed handler function for events. */
    void (*m_eventHandler)(SPxRadarReplayOldHdr *obj, int reasonCode);

    /*
     * Protected functions.
     */
    /* Fn called by SetFileName(), allows derived classes to do things. */
    virtual int fileChanged(void) { return(SPX_NO_ERROR); }

    /* Fn called by thread on read-errors (including end of file). */
    void handleReadError(int);

private:
    /*
     * Private variables.
     */
    /* Worker thread to do the reading etc. */
    SPxThread *m_thread;

    /* ZLIB decompression. */
    SPxCompress *m_compress;

}; /* SPxRadarReplayOldHdr */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_RADARREPLAYOLDHDR_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
