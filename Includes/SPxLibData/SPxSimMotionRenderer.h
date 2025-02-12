/*********************************************************************
*
* (c) Copyright 2012 - 2016, 2019, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for SPxSimMotionRenderer base class which supports the
*	rendering of simulated motions from a radar simulator.
*
* Revision Control:
*   11/07/19 v1.23   AGC	Smoother motion profile rendering.
*
* Previous Changes:
*   03/06/19 1.22   AGC	Handle motion only deletion.
*   08/05/18 1.21   BTB	Move drawCaption to SPxRenderer to allow wider use.
*   23/08/16 1.20   AGC	Event function prototype updated.
*   22/01/16 1.19   AGC	Mutex protection for segment access.
*   11/01/16 1.18   AGC	Reduce calls to Radar Simulator to get initial platform location.
*   07/11/14 1.17   AGC	Update radar sim event handler.
*   04/07/14 1.16   AGC	Improve mouse interaction with other renderers.
*   26/06/14 1.15   AGC	Support use with single SPxSimMotion object.
*			Revert to correct mode in SetMode().
*   06/06/14 1.14   AGC	Override base class enabled command.
*   02/06/14 1.13   AGC	Move motion/segment creation here from new motion dialog.
*			Add snapping and captions.
*   16/05/14 1.12   AGC	Add context menu item.
*   07/10/13 1.11   AGC	Move rendering to renderAll().
*   25/04/13 1.10   AGC	Add display of tangent at end of current motion.
*   28/03/13 1.9    AGC	Draw message about Shift key for fixed move.
*   18/03/13 1.8    AGC	Draw link for targets with motion offsets.
*   08/03/13 1.7    AGC	Use max motion turn rate limits.
*   04/03/13 1.6    AGC	Support editing motions with the mouse.
*			Finish motion with double click.
*			Support undo motion edit.
*   12/02/13 1.5    AGC	Initial support for heading-up display.
*   25/01/13 1.4    AGC	Allow view changes while creating new motion.
*			Add index to segment iteration.
*   23/11/12 1.3    AGC	Interface with SPxMouseControl/SPxMouseInterface.
*   09/11/12 1.2    AGC	Add callHandler option to FindNearMotion().
*   26/10/12 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_SIM_MOTION_RENDERER_H
#define _SPX_SIM_MOTION_RENDERER_H

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For base class. */
#include "SPxLibUtils/SPxRenderer.h"

/*
 * Types
 */

/* Forward declarations. */
class SPxRadarSimulator;
struct SPxLatLong_tag;
class SPxSimMotion;
class SPxSimSegment;
struct SPxSimState;
struct SPxSimSegState;
class SPxSimTarget;
class SPxSimMotionNewDlgWin;
struct SPxRadarSimEventParams;

enum SPxSimMotionRendererMode
{
    SPX_SIM_MOTION_RENDERER_MODE_PASSIVE = 0,
    SPX_SIM_MOTION_RENDERER_MODE_EDIT = 1,
    SPX_SIM_MOTION_RENDERER_MODE_CREATE = 2
};

enum SPxSimMotionRendererNewSegMode
{
    SPX_SIM_MOTION_RENDERER_NEW_SEG_MODE_MIXED = 0,
    SPX_SIM_MOTION_RENDERER_NEW_SEG_MODE_CURVE = 1,
    SPX_SIM_MOTION_RENDERER_NEW_SEG_MODE_STRAIGHT = 2,
    SPX_SIM_MOTION_RENDERER_NEW_SEG_MODE_STRAIGHT_MAX_TURN = 3
};

typedef void (*SPxSimMotionRendererSetMotionFn_t)(SPxSimMotion *motion,
						  SPxSimSegment *segment,
						  void *userArg);

/* Define our class */
class SPxSimMotionRenderer : virtual public SPxRenderer
{
public:
    /* Constructor and destructor. */
    explicit SPxSimMotionRenderer(SPxRadarSimulator *radarSim);
    explicit SPxSimMotionRenderer(SPxSimMotion *motion);
    virtual ~SPxSimMotionRenderer(void);

    /* Mouse handling. */
    virtual int HandleMousePress(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseRelease(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseLeave(void);
    virtual int HandleMouseMove(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleDoubleClick(UINT32 flags, int x, int y, SPxCursor_t *cursor);

    SPxErrorCode ShowOnlySelected(int state);
    int IsShowingOnlySelected(void) const;

    /* Set current mode. */
    SPxErrorCode SetMode(SPxSimMotionRendererMode mode, int undo=FALSE);
    SPxSimMotionRendererMode GetMode(void) const;
    SPxErrorCode UndoCreate(void);

    SPxErrorCode SetNewSegmentMode(SPxSimMotionRendererNewSegMode newSegMode);
    SPxSimMotionRendererNewSegMode GetNewSegmentMode(void) const;
    SPxErrorCode SetInitialSpeed(double speedMps);
    double GetInitialSpeed(void) const;
    SPxErrorCode SetInitialHeight(double heightMetres);
    double GetInitialHeight(void) const;
    SPxErrorCode SetNewMotionDialog(SPxSimMotionNewDlgWin *dlg);

    /* Set current motion/segment. */
    SPxErrorCode SetCurrMotion(SPxSimMotion *motion, SPxSimSegment *segment);
    SPxSimMotion *GetCurrMotion(void);
    SPxSimSegment *GetCurrSegment(void);

    /* Callbacks for setting the current motion/segment. */
    SPxErrorCode AddSetMotionCallback(SPxSimMotionRendererSetMotionFn_t fn, void *userArg);
    SPxErrorCode RemoveSetMotionCallback(SPxSimMotionRendererSetMotionFn_t fn, void *userArg);

    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    int setCurrMotion(SPxSimMotion *motion, SPxSimSegment *segment);
    void saveMousePos(int x, int y, int inView=TRUE);
    virtual SPxErrorCode renderAll(void);
    int findNearMotion(int x, int y);
    int findNearJoin(int x, int y);
    void setEditCursor(int flags, SPxCursor_t *cursor);
    void setCreateCursor(int flags);
    int isRestrictedMoveAvailable(void) const;
    int startMove(int x, int y);
    void stopMove(void);
    int selectMotion(int x, int y);
    int moveEditSegment(UINT32 flags, int x, int y);
    int moveCreateSegment(UINT32 flags, int x, int y);
    int createSegment(UINT32 flags, int x, int y);
    void resetMaxTurn(void);
    void removeLastSeg(void);
    void closeNewMotionDialog(SPxSimMotionRendererMode newMode=SPX_SIM_MOTION_RENDERER_MODE_EDIT);
    void checkCurrMotion(void);
    void addUndoSeg(SPxSimSegment *seg1, SPxSimSegment *seg2);
    void addUndoMotion(SPxSimSegment *seg, SPxSimMotion *motion);
    int isMaxTurn(SPxSimSegment *seg) const;
    SPxErrorCode modifyTurnSegment(SPxSimMotion *motion, SPxSimSegment *segment,
	SPxSimState *startState, SPxLatLong_t *endLatLong);
    SPxErrorCode modifyMaxTurnSegment(SPxSimMotion *motion, SPxSimSegment *segment,
	SPxSimState *startState, SPxLatLong_t *endLatLong);
    static SPxErrorCode findMotionWrapper(unsigned int index, SPxSimMotion *motion, void *userArg);
    SPxErrorCode findMotion(unsigned int index, SPxSimMotion *motion);
    static SPxErrorCode findSegmentWrapper(unsigned int index, SPxSimSegment *segment, void *userArg);
    SPxErrorCode findSegment(unsigned int index, SPxSimSegment *segment);
    static SPxErrorCode drawMotionWrapper(unsigned int index, SPxSimMotion *motion, void *userArg);
    SPxErrorCode drawMotion(unsigned int index, SPxSimMotion *motion);
    static SPxErrorCode drawSegmentWrapper(unsigned int index, SPxSimSegment *segment, void *userArg);
    SPxErrorCode drawSegment(unsigned int index, SPxSimSegment *segment);
    static SPxErrorCode drawMotionTargetLinkWrapper(unsigned int index, SPxSimTarget *target, void *userArg);
    SPxErrorCode drawMotionTargetLink(unsigned int index, SPxSimTarget *target);
    SPxErrorCode drawMarkerStart(UINT32 colour, SPxSimState *state);
    SPxErrorCode drawMarkerEnd(UINT32 colour, SPxSimState *state);
    SPxErrorCode drawMotionName(const SPxSimState *state, UINT32 colour);
    int checkDirection(int xPixels, int yPixels, double courseDegs);
    SPxErrorCode getMoveCircle(SPxRendererXYD_t *centre, double *radiusMetres,
	double *angleDegs, double *sweepDegs, SPxLatLong_t *centreLL, int fixed);
    static void createUndoSegment(SPxSimSegment *seg, SPxSimSegState *state);
    static SPxErrorCode undoFn(void *userObj1, void *userObj2, void *userArg);
    static void undoSegMove(SPxSimSegment *seg, SPxSimSegState *state);
    static SPxErrorCode undoSegCreate(void *userObj, void *userObj2, void *userArg);
    static SPxErrorCode undoSegSplit(void *userObj, void *userObj2, void *userArg);
    void undoAll(void);
    static SPxErrorCode updateUndoActionsRem(void *userArg, unsigned int id, const char *name,
					     void *userObj, void *userObj2, void *undoUserArg);
    static SPxErrorCode updateUndoActionsAdd(void *userArg, unsigned int id, const char *name,
					     void *userObj, void *userObj2, void *undoUserArg);
    static SPxErrorCode countUndoFn(void *userArg, unsigned int id, const char *name,
				    void *userObj, void *userObj2, void *undoUserArg);
    static SPxErrorCode eventFnWrapper(UINT32 flags, const SPxRadarSimEventParams *params, void *userArg);
    SPxErrorCode eventFn(UINT32 flags);
    static SPxErrorCode checkMotion(unsigned int index, SPxSimMotion *motion, void *userArg);
    static SPxErrorCode checkSegment(unsigned int index, SPxSimSegment *segment, void *userArg);
    static void splitSegmentWrapper(SPxPopupMenu *menu, int id, void *userArg);
    void splitSegment(void);
    static int motionDeletedFn(void *invokingObject,
			       void *userObject,
			       void *arg);

}; /* SPxSimMotionRenderer */

#endif /* _SPX_SIM_MOTION_RENDERER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
