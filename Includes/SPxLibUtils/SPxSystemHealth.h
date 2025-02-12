/*********************************************************************
*
* (c) Copyright 2009 - 2021, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header file for SPxSystemHealth class.
*
*
* Revision Control:
*   16/03/21 v1.17   SP 	Add function for auto clearing health bits.
*
* Previous Changes:
*   16/03/20 1.16   AJH	Auto-clearing timeout units now msec.
*   12/03/20 1.15   AJH	Support auto-clearing of health byte.
*   07/02/20 1.14   REW	Support applications with no source objects.
*   14/12/18 1.13   AJH	Add SRC_NO_VIDEO.
*   27/03/18 1.12   AJH	Add temperature to health byte.
*   08/03/17 1.11   AGC	Health byte now atomic.
*   23/02/17 1.10   REW	Add SPX_SYSTEM_HEALTH_BIT_SLAVE support.
*   02/11/16 1.9    REW	Add SPX_SYSTEM_HEALTH_SRC_SECONDARY_ERROR.
*   04/10/13 1.8    AGC	Simplify headers.
*   13/09/11 1.7    AGC	Fix icc warning by making GetSourceStatus() const.
*   07/09/11 1.6    AGC	Add source health handler function support.
*			Add AV_SETUP and AV_NO_DATA system health source bits.
*   07/10/10 1.5    REW	Remove duplicate DeleteAll() functions.
*   24/09/10 1.4    REW	Add using SPxObj::GetParameter().
*   15/09/10 1.3    REW	Make destructor virtual.
*   10/02/09 1.2    REW	Define access control for base class.
*   09/02/09 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_SYSTEMHEALTH_H
#define _SPX_SYSTEMHEALTH_H

/*
 * Other headers required.
 */

/* We need SPxLibUtils for common types. */
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxTimer.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Definitions for the health byte bitmask. */
#define	SPX_SYSTEM_HEALTH_BIT_ERROR	0x01	/* Generic error */
#define	SPX_SYSTEM_HEALTH_BIT_LICENSE	0x02	/* Unlicensed */
#define	SPX_SYSTEM_HEALTH_BIT_OVERLOAD	0x04	/* Overload detected */
#define	SPX_SYSTEM_HEALTH_BIT_SLAVE	0x08	/* Operating in slave mode */
#define	SPX_SYSTEM_HEALTH_BIT_TEMPERATURE	0x10	/* Over-temperature */

/* Definitions for the SourceStatus values (top byte of 16-bit value, the
 * bottom byte is source-specific).
 */
#define	SPX_SYSTEM_HEALTH_SRC_SETUP	    0x0100	/* Radar source - setup problem (e.g. net) */
#define	SPX_SYSTEM_HEALTH_SRC_NO_RTNS	    0x0200	/* Radar source - No returns detected */
#define	SPX_SYSTEM_HEALTH_SRC_NO_AZI	    0x0400	/* Radar source - No azimuth changes */
#define	SPX_SYSTEM_HEALTH_SRC_SECONDARY_ERROR 0x0800	/* Secondary data error */
#define SPX_SYSTEM_HEALTH_SRC_AV_SETUP	    0x1000	/* AV source/encoder - setup problem (e.g. net) */
#define SPX_SYSTEM_HEALTH_SRC_AV_NO_DATA    0x2000	/* AV source - No input data */
#define	SPX_SYSTEM_HEALTH_SRC_NO_VIDEO	    0x4000	/* Radar source - No radar video */
#define	SPX_SYSTEM_HEALTH_SRC_ERROR	    0x8000	/* Radar source - General error */


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* System source health retrieval function. */
typedef UINT16 (*SPxSystemHealthSourceFn_t)(void *userArg);

/* Structure for storing source health retrieval callbacks. */
struct SPxSystemHealthSourceItem
{
    SPxSystemHealthSourceFn_t fn;
    void *userArg;
};

/*
 * Define the class.
 */
class SPxSystemHealth :public SPxObj
{
public:
    /*
     * Public functions.
     */
    /* Constructor/destructor. */
    SPxSystemHealth(void);
    virtual ~SPxSystemHealth(void);

    /* Add/remove custom source health retrieval handlers. */
    SPxErrorCode AddSourceHandler(SPxSystemHealthSourceFn_t fn, void* userArg);
    SPxErrorCode RemoveSourceHandler(SPxSystemHealthSourceFn_t fn, void* userArg);

    /* Retrieval functions. */
    UINT8 GetHealth(void)		{ return static_cast<UINT8>(m_health); }
    UINT16 GetSourceStatus(void) const;

    /* Set/latch functions. */
    void LatchOverload(void)	{ Latch(SPX_SYSTEM_HEALTH_BIT_OVERLOAD);}
    void LatchLicense(void)	{ Latch(SPX_SYSTEM_HEALTH_BIT_LICENSE); }
    void LatchError(void)	{ Latch(SPX_SYSTEM_HEALTH_BIT_ERROR); }
    void LatchSlave(void)	{ Latch(SPX_SYSTEM_HEALTH_BIT_SLAVE); }
    void LatchTemperature(void)	{ Latch(SPX_SYSTEM_HEALTH_BIT_TEMPERATURE); }
    void Latch(unsigned int healthBits);

    /* Reset functions. */
    void ClearOverload(void)	{ m_health &= ~SPX_SYSTEM_HEALTH_BIT_OVERLOAD;}
    void ClearLicense(void)	{ m_health &= ~SPX_SYSTEM_HEALTH_BIT_LICENSE;}
    void ClearError(void)	{ m_health &= ~SPX_SYSTEM_HEALTH_BIT_ERROR; }
    void ClearSlave(void)	{ m_health &= ~SPX_SYSTEM_HEALTH_BIT_SLAVE; }
    void ClearTemperature(void)	{ m_health &= ~SPX_SYSTEM_HEALTH_BIT_TEMPERATURE; }
    void ClearAll(void)		{ m_health = 0; }
    void SetAutoClear(unsigned int healthBits, unsigned int timeoutMsecs);

    /* Configuration options. */
    void SetSourceIsExpected(int sourceIsExpected)
    {
	m_sourceIsExpected = (sourceIsExpected ? TRUE : FALSE);
    }
    int GetSourceIsExpected(void) const { return(m_sourceIsExpected); }

    /* Generic parameter control. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    using SPxObj::GetParameter;
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /*
     * Public variables.
     */

    /*
     * Static functions for class.
     */
    static SPxSystemHealth *GetSystemHealthObject(void);
    static SPxErrorCode GlobalLatchOverload(void);
    static SPxErrorCode GlobalLatchLicense(void);
    static SPxErrorCode GlobalLatchError(void);
    static SPxErrorCode GlobalLatchTemperature(void);

private:
    /*
     * Private functions.
     */
    void handleTimer(void);

    /*
     * Private variables.
     */
    SPxAtomic<int> m_health;	/* Composite health byte. */
    int m_sourceIsExpected;	/* TRUE if at least 1 radar source expected. */
    SPxTimer *m_timer;		/* For handling timeouts. */
    unsigned int m_autoClearBits;   /* Bits in health byte to clear. */
    unsigned int m_autoClearTimeoutMsecs;    /* Timeout to clear latched bits. */
    UINT32 m_autoClearLastLatch;	    /* Time of last latched bit. */

    /*
     * Static variables for class.
     */

}; /* SPxSystemHealth */


/*********************************************************************
*
*   Macros
*
**********************************************************************/


/*********************************************************************
*
*   Variable definitions
*
**********************************************************************/

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_SYSTEMHEALTH_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
