/*********************************************************************
*
* (c) Copyright 2007 - 2016, 2018, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for the span-detector base class.
*
* Revision Control:
*   11/05/21 v1.14   AGC	Support IFF Mode 6.
*
* Previous Changes:
*   19/01/18 1.13   REW	Add iffExtFlags etc.
*   24/03/16 1.12   REW	Remove SPX_SPANDETECT_IFF_FLAGS_... definitions
*			and use SPX_PACKET_TRACK_SECONDARY_IFF_FLAGS
*			instead.
*   11/03/16 1.11   REW	Add SPX_SPANDETECT_IFF_FLAGS_MODE_1_12BIT.
*   01/09/14 1.10   REW	Add SPX_SPANDETECT_IFF_PATTERN_AUTO option.
*   29/08/14 1.9    REW	Add IFF uncertain flag.
*   27/08/14 1.8    REW	Support IFF extraction patterns.
*   29/07/14 1.7    REW	Support IFF extraction modes.
*   15/10/13 1.6    AGC	Fix warnings from SPxLibAVMfc.
*   04/10/13 1.5    AGC	Simplify headers.
*   27/11/12 1.4    REW	Add CentroidMode support.
*   16/12/09 1.3    REW	Add StateChanged().
*   10/10/07 1.2    REW	Add aziWidthDegs to ReturnStarted().
*   09/10/07 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_SPANDETECT_H
#define _SPX_SPANDETECT_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types, events, errors etc. */
#include "SPxLibUtils/SPxObj.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* CentroidMode options. */
#define	SPX_SPANDETECT_CENTROID_WEIGHTED	0
#define	SPX_SPANDETECT_CENTROID_INTENSITY	1
#define	SPX_SPANDETECT_CENTROID_LEADINGEDGE	2
#define	SPX_SPANDETECT_CENTROID_IFF		3

/* IFF patterns. */
#define	SPX_SPANDETECT_IFF_PATTERN_AUTO		0	/* Auto identify */
#define	SPX_SPANDETECT_IFF_PATTERN_A		1	/* Just mode 3/A */
#define	SPX_SPANDETECT_IFF_PATTERN_AC		2	/* A and C interlace */
#define	SPX_SPANDETECT_IFF_PATTERN_AAC		3	/* A, A, C interlace */
#define	SPX_SPANDETECT_IFF_PATTERN_1AC2AC	4	/* Military too */
#define	SPX_SPANDETECT_IFF_PATTERN_6		6	/* Just mode 6 */

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
struct SPxReturnHeader_tag;
class SPxRunProcess;

/*
 * Structure holding information about a span.
 */
typedef struct SPxSpan_tag {
    unsigned int azi16Bit;		/* 16-bit azimuth number */
    unsigned int firstSampleIdx;	/* First sample index */
    unsigned int lastSampleIdx;		/* Last sample index */
    unsigned int totalStrength;		/* Sum of all samples */
    REAL32 rangeIdx;			/* Centroided sample index */
    UINT16 iffCode;			/* IFF code if known */
    UINT8 iffFlags;		/* SPX_PACKET_TRACK_SECONDARY_IFF_FLAGS... */
    UINT8 iffExtFlags;		/* SPX_PACKET_TRACK_SECONDARY_IFF_EXT_FLAGS..*/
} SPxSpan_t;


/*
 * Base class for all span-detector classes.  Pure-virtual, must be derived.
 */
class SPxProSpanDetector :public SPxObj
{
public:
    /*
     * Public functions.
     */
    /* Constructor/destructor. */
    SPxProSpanDetector()		{ return; }
    virtual ~SPxProSpanDetector()	{ return; }

    /* Span reporting. */
    virtual void ReturnStarted(unsigned int /*azi16Bit*/,
				SPxReturnHeader_tag * /*header*/,
				SPxRunProcess * /*rp*/,
				double /*aziWidthDegs*/)
    {
	return;
    }
    virtual void NewSpan(const SPxSpan_t *span) = 0;	/* MUST IMPLEMENT */
    virtual void ReturnFinished(unsigned int /*azi16Bit*/)
    {
	return;
    }
    virtual void StateChanged(int /*nowEnabled*/)
    {
	return;
    }

private:
    /*
     * Private functions.
     */
};

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPX_SPANDETECT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
