/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxAeroNavAid class.
*
* Revision Control:
*   28/10/20 v1.5    REW	More corridor functions.
*
* Previous Changes:
*   16/10/20 1.4    REW	Support NDB.
*   15/10/20 1.3    REW	Support long names and frequencies.
*   14/10/20 1.2    REW	Add support for corridors.
*   13/10/20 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_AERO_NAV_AID_H
#define _SPX_AERO_NAV_AID_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxViewControl.h"	/* For SPxLatLong_t */

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

/* Forward-declare other classes we use. */
class SPxAeroDb;

/*
 * Define our class.  It would be overkill to derive from SPxObj.
 */
class SPxAeroNavAid
{
public:
    enum SPxANAType
    {
	SPX_ANA_TYPE_WAYPOINT = 0,
	SPX_ANA_TYPE_CORRIDOR = 1,
	SPX_ANA_TYPE_VOR = 2,
	SPX_ANA_TYPE_NDB = 3
    };

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxAeroNavAid(SPxANAType type, const char *name);
    virtual ~SPxAeroNavAid(void);

    /* Position functions. */
    SPxErrorCode SetLatLong(double latDegs, double longDegs);
    int IsLatLongSet(void) const		{ return(m_latLongSet); }
    double GetLatDegs(void) const		{ return(m_latDegs); }
    double GetLongDegs(void) const		{ return(m_longDegs); }

    /* Corridor functions. */
    SPxErrorCode SetCorridor(const char *str);
    const SPxAeroNavAid *GetNextCorridorPoint(SPxAeroDb *db,
					    const SPxAeroNavAid *prev) const;
    const SPxAeroNavAid *GetPrevCorridorPoint(SPxAeroDb *db,
					    const SPxAeroNavAid *next) const;
    const SPxAeroNavAid *GetNearestCorridorPoint(SPxAeroDb *db,
					    const SPxLatLong_t *latLong) const;
    const SPxAeroNavAid *GetFirstCorridorPoint(SPxAeroDb *db) const
    {
	return( GetNextCorridorPoint(db, NULL) );
    }
    const SPxAeroNavAid *GetLastCorridorPoint(SPxAeroDb *db) const
    {
	return( GetPrevCorridorPoint(db, NULL) );
    }
    int IsABeforeBInCorridor(const SPxAeroNavAid *a, const SPxAeroNavAid *b)
									const;

    /* Long name (not the identifier). */
    SPxErrorCode SetLongName(const char *longName);
    const char *GetLongName(void) const		{ return(m_longName); }

    /* Frequency information. */
    SPxErrorCode SetFreqMHz(double freqMHz);
    double GetFreqMHz(void) const		{ return(m_freqMHz); }

    /* Other acccess functions. */
    SPxANAType GetNavAidType(void) const	{ return(m_type); }
    const char *GetNavAidName(void) const	{ return(m_anaName); }


private:
    /*
     * Private variables.
     */
    SPxANAType m_type;		/* Type of aid */
    char *m_anaName;		/* Name of point (!= name of SPx object) */
    char *m_longName;		/* Long name/label, not just the identifier */

    /* NavAid represented by point. */
    int m_latLongSet;		/* Is position set? */
    double m_latDegs;		/* Latitude, +ve north */
    double m_longDegs;		/* Longitude, +ve east */

    /* Series of other points.  */
    char *m_corridor;		/* List of points (by name). */

    /* Frequency. */
    double m_freqMHz;		/* Zero if not set */

    /*
     * Private functions.
     */

    
}; /* SPxAeroNavAid */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_AERO_NAV_AID_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
