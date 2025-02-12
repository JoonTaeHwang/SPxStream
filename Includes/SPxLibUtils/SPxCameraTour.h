/*********************************************************************
*
* (c) Copyright 2019 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxCameraTour class, used to define a tour for a specific
*   SPxCamera object.
*
* Revision Control:
*   14/06/22 v1.14  BTB	Add ReloadTourDefinition() and GetNextStage().
*
* Previous Changes:
*   17/06/21 1.13  AJH	Add remote control for camera tour stages.
*   22/03/21 1.12  BTB	Support reloading/changing tours.
*   01/09/20 1.11  BTB	Support reversing at the end of a tour.
*   07/07/20 1.10  AGC	Improve current stage indication.
*   07/07/20 1.9   AGC	Improve behaviour with slave cameras.
*   07/07/20 1.8   AGC	Add HasChanges() function.
*   03/07/20 1.7   AGC	Improvements with slave cameras.
*   19/02/20 1.6   BTB	Move parameter definitions to source file.
*   23/08/19 1.5   BTB	Fully manage current tour stage internally.
*   22/08/19 1.4   BTB	Add DeleteAllStages().
*   21/08/19 1.3   BTB	Support editing of tours.
*   04/06/19 1.2   BTB	Change all times to be in seconds.
*   31/05/19 1.1   BTB	Initial version.
**********************************************************************/

#ifndef _SPX_CAMERA_TOUR_H
#define _SPX_CAMERA_TOUR_H

/* Library headers. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"

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

/* Information for a single stage of the tour. */
struct SPxCameraTourStage
{
    SPxCameraTourStage()
	: bearingDegs(0.0),
	tiltDegs(0.0),
	fovDegs(0.0),
	fovDegsSecondary(0.0),
	waitTimeSecs(0.0),
	bearingSet(FALSE),
	tiltSet(FALSE),
	fovSet(FALSE),
	fovSecondarySet(FALSE)
    {}

    double bearingDegs;
    double tiltDegs;
    double fovDegs;
    double fovDegsSecondary;
    double waitTimeSecs;

    /* Flags to state which elements of camera
     * movement have been specified for this stage.
     */
    int bearingSet;
    int tiltSet;
    int fovSet;
    int fovSecondarySet;
};

/* Forward declare any classes we use. */
class SPxCallbackList;

/*
 * Define our class.
 */
class SPxCameraTour : public SPxObj
{
public:

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxCameraTour(void);
    virtual ~SPxCameraTour(void);

    /* Manage inactive period before tour should start. */
    virtual void SetIdleTimeSecs(UINT32 time);
    virtual UINT32 GetIdleTimeSecs(void) const;

    /* Get/set whether tour editing is enabled. */
    virtual void EnableEditing(int enable);
    virtual int EditingEnabled(void) const { return m_editEnabled; }

    /* Get/set whether or not the camera is currently on tour. */
    virtual void SetOnTour(int onTour, int isSlave);
    virtual int GetOnTour(void) const;

    /* Tour editing. */
    virtual SPxErrorCode SaveTourDefinition(void);
    virtual SPxErrorCode ReloadTourDefinition(void);
    virtual int HasChanges(void) const;
    virtual SPxErrorCode AppendStage(const SPxCameraTourStage *stage);
    virtual SPxErrorCode InsertStage(const SPxCameraTourStage *stage, unsigned int stageIndex);
    virtual SPxErrorCode UpdateStage(const SPxCameraTourStage *stage, unsigned int stageIndex);
    virtual SPxErrorCode DeleteStage(unsigned int stageIndex);
    virtual SPxErrorCode DeleteAllStages(void);
    virtual SPxErrorCode SetWaitTime(unsigned int stageIndex, double waitTimeSecs);

    /* Stage information. */
    virtual unsigned int GetNumStages(void);
    virtual SPxErrorCode SetCurrentStage(unsigned int stageIndex);
    virtual SPxErrorCode GetCurrentStage(unsigned int *stageIndex) const;
    virtual SPxErrorCode GetNextStage(unsigned int *stageIndex) const;
    virtual SPxErrorCode GetStageInfo(unsigned int stageIndex, SPxCameraTourStage *stageRtn) const;
    virtual int IsMovingToStage(int isSlave = FALSE);
    virtual void UpdatePosition(double panDegs, int panSupported,
				double tiltDegs, int tiltSupported,
				double fovDegs, int fovSupported,
				double fovDegsSec, int fovSecSupported,
				int isSlave=FALSE);
    virtual int GetStageInfoToMoveTo(SPxCameraTourStage *stageRtn,
				     int isSlave=FALSE);

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);
    virtual SPxErrorCode SetStateFromConfig(void);

    /* Callback handling. */
    virtual SPxErrorCode AddUpdateCallback(SPxCallbackListFn_t fn, void *userObj);
    virtual SPxErrorCode RemoveUpdateCallback(SPxCallbackListFn_t fn, void *userObj);

protected:

private:

    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;	/* Private structure. */

    unsigned int m_currentStage;    /* Stage the tour is currently on. */
    UINT32 m_idleTimeSecs;	/* Time camera needs to be idle before starting tour. */
    double m_defaultStageWaitTimeSecs;	/* Default time to wait at each stage. */
    UINT32 m_maxMoveTimeSecs;	/* Max time allowed for a moving to a stage. */
    int m_reverseAtEnd;         /* Whether to move back in reverse on reaching the end stage. */
    int m_editEnabled;		/* Whether or not tour editing is enabled. */
    int m_onTour;		/* Whether or not the camera is currently on tour. */
    int m_isReversed;           /* Is the tour currently moving in reverse? */
    int m_slaveCameraEnabled;	/* Is a slave camera in use? */
    int m_onTourSlave;		/* Does the slave camera think the camera should be on tour. */
    int m_moveInProgress;	/* Whether or not the camera is currently moving to a stage. */
    int m_moveInProgressSlave;	/* Is a slave camera currently moving to a stage? */
    UINT32 m_moveStartTimeMs;	/* When the current move started. */
    UINT32 m_moveStartTimeMsSlave; /* When the current slave camera move started. */
    int m_atStage;		/* Whether or not the camera is currently at a stage. */
    UINT32 m_stageArriveTimeMs;	/* When the camera got to the current stage. */
    SPxAutoPtr<SPxCallbackList> m_updateCallbacks; /* Callback list to be invoked on updates (e.g. reloading). */

    /*
     * Private functions.
     */

    /* Manage default time to wait at each stage. */
    virtual void setDefaultStageWaitTimeSecs(double time);
    virtual double getDefaultStageWaitTimeSecs(void);

    /* Manage max time allowed for moving to a stage. */
    virtual void setMaxMoveTimeSecs(UINT32 time);
    virtual UINT32 getMaxMoveTimeSecs(void);

    /* Manage whether to move in reverse on reaching the end of the tour. */
    virtual void setIsReversingAtEnd(int enable);
    virtual int getIsReversingAtEnd(void);

    /* Manage tour definition file. */
    SPxErrorCode setDefinitionFilename(const char *filename);
    SPxErrorCode getDefinitionFilename(char *buffer, int bufLen) const;
    SPxErrorCode loadDefinition(void);
    static SPxErrorCode parseStage(char *stageStr, SPxCameraTourStage *stageRtn);

}; /* SPxCameraTour */

#endif /* SPX_CAMERA_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
