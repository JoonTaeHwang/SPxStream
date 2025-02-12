/*********************************************************************
*
* (c) Copyright 2015 - 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxGate.h,v $
* ID: $Id: SPxGate.h,v 1.3 2017/07/13 15:40:13 rew Exp $
*
* Purpose:
*   Header for SPxGate class.
*
* Revision Control:
*   13/07/17 v1.3    REW	Include required SPxCriticalSection.h
*
* Previous Changes:
*   23/08/16 1.2    AGC	Support directional gates.
*			Support gate approval.
*			Support pre and post-alarm gates.
*			Support display name.
*   15/07/15 1.1    SP 	Initial version.
**********************************************************************/

#ifndef _SPX_GATE_H
#define _SPX_GATE_H

/*
 * Other headers required.
 */

/* Library headers. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxTime.h"

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

/* Forward declare any other classes we need. */
class SPxCriticalSection;

/*
 * Define our class.
 */
class SPxGate : public SPxObj
{

/* The database class can access our internals directly. */
friend class SPxThreatDatabase;

public:

    /*
     * Public types.
     */

    enum Direction
    {
	DIR_BOTH = 0,
	DIR_UNI = 1
    };

    enum DetectType
    {
	DETECT_NORMAL = 0,
	DETECT_PRE = 1,
	DETECT_POST = 2
    };

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxGate(void);
    virtual ~SPxGate(void);

    /* Display name. */
    SPxErrorCode SetDisplayName(const char *displayName);
    const char *GetDisplayName(void) const;

    SPxErrorCode Approve(double numSecs);
    int IsApproved(void) const;

    /* Location and positioning. */
    virtual SPxErrorCode SetStartLatLong(double latDegs, double lonDegs);
    virtual SPxErrorCode GetStartLatLong(double *latDegs, double *lonDegs) const;
    virtual SPxErrorCode SetEndLatLong(double latDegs, double lonDegs);
    virtual SPxErrorCode GetEndLatLong(double *latDegs, double *lonDegs) const;
    virtual int IsLatLongSet(void) const { return m_isStartLatLongSet & m_isEndLatLongSet; }
    virtual SPxErrorCode SetDirection(Direction direction);
    virtual Direction GetDirection(void) const;
    virtual SPxErrorCode SetDetectType(DetectType detectType);
    virtual DetectType GetDetectType(void) const;
  
    /* Configuration. */
    virtual SPxErrorCode SetAlarm(int isAlarm);
    virtual int IsAlarm(void) const { return m_isAlarm; }
    virtual SPxErrorCode GetAlarmTime(SPxTime_t *alarmTime) const;
    virtual double GetSecondsSinceAlarm(void) const;
    virtual SPxErrorCode SetAlarmColour(UINT32 argb);
    virtual UINT32 GetAlarmColour(void) const { return m_alarmARGB; }
    virtual SPxErrorCode SetNoAlarmColour(UINT32 argb);
    virtual UINT32 GetNoAlarmColour(void) const { return m_noAlarmARGB; }
    virtual SPxErrorCode SetApprovedColour(UINT32 argb);
    virtual UINT32 GetApprovedColour(void) const { return m_approvedARGB; }
    virtual UINT32 GetColour(void) const;
    virtual SPxErrorCode SetWeighting(double val);
    virtual double GetWeighting(void) const { return m_weighting; }

    /* User data. */
    virtual void SetUserData(void *ptr) { m_userData = ptr; }
    virtual void *GetUserData(void) const { return m_userData; }

    /* Generic get/set parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /*
     * Public static functions.
     */

private:

    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;        /* Private structure. */
    mutable SPxCriticalSection m_mutex;  /* Object mutex. */
    void *m_userData;            /* User defined data. */
    double m_startLatDegs;       /* Start latitude. */
    double m_startLonDegs;       /* Start longitude. */
    int m_isStartLatLongSet;     /* Is start lat/long valid? */
    double m_endLatDegs;         /* Start latitude. */
    double m_endLonDegs;         /* Start longitude. */
    int m_isEndLatLongSet;       /* Is end lat/long valid? */
    Direction m_direction;	 /* Direction of gate. */
    DetectType m_detectType;	 /* Type of detection. */
    UINT32 m_alarmARGB;          /* Colour to use when alarm set. */
    UINT32 m_noAlarmARGB;        /* Colour to use when alarm not set. */
    int m_isAlarm;               /* Is in alarm state? */
    SPxTime_t m_alarmTime;	 /* Time of last alarm. */
    double m_weighting;          /* Severity for threat calculation. */
    mutable int m_approved;	 /* Is the gate approved. */
    SPxTime_t m_approveEndTime;	 /* End time for approval. */
    UINT32 m_approvedARGB;	 /* Colour to use when approved. */  

    /*
     * Private functions.
     */

}; /* SPxGate */

#endif /* SPX_GATE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
