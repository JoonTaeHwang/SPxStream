/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxSafetyNetRendererD2D class which supports the display
*	of safety net alerts.
*
* Revision Control:
*   03/06/19 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_SAFETY_NET_RENDERER_D2D_H
#define _SPX_SAFETY_NET_RENDERER_D2D_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibNet/SPxSafetyNetRenderer.h"
#include "SPxLibWin/SPxRendererD2D.h"


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
class SPxSafetyNetRendererD2D: public SPxSafetyNetRenderer, public SPxRendererD2D
{
public:
    /*
     * Public functions.
     */
    explicit SPxSafetyNetRendererD2D(SPxSafetyNetEngine *engine);
    explicit SPxSafetyNetRendererD2D(SPxSafetyNetReceiver *receiver);
    virtual ~SPxSafetyNetRendererD2D(void);

protected:
    /* Protected functions. */
    virtual SPxSoftButton *CreateSoftButton(void);

}; /* SPxSafetyNetRendererD2D */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_SAFETY_NET_RENDERER_D2D_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
