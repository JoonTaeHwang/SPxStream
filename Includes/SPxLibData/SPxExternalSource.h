/*********************************************************************
*
* (c) Copyright 2008 - 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxExternalSource.h,v $
* ID: $Id: SPxExternalSource.h,v 1.6 2017/07/21 09:47:35 rew Exp $
*
* Purpose:
*	Header for SPxExternalSource objects.
*
*
* Revision Control:
*   21/07/17 v1.6    REW	Add SetScanMode().
*
* Previous Changes:
*   04/10/13 1.5    AGC	Simplify headers.
*   07/07/11 1.4    AGC	Remove unused param for GetNumSamples() (REW)
*   15/09/10 1.3    REW	Make destructor virtual.
*   14/10/09 1.2    REW	SetStartRangeMetres() doesn't error on zero
*			if end range not yet set.
*   10/10/08 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_EXTERNAL_SOURCE_H
#define _SPX_EXTERNAL_SOURCE_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxError.h"

/* We need the radar source base class header. */
#include "SPxLibData/SPxRadarSource.h"

/* We also need SPxRIB definitions. */
#include "SPxLibData/SPxRib.h"


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

/*
 * Define our class, derived from generic radar sources.
 */
class SPxExternalSource :public SPxRadarSource
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    SPxExternalSource(SPxRIB *buffer);
    virtual ~SPxExternalSource();

    /* Number of samples (nominal, some spokes may be shorter). */
    SPxErrorCode SetNumSamples(unsigned int numSamples)
    {
	if( numSamples < 4 )
	{
	    SPxError(SPX_ERR_USER, SPX_ERR_BAD_ARGUMENT, numSamples, 4,
			"SPxExternalSource", "SetNumSamples()");
	    return(SPX_ERR_BAD_ARGUMENT);
	}
	m_numSamples = numSamples;	
	return(SPX_NO_ERROR);
    }
    unsigned int GetNumSamples(void) const	{ return(m_numSamples); }

    /* Range. */
    SPxErrorCode SetStartRangeMetres(double metres)
    {
	if( (metres < 0.0)
	    || ((m_endRangeMetres != 0.0) && (metres >= m_endRangeMetres)) )
	{
	    SPxError(SPX_ERR_USER, SPX_ERR_BAD_ARGUMENT, (int)metres, 0,
			"SPxExternalSource", "SetStartRangeMetres()");
	    return(SPX_ERR_BAD_ARGUMENT);
	}
	m_startRangeMetres = metres;
	return(SPX_NO_ERROR);
    }
    SPxErrorCode SetEndRangeMetres(double metres)
    {
	if( (metres <= 0.0) || (metres <= m_startRangeMetres) )
	{
	    SPxError(SPX_ERR_USER, SPX_ERR_BAD_ARGUMENT,
			(int)metres, (int)m_startRangeMetres,
			"SPxExternalSource", "SetEndRangeMetres()");
	    return(SPX_ERR_BAD_ARGUMENT);
	}
	m_endRangeMetres = metres;
	return(SPX_NO_ERROR);
    }
    double GetStartRangeMetres(void) const	{ return(m_startRangeMetres); }
    double GetEndRangeMetres(void) const	{ return(m_endRangeMetres); }

    /* Scan mode. */
    SPxErrorCode SetScanMode(SPxScanMode scanMode)
    {
	m_scanMode = scanMode;
	return(SPX_NO_ERROR);
    }
    SPxScanMode GetScanMode(void) const		{ return(m_scanMode); }

    /* Data generation. */
    SPxErrorCode NewReturn(SPxReturnHeader *hdr, unsigned char *data);
    SPxErrorCode NewReturn(UINT16 azi16Bits, UINT16 gapUsecs, UINT16 numBytes,
				unsigned char *data);

private:
    /*
     * Private fields.
     */
    /* Radar input buffer where data is written. */
    SPxRIB *m_rib;

    /* Resolution. */
    unsigned int m_numSamples;		/* Set by user */

    /* Range. */
    double m_startRangeMetres;		/* Start range in metres. */
    double m_endRangeMetres;		/* End range in metres */

    /* Scan mode. */
    SPxScanMode m_scanMode;		/* Scan mode */

    /* Fields for filling in return header if the caller isn't. */
    UINT16 m_ribCount;			/* For "count" field */
    SPxTime_t m_ribLastTime;		/* For "timeInterval" field */

    /*
     * Private functions.
     */
}; /* SPxExternalSource class */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_EXTERNAL_SOURCE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
