/*********************************************************************
*
* (c) Copyright 2007, 2008, 2010, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxSyncCombineSrc.h,v $
* ID: $Id: SPxSyncCombineSrc.h,v 1.5 2013/10/04 15:31:08 rew Exp $
*
* Purpose:
*   Header for Sync-Combining radar source object.
*
* Revision Control:
*   04/10/13 v1.5    AGC	Simplify headers.
*
* Previous Changes:
*   15/09/10 1.4    REW	Make destructor virtual.
*   13/11/08 1.3    REW	GetParameter() returns value in a string.
*   19/11/07 1.2    REW	Detect full RIBs.
*   22/10/07 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_SYNCCOMBINESRC_H
#define _SPX_SYNCCOMBINESRC_H

/*
 * Other headers required.
 */
/* We need SPXxLibUtils for common types, events, errors etc. */
#include "SPxLibUtils/SPxCriticalSection.h"

/* We need the base class header. */
#include "SPxLibData/SPxRadarSource.h"


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

/* Forward declarations. */
class SPxRIB;
class SPxPIM;
class SPxRunProcess;

/* Type for the individual inputs feeding this combiner. */
typedef struct SPxSyncCombineSrcInput_tag {
    /* Which channel are we? */
    unsigned int idx;		/* Index (i.e. 0 or 1) */

    /* What objects feed this input? */
    SPxRunProcess *rp;		/* Run-process for this input */
    SPxPIM *pim;		/* PIM for this input */

    /* What azimuth has this input reached? */
    int aziIdx;			/* Azimuth index (in PIM numbering) */
    UINT32 aziTime;		/* Time of last azimuth index */
} SPxSyncCombineSrcInput_t;


/*
 * Define our class, derived from a generic radar source.
 */
class SPxSyncCombineSrc :public SPxRadarSource
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxSyncCombineSrc(SPxRIB *buffer);
    virtual ~SPxSyncCombineSrc();

    /* Parameter assignment. */
    int SetParameter(char *parameterName, char *parameterValue);
    int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* Function for processes to pass raw data in. */
    int NewData(SPxRunProcess *rp, SPxPIM *pim, unsigned int aziIdx);

private:
    /*
     * Private fields.
     */
    /* Radar input buffer where data is written. */
    SPxRIB *m_rib;
    int m_ribFull;		/* Do we think RIB filled up? */
    int m_ribCount;		/* Incrementing count for written returns */
    UINT32 m_lastWarningTime;	/* Time of last reported warning */

    /* Critical section protection. */
    SPxCriticalSection m_mutex;

    /* Per-input information. */
    SPxSyncCombineSrcInput_t m_inputs[2];

    /* Configuration. */
    unsigned char *m_buffer;	/* For combining two spokes */
    unsigned int m_numSamples;	/* Max number of samples from inputs */
    int m_numAzimuths;		/* Number of azimuths in PIMs */

    /*
     * Private functions.
     */
};

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_SYNCCOMBINESRC_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
