/*********************************************************************
*
* (c) Copyright 2011, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxAISRendererWin.h,v $
* ID: $Id: SPxAISRendererWin.h,v 1.4 2011/04/13 07:24:35 rew Exp $
*
* Purpose:
*	Header for SPxAISRendererWin class which supports the display
*	of AIS messages in a Windows-based application.
*
* Revision Control:
*   12/04/11 v1.4    SP 	Remove unused Get/SetParameter() functions.
*
* Previous Changes:
*   06/04/11 1.3    SP 	Also derive from SPxRendererWin.
*   04/03/11 1.2    SP 	Implement class.
*   02/03/11 1.1    REW	Initial Version, stubbed out.
**********************************************************************/

#ifndef _SPX_AIS_RENDERER_WIN_H
#define _SPX_AIS_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibNet/SPxAISRenderer.h"
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
class SPxAISRenderer;
class SPxRendererWin;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxAISRendererWin : public SPxAISRenderer, public SPxRendererWin
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxAISRendererWin(SPxAISDecoder *decoder);
    virtual ~SPxAISRendererWin(void);

protected:
    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

    /*
     * Private functions.
     */

}; /* SPxAISRendererWin */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_AIS_RENDERER_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
