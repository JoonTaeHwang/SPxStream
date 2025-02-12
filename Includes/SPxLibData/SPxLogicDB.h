/*********************************************************************
 *
 * (c) Copyright 2012, Cambridge Pixel Ltd.
 *
 * File: $RCSfile: SPxLogicDB.h,v $
 * ID: $Id: SPxLogicDB.h,v 1.2 2013/01/14 12:06:59 rew Exp $
 *
 * Purpose:
 *	Header for SPxLogicDB class.
 *
 * Revision Control:
 *  11/01/13 v1.2    AGC	Add SetCheckIntervalMSecs() function.
 *
 * Previous Changes:
 *  21/12/12 1.1    AGC	Initial Version.
 **********************************************************************/

#ifndef _SPX_LOGIC_DB_H
#define _SPX_LOGIC_DB_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
 *
 *   Type definitions
 *
 **********************************************************************/

/* Forward declarations. */
class SPxLogicGroup;
struct SPxLogicDBPriv;

/* Types of reporter. */
enum SPxLogicReporter
{
    SPX_LOGIC_DB_REPORTER_MHT	    = 0,
    SPX_LOGIC_DB_REPORTER_NET_SPX   = 1,
    SPX_LOGIC_DB_REPORTER_AIS	    = 2,
    SPX_LOGIC_DB_REPORTER_ADSB	    = 3
};

/* Flags for targets. */
enum SPxLogicTargetFlags
{
    SPX_LOGIC_TARGET_NONE	= 0,
    SPX_LOGIC_TARGET_CREATED	= 1,
    SPX_LOGIC_TARGET_DELETED	= 2
};

/* Key for target database. */
struct SPxLogicKey
{
    SPxLogicReporter type;
    UINT32 reporterID;
    UINT32 targetID;

    /* Comparison function for keys. */
    bool operator<(const SPxLogicKey& other) const
    {
	if( type < other.type ) return true;
	if( type > other.type ) return false;
	if( reporterID < other.reporterID ) return true;
	if( reporterID > other.reporterID ) return false;
	if( targetID < other.targetID ) return true;
	if( targetID > other.targetID ) return false;
	return false;
    }
};

/* Logic target properties structure. */
struct SPxLogicTargetProp
{
    SPxTime_t time;	    /* Time for this set of properties. */

    int isRangeAziValid;    /* Is the following range/azi valid? */
    double rangeMetres;	    /* Range of target in metres. */
    double aziDegs;	    /* Azimuth of target in degrees. */

    int isLatLongValid;	    /* Is the following lat/long valid? */
    double latDegs;	    /* Latitude in degrees. */
    double longDegs;	    /* Longitude in degrees. */

    double speedMps;	    /* Target speed in metres/second. */
    double courseDegs;	    /* Target course in degrees. */
};

/* Complete target description. */
struct SPxLogicTarget
{
    SPxLogicTargetProp initial;	    /* Initial target properties. */
    SPxLogicTargetProp previous;    /* Previous target properties. */
    SPxLogicTargetProp current;	    /* Current target properties. */
};

/* Function types. */
typedef int (*SPxLogicDBTargetFn)(const SPxLogicKey *key,
				  const SPxLogicTarget *target,
				  UINT32 flags,
				  void *userArg);

/*
 * Define our class.
 */
class SPxLogicDB : public SPxObj
{
public:
    /* Public functions. */
    explicit SPxLogicDB(void);
    virtual ~SPxLogicDB(void);

    SPxErrorCode Load(const char *filename);
    SPxErrorCode Save(const char *filename);

    SPxErrorCode SetCheckIntervalMSecs(UINT32 checkIntervalMSecs);
    UINT32 GetCheckIntervalMSecs(void) const;
    SPxErrorCode SetPurgeTimeSecs(double purgeTimeSecs);
    double GetPurgeTimeSecs(void) const;

    /* Function called by target receivers, generators. */
    SPxErrorCode Target(SPxLogicKey *key, SPxLogicTargetProp *target);

    /* Parameters set/get */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /* Private variables. */
    SPxAutoPtr<SPxLogicDBPriv> m_p;

    /* Functions called by SPxLogicGroup. */
    SPxErrorCode AddLogicGroup(SPxLogicGroup *group);
    SPxErrorCode RemoveLogicGroup(SPxLogicGroup *group);
    int IterateTargets(SPxLogicDBTargetFn fn, void *userArg);

    /* Other private functions. */
    static void checkRulesWrapper(void *userArg);
    void checkRules(void);

    friend class SPxLogicGroup;

}; /* SPxLogicDB */

#endif /* _SPX_LOGIC_DB_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
