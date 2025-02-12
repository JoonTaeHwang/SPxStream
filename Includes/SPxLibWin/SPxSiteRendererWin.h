/*********************************************************************
 *
 * (c) Copyright 2019, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxSiteRenderer base class which supports the 
 *   display of objects contains in a SPxSiteDatabase in a 
 *   Windows-based application.
 *
 * Revision Control:
 *   11/12/19 v1.1   SP 	Initial version.
 *
 * Previous Changes:
 *
 **********************************************************************/

#ifndef _SPX_SITE_RENDERER_WIN_H
#define _SPX_SITE_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxSiteRenderer.h"
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
 **********************************************************************/


/*********************************************************************
 *
 *   Type definitions
 *
 **********************************************************************/

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxSiteRenderer;
class SPxRendererWin;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxSiteRendererWin : public SPxSiteRenderer, public SPxRendererWin
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxSiteRendererWin(SPxSiteDatabase *database);
    virtual ~SPxSiteRendererWin(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxSiteRendererWin */


/*********************************************************************
 *
 *   Function prototypes
 *
 **********************************************************************/

#endif /* SPX_SITE_RENDERER_WIN_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
