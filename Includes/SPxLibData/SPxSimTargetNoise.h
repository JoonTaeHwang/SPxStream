/*********************************************************************
*
* (c) Copyright 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxSimTargetNoise.h,v $
* ID: $Id: SPxSimTargetNoise.h,v 1.3 2017/04/12 09:05:33 rew Exp $
*
* Purpose:
*	Header for SPxSimTargetNoise object which generates noise
*	for simulated target positions.
*
* Revision Control:
*   12/04/17 v1.3    AGC	Support set/get of noise seeds.
*
* Previous Changes:
*   15/03/17 1.2    AGC	Simplify interface.
*   04/01/17 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_SIM_TARGET_NOISE_H
#define _SPX_SIM_TARGET_NOISE_H

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/*
 * Types
 */

/* Forward declarations. */

/* Define our class. */
class SPxSimTargetNoise
{
public:
    /* Public types. */
    enum Distribution
    {
	DIST_TRIANGLE = 0,
	DIST_NORMAL = 1,
	DIST_SQUARE = 2
    };

    /* Public functions. */
    SPxSimTargetNoise(void);
    virtual ~SPxSimTargetNoise(void);
        
    SPxErrorCode SetDistribution(Distribution distribution);
    SPxErrorCode GetDistribution(Distribution *distribution) const;

    SPxErrorCode SetMean(unsigned int index, double rangeErrorMean);
    SPxErrorCode GetMean(unsigned int index, double *rangeErrorMean) const;
    SPxErrorCode SetStdDev(unsigned int index, double rangeErrorStdDev);
    SPxErrorCode GetStdDev(unsigned int index, double *rangeErrorStdDev) const;

    SPxErrorCode Update(void);
    double GetError(unsigned int index) const;
    
    SPxErrorCode SetSeed(const UINT32 *seeds, unsigned int numSeeds);
    SPxErrorCode GetSeed(UINT32 *seeds, unsigned int numSeeds) const;

private:
     /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    void resetDists(void);

}; /* SPxSimTargetNoise. */

#endif /* _SPX_SIM_TARGET_NOISE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
