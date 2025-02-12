/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxGraphicsRendererWpf class which supports the display
*   of overlay or underlay graphics in a WPF-based application.
*
* Revision Control:
*   24/09/19 v1.1    AGC	Initial Version.
*
* Previous Changes:
*
**********************************************************************/

#pragma once

/* We need the base classes. */
#include "SPxLibUtils/SPxGraphicsRenderer.h"
#include "SPxRendererWpf.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxGraphicsRenderer;
class SPxRendererWpf;
class SPxNavData;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxGraphicsRendererWpf : public SPxGraphicsRenderer, public SPxRendererWpf
{
public:
    SPxGraphicsRendererWpf(SPxNavData *navData = NULL);
    virtual ~SPxGraphicsRendererWpf(void);

}; /* SPxGraphicsRendererWpf */

/*********************************************************************
*
* End of file
*
**********************************************************************/
