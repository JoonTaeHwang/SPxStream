/*********************************************************************
*
* (c) Copyright 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxButterworthFilter.h,v $
* ID: $Id: SPxButterworthFilter.h,v 1.6 2014/07/04 15:22:46 rew Exp $
*
* Purpose:
*   Generic 1-dimensional Butterworth Filter.
*
* Revision Control:
*   04/07/14 v1.6    AGC	Fix icc remarks.
*
* Previous Changes:
*   04/07/14 1.5    AGC	Move some code to source file.
*   01/05/14 1.4    AGC	Use correct iterator type.
*   16/01/14 1.3    AGC	Remove disabling of icc 383 remark.
*   16/01/14 1.2    AGC	Initialise m_type in constructor.
*			Disable icc 383 remark.
*   15/01/14 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_BUTTERWORTH_FILTER_H
#define _SPX_BUTTERWORTH_FILTER_H

#include "SPxLibUtils/SPxCircularBuffer.h"
#include "SPxLibUtils/SPxMatrix.h"

/* Define our class. */
template<class T, std::size_t Order>
class SPxButterworthFilter
{
public:
    /* Filter type. */
    enum Type
    {
	LOW_PASS,
	HIGH_PASS,
	BAND_PASS,
	BAND_STOP
    };

    struct Pair
    {
	Pair() : first(), second() {}
	Pair(T first_, T second_)
	    : first(first_), second(second_) {}

	T first;
	T second;
    };

    /* Max array size (required for band pass/stop filters). */
    static const std::size_t ArrSize = Order * 2 + 1;

    /* Convenience types. */
    typedef SPxVector<Pair, ArrSize> Vector;
    typedef SPxCircularBuffer<Pair> Buffer;

    /* Public functions. */
    explicit SPxButterworthFilter(void);
    ~SPxButterworthFilter(void);
    void Create(Type type, T normFreq1, T normFreq2=T());

    void Reset();
    T Filter(T val) const;

private:
    /* Private variables. */
    Type m_type;		    /* Filter type. */
    Vector m_coefficients;	    /* Recurrence equation coefficients. */
    T m_gain;			    /* Gain. */
    mutable Buffer m_historyLH;	    /* Historic data for Low/High Pass filters. */
    mutable Buffer m_historyBand;   /* Historic data for Band Pass/Stop filters. */

    /* Private functions. */
    void create(Type type, T normStartFreq, T normEndFreq);

    /* Query if the filter is a low or high pass filter (or a band pass filter). */
    bool typeIsLH(void) const;
};

#endif /* _SPX_BUTTERWORTH_FILTER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
