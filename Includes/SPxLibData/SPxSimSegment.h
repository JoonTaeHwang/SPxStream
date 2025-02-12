/*********************************************************************
*
* (c) Copyright 2012 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxSimSegment object which represents a part of
*	SPxSimMotion.
*
*
* Revision Control:
*   10/08/22 v1.17   AGC	Update speed/course accounting for motion profile offset.
*
* Previous Changes:
*   02/07/21 1.16   AGC	Add GetCentrePoint().
*   29/08/18 1.15   AGC	Performance improvements with lots of segments.
*   07/06/18 1.14   AGC	Improve start-up performance.
*   22/01/16 1.13   AGC	Derive from SPxObj.
*   01/10/15 1.12   AGC	Some functions now static and public.
*   02/06/14 1.11   AGC	Support child motions.
*   16/05/14 1.10   AGC	Support remote control.
*   04/10/13 1.9    AGC	Simplify headers.
*   14/06/13 1.8    AGC	Preserve motion shape on speed change.
*   06/06/13 1.7    AGC	Add end height option.
*   26/04/13 1.6    AGC	Support choosing position for new segments.
*   25/04/13 1.5    AGC	Support end speed segments.
*   04/03/13 1.4    AGC	Add SetStartStateAll() function.
*			Support undo segment create/delete.
*   25/01/13 1.3    AGC	Support new types of segment.
*   09/11/12 1.2    AGC	Segments no longer have start position.
*   26/10/12 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_SIM_SEGMENT_H
#define _SPX_SIM_SEGMENT_H

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxErorrCode. */
#include "SPxLibUtils/SPxError.h"

/*
 * Types
 */

/* Forward declarations. */
class SPxSimMotion;
struct SPxSimState;
struct SPxLatLong_tag;

enum SPxSimSegAccel
{
    SPX_SIM_SEG_ACCEL_LINEAR = 0,
    SPX_SIM_SEG_ACCEL_TURN_G = 1,
    SPX_SIM_SEG_ACCEL_TURN_DEGS = 2,
    SPX_SIM_SEG_ACCEL_END_SPEED = 3,
    SPX_SIM_SEG_ACCEL_END_POS = 4
};

enum SPxSimSegLength
{
    SPX_SIM_SEG_LEN_TIME = 0,
    SPX_SIM_SEG_LEN_DIST_OR_ANGLE = 1
};

/* Define our class. */
class SPxSimSegment
{
public:
    /* Public functions. */
    explicit SPxSimSegment(SPxSimMotion *motion, SPxSimSegment *nextSeg=NULL,
			   SPxSimMotion *descMotion=NULL,
			   int mutexAlreadyLocked=FALSE,
			   const SPxLatLong_tag *initPlatLatLong=NULL,
			   int swapStartStates=TRUE);
    virtual ~SPxSimSegment(void);

    SPxErrorCode SetVisible(int visible);
    int IsVisible(void) const;
    SPxErrorCode SetStartState(const SPxSimState *startState);
    SPxErrorCode GetStartState(SPxSimState *startState) const;
    SPxErrorCode EnableEndHeight(int enableEndHeight);
    int IsEndHeightEnabled(void) const;
    SPxErrorCode SetEndHeightMetres(double endHeightMetres);
    double GetEndHeightMetres(void) const;

    SPxErrorCode SetPod(double pod);
    double GetPod(void) const;

    SPxErrorCode SetAccelType(SPxSimSegAccel accel);
    SPxSimSegAccel GetAccelType(void) const;
    SPxErrorCode SetAccelLinear(double accelLinear);
    double GetAccelLinear(void) const;
    SPxErrorCode SetAccelEndSpeed(double accelEndSpeedMps);
    double GetAccelEndSpeed(void) const;
    SPxErrorCode SetAccelTurnG(double accelTurnG);
    double GetAccelTurnG(void) const;
    SPxErrorCode SetAccelTurnDegs(double accelTurnDegs);
    double GetAccelTurnDegs(void) const;
    SPxErrorCode SetAccelEndPos(const SPxLatLong_tag *accelEndPos);
    SPxErrorCode GetAccelEndPos(SPxLatLong_tag *accelEndPos) const;

    SPxErrorCode SetLengthType(SPxSimSegLength length);
    SPxSimSegLength GetLengthType(void) const;
    SPxErrorCode SetTimeSecs(double timeSecs);
    double GetTimeSecs(void) const;
    SPxErrorCode SetDistMetres(double distMetres);
    double GetDistMetres(void) const;
    SPxErrorCode SetAngleDegs(double angleDegs);
    double GetAngleDegs(void) const;
    double GetRadiusMetres(const SPxSimState *state) const;
    SPxErrorCode GetCentrePoint(const SPxSimState *state,
				SPxLatLong_tag *centreRtn) const;

    SPxSimMotion *GetMotion(void);

    SPxErrorCode Apply(SPxSimState *state, double intervalSecs,
		       const SPxPoint *offset=NULL,
		       double *radiusRtn=NULL,
		       SPxLatLong_tag *centreRtn=NULL) const;

    SPxErrorCode Move(int move);

    /* Static functions that apply segment like motion to simulated state. */
    static SPxErrorCode ApplyLinear(SPxSimState *state, double intervalSecs,
				    double accelLinear);
    static SPxErrorCode ApplyTurnG(SPxSimState *state, double intervalSecs,
				   double accelTurnG,
				   const SPxPoint *offset=NULL,
				   double *radiusRtn=NULL, SPxLatLong_tag *centreRtn=NULL);
    static SPxErrorCode ApplyTurnDegs(SPxSimState *state, double intervalSecs,
				      double accelTurnDegsPerSec,
				      const SPxPoint *offset=NULL,
				      double *radiusRtn=NULL, SPxLatLong_tag *centreRtn=NULL);

    /* Generic parameter assignment. */
    SPxErrorCode SetSegParameter(const char *name, const char *value);
    SPxErrorCode GetSegParameter(const char *name, char *valueBuf, int bufLen);

    /* Deletion callback handling, so that
     * SPxSimSegment can be used with SPxObjPtr
     * without needing to derive from SPxObj.
     */
    int AddDeletionCallback(SPxCallbackListFn_t fn, void *userPtr, int silent = FALSE);
    int RemoveDeletionCallback(SPxCallbackListFn_t fn, void *userPtr);
    int AddDeletionCallback(void *fn, void *userPtr, int silent = FALSE)
    {
	return AddDeletionCallback((SPxCallbackListFn_t)fn, userPtr, silent);
    }
    int RemoveDeletionCallback(void *fn, void *userPtr)
    {
	return RemoveDeletionCallback((SPxCallbackListFn_t)fn, userPtr);
    }

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    unsigned int m_index;

    /* Private functions. */
    SPxErrorCode applyEndSpeed(SPxSimState *state, double intervalSecs,
			       double accelEndSpeed) const;
    SPxErrorCode applyEndPos(SPxSimState *state, double intervalSecs) const;
    static SPxErrorCode undoCreate(void *userObj1, void *userObj2, void *userArg);
    static SPxErrorCode undoDelete(void *userObj1, void *userObj2, void *userArg);

    /* Called by SPxSimMotion. */
    void SetIndex(unsigned int index) { m_index = index; }
    unsigned int GetIndex(void) const { return m_index; }
    SPxErrorCode RemoveMotion(void);
    SPxErrorCode SetAccelTypeInternal(SPxSimSegAccel accelType);
    SPxErrorCode SetAccelTurnGInternal(double accelTurnG);
    SPxErrorCode SetAccelTurnDegsInternal(double accelTurnDegs);
    SPxErrorCode SetAccelEndPosInternal(const SPxLatLong_tag *accelEndPos);
    SPxErrorCode SetTimeSecsInternal(double timeSecs);
    SPxErrorCode SetStartStateInternal(const SPxSimState *startState,
				       int mutexLocked=FALSE);
    unsigned int GetID(void) const;
    const char *GetDesc(void) const;

    friend class SPxSimMotion;

}; /* SPxSimSegment. */

#endif /* _SPX_SIM_SEGMENT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
