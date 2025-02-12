/*********************************************************************
 *
 * (c) Copyright 2017 - 2018, 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxRefPoint class.
 *
 * Revision Control:
 *   28/02/20 v1.6   REW	Support altitude for reference points.
 *
 * Previous Changes:
 *   22/10/18 1.5   AGC	Suppress erroneous cppcheck warning.
 *   19/09/18 1.4   SP 	Handle nav data deletion.
 *   17/07/18 1.3   SP 	Support update from nav data.
 *   16/07/18 1.2   SP 	Add parameter support.
 *   22/08/17 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_REF_POINT_H
#define _SPX_REF_POINT_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxTimer.h"

/*********************************************************************
 *
 *   Constants
 *
 *********************************************************************/

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

/* Forward declare other classes we use. */
struct SPxRefPointPriv;
class SPxNavData;

/*
 * Define our class.
 */
class SPxRefPoint : public SPxObj
{
public:
    /*
     * Public types.
     */

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxRefPoint(void);
    virtual ~SPxRefPoint(void);

    /* General. */
    virtual SPxErrorCode SetID(UINT32 id);
    virtual UINT32 GetID(void) const;
    virtual SPxErrorCode SetDisplayName(const char *name);
    virtual const char *GetDisplayName(void) const;

    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetNavData(SPxNavData *navData);
    virtual SPxErrorCode SetLatLong(double latDegs, 
                                    double longDegs);
    virtual SPxErrorCode GetLatLong(double *latDegsRtn, 
                                    double *longDegsRtn) const;
    virtual SPxErrorCode SetAltitude(double altitudeMetres);
    virtual SPxErrorCode ClearAltitude(void);
    virtual SPxErrorCode GetAltitude(double *altitudeMetresRtn) const;
    virtual SPxErrorCode GetDistanceFromPoint(double latDegs, 
                                              double longDegs,
                                              double *distMetresRtn) const;

    /* User data. */
    virtual void SetUserData(void *ptr) { m_userData = ptr; }
    virtual void *GetUserData(void) const { return(m_userData); }

    /*
     * Public static functions.
     */

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:

    /*
     * Private variables.
     */
    SPxAutoPtr<SPxRefPointPriv> m_p;
    mutable SPxCriticalSection m_mutex;      /* Object mutex. */
    UINT32 m_id;                             /* Unique ID. */
    void *m_userData;                        /* User data. */
    double m_latDegs;                        /* Latitude in degrees. */
    double m_longDegs;                       /* Longitude in degrees. */
    double m_altitudeMetres;		     /* Altitude in metres. */
    int m_altitudeIsSet;		     /* Flag */
    SPxNavData *m_navData;                   /* Optional nav data object. */
    SPxTimer m_timer;                        /* Update timer. */

    /*
     * Private functions.
     */

    /* General. */
    void syncToNavData(void);

    /*
     * Private static functions.
     */

    static void positionUpdateTimer(void *userArg);
    static int navDataDeleteFn(void *invokingObject, 
                               void *userObject, 
                               void * /*arg*/);

}; /* SPxRefPoint */

#endif /* SPX_REF_POINT_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
