/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxExtRendererWpf class which supports the display
*   of external graphics in a WPF-based application.
*
* Revision Control:
*   29/06/21 v1.1    AGC	Initial Version.
*
* Previous Changes:
*
**********************************************************************/

#pragma once

/* We need the base classes. */
#include "SPxLibUtils/SPxExtRenderer.h"
#include "SPxRendererWpf.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxExtRenderer;
class SPxRendererWpf;
class SPxRendererObj;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxExtRendererWpf : public SPxExtRenderer, public SPxRendererWpf
{
public:
    explicit SPxExtRendererWpf(SPxRendererObj *rendererObj);
    virtual ~SPxExtRendererWpf(void);

}; /* SPxExtRendererWpf */

/*********************************************************************
*
* End of file
*
**********************************************************************/
