/*********************************************************************
*
* (c) Copyright 2021 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxDopplerStore class.
*
* Revision Control:
*   08/06/22 v1.14   AJH	Support P437 format.
*
* Previous Changes:
*   03/03/21 1.13   REW	Prepare to support replay.
*   03/03/21 1.12   REW	Improve previous change.
*   03/03/21 1.11   REW	Avoid introducing ZLib dependency.
*   02/03/21 1.10   REW	Support SetRecordingProcess().
*   26/02/21 1.9    REW	Rework stats functions to simplify for future.
*   26/02/21 1.8    REW	Support max velocity param for real speeds.
*   11/02/21 1.7    REW	Add FilterReturn() function (incomplete).
*   08/02/21 1.6    REW	Support max age limit.
*   03/02/21 1.5    REW	Support debug.
*   02/02/21 1.4    REW	Support azimuth offsets.
*   29/01/21 1.3    REW	Add GetStats().
*   29/01/21 1.2    REW	Support GetColourLUT().
*   28/01/21 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_DOPPLER_STORE_H
#define _SPX_DOPPLER_STORE_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibProc/SPxProcStd.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Define the formats of Doppler data we'll get.
 * Limit this to 16-bits for recording.
 */
#define	SPX_DOPPLER_FORMAT_NONE		0
#define	SPX_DOPPLER_FORMAT_P172		172
#define	SPX_DOPPLER_FORMAT_P437		437

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

/* Forward-declare other classes we use. */
class SPxCriticalSection;
class SPxRIB;
class SPxRunProcess;

/*
 * Define our class.
 */
class SPxDopplerStore : public SPxObj
{
public:
    /* Recording callback function. */
    typedef SPxErrorCode(*RecordFn)(SPxRunProcess *rp,
					UINT16 packetType,
					const void *payloadHdr,
					unsigned int payloadHdrBytes,
					const void *payload,
					unsigned int payloadBytes,
					const void *payload2,
					unsigned int payloadBytes2,
					UCHAR channelIndex);


    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxDopplerStore(SPxRIB *rib);
    virtual ~SPxDopplerStore(void);

    /* Configuration */
    SPxErrorCode SetNumAzis(unsigned int numAzis);
    unsigned int GetNumAzis(void) const { return(m_numAzis); }

    /* Azimuth offset (16-bit number, similar to SPxPIM functions). */
    SPxErrorCode SetAzimuthOffset(int offset);
    int GetAzimuthOffset(void) const { return(m_azimuthOffset); }

    /* Limit on age of data to use. */
    SPxErrorCode SetMaxAgeMsecs(UINT32 msecs);
    UINT32 GetMaxAgeMsecs(void) const { return(m_maxAgeMsecs); }

    /* Decoding options. */
    SPxErrorCode SetMaxVel(double maxVelMps);
    double GetMaxVel(void) const { return(m_maxVel); }

    /* New data. */
    SPxErrorCode NewData(int format, const struct SPxReturnHeader_tag *hdr,
			const unsigned char *data, unsigned int dataSizeBytes);

    /* Data access. */
    SPxErrorCode GetAreaStats(double startRangeMetres, double endRangeMetres,
				double startAziDegs, double endAziDegs,
				struct SPxBlobDopplerData_tag *stats);
    SPxErrorCode GetSpeedAtPoint(double rangeMetres, double aziDegs,
				double *speedMpsPtr);

    /* Data application. */
    SPxErrorCode FilterReturn(SPxReturnHeader_tag *hdr, void *data,
				double minSpeed, double maxSpeed);

    /* Access functions. */
    unsigned char *GetColourLUT(void)	{ return(m_colourLUT); }

    /* Recording support. */
    SPxErrorCode SetRecordingProcess(SPxRunProcess *recProc,
					RecordFn recordFn=SPxProRecordPacketB);

    /* Debug support. */
    SPxErrorCode SetDebugFilename(const char *filename);
    const char *GetDebugFilename(void) const	{ return(m_debugFilename); }

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:
    /*
     * Private variables.
     */
    /* Internal fields. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Mutex protection. */
    SPxCriticalSection *m_mutex;	/* Protection mutex  */

    /* RIB. */
    SPxRIB * const m_rib;		/* RIB, or NULL if none */

    /* Configuration. */
    unsigned int m_numAzis;		/* Number of azimuths */
    int m_azimuthOffset;		/* The azimuth offset (16-bit azi) */

    /* Buffer for building output data. */
    unsigned char *m_outputBuf;		/* Enough for single spoke */

    /* Colour LUT. */
    unsigned char *m_colourLUT;		/* For passing to SetRadarLUT() */

    /* Processing options. */
    UINT32 m_maxAgeMsecs;		/* 0, or oldest age to use */

    /* Decoding options. */
    double m_maxVel;			/* Max velocity, zero if not known */

    /* Recording support. */
    RecordFn m_recFn;			/* Function for recording */
    SPxRunProcess *m_recProcess;	/* Process for recording */

    /* Debug support. */
    char *m_debugFilename;		/* Name of file, or NULL */
    UINT32 m_debugStartMsecs;		/* Debug reference timestamp */

    /*
     * Private functions.
     */
    SPxErrorCode initStore(int forceReset=FALSE);
    void freeStore(void);
    double getSpeedForChannel(int channel);

}; /* SPxDopplerStore */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_DOPPLER_STORE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
