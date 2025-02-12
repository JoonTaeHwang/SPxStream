/*********************************************************************
*
* (c) Copyright 2013, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxExtRendererGtk.h,v $
* ID: $Id: SPxExtRendererGtk.h,v 1.1 2013/10/07 13:52:05 rew Exp $
*
* Purpose:
*	Header for SPxExtRendererGtk class which supports rendering by
*	an external object in a GTK+-based application.
*
* Revision Control:
*   07/10/13 v1.1    AGC 	Initial Version.
*
* Previous Changes:
*
**********************************************************************/

#ifndef _SPX_EXT_RENDERER_GTK_H
#define _SPX_EXT_RENDERER_GTK_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxExtRenderer.h"
#include "SPxLibX11/SPxRendererGtk.h"

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxExtRenderer;
class SPxRendererGtk;
class SPxRendererObj;
struct _cairo;

class SPxExtRendererGtk :  public SPxExtRenderer, public SPxRendererGtk
{
public:
    /* Constructor and destructor. */
    SPxExtRendererGtk(SPxRendererObj *obj);
    virtual ~SPxExtRendererGtk(void);
    struct _cairo *GetRenderer(void) { return getRenderer(); }

}; /* SPxExtRendererGtk */

#endif /* SPX_EXTDERER_GTK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
