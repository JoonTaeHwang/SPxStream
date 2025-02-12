/*********************************************************************
*
* (c) Copyright 2007, 2009, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPx Message Reporter base class.
*
*
* Revision Control:
*   18/06/21 v1.3    AGC	Move SPxScSrcNetMsgRpt functionality here.
*
* Previous Changes:
*   29/01/09 1.2    DGJ	Move constructor and destructor into .cpp.
*   03/09/07 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_MSGRPT_H
#define _SPX_MSGRPT_H

/* Need common definitions */
#include "SPxLibUtils/SPxCommon.h"
#include "SPxObj.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Interface of message reporter objects. */
class SPxMessageReporterIface
{
public:
    SPxMessageReporterIface() {}
    virtual ~SPxMessageReporterIface() {}

    virtual int SendMessage(const char *buf, int len = 0) = 0;
};

/*
 * SPx Message Reporter.
 */
class SPxMessageReporter : public SPxObj
{
public:
    /* Constructor and destructor. */
    SPxMessageReporter(void);
    virtual ~SPxMessageReporter(void);

    int Create(SPxMessageReporterIface *msgRptr);

    /* Reporting function. */
    void Report(const char *msg);

private:
    SPxMessageReporterIface *m_msgRptr;

}; /* SPxMessageReporter class */

/* Add a typedef for old SPxScSourceNetMsgRpt class for compatibility. */
typedef SPxMessageReporter SPxScSourceNetMsgRpt;

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_MSGRPT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
