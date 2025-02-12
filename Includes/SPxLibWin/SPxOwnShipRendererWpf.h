/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxOwnShipRendererWpf class which supports the display
*   of an own ship marker in a WPF-based application.
*
* Revision Control:
*   18/10/19 v1.1    BTB 	Initial Version.
*
* Previous Changes:
* 
**********************************************************************/

#pragma once

/* We need the base classes. */
#include "SPxLibUtils/SPxOwnShipRenderer.h"
#include "SPxRendererWpf.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxOwnShipRenderer;
class SPxRendererWpf;
class SPxNavData;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxOwnShipRendererWpf : public SPxOwnShipRenderer, public SPxRendererWpf
{
public:
    SPxOwnShipRendererWpf(SPxNavData *navData = NULL);
    virtual ~SPxOwnShipRendererWpf(void);

}; /* SPxOwnShipRendererWpf */

/*********************************************************************
*
* End of file
*
**********************************************************************/
