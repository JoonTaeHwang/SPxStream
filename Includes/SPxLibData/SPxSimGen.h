/*********************************************************************
*
* (c) Copyright 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxSimGen.h,v $
* ID: $Id: SPxSimGen.h,v 1.2 2016/08/23 14:36:07 rew Exp $
*
* Purpose:
*   Header for SPxSimGen - a class that generates a simulation of
*   copies of all targets with associated motion profiles.
*
*   Deleting the SPxSimGen object stops updating the simulation.
*
*
* Revision Control:
*   23/08/16 v1.2    AGC	Event function prototype updated.
*
* Previous Changes:
*   07/11/14 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_SIM_GEN_H
#define _SPX_SIM_GEN_H

#include "SPxLibUtils/SPxAutoPtr.h"

#include "SPxLibUtils/SPxObj.h"

/* Forward declarations. */
class SPxRadarSimulator;
class SPxSimMotion;
class SPxSimTarget;
struct SPxRadarSimEventParams;

class SPxSimGen : public SPxObj
{
public:
    explicit SPxSimGen(SPxRadarSimulator *radarSim);
    virtual ~SPxSimGen(void);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    static SPxErrorCode targetFn(unsigned int index, SPxSimTarget *target, void *userArg);
    SPxErrorCode eventFn(UINT32 flags, const SPxRadarSimEventParams *params);
    void clearCompleteFutures(void);

};

#endif /* _SPX_SIM_GEN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
