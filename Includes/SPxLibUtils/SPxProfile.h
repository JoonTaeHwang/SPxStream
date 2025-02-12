/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header file for SPxProfile class.
*
* Revision Control:
*   13/11/19 v1.3   AJH	Fix initialisation race condition.
*
* Previous Changes:
*   11/11/19 1.2   AJH	Add mutex lock.
*   06/09/19 1.1   AJH	Initial version.
**********************************************************************/

#ifndef _SPX_PROFILE_H
#define _SPX_PROFILE_H

/*
 * Other headers required.
 */

/* We need SPxLibUtils for common types. */
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxCriticalSection.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/
#define SPX_PROFILE_NUM_METRIC	    16

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

struct SPxProfileMetric
{
    char *name;
    UINT64 startTime;
    UINT64 totalTime;
    UINT64 count;
    UINT64 bytes;
};

/*
 * Define the class.
 */
class SPxProfile :public SPxObj
{
public:
    /*
     * Public functions.
     */
    SPxErrorCode StartTimer(const char *name);
    SPxErrorCode StopTimer(const char *name, UINT64 bytes=0);
    SPxErrorCode PrintMetrics(bool reset=FALSE);

    /*
     * Public variables.
     */

    /*
     * Static functions for class.
     */
    static SPxProfile *GetProfileObject(int enable=0);

private:
    /*
     * Private functions.
     */
    /* Constructor/destructor. These are private so that only GetProfileObject()
     * can create the global profile object.
     */
    SPxProfile(void);
    virtual ~SPxProfile(void);

    SPxProfileMetric *findMetric(const char *name);

    /*
     * Private variables.
     */
    UINT64 m_startTime;
    SPxCriticalSection m_mutex;

    SPxProfileMetric m_metric[SPX_PROFILE_NUM_METRIC];

    /*
     * Static variables for class.
     */

}; /* SPxProfile */


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

#endif /* _SPX_PROFILE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
