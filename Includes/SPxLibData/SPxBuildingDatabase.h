/*********************************************************************
*
* (c) Copyright 2013 - 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxBuildingDatabase.h,v $
* ID: $Id: SPxBuildingDatabase.h,v 1.7 2017/10/31 16:08:42 rew Exp $
*
* Purpose:
*	Header for SPxBuildingDatabase object which manages a set of
*	buildings which are groups of shapes where each shape has a
*	height.
*
*
* Revision Control:
*   31/10/17 v1.7    AGC	Fix warnings.
*
* Previous Changes:
*   16/05/14 1.6    AGC	Fix icc warnings.
*   16/05/14 1.5    AGC	Implement SPxAreaObj interface.
*   08/05/14 1.4    AGC	Support loading shapefiles.
*   28/08/13 1.3    AGC	Improvements around antimeridian.
*   14/08/13 1.2    AGC	Improve building events.
*   30/07/13 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_BUILDING_DATABASE_H
#define _SPX_BUILDING_DATABASE_H

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For base classes. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxAreaObj.h"
#include "SPxLibUtils/SPxLock.h"

/*
 * Types
 */

/* Forward declarations. */
class SPxSimBuilding;
class SPxViewControl;
struct SPxBuildingDatabasePriv;

/* Callback types. */
typedef SPxErrorCode (*SPxBuildingDatabaseFn_t)(double minLatDegs,
						double maxLatDegs,
						double minLongDegs,
						double maxLongDegs,
						int crossZeroLong,
						void *userArg);

/* Define our class. */
class SPxBuildingDatabase : public SPxObj, public SPxAreaObj, public SPxLock
{
public:
    /* Public functions. */
    SPxBuildingDatabase(void);
    virtual ~SPxBuildingDatabase(void);

    SPxErrorCode Reset(void);

    unsigned int GetNumBuildings(void) const;
    SPxErrorCode GetBuildings(SPxSimBuilding **buildings, unsigned int numBuildings) const;

    /* Main data retrieval function. */
    double GetHeight(double latDegs, double longDegs) const;

    /* Load/save buildings. */
    virtual int IsSaveSupported(void) const { return TRUE; }
    virtual SPxErrorCode Load(const char *filename) { return Load(filename, TRUE); }
    SPxErrorCode Load(const char *filename, int reset);
    virtual SPxErrorCode Save(const char *filename) const;
    const char *GetShortPath(void) const;
    const char *GetFullPath(void) const;

    SPxErrorCode LoadShapefile(const char *filename, double heightMetres=50.0,
	SPxViewControl *viewCtrl=NULL);

    /* Add/remove event callbacks. */
    SPxErrorCode AddEventCallback(SPxBuildingDatabaseFn_t fn, void *userArg);
    SPxErrorCode RemoveEventCallback(SPxBuildingDatabaseFn_t fn, void *userArg);

    /* Standard parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* SPxAreaObj interface. */
    virtual const char *GetItemName(void) const { return SPxObj::GetConstName(); }
    virtual SPxErrorCode IterateObjects(SPxAreaObjFn fn, void *userArg) const;

    /* SPxLock interface. */
    virtual SPxErrorCode Initialise(void);
    virtual SPxErrorCode Enter(void);
    virtual SPxErrorCode TryEnter(void);
    virtual SPxErrorCode Leave(void);

private:
    /* Private variables. */
    SPxAutoPtr<SPxBuildingDatabasePriv> m_p;

    /* Functions called by SPxSimBuilding. */
    int GetNumBuildingsCreated(void) const;
    SPxErrorCode AddBuilding(SPxSimBuilding *building);
    SPxErrorCode RemoveBuilding(SPxSimBuilding *building);
    SPxErrorCode SignalEvent(double minLatDegs,
			     double maxLatDegs,
			     double minLongDegs,
			     double maxLongDegs,
			     int crossZeroLong);

    /* Private functions. */

    using SPxAreaObj::AddEventCallback;
    using SPxAreaObj::RemoveEventCallback;

    friend class SPxSimBuilding;

}; /* SPxBuildingDatabase. */

#endif /* _SPX_BUILDING_DATABASE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
