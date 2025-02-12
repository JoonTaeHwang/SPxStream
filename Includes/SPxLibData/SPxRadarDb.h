/*********************************************************************
*
* (c) Copyright 2007 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for Radar Database class.
*
* Revision Control:
*   09/01/19 v1.13   AGC	Fix build warnings.
*
* Previous Changes:
*   09/01/19 1.12   AGC	Support hiding GetParameter() errors.
*   12/04/17 1.11   AGC	Improve mutex protection.
*   04/10/13 1.10   AGC	Simplify headers.
*   29/03/12 1.9    AGC	Use const for some parameters.
*   28/10/10 1.8    AGC	Add SetParameter()/GetParameter().
*   15/09/10 1.7    REW	Make destructor virtual.
*   20/08/07 1.6    DGJ	Add various Set functions.
*   02/07/07 1.5    REW	Make addEntry() public, so AddEntry().
*   29/06/07 1.4    DGJ	Derive from SPxObj
*   07/06/07 1.3    REW	Tidy up, comments, reorder, includes.
*   24/05/07 1.2    REW	Added header, changed to SPx etc.
*   17/04/07 1.1    DGJ	Initial Version
**********************************************************************/

#ifndef _SPX_RADAR_DB_H
#define _SPX_RADAR_DB_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types, errors etc. */
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Maximum length of radar name. */
#define	SPX_RADARDB_MAX_NAME_LEN	64


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

/*
 * Define the class that represents a single radar in the database.
 */
class SPxRadar :public SPxObj
{
private:
    /*
     * Private fields.
     */
    mutable SPxCriticalSection m_mutex;

    /* Name and ID */
    char m_name[SPX_RADARDB_MAX_NAME_LEN];	/* Simple text name */
    int m_id;					/* A reference id. */

    /* Position */
    double m_x;					/* World x position */
    double m_y;					/* World y position */

    /* Expected characteristics. */
    double m_scanPeriod;			/* Expected period in secs */
    double m_prf;				/* Expected prf in Hz */
    double m_range;				/* Range in world units */
    double m_beamWidth;				/* Beam width in degrees. */

    /* Linked list support. */
    SPxRadar *m_next;				/* Next link in chain */

    /*
     * Private functions.
     */

public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor. */
    SPxRadar(const char *name, double x, double y, int id, 
	    double period, double prf, double range, double beamWidth);

    void SetRadarName(const char*);
    void SetRange(double);
    void SetPosition(double x, double y);
    void SetBeamwidth(double beamwidth);
    void SetPeriod(double period);
    void SetPrf(double prf);
    const char *GetName(void) const;
    double GetRange(void) const;
    double GetX(void) const;
    double GetY(void) const;
    double GetBeamwidth(void) const;
    int GetID(void) const;
    double GetPeriod(void) const;
    double GetPrf(void)	const;

    /* Linked list support. */
    SPxRadar *GetNext(void)
    {
	return m_next;
    }
    void SetNext(SPxRadar *next)
    {
	m_next = next;
    }
}; /* SPxRadar */


/*
 * Define the class that represents an entire database of radars.
 */
class SPxRadarDatabase :public SPxObj
{
private:
    /*
     * Private fields.
     */
    /* List of radars being maintained by database. */
    int m_numRadars;
    SPxRadar *m_firstRadar;
    SPxRadar *m_lastRadar;

    /*
     * Private functions.
     */

public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor etc. */
    SPxRadarDatabase(void);
    virtual ~SPxRadarDatabase(void);

    /* Information retrieval. */
    int GetNumRadars(void)
    {
	return m_numRadars;
    }
    SPxRadar *GetFirst(void)
    {
	return m_firstRadar;
    }

    /* Search functions. */
    SPxRadar * FindRadarByName(const char *);
    SPxRadar * FindRadarByID(int);

    /* Functions for adding radars to the database. */
    SPxErrorCode Open(const char *fileName);
    SPxErrorCode AddEntry(const char *name,
			double x, double y,
			int id,
			double period, double prf,
			double range, double beamWidth);

    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen)
    {
	return GetParameter(parameterName, valueBuf, bufLen, FALSE);
    }
    int GetParameter(char *parameterName, char *valueBuf, int bufLen, int genErrors);

private:
    using SPxObj::GetParameter;

}; /* SPxRadarDatabase */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_RADAR_DB_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
