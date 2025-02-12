/*********************************************************************
*
* (c) Copyright 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxSafetyNetRendererWin.h,v $
* ID: $Id: SPxSafetyNetRendererWin.h,v 1.2 2014/11/19 14:16:03 rew Exp $
*
* Purpose:
*	Header for SPxSafetyNetRendererWin class which supports the display
*	of safety net alerts.
*
* Revision Control:
*   19/11/14 v1.2    REW	Implement CreateSoftButton().
*
* Previous Changes:
*   22/09/14 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_SAFETY_NET_RENDERER_WIN_H
#define _SPX_SAFETY_NET_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibNet/SPxSafetyNetRenderer.h"
#include "SPxLibWin/SPxRendererWin.h"


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxSafetyNetEngine;
class SPxSafetyNetReceiver;
class SPxSafetyNetRenderer;
class SPxSoftButton;
class SPxRendererWin;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxSafetyNetRendererWin : public SPxSafetyNetRenderer, public SPxRendererWin
{
public:
    /*
     * Public functions.
     */
    SPxSafetyNetRendererWin(SPxSafetyNetEngine *engine);
    SPxSafetyNetRendererWin(SPxSafetyNetReceiver *receiver);
    virtual ~SPxSafetyNetRendererWin(void);

protected:
    /* Protected functions. */
    virtual SPxSoftButton *CreateSoftButton(void);

}; /* SPxSafetyNetRendererWin */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_SAFETY_NET_RENDERER_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
