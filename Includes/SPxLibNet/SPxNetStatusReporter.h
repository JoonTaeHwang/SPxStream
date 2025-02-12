/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxNetStatusReporter object which supports sending
*	regular status messages on the network.
*
*
* Revision Control:
*   27/09/19 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_NET_STATUS_REPORTER_H
#define _SPX_NET_STATUS_REPORTER_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types, events, errors etc. */
#include "SPxLibUtils/SPxError.h"

/* We need our base class. */
#include "SPxLibNet/SPxPacketSend.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Options for report type. */
#define	SPX_NET_STATUS_REPORTER_TYPE_INOSD	1


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
class SPxNavData;
class SPxTimer;

/*
 * Define our class, derived from the generic packet sender class.
 */
class SPxNetStatusReporter :public SPxPacketSender
{
public:
    /*
     * Public functions.
     */
    explicit SPxNetStatusReporter(SPxNavData *navData);
    virtual ~SPxNetStatusReporter(void);

    /* Note that most functions (address/port etc.) are in base class. */

    /* Configure settings. */
    SPxErrorCode SetReportType(unsigned int reportType)
    {
	m_reportType = reportType;
	return(SPX_NO_ERROR);
    }
    unsigned int GetReportType(void) const	{ return(m_reportType); }

    /* Generic parameter control. */
    int SetParameter(char *parameterName, char *parameterValue);
    int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /*
     * Private variables.
     */
    /* Settings. */
    unsigned int m_reportType;		/* 0 means no output. */

    /* Passed-in objects. */
    SPxNavData *m_navData;		/* Source of nav info */

    /* Timer support. */
    SPxTimer *m_timer;			/* Timer for events */

    /*
     * Private functions.
     */
    void handleTimer(void);		/* Function to do the work. */
    void sendINOSD(void);		/* Particular report type. */

}; /* SPxNetStatusReporter class */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPX_NET_STATUS_REPORTER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
