/*********************************************************************
*
* (c) Copyright 2007, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxRadarReplayRaw.h,v $
* ID: $Id: SPxRadarReplayRaw.h,v 1.2 2007/12/11 15:35:32 rew Exp $
*
* Purpose:
*   Header for Simple Radar Replay.
*
* Revision Control:
*   11/12/07 v1.2    REW	Now derived from SPxRadarReplayOldHdr.
*
* Previous Changes:
*   01/08/07 1.1    REW	Initial Version from SPxRadarReplay.h
**********************************************************************/

#ifndef _SPX_RADARREPLAYRAW_H
#define _SPX_RADARREPLAYRAW_H

/*
 * Other headers required.
 */
/* We need the base class header, which includes lots of others. */
#include "SPxLibData/SPxRadarReplayOldHdr.h"


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

/* A stucture that a client of class SPxRadarReplayRaw fills in to
 * provide relevant data for the decoding of the file. 
 */
typedef struct
{
    int rangeSamplesPerReturn;	// Number of range samples (8-bits) per return.
    int azimuthsPerScan;	// The azimuth number for 360 degrees 
    UINT16 timeBetweenReturns;	// Time between returns in microseconds 
    int flags;			// Assorted flags.
    REAL32 rangeOfFirstSample;	// World range of first sample in each return
    REAL32 rangeOfLastSample;	// World range of last sample in each return.
    UINT32 endOfDataMarker;	// The marker that flags the end of return
    void (*eventHandler)(SPxRadarReplayOldHdr *, int); // User-supplied handler
} SPxReplayRawInfo;

/*
 * Class for basic raw replay of radar recording.
 */
class SPxRadarReplayRaw :public SPxRadarReplayOldHdr
{
public:
    /*
     * Public functions.
     */
    /* Constructor/destructor. */
    SPxRadarReplayRaw(SPxRIB *buffer);
    virtual ~SPxRadarReplayRaw();

    /* Creation functions, original and override base class one. */
    int Create(char *fileName, SPxReplayRawInfo *infoBlock);
    int Create(char *fileName);

    /* Override some of the base class functions. */
    void *ThreadFunc(SPxThread *thread);

protected:
    /* Override some of the base class functions. */
    int fileChanged(void);

private:
    /*
     * Private variables. Should probably move some of these to base class.
     */
    /* Line buffer for building returns etc. */
    unsigned char *m_lineBuffer;	/* Return header, then data */

    /* Copy of infoBlock (if supplied). */
    SPxReplayRawInfo m_infoBlock;

    /*
     * Private functions.
     */

}; /* SPxRadarReplayRaw() */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_RADARREPLAYRAW_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
