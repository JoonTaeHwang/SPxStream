/*********************************************************************
*
* (c) Copyright 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxSafetyNetRendererGtk.h,v $
* ID: $Id: SPxSafetyNetRendererGtk.h,v 1.1 2014/09/22 13:16:53 rew Exp $
*
* Purpose:
*	Header for SPxSafetyNetRendererGtk class which supports the display
*	of safety net alerts.
*
* Revision Control:
*   22/09/14 v1.1    REW	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_SAFETY_NET_RENDERER_GTK_H
#define _SPX_SAFETY_NET_RENDERER_GTK_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibNet/SPxSafetyNetRenderer.h"
#include "SPxLibX11/SPxRendererGtk.h"


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxSafetyNetEngine;
class SPxSafetyNetReceiver;
class SPxSafetyNetRenderer;
class SPxRendererGtk;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxSafetyNetRendererGtk : public SPxSafetyNetRenderer, public SPxRendererGtk
{
public:
    /*
     * Public functions.
     */
    SPxSafetyNetRendererGtk(SPxSafetyNetEngine *engine);
    SPxSafetyNetRendererGtk(SPxSafetyNetReceiver *receiver);
    virtual ~SPxSafetyNetRendererGtk(void);

}; /* SPxSafetyNetRendererGtk */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_SAFETY_NET_RENDERER_GTK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
