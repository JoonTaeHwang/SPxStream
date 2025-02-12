/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header file for clock class.
*
* Revision Control:
*   09/02/22 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_CLOCK_H_
#define _SPX_CLOCK_H_

/*
 * Other headers required.
 */
/* Include SPx headers we need. */
#include "SPxLibUtils/SPxObj.h"		/* For base class */


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
 * Forward-declare other classes in case headers are in wrong order.
 */
struct SPxTime_tag;

/*
 * Define our class.
 */
class SPxClock : public SPxObj
{
public:
    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxClock(void);
    virtual ~SPxClock();

    virtual INT64 GetTickerMsecs(void) const;
    virtual INT64 GetTickerPrecise(void) const;
    virtual int GetEpoch(SPxTime_tag *now) const;

    virtual INT64 GetTickerMsecsAtEpoch(const SPxTime_tag *epochTime) const;
    virtual int GetEpochAtTickerMsecs(SPxTime_tag *epochTime, INT64 tickerTime) const;

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

    /*
     * Private functions.
     */

}; /* SPxClock */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPX_CLOCK_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/
