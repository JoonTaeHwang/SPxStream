/*********************************************************************
*
* (c) Copyright 2015, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxViewControlRendererQt.h,v $
* ID: $Id: SPxViewControlRendererQt.h,v 1.2 2015/02/09 12:36:21 rew Exp $
*
* Purpose:
*	Header for SPxViewControlRendererQt class which supports the
*	rendering of view control objects: view control soft buttons,
*	range scale, and cursor position.
*
* Revision Control:
*   09/02/15 v1.2    AGC	Use new base class soft button support.
*
* Previous Changes:
*   26/01/15 1.1    JP 	Initial Version.
* 
**********************************************************************/

#ifndef _SPX_VIEW_CONTROL_RENDERER_QT_H
#define _SPX_VIEW_CONTROL_RENDERER_QT_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxViewControlRenderer.h"
#include "SPxLibQt/SPxRendererQt.h"


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
class SPxRendererWin;
class SPxWorldMap;
class SPxTerrainDatabase;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxViewControlRendererQt : public SPxViewControlRenderer, public SPxRendererQt
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxViewControlRendererQt(SPxWorldMap *worldMap=NULL,
				       SPxTerrainDatabase *tdb=NULL);
    virtual ~SPxViewControlRendererQt(void);
    virtual SPxErrorCode Create(void);

}; /* SPxViewControlRendererQt */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_VIEW_CONTROL_RENDERER_QT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
