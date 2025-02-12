/*********************************************************************
*
* (c) Copyright 2014, 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxSafetyNetRenderer class, for rendering safety alerts
*	in a window.
*
*
* Revision Control:
*   30/01/20 v1.5    AGC	Add deletion callsbacks for engine/receiver.
*
* Previous Changes:
*   19/11/14 1.4    REW	Derive from SPxAlertRenderer not SPxRenderer.
*   22/10/14 1.3    REW	Remove need for map header file.
*   25/09/14 1.2    REW	Add purge control.
*   24/09/14 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_SAFETY_NET_RENDERER_H
#define _SPX_SAFETY_NET_RENDERER_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxAlertRenderer.h"		/* Our base class */

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

/*
 * Forward-declare other classes we use in case headers are in wrong order.
 */
class SPxSafetyNetEngine;
class SPxSafetyNetReceiver;
class SPxSoftButton;


/*
 * Define our class, derived from the abstract renderer base class.
 */
class SPxSafetyNetRenderer : virtual public SPxAlertRenderer
{
public:
    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    SPxSafetyNetRenderer(SPxSafetyNetEngine *engine);
    SPxSafetyNetRenderer(SPxSafetyNetReceiver *receiver);
    virtual ~SPxSafetyNetRenderer();

    /* Colour of alerts. */
    SPxErrorCode SetColour(UINT32 argb);
    UINT32 GetColour(void) const { return(m_colour); }

    /* Flashing of alerts. */
    SPxErrorCode SetFlashHz(unsigned int hz);  /* 0 for no flash */
    unsigned int GetFlashHz(void) const { return(m_flashHz); }

    /* Purging of alerts. */
    SPxErrorCode SetPurgeSecs(unsigned int secs);  /* 0 for no purge */
    unsigned int GetPurgeSecs(void) const { return(m_purgeSecs); }

    /* See base classes for other configuration functions. */

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    /*
     * Protected functions.
     */
    /* Override some functions from our base class.  */
    virtual void HandleOperatorConfirm(SPxAlert_t *alert);
    virtual void AlertIsBeingCleared(const SPxAlert_t *alert);

    /* Define some functions that our derived classes must provide. */
    virtual SPxSoftButton *CreateSoftButton(void) = 0;

private:
    /*
     * Private fields.
     */
    /* Internal implementation details. */
    struct impl;
    SPxAutoPtr<impl> m_priv;

    /* Mutex. */
    SPxCriticalSection *m_mutex;	/* Protect our internal details. */

    /* Source of alerts (one or the other, local or remote). */
    SPxSafetyNetEngine *m_alertEngine;
    SPxSafetyNetReceiver *m_alertReceiver;

    /* Appearance. */
    UINT32 m_colour;			/* ARGB */
    unsigned int m_flashHz;		/* Zero for no flash */
    unsigned int m_purgeSecs;		/* Purge alerts if no updates */

    /* Status etc. */
    SPxAlert_t *m_statusAlertButton;	/* Button for status */
    UINT32 m_statusAlertClearedMsecs;	/* When did user clear it? */
    UINT32 m_lastStatusValue;		/* Last status value shown */
    int m_statusChanged;		/* Has the status changed? */
    unsigned int m_tooManyAlerts;	/* Status flag */

    /*
     * Private functions.
     */
    /* Creation. */
    void commonConstruct(void);

    /* Alert handlers. */
    static int staticAlertHandler(void *invokingObj, void *userArg, void *arg);
    SPxErrorCode alertHandler(struct SPxSafetyNetAlert_tag *alert);

    /* Rendering. */
    virtual SPxErrorCode renderAll(void);

    /* Maintenance. */
    SPxErrorCode removeAlertFromMap(UINT16 id);

    /* Deletion callbacks. */
    static int staticEngineDeleted(void *invokingObj, void *userArg, void *arg);
    static int staticReceiverDeleted(void *invokingObj, void *userArg, void *arg);

}; /* SPxSafetyNetRenderer class */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_SAFETY_NET_RENDERER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
