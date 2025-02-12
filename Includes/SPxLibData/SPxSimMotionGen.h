/*********************************************************************
*
* (c) Copyright 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxSimMotionGen.h,v $
* ID: $Id: SPxSimMotionGen.h,v 1.3 2016/08/23 14:36:07 rew Exp $
*
* Purpose:
*   Header for SPxSimMotionGen - a class that generates a motion
*   profile based on the movements of a target.
*
*
* Revision Control:
*   23/08/16 v1.3    AGC	Event function prototype updated.
*
* Previous Changes:
*   10/11/14 1.2    AGC	Keep new target up to date with input target.
*   07/11/14 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_SIM_MOTION_GEN_H
#define _SPX_SIM_MOTION_GEN_H

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxFuture.h"

#include "SPxLibUtils/SPxObj.h"

/* Forward declarations. */
class SPxSimTarget;
class SPxSimMotion;
struct SPxSimState;
struct SPxRadarSimEventParams;

class SPxSimMotionGen : public SPxObj
{
public:
    explicit SPxSimMotionGen(SPxSimTarget *inputTarget,
	SPxSimTarget *copyTarget, int autoDelete=FALSE);
    virtual ~SPxSimMotionGen(void);

    SPxFuture<SPxSimMotion*> GetMotionProfile(void);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    void create(void);
    SPxErrorCode tgtEventFn(UINT32 flags);
    SPxErrorCode simEventFn(UINT32 flags, const SPxRadarSimEventParams *params);
    void update(void);
    int getTargetState(SPxSimState *state, char *name, unsigned int bufLen);
    void clearCompleteFutures(void);
    SPxSimMotion *getMotionProfile(void);
};

#endif /* _SPX_SIM_MOTION_GEN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
