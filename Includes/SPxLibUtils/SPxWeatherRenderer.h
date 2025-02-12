/*********************************************************************
*
* (c) Copyright 2021, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxWeatherRenderer base class which supports the display
*   of weather vectors and contours, but only via a derived class.
*
* Revision Control:
*   29/07/22 v1.4    AGC	Support indicating selected contour.
*
* Previous Changes:
*   01/03/21 1.3    AGC	Support hiding key.
*   16/02/21 1.2    BTB	Move configuration to SPxWeatherDatabase.
*   08/02/21 1.1    BTB	Initial Version.
* 
**********************************************************************/

#ifndef _SPX_WEATHER_RENDERER_H
#define _SPX_WEATHER_RENDERER_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxRenderer.h"

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

/* Forward declare other classes required. */
class SPxWeatherDatabase;

/*
 * Define our class, derived from the abstract renderer base class.
 */
class SPxWeatherRenderer : virtual public SPxRenderer
{
public:
    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxWeatherRenderer(SPxWeatherDatabase *database);
    virtual ~SPxWeatherRenderer(void);

    SPxErrorCode EnableShow(int enable);
    int IsShowEnabled(void) const;
    SPxErrorCode EnableEdit(int enable);
    int IsEditEnabled(void) const;

    SPxErrorCode SetShowKey(int state);
    int GetShowKey(void) const;

    SPxErrorCode SetSelectedContour(unsigned int id);

protected:
    /*
     * Protected functions.
     */

    /* Parameter setting/getting */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    SPxWeatherDatabase *m_database;

    /*
     * Private functions.
     */

    virtual SPxErrorCode renderAll(void);
    SPxErrorCode drawContour(unsigned int id,
			     UINT32 colour,
			     int filled,
			     int selected);

}; /* SPxWeatherRenderer */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_WEATHER_RENDERER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
