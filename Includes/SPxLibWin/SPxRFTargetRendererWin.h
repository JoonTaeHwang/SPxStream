/*********************************************************************
 *
 * (c) Copyright 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxRFTargetRendererWin class which supports
 *   the display of RF target graphics in a Windows-based application.
 *
 * Revision Control:
 *   25/10/18 v1.2   SP 	Support changes to base class.
 *
 * Previous Changes:
 *   09/01/18 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_RF_TARGET_RENDERER_WIN_H
#define _SPX_RF_TARGET_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibNet/SPxRFTargetRenderer.h"
#include "SPxLibWin/SPxRendererWin.h"


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
class SPxRFTargetRenderer;
class SPxRendererWin;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxRFTargetRendererWin : public SPxRFTargetRenderer, public SPxRendererWin
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxRFTargetRendererWin(SPxRFTargetDatabase *targetDB,
                           SPxThreatDatabase *threatDB);
    virtual ~SPxRFTargetRendererWin(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxRFTargetRendererWin */


/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* SPX_RF_TARGET_RENDERER_WIN_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
