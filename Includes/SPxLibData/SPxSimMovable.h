/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxSimMovable object which represents anything that
*	can move in the simulated SPxRadarSimulator world.
*
*
* Revision Control:
*   26/07/21 v1.4    AGC	Support always jumping targets to motion profile.
*
* Previous Changes:
*   02/07/21 1.3    AGC	Add JumpToMotionProfile().
*   17/03/21 1.2    AGC	Suppress cppcheck warning.
*   02/03/21 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_SIM_MOVABLE_H
#define _SPX_SIM_MOVABLE_H

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxErrorCode. */
#include "SPxLibUtils/SPxError.h"

/* For SPxEventHandler. */
#include "SPxLibUtils/SPxEventHandler.h"

/* For base classes. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibData/SPxJoystickSim.h"

/*
 * Types
 */

/* Movable visibility. */
enum SPxSimMovableVis
{
    SPX_SIM_MOV_VISIBLE	    = 0,
    SPX_SIM_MOV_HIDDEN	    = 1,
    SPX_SIM_MOV_POD_HIDDEN  = 2
};

/* Position source. */
enum SPxSimMovableDynamicsSource
{
    SPX_SIM_MOV_DYN_SRC_FREE	    = 0,
    SPX_SIM_MOV_DYN_SRC_MOTION	    = 1,
    SPX_SIM_MOV_DYN_SRC_JOYSTICK    = 2
};

/* Forward declarations. */
class SPxRadarSimulator;
class SPxSimMotion;
struct SPxSimState;
struct SPxLatLong_tag;
struct SPxRadarSimEventParams;

/* Define our class. */
class SPxSimMovable : public SPxObj, public SPxJoystickSimIface
{
public:
    /* Public types. */
    typedef SPxErrorCode (*EventHandler)(UINT32 flags);

    /* Public functions. */
    explicit SPxSimMovable(SPxRadarSimulator *radarSim);
    explicit SPxSimMovable(SPxSimMovable *movable);
    virtual ~SPxSimMovable(void);

    SPxErrorCode SetDynamicsSource(SPxSimMovableDynamicsSource dynamicsSource,
				   int jumpTo=TRUE);
    SPxErrorCode GetDynamicsSource(SPxSimMovableDynamicsSource *dynamicsSource) const;
    virtual int IsSourceJoystick(void) const;

    /* Free-running targets. */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetCurrentState(const SPxSimState *state);
    SPxErrorCode SetCurrentState(const SPxSimState *state,
	const SPxLatLong_tag *initPlatLatLong);
    virtual SPxErrorCode GetCurrentState(SPxSimState *state) const
    {
	return GetCurrentState(state, FALSE);
    }
    SPxErrorCode GetCurrentState(SPxSimState *state,
				 int mutexAlreadyLocked) const;
    SPxErrorCode SetHeadingOffsetDegs(double headingOffsetDegs);
    SPxErrorCode GetHeadingOffsetDegs(double *headingOffsetDegs,
				      int mutexAlreadyLocked = FALSE) const;

    /* Position from motion profile. */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetMotionProfile(SPxSimMotion *motionProfile,
					  int jumpTo=TRUE);
    SPxErrorCode GetMotionProfile(SPxSimMotion **motionProfile) const;
    SPxErrorCode SetMotionOffsetSecs(double motionOffsetSecs);
    SPxErrorCode GetMotionOffsetSecs(double *motionOffsetSecs) const;
    SPxErrorCode SetMotionOffsetXMetres(double motionOffsetXMetres);
    SPxErrorCode GetMotionOffsetXMetres(double *motionOffsetXMetres) const;
    SPxErrorCode SetMotionOffsetYMetres(double motionOffsetYMetres);
    SPxErrorCode GetMotionOffsetYMetres(double *motionOffsetYMetres) const;
    SPxErrorCode SetMotionOffsetHMetres(double motionOffsetHMetres);
    SPxErrorCode GetMotionOffsetHMetres(double *motionOffsetHMetres) const;
    SPxSimMovableVis IsVisible(int mutexAlreadyLocked=FALSE) const;
    SPxErrorCode GetMotionTimeSecs(double *motionTimeSecs) const;
    SPxErrorCode JumpToMotionProfile(SPxSimMotion *motionProfile);

    /* Retrieve parent radar simulator. */
    SPxErrorCode GetRadarSim(SPxRadarSimulator **radarSim);

    /* Add/remove event callbacks. */
    template<typename F, typename O>
    SPxErrorCode AddEventCallback(F fn, O *obj)
    {
	return m_fns.Add(fn, obj);
    }
    template<typename F, typename O>
    SPxErrorCode RemoveEventCallback(F fn, O *obj)
    {
	return m_fns.Remove(fn, obj);
    }

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

protected:
    SPxEventHandler<EventHandler> m_fns;

    virtual void StateChanged(void) {};
    virtual SPxErrorCode Update(double currSecs, double diffSecs,
				SPxLatLong_tag *initPlatLatLong,
				int changed = FALSE);
    virtual void MotionProfileWrapped(void) {};
    virtual SPxErrorCode RemoveRadarSim(void);
    virtual void ApplyDesc(const char *buffer);
    void SetApplyingDesc(int applyingDesc);
    int IsApplyingDesc(void) const;
    virtual const char *GetDesc(void) const;
    virtual SPxErrorCode ApplyDelayed(void);
    virtual int CanSetInitialState(void) const { return TRUE; }

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Called by SPxRadarSimulator. */
    void SetFile(const char *filename);
    int WasFromFile(const char *filename);
    SPxErrorCode SetDelayedMotion(const char *delayedMotion);
    SPxErrorCode SignalEvent(UINT32 flags, const SPxRadarSimEventParams *params);

    /* Called by SPxSimMotion. */
    SPxErrorCode RemoveMotion(void);

    /* Private functions. */
    SPxErrorCode updateFree(double currSecs, double diffSecs, 
	SPxLatLong_tag *initPlatLatLong, int changed=FALSE);
    SPxErrorCode updateMotion(double currSecs, double diffSecs, 
	SPxLatLong_tag *initPlatLatLong, int changed=FALSE);
    static SPxErrorCode eventFnWrapper(UINT32 flags,
				       const SPxRadarSimEventParams *params,
				       void *userArg);
    SPxErrorCode eventFn(UINT32 flags);
    SPxSimMotion *findMotion(const char *name);
    SPxErrorCode setDynamicsSource(SPxSimMovableDynamicsSource dynamicsSource);
    SPxErrorCode setInitialState(void);
    SPxErrorCode getInitialState(SPxSimState *state) const;
    SPxErrorCode signalEvent(void);
    SPxErrorCode signalEvent(UINT32 flags,
			     const SPxRadarSimEventParams *params=NULL);

    friend class SPxRadarSimulator;
    friend class SPxSimMotion;

}; /* SPxSimMovable. */

#endif /* _SPX_SIM_MOVABLE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
