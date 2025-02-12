/*********************************************************************
*
* (c) Copyright 2013, 2014, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxSimBuilding object which represents a building
*	made up of one or more shapes.
*
*
* Revision Control:
*   10/02/21 v1.7    AGC	Implement SPxAreaObj::IsItemNameSet().
*
* Previous Changes:
*   16/05/14 1.6    AGC	Implement SPxAreaObj interface.
*   08/05/14 1.5    AGC	Support holes.
*			Support selecting buildings.
*   28/08/13 1.4    AGC	Improvements around antimeridian.
*   14/08/13 1.3    AGC	All mutex locks now done in building database.
*   01/08/13 1.2    AGC	Improve mutex protection.
*   30/07/13 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_SIM_BUILDING_H
#define _SPX_SIM_BUILDING_H

/*
 * Other headers required.
 */

/* For SPxErrorCode. */
#include "SPxLibUtils/SPxError.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For base class. */
#include "SPxLibUtils/SPxAreaObj.h"

/*
 * Types
 */

#define SPX_SIM_BUILDING_INVALID_HEIGHT (-32768.0)

/* Forward declarations. */
class SPxBuildingDatabase;
class SPxSimBuildingShape;
struct SPxSimBuildingPriv;

/* Define our class. */
class SPxSimBuilding : public SPxAreaObj
{
public:
    /* Public functions. */
    explicit SPxSimBuilding(SPxBuildingDatabase *bdb, const char *name=NULL);
    virtual ~SPxSimBuilding(void);

    SPxErrorCode Reset(void);

    /* Building name. */
    SPxErrorCode SetName(const char *name);
    SPxErrorCode GetName(char *buffer, unsigned int bufLen) const;

    unsigned int GetNumShapes(void) const;
    SPxErrorCode GetShapes(SPxSimBuildingShape **shapes,
	unsigned int numShapes) const;

    /* Get the height of the building at a specified lat/long. */
    double GetHeight(double latDegs, double longDegs) const;

    virtual SPxErrorCode SetSelected(int selected);
    virtual int IsSelected(void) const;

    /* SPxAreaObj interface. */
    virtual int IsNameSettable(void) const { return TRUE; }
    virtual int IsItemNameSet(void) const;
    virtual SPxErrorCode SetItemName(const char *name) { return SetName(name); }
    virtual const char *GetItemName(void) const;
    virtual unsigned int GetNumTypes(void) const { return 1; }
    virtual const char *GetTypeName(unsigned int /*typeIndex*/) const { return "Shape"; }
    virtual SPxAreaObj *CreateArea(unsigned int typeIndex);
    virtual SPxErrorCode DeleteAreas(void) { return Reset(); }
    virtual SPxErrorCode IterateObjects(SPxAreaObjFn fn, void *userArg) const;

private:
    /* Private variables. */
    SPxAutoPtr<SPxSimBuildingPriv> m_p;

    /* Functions called by SPxBuildingDatabase. */
    SPxErrorCode RemoveDatabase(void);
    void ApplyDesc(const char *buffer);
    const char *GetDesc(void) const;
    void SetFile(const char *filename);
    int WasFromFile(const char *filename);
    SPxErrorCode SignalAreaEvent(void);

    /* Functions called by SPxSimBuildingShape. */
    SPxErrorCode AddShape(SPxSimBuildingShape *shape);
    SPxErrorCode RemoveShape(SPxSimBuildingShape *shape);
    int GetNumShapesCreated(void) const;
    void RecalcLimits(void);
    SPxErrorCode SignalEvent(double minLatDegs, double maxLatDegs,
	double minLongDegs, double maxLongDegs, int crossZeroLong);

    /* Private functions. */

    friend class SPxBuildingDatabase;
    friend class SPxSimBuildingShape;
    friend class SPxRadarSimulator;

}; /* SPxSimBuilding. */

#endif /* _SPX_SIM_BUILDING_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
