/*********************************************************************
*
* (c) Copyright 2015, 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxPIDFilter.h,v $
* ID: $Id: SPxPIDFilter.h,v 1.7 2017/02/21 15:57:00 rew Exp $
*
* Purpose:
*   Generic Proportional-Integral-Derivative (PID) Filter.
*
* Revision Control:
*   21/02/17 v1.7    SP 	Add SetMaxOutput().
*
* Previous Changes:
*   31/01/17 1.6    AGC	Remove speed.
*   13/01/17 1.5    AGC	Remove unimplemented GetOutput().
*   10/12/15 1.4    AGC	Support filtering speed.
*			Support use of supplied timestamps.
*   11/05/15 1.3    AGC	Support querying variance of errors.
*   14/04/15 1.2    AGC	Support multiple filters with same coefficients.
*   26/03/15 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_PID_FILTER_H
#define _SPX_PID_FILTER_H

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"

/* Forward declarations. */
struct SPxTime_tag;

/* Define our class. */
class SPxPIDFilter : public SPxObj
{
public:
    SPxPIDFilter(void);
    virtual ~SPxPIDFilter(void);

    void SetP(double p);
    double GetP(void) const;
    void SetI(double i);
    double GetI(void) const;
    void SetD(double d);
    double GetD(void) const;
    void SetMaxOutput(double max, int index=0);
    double Update(double err, int index=0, const SPxTime_tag *timeEpoch=NULL);
    int HaveOutput(int index=0) const;
    double GetVariance(int index=0) const;
    void Reset(void);

    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    double update(double err, int index, const SPxTime_tag *timeEpoch);

};

#endif /* _SPX_PID_FILTER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
