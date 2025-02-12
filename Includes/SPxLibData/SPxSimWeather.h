/*********************************************************************
*
* (c) Copyright 2021, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxSimWeather object which represents a simulated
*	piece of weather within the SPxRadarSimulator radar source.
*
*
* Revision Control:
*   29/07/22 v1.4    AGC	Support weather in generated radar video.
*
* Previous Changes:
*   17/03/21 1.3    AGC	Suppress cppcheck warning.
*   03/03/21 1.2    AGC	Improve point within calculation.
*   02/03/21 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_SIM_WEATHER_H
#define _SPX_SIM_WEATHER_H

/*
 * Other headers required.
 */

/* For SPxPoint */
#include "SPxLibUtils/SPxTypes.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxErrorCode. */
#include "SPxLibUtils/SPxError.h"

/* For base classes. */
#include "SPxLibData/SPxSimMovable.h"
#include "SPxLibUtils/SPxAreaObj.h"

/*
 * Types
 */

/* Forward declarations. */
class SPxRadarSimulator;
struct SPxSimState;
struct SPxLatLong_tag;

/* Define our class. */
class SPxSimWeather : public SPxSimMovable, public SPxAreaObj
{
public:
    /* Public functions. */
    explicit SPxSimWeather(SPxRadarSimulator *radarSim, const char *name=NULL);
    explicit SPxSimWeather(SPxSimWeather *weather, const char *name, int checkName=TRUE);
    virtual ~SPxSimWeather(void);

    /* Weather name. */
    SPxErrorCode SetName(const char *name);
    SPxErrorCode GetName(char *buffer, unsigned int bufLen) const;
    virtual const char *GetItemName(void) const SPX_VIRTUAL_OVERRIDE;

    SPxErrorCode SetIntensity(unsigned int intensity);
    SPxErrorCode GetIntensity(unsigned int *intensity,
			      int mutexAlreadyLocked=FALSE) const;

    SPxErrorCode SetPoints(const SPxPoint *points, unsigned int pointsLen);
    // cppcheck-suppress virtualCallInConstructor
    virtual unsigned int GetNumPoints(void) const SPX_VIRTUAL_OVERRIDE
    {
	return GetNumPoints(FALSE);
    }
    virtual unsigned int GetNumPoints(int mutexAlreadyLocked) const;
    SPxErrorCode GetPoints(SPxPoint *points, unsigned int pointsLen,
			   int mutexAlreadyLocked=FALSE) const;

    UINT32 GetUniqueID(void) const;

    /* SPxAreaObj interface. */
    virtual const SPxAreaObj *GetParentArea(void) const SPX_VIRTUAL_OVERRIDE;
    virtual SPxAreaObj *GetParentArea(void) SPX_VIRTUAL_OVERRIDE;
    virtual SPxAreaUnits GetUnits(void) const SPX_VIRTUAL_OVERRIDE { return SPX_AREA_UNITS_LAT_LONG; }
    virtual int IsMovable(void) const SPX_VIRTUAL_OVERRIDE { return TRUE; }
    virtual SPxErrorCode SetPos(double x, double y) SPX_VIRTUAL_OVERRIDE;
    virtual SPxErrorCode GetPos(double *x, double *y) const SPX_VIRTUAL_OVERRIDE;
    virtual int IsPointWithin(double x, double y, double margin) const SPX_VIRTUAL_OVERRIDE;
    virtual SPxErrorCode SetPointPos(unsigned int idx, double x, double y, int fixedRatio) SPX_VIRTUAL_OVERRIDE;
    virtual SPxErrorCode GetPointPos(unsigned int idx, double *x, double *y) const SPX_VIRTUAL_OVERRIDE;
    virtual int IsEditable(void) const SPX_VIRTUAL_OVERRIDE { return TRUE; }
    virtual SPxErrorCode AddPoint(double x, double y) SPX_VIRTUAL_OVERRIDE;
    virtual SPxErrorCode RemovePoint(unsigned int idx) SPX_VIRTUAL_OVERRIDE;
    virtual SPxErrorCode AppendPoint(double x, double y) SPX_VIRTUAL_OVERRIDE;
    virtual int IsAutoCloseEnabled(void) const SPX_VIRTUAL_OVERRIDE { return TRUE; }

    /* SPxLock interface. */
    SPxErrorCode Enter(void) const;
    SPxErrorCode Leave(void) const;

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value) SPX_VIRTUAL_OVERRIDE;
    virtual int GetParameter(char *name, char *valueBuf, int bufLen) SPX_VIRTUAL_OVERRIDE;

    using SPxSimMovable::Update;

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Called by SPxRadarSimulator. */
    SPxErrorCode SetNameInternal(const char *name);
    SPxErrorCode SetUniqueID(UINT32 uniqueID);
    virtual SPxErrorCode RemoveRadarSim(void) SPX_VIRTUAL_OVERRIDE;
    virtual void ApplyDesc(const char *buffer) SPX_VIRTUAL_OVERRIDE;
    virtual const char *GetDesc(void) const SPX_VIRTUAL_OVERRIDE;

    /* Private functions. */
    SPxErrorCode signalEvent(void);

    friend class SPxRadarSimulator;
    friend class SPxSimMotion;

}; /* SPxSimWeather. */

#endif /* _SPX_SIM_WEATHER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
