/*********************************************************************
 *
 * (c) Copyright 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxDetectedObjRendererWin class which supports
 *   rendering in a Windows-based application using Direct2D.
 *
 * Revision Control:
 *   25/06/21 v1.1   SP 	Initial version.
 *
 * Previous Changes:
 *
 *********************************************************************/

#ifndef _SPX_DETECTED_OBJ_D2D_H
#define _SPX_DETECTED_OBJ_D2D_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxDetectedObjRenderer.h"
#include "SPxLibWin/SPxRendererD2D.h"


/*********************************************************************
 *
 *   Constants
 *
 **********************************************************************/


/*********************************************************************
 *
 *   Macros
 *
 *********************************************************************/


/*********************************************************************
 *
 *   Type definitions
 *
 *********************************************************************/

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxDetectedObjRenderer;
class SPxRendererD2D;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxDetectedObjRendererD2D : public SPxDetectedObjRenderer, public SPxRendererD2D
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxDetectedObjRendererD2D(SPxObjectDetector *detector);
    virtual ~SPxDetectedObjRendererD2D(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxDetectedObjRendererWin */


/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* SPX_DETECTED_OBJ_D2D_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
