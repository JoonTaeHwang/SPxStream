/*********************************************************************
*
* (c) Copyright 2013 - 2014, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxSimBuildingShape object which represents a
*	shape that is part of a building.
*
*
* Revision Control:
*   10/02/21 v1.8    AGC	Implement SPxAreaObj::IsItemNameSet().
*
* Previous Changes:
*   18/07/14 1.7    AGC	Add AppendPoint() function.
*   16/05/14 1.6    AGC	Implement SPxAreaObj interface.
*   08/05/14 1.5    AGC	Support holes.
*			Support selecting shapes.
*   28/08/13 1.4    AGC	Improve efficiency.
*			Improvements around antimeridian.
*   14/08/13 1.3    AGC	All mutex locks now done in building database.
*   01/08/13 1.2    AGC	Improve mutex protection.
*   30/07/13 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_SIM_BUILDING_SHAPE_H
#define _SPX_SIM_BUILDING_SHAPE_H

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

/* Forward declarations. */
class SPxSimBuilding;
struct SPxLatLong_tag;
struct SPxSimBuildingShapePriv;

/* Define our class. */
class SPxSimBuildingShape : public SPxAreaObj
{
public:
    /* Public functions. */
    explicit SPxSimBuildingShape(SPxSimBuilding *building, const char *name=NULL);
    virtual ~SPxSimBuildingShape(void);

    /* Shape name. */
    SPxErrorCode SetName(const char *name);
    SPxErrorCode GetName(char *buffer, unsigned int bufLen) const;

    SPxErrorCode SetHeightMetres(double heightMetres);
    SPxErrorCode GetHeightMetres(double *heightMetres) const;

    SPxErrorCode SetHole(int hole);
    int IsHole(void) const;

    /* Get the height of the building shape at a specified lat/long. */
    double GetHeight(double latDegs, double longDegs) const;

    virtual unsigned int GetNumPoints(void) const;
    SPxErrorCode GetPoints(SPxLatLong_tag *points, unsigned int numPoints) const;
    SPxErrorCode AddPoints(const SPxLatLong_tag *points, unsigned int numPoints=1);
    virtual SPxErrorCode RemovePoint(unsigned int index);

    virtual SPxErrorCode SetSelected(int selected);
    virtual int IsSelected(void) const;

    SPxSimBuilding *GetBuilding(void);

    /* SPxAreaObj interface. */
    virtual int IsNameSettable(void) const { return TRUE; }
    virtual int IsItemNameSet(void) const;
    virtual SPxErrorCode SetItemName(const char *name) { return SetName(name); }
    virtual const char *GetItemName(void) const;
    virtual SPxAreaUnits GetUnits(void) const { return SPX_AREA_UNITS_LAT_LONG; }
    virtual int IsMovable(void) const { return TRUE; }
    virtual SPxErrorCode SetPos(double longDegs, double latDegs);
    virtual SPxErrorCode GetPos(double *longDegs, double *latDegs) const;
    virtual int IsPointWithin(double longDegs, double latDegs, double margin) const;
    virtual SPxErrorCode SetPointPos(unsigned int idx, double longDegs, double latDegs, int fixedRatio=FALSE);
    virtual SPxErrorCode GetPointPos(unsigned int idx, double *longDegs, double *latDegs) const;
    virtual SPxErrorCode SetSelectedPoint(unsigned int idx);
    virtual unsigned int GetSelectedPoint(void) const;
    virtual int IsEditable(void) const { return TRUE; }
    virtual SPxErrorCode AddPoint(double longDegs, double latDegs);
    virtual SPxErrorCode AppendPoint(double longDegs, double latDegs);

private:
    /* Private variables. */
    SPxAutoPtr<SPxSimBuildingShapePriv> m_p;

    /* Functions called by SPxSimBuilding. */
    const char *GetDesc(void) const;
    SPxErrorCode RemoveBuilding(void);
    void GetLimits(double *minLatDegs, double *maxLatDegs,
	double *minLongDegs, double *maxLongDegs, int *crossZeroLong);

    /* Private functions. */
    void RecalcLimits(int includeCurrent=FALSE);

    friend class SPxSimBuilding;

}; /* SPxSimBuildingShape. */

#endif /* _SPX_SIM_BUILDING_SHAPE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
