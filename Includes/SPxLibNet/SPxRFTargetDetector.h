/*********************************************************************
 *
 * (c) Copyright 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxRFTargetDetector base class.
 *
 * Revision Control:
 *   25/10/18 v1.10  SP 	Move database bits to SPxRFTargetDatabase.
 *
 * Previous Changes:
 *   22/10/18 1.9   SP 	Add GetSourceName().
 *   18/10/18 1.8   SP 	Fix argument type.
 *   17/10/18 1.7   SP 	Further changes to support new detector.
 *   15/10/18 1.6   SP 	Move detector specific code to a derived class.
 *   31/01/18 1.5   SP 	Allow detector to be created without
 *                      starting decode thread.
 *   25/01/18 1.4   SP 	Support purging of non-updating targets.
 *   24/01/18 1.3   SP 	Add optional filter arg to GetTarget().
 *   10/01/18 1.2   SP 	Minor changes.
 *   09/01/18 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_RF_TARGET_DETECTOR_H
#define _SPX_RF_TARGET_DETECTOR_H

/*
 * Other headers required.
 */

/* Other classes that we need. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxAutoPtr.h"

/* Forward declare any classes required. */
class SPxRFTargetDatabase;

/*********************************************************************
 *
 *   Constants
 *
 *********************************************************************/

/* Parameters. */

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

/*
 * Define our class.
 */
class SPxRFTargetDetector : public SPxObj
{
    /* The objects these can access our internals directly. */
    friend class SPxRFTarget;

public:

    /*
     * Public types.
     */

    /* Types of detector. */
    typedef enum
    {
        DTR_TYPE_NONE,
        DTR_TYPE_REDRONE,   /* ReDrone. */
        DTR_TYPE_ARGOS      /* MyDefence Watchdog, Wolfpack & Wingman. */

    } DtrType_t;

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxRFTargetDetector(SPxRFTargetDatabase *database);
    virtual ~SPxRFTargetDetector(void);
    virtual SPxErrorCode Create(void);
    virtual SPxErrorCode Shutdown(void);

    /* Access. */
    virtual SPxRFTargetDatabase *GetDatabase(void) { return m_database; }
    virtual UINT32 GetDetectorID(void) { return m_detectorID; }

    /* Source info. */
    virtual SPxErrorCode IterateSources(SPxCallbackListFn_t fn, void *userArg);

    /* Target size. */
    virtual double GetTargetWidthDegs(void) { return m_targetWidthDegs; }
    virtual double GetTargetLengthMetres(void) { return m_targetLengthMetres; }

    /* Debug. */
    virtual void SetLogFile(FILE *logFile);
    virtual void SetVerbosity(unsigned int verbosity);
    virtual unsigned int GetVerbosity(void);

protected:

    /*
     * Protected variables.
     */

    /* General. */
    FILE *m_logFile;                        /* Debug log file. */
    unsigned int m_verbosity;               /* Debug versbosity. */

    /*
     * Protected functions.
     */

    /* Access. */
    double getLatDegs(void) { return m_latDegs; }
    double getLongDegs(void) { return m_longDegs; }
    double getBearingDegs(void) { return m_bearingDegs; }

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:

    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    /* Private member variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* General. */
    SPxRFTargetDatabase *m_database;        /* Parent database. */
    UINT32 m_detectorID;                    /* ID of this detector. */
    double m_latDegs;                       /* Latitude in degrees. */
    double m_longDegs;                      /* Longitude in degrees. */
    double m_bearingDegs;                   /* Bearing offset in degrees. */
    double m_targetWidthDegs;               /* Target sector width in degrees. */
    double m_targetLengthMetres;            /* Target sector length in metres. */

    /*
     * Private functions.
     */

    /*
     * Private static functions.
     */

}; /* SPxRFTargetDetector */

/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* SPX_RF_TARGET_DETECTOR_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
