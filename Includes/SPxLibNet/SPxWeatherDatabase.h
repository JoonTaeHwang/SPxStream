/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxWeatherDatabase class which supports the receipt
*   and handling of weather data messages.
*
* Revision Control:
*   08/03/21 v1.6    BTB	Add configuration for how long to consider pictures valid.
*
* Previous Changes:
*   26/02/21 1.5    AGC	Support use in simulator.
*   19/02/21 1.4    BTB	Streamline display configuration.
*   16/02/21 1.3    BTB	Add display configuration.
*   09/02/21 1.2    BTB	Don't create own ASTERIX decoder.
*   08/02/21 1.1    BTB	Initial Version.
**********************************************************************/

#ifndef _SPX_WEATHER_DATABASE_H
#define _SPX_WEATHER_DATABASE_H

#include "SPxLibUtils/SPxSharedPtr.h"

/* Include base class. */
#include "SPxLibUtils/SPxObj.h"


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxAsterixDecoder;
class SPxAsterixMsg;
class SPxWeatherPicture;

/*
 * Define our class.
 */
class SPxWeatherDatabase : public SPxObj
{
public:
    /*
     * Public types.
     */
    struct PicDeleter;
    typedef SPxSharedPtr<SPxWeatherPicture, SPxWeatherDatabase::PicDeleter> PicPtr;

    enum MonoMode
    {
        MONO_MODE_TRANSPARENCY = 0,
        MONO_MODE_GAIN = 1,
        MONO_MODE_BOTH = 2
    };

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxWeatherDatabase(void);
    virtual ~SPxWeatherDatabase(void);

    SPxErrorCode SetAsterixSource(SPxAsterixDecoder *decoder);

    PicPtr GetCurrentPicture(void);
    PicPtr CreatePendingPicture(void);
    SPxErrorCode SetPendingPictureCurrent(void);

    SPxErrorCode SetRefLatLong(double latDegs, double longDegs);
    double GetRefLatDegs(void) const { return(m_refLatDegs); }
    double GetRefLongDegs(void) const { return(m_refLongDegs); }

    SPxErrorCode SetMaxPicAge(double ageSecs);
    double GetMaxPicAge(void) const { return(m_maxPicAgeSecs); }
    SPxErrorCode SetPicInvalidationTime(double timeSecs);
    double GetPicInvalidationTime(void) const { return(m_picInvalidationTimeSecs); }

    UINT32 GetCurrentColour(unsigned int intensity) const;

    /* Functions to configure global display settings */
    SPxErrorCode SetContoursFilled(int fill);
    int GetContoursFilled(void) const;
    SPxErrorCode SetAlpha(UINT8 alpha);
    UINT8 GetAlpha(void) const;
    SPxErrorCode SetUsingMultiColour(int use);
    int GetUsingMultiColour(void) const;
    SPxErrorCode SetMonoColourMode(MonoMode mode);
    MonoMode GetMonoColourMode(void) const;
    SPxErrorCode SetMonoColour(UINT32 rgb);
    UINT32 GetMonoColour(void) const;
    SPxErrorCode SetIntensityColour(unsigned int intensity, UINT32 rgb);
    SPxErrorCode GetIntensityColour(unsigned int intensity, UINT32 *rgbRtn) const;

    /* Generic parameter assignment. */
    int SetParameter(char *parameterName, char *parameterValue);
    int GetParameter(char *parameterName, char *valueBuf, int bufLen);

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

    /* Reference location (required for conversion). */
    double m_refLatDegs;			/* Positive north */
    double m_refLongDegs;			/* Positive east */

    /* Maximum age for a picture to still be valid. */
    double m_maxPicAgeSecs;
    /* Time after a blank picture before all preceding pictures become invalid. */
    double m_picInvalidationTimeSecs;

    /*
     * Private functions.
     */

    void convertAsterixMsg(SPxAsterixMsg *msg);
    void recalcColChanSteps(void);

    /*
     * Private static functions.
     */

    static void asterixHandler(const SPxAsterixDecoder *decoder,
                               void *userArg,
                               UINT8 category,
                               SPxAsterixMsg *msg);

}; /* SPxWeatherDatabase */


#endif /* _SPX_WEATHER_DATABASE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
