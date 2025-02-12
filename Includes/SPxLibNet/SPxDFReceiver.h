/*********************************************************************
 *
 * (c) Copyright 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *	Header for SPxDFReceiver, a base class for Direction Finder receivers.
 *
 * Revision Control:
 *  22/06/18 v1.4    AGC	Fix new cppcheck warnings.
 *
 * Previous Changes:
 *  22/02/18 1.3    REW	Support access to detections.
 *  20/02/18 1.2    REW	Remove scanning interval etc.
 *  20/02/18 1.1    REW	Initial Version.
 *********************************************************************/

#ifndef _SPX_DF_RECEIVER_H
#define _SPX_DF_RECEIVER_H

/*
 * Other headers required.
 */

/* Other classes that we need. */
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
 *
 *   Constants
 *
 *********************************************************************/

/* Maximum number of detections supported. */
#define	SPX_DF_RECEIVER_MAX_DETECTIONS	4

/*********************************************************************
 *
 *   Macros
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Type definitions
 *
 *********************************************************************/

/* Forward declare any classes required. */
class SPxCriticalSection;
class SPxThread;

/*
 * Define our class.
 */
class SPxDFReceiver : public SPxObj
{
public:
    /*
     * Public types.
     */
    /* DF detection information. */
    typedef struct
    {
	int isValid;			/* Is this detection valid? */
	char id[128];			/* Unique ID */
	double bearingDegs;		/* Degrees clockwise */
	UINT32 timestampMsecs;		/* Time of last update */
	double fromLatDegs;		/* Reference latitude */
	double fromLongDegs;		/* Reference longitude */
    } Detection_t;

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxDFReceiver(void);
    virtual ~SPxDFReceiver(void);

    /* Connection configuration.  Note: address may be NULL. */
    virtual SPxErrorCode SetAddressString(const char *address);
    virtual SPxErrorCode SetPortNumber(unsigned int port);
    virtual const char *GetAddressString(void) const	{ return(m_address); }
    virtual unsigned int GetPortNumber(void) const	{ return(m_port); }

    /* Search actions. */
    virtual SPxErrorCode Scan(void) = 0;	/* Requires derived version */
    SPxErrorCode StartScanningThread(void);
    int IsScanningThreadRunning(void) const
    {
	return( m_scanThread ? TRUE : FALSE );
    }
    SPxErrorCode StopScanningThread(void);

    /* Detection handling. */
    SPxErrorCode GetDetection(SPxDFReceiver::Detection_t *detection,
			      unsigned int idx);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    /*
     * Protected variables.
     */
    /* Control. */
    SPxCriticalSection *m_mutex;

    /* Connection info. */
    char *m_address;			/* Connection address */
    unsigned int m_port;		/* Port number */

    /* Detections. */
    Detection_t m_detections[SPX_DF_RECEIVER_MAX_DETECTIONS];


private:
    /*
     * Private variables.
     */
    /* Scanning thread. */
    SPxThread *m_scanThread;		/* Thread object */

    /*
     * Private functions.
     */
    /* Thread functions. */
    static void *scanThreadWrapper(SPxThread *thread);
    void *scanThreadFn(SPxThread *thread);


}; /* SPxDFReceiver */

/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* SPX_DF_RECEIVER_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
