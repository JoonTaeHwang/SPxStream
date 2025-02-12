/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
*
* Purpose:
*   Header file for timed actions in SPx.
*
* Revision Control:
*   14/12/18 v1.1   AJH	Initial version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_CRONJOB_H
#define _SPX_CRONJOB_H

/* Make sure we have the common definitions and headers. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxTimer.h"
#include "SPxLibNet/SPxRemoteServer.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/
#define SPX_CRONJOB_MAX_JOBS	    10

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/
struct spx_cronjob_job_t
{
    unsigned int hours;
    unsigned int mins;
    char object[128];
    char param[128];
    char value[128];
    int triggered;
};

class SPxCronJob : public SPxObj
{
private:
    SPxRemoteServer *m_remoteServer;
    spx_cronjob_job_t m_job[SPX_CRONJOB_MAX_JOBS];
    unsigned int m_numJobs;
    SPxTimer *m_timer;

    /* Callback for schedule timer. */
    void handleTimer(void);

protected:

public:
    /* Constructor and destructor. */
    SPxCronJob(SPxRemoteServer *remoteServer);
    virtual ~SPxCronJob(void);

    /* Initialisation function. */
    SPxErrorCode Create(void);
};

#endif /* _SPX_CRONJOB_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
