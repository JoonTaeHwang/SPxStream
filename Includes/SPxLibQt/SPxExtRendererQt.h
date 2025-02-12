/*********************************************************************
*
* (c) Copyright 2015, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxExtRendererQt class which supports rendering by
*	an external object in a Qt based application.
*
* Revision Control:
*   10/04/15 v1.1    JP 	Initial Version.
*
* Previous Changes:
*
**********************************************************************/

#ifndef _SPX_EXT_RENDERER_QT_H
#define _SPX_EXT_RENDERER_QT_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxExtRenderer.h"
#include "SPxLibQt/SPxRendererQt.h"

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxExtRenderer;
class SPxRendererQt;
class SPxRendererObj;

class SPxExtRendererQt :  public SPxExtRenderer, public SPxRendererQt
{
public:
    /* Constructor and destructor. */
    SPxExtRendererQt(SPxRendererObj *obj);
    virtual ~SPxExtRendererQt(void);

}; /* SPxExtRendererQt */

#endif /* _SPX_EXT_RENDERER_QT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
