/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxWeatherRendererD2D class which supports the display
*   of weather vectors and contours in a Direct2D-based application.
*
* Revision Control:
*   01/03/21 v1.1    AGC	Initial Version.
*
* Previous Changes:
* 
**********************************************************************/

#ifndef _SPX_WEATHER_RENDERER_D2D_H
#define _SPX_WEATHER_RENDERER_D2D_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxWeatherRenderer.h"
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
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxWeatherRenderer;
class SPxRendererD2D;
class SPxWeatherDatabase;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxWeatherRendererD2D : public SPxWeatherRenderer, public SPxRendererD2D
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxWeatherRendererD2D(SPxWeatherDatabase *database);
    virtual ~SPxWeatherRendererD2D(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

}; /* SPxWeatherRendererWin */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_WEATHER_RENDERER_D2D_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
