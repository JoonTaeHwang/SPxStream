/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxWeatherPicture class which stores and manages
*   a snapshot of weather data.
*
* Revision Control:
*   08/03/21 v1.4    BTB	Store creation time.
*
* Previous Changes:
*   26/02/21 1.3    AGC	Support use in simulator.
*   16/02/21 1.2    BTB	Make number of intensity levels externally available.
*   08/02/21 1.1    BTB	Initial Version.
**********************************************************************/

#ifndef _SPX_WEATHER_PICTURE_H
#define _SPX_WEATHER_PICTURE_H

/* Include base class. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxTime.h"

/*
 * Constants.
 */

static const unsigned int SPX_WEATHER_PIC_NUM_INTENSITY_LEVELS = 7;

/* Forward declarations. */
struct SPxLatLong_tag;

/*
 * Define our class.
 */
class SPxWeatherPicture : public SPxObj
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    SPxTime_t GetCreationTime(void) const;

    unsigned int GetNumDataItems(void) const;

    unsigned int GetNumVectors(unsigned int intensity) const;
    unsigned int GetVectorID(unsigned int intensity, unsigned int index) const;
    SPxErrorCode GetVector(unsigned int id, SPxLatLong_tag *start, SPxLatLong_tag *end) const;
    SPxErrorCode AddVector(unsigned int intensity, const SPxLatLong_tag *start, const SPxLatLong_tag *end,
                           unsigned int id = 0, unsigned int *idRtn = NULL);
    SPxErrorCode SetVectorIntensity(unsigned int id, unsigned int intensity);
    SPxErrorCode RemoveVector(unsigned int id);

    unsigned int GetNumContours(unsigned int intensity) const;
    unsigned int GetContourID(unsigned int intensity, unsigned int index) const;
    unsigned int GetNumContourPoints(unsigned int id) const;
    SPxErrorCode GetContour(unsigned int id, SPxLatLong_tag *points, unsigned int *numPoints) const;
    SPxErrorCode AddContour(unsigned int intensity, unsigned int numPoints, const SPxLatLong_tag *points,
                            unsigned int id = 0, unsigned int *idRtn = NULL);
    SPxErrorCode AddPointsToContour(unsigned int id, const SPxLatLong_tag *points, unsigned int numPoints);
    SPxErrorCode CloseContour(unsigned int id);
    SPxErrorCode SetContourIntensity(unsigned int id, unsigned int intensity);
    SPxErrorCode GetContourIntensity(unsigned int id, unsigned int *intensity) const;
    SPxErrorCode RemoveContour(unsigned int id);

    protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    /*
     * Private functions.
     */
    /* Constructor and destructor. */
    SPxWeatherPicture(void);
    virtual ~SPxWeatherPicture(void);

    friend class SPxWeatherDatabase;

}; /* SPxWeatherPicture */

#endif /* _SPX_WEATHER_PICTURE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
