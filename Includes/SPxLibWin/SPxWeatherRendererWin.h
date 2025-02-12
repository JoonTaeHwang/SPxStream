/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxWeatherRendererWin class which supports the display
*   of weather vectors and contours in a Windows-based application.
*
* Revision Control:
*   16/02/21 v1.1    BTB	Initial Version.
*
* Previous Changes:
* 
**********************************************************************/

#ifndef _SPX_WEATHER_RENDERER_WIN_H
#define _SPX_WEATHER_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxWeatherRenderer.h"
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
class SPxWeatherRenderer;
class SPxRendererWin;
class SPxWeatherDatabase;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxWeatherRendererWin : public SPxWeatherRenderer, public SPxRendererWin
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxWeatherRendererWin(SPxWeatherDatabase *database);
    virtual ~SPxWeatherRendererWin(void);

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

#endif /* _SPX_WEATHER_RENDERER_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
