/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxViewControlRendererWpf class which supports the 
*	display of view control buttons in a WPF-based application.
*
* Revision Control:
*   03/02/20 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef SPX_VIEW_CONTROL_RENDERER_WPF_H
#define SPX_VIEW_CONTROL_RENDERER_WPF_H

/*
 * Other headers required.
 */

 /* We need the base classes. */
#include "SPxLibUtils/SPxViewControlRenderer.h"
#include "SPxLibWin/SPxRendererWpf.h"


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
class SPxViewControlRenderer;
class SPxRendererWpf;
class SPxWorldMap;
class SPxTerrainDatabase;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxViewControlRendererWpf : public SPxViewControlRenderer, public SPxRendererWpf
{
public:
    /* Constructor and destructor. */
    explicit SPxViewControlRendererWpf(SPxWorldMap *worldMap = NULL,
                                       SPxTerrainDatabase *tdb = NULL);
    virtual ~SPxViewControlRendererWpf(void);

private:

}; /* SPxViewControlRendererWpf */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_VIEW_CONTROL_RENDERER_WPF_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
