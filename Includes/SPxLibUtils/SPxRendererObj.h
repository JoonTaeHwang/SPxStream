/*********************************************************************
*
* (c) Copyright 2013 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*   Abstract base class for renderer objects that can be used with an
*   external renderer, SPxExtRenderer.
*
*
* Revision Control:
*   03/05/18 v1.6    BTB	Support mouse events a better way.
*
* Previous Changes:
*   02/05/18 1.5    AGC	Support mouse events.
*   29/08/17 1.4    AGC	Add Deinit() function.
*   09/02/15 1.3    AGC	Add Init() function.
*   18/11/13 1.2    AGC	Fix clang warning.
*   04/11/13 1.1    AGC	Move to new header from SPxExtRenderer.
**********************************************************************/

#ifndef _SPX_RENDERER_OBJ_H
#define _SPX_RENDERER_OBJ_H

/*
 * Other headers required.
 */

/* For SPxErrorCode. */
#include "SPxLibUtils/SPxError.h"

/* For SPxMouseInterface */
#include "SPxLibUtils/SPxMouseControl.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxExtRenderer;

/* Interface for external renderer objects. */
class SPxRendererObj : public SPxMouseInterface
{
public:
    SPxRendererObj(void) {}
    virtual ~SPxRendererObj(void) {}
    virtual SPxErrorCode Init(SPxExtRenderer * /*renderer*/) { return SPX_NO_ERROR; }
    virtual SPxErrorCode Deinit(SPxExtRenderer * /*renderer*/) { return SPX_NO_ERROR; }
    virtual SPxErrorCode Render(SPxExtRenderer *renderer)=0;

    virtual int HandleMousePress(UINT32 /*flags*/, int /*x*/, int /*y*/, SPxCursor_t * /*cursor*/) { return FALSE; }
    virtual int HandleMouseRelease(UINT32 /*flags*/, int /*x*/, int /*y*/, SPxCursor_t * /*cursor*/) { return FALSE; }
    virtual int HandleDoubleClick(UINT32 /*flags*/, int /*x*/, int /*y*/, SPxCursor_t * /*cursor*/) { return FALSE; }
    virtual int HandleMouseMove(UINT32 /*flags*/, int /*x*/, int /*y*/, SPxCursor_t * /*cursor*/) { return FALSE; }
    virtual int HandleMouseLeave(void) { return FALSE; }
    virtual int HandleMouseWheel(UINT32 /*flags*/, int /*x*/, int /*y*/, double /*delta*/) { return FALSE; }
};

#endif /* _SPX_RENDERER_OBJ_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
