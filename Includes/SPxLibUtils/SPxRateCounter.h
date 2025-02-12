/*********************************************************************
*
* (c) Copyright 2011 - 2016, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxRateCounter.h,v $
* ID: $Id: SPxRateCounter.h,v 1.10 2016/10/03 13:31:35 rew Exp $
*
* Purpose:
*	Header for Rate Counter class.
*
*	Convenience class for determining a rate.
*
* Revision Control:
*   03/10/16 v1.10   AGC	Add int/double overloads for GetRate/GetRateF().
*
* Previous Changes:
*   13/01/15 1.9    AGC	Add GetNum() for floating pointer intervals.
*   18/11/13 1.8    AGC	Support floating point intervals.
*   01/10/12 1.7    AGC	Move implementation to cpp file.
*   28/02/12 1.6    AGC	Return double from GetAverage().
*   02/11/11 1.5    AGC	Add roundMsecs to GetRateF().
*   22/09/11 1.4    AGC	Add GetRateF() and Clear() functions.
*   13/07/11 1.3    AGC	Add GetNum() function.
*			Add default argument to Add() function.
*   27/06/11 1.2    AGC	Add GetAverage()/GetMin()/GetMax() functions.
*   09/06/11 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_RATE_COUNTER_H
#define _SPX_RATE_COUNTER_H

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For common types. */
#include "SPxLibUtils/SPxCommon.h"

/* For SPxErrorCode enumeration. */
#include "SPxLibUtils/SPxError.h"

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

/*
 * Define our class, derived from the generic packet decoder.
 */
class SPxRateCounter
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxRateCounter(double maxIntervalSecs=5.0);
    explicit SPxRateCounter(int maxIntervalSecs);
    explicit SPxRateCounter(unsigned int maxIntervalSecs);
    virtual ~SPxRateCounter(void);

    /* Add a new value to the total. */
    SPxErrorCode Add(double value);
    SPxErrorCode Add(int value=0) { return Add(static_cast<double>(value)); }
    SPxErrorCode Add(unsigned int value) { return Add(static_cast<double>(value)); }

    /* Get the current count/second over the interval. */
    int GetRate(int intervalSecs) const;
    int GetRate(unsigned int intervalSecs) const;
    int GetRate(double intervalSecs = 1.0) const;
    float GetRateF(int intervalSecs, unsigned int roundMsecs = 0) const;
    float GetRateF(unsigned int intervalSecs, unsigned int roundMsecs = 0) const;
    float GetRateF(double intervalSecs = 1.0, unsigned int roundMsecs = 0) const;
    double GetAverage(double intervalSecs=1.0) const;
    double GetAverage(int intervalSecs=1) const { return GetAverage(static_cast<double>(intervalSecs)); }
    double GetAverage(unsigned int intervalSecs=1) const { return GetAverage(static_cast<double>(intervalSecs)); }
    int GetMin(unsigned int intervalSecs=1) const;
    int GetMax(unsigned int intervalSecs=1) const;
    int GetNum(double intervalSecs=1.0) const;
    int GetNum(int intervalSecs=1) const { return GetNum(static_cast<double>(intervalSecs)); }
    int GetNum(unsigned int intervalSecs=1) const { return GetNum(static_cast<double>(intervalSecs)); }

    /* Flush all values older than the interval. */
    SPxErrorCode Flush(int mutexAlreadyLocked=FALSE);

    /* Clear all values. */
    SPxErrorCode Clear(void);

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

}; /* SPxRateCounter */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPX_RATE_COUNTER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
