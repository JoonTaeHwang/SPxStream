/*********************************************************************
*
* (c) Copyright 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxTrackFilter.h,v $
* ID: $Id: SPxTrackFilter.h,v 1.1 2017/01/31 14:35:16 rew Exp $
*
* Purpose:
*	Header for SPxTrackFilter class which supports the filtering of
*	track according to rules such as speed, position etc.
*
* Revision Control:
*   31/01/17 v1.1    REW	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_TRACK_FILTER_H
#define _SPX_TRACK_FILTER_H

/*
 * Other headers required.
 */

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


/* Forward-declare classes we use in case headers are in wrong order. */
class SPxRadarTrack;
class SPxWorldMap;

/*
 * Define our class, derived from the generic base object.
 */
class SPxTrackFilter : public SPxObj
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxTrackFilter(void);
    virtual ~SPxTrackFilter(void);

    /* Function to give it the world map object for distance-from-land rule. */
    SPxErrorCode SetWorldMap(SPxWorldMap *worldMap);

    /* Check a track against the filter rules, returning TRUE if the
     * track passes the rules and FALSE if not.
     */
    int CheckTrack(const SPxRadarTrack *track) const;

    /*
     * Configure filter rules.
     */
    /* Distance from radar. */
    void SetRangeEnabled(int enable)		{ m_rangeEnabled = enable; }
    int GetRangeEnabled(void) const		{ return(m_rangeEnabled); }
    void SetRangeStartMetres(double m)		{ m_rangeStartM = m; }
    double GetRangeStartMetres(void) const	{ return(m_rangeStartM); }
    void SetRangeStopMetres(double m)		{ m_rangeStopM = m; }
    double GetRangeStopMetres(void) const	{ return(m_rangeStopM); }

    /* Distance from lat/long. */
    void SetDistFromPointEnabled(int enable)	{ m_ptDistEnabled = enable; }
    int GetDistFromPointEnabled(void) const	{ return(m_ptDistEnabled); }
    void SetDistFromPointStartMetres(double m)	{ m_ptDistStartM=m; }
    double GetDistFromPointStartMetres(void) const { return(m_ptDistStartM); }
    void SetDistFromPointStopMetres(double m)	{ m_ptDistStopM=m; }
    double GetDistFromPointStopMetres(void) const { return(m_ptDistStopM); }
    void SetDistFromPointLatLong(double latDegs, double longDegs)
    {
	m_ptDistLatDegs = latDegs;
	m_ptDistLongDegs = longDegs;
    }
    double GetDistFromPointLatDegs(void) const	{ return(m_ptDistLatDegs); }
    double GetDistFromPointLongDegs(void) const	{ return(m_ptDistLongDegs); }

    /* Distance from land. */
    void SetDistFromLandEnabled(int enable)	{ m_landDistEnabled = enable; }
    int GetDistFromLandEnabled(void) const	{ return(m_landDistEnabled); }
    void SetDistFromLandStartMetres(double m)	{ m_landDistStartM=m;}
    double GetDistFromLandStartMetres(void) const { return(m_landDistStartM);}
    void SetDistFromLandStopMetres(double m)	{ m_landDistStopM=m;}
    double GetDistFromLandStopMetres(void) const { return(m_landDistStopM);}

    /* Speed. */
    void SetSpeedEnabled(int enable)		{ m_speedEnabled = enable; }
    int GetSpeedEnabled(void) const		{ return(m_speedEnabled); }
    void SetSpeedStartMps(double mps)		{ m_speedStartMps = mps; }
    double GetSpeedStartMps(void) const		{ return(m_speedStartMps); }
    void SetSpeedStopMps(double mps)		{ m_speedStopMps = mps; }
    double GetSpeedStopMps(void) const		{ return(m_speedStopMps); }

    /* Track classification. */
    void SetTrackClassEnabled(int enable)	{ m_trackClassEnabled =enable;}
    int GetTrackClassEnabled(void) const	{ return(m_trackClassEnabled);}
    void SetTrackClassStart(int tc)		{ m_trackClassStart = tc; }
    int GetTrackClassStart(void) const		{ return(m_trackClassStart); }
    void SetTrackClassStop(int tc)		{ m_trackClassStop = tc; }
    int GetTrackClassStop(void) const		{ return(m_trackClassStop); }

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */
    SPxWorldMap *m_worldMap;		/* If available */

    /* Distance from radar. */
    int m_rangeEnabled;
    double m_rangeStartM;
    double m_rangeStopM;

    /* Distance from lat/long. */
    int m_ptDistEnabled;
    double m_ptDistStartM;
    double m_ptDistStopM;
    double m_ptDistLatDegs;
    double m_ptDistLongDegs;

    /* Distance from land. */
    int m_landDistEnabled;
    double m_landDistStartM;
    double m_landDistStopM;

    /* Speed. */
    int m_speedEnabled;
    double m_speedStartMps;
    double m_speedStopMps;

    /* Track classification. */
    int m_trackClassEnabled;
    int m_trackClassStart;
    int m_trackClassStop;
  
    /*
     * Private functions.
     */

}; /* SPxTrackFilter */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_TRACK_FILTER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
