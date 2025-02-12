/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for radar interface that objects must implement to be
*	used by SPxSimTerrain.
*
*
* Revision Control:
*   15/11/21 v1.1    AGC	Initial version.
*
* Previous Changes:
**********************************************************************/
#ifndef _SPX_SIM_TERRAIN_RADAR_IFACE_H
#define _SPX_SIM_TERRAIN_RADAR_IFACE_H

#include "SPxLibUtils/SPxTypes.h"
#include "SPxLibUtils/SPxError.h"

/* Forward declarations. */
struct SPxLatLong_tag;
class SPxSimTerrainRadarIface;

/* Callback types. */
typedef SPxErrorCode(*SPxSimTerrainRadarChangeFn_t)(SPxSimTerrainRadarIface *radar,
						    UINT32 flags,
						    void *userArg);

class SPxSimTerrainRadarIface
{
public:
    /* Public functions. */
    explicit SPxSimTerrainRadarIface(void) {}
    virtual ~SPxSimTerrainRadarIface(void) {}

    virtual SPxErrorCode GetIndex(unsigned int * /*index*/) const = 0;

    /* Radar position. */
    virtual int IsStatic(int mutexAlreadyLocked=FALSE) const = 0;
    virtual SPxErrorCode GetLatLong(SPxLatLong_tag *latLong,
				    int mutexAlreadyLocked=FALSE) const = 0;
    virtual SPxErrorCode GetHeightAboveSeaMetres(double *heightAboveSeaMetres,
						 int mutexAlreadyLocked=FALSE) const = 0;

    /* Radar properties. */
    virtual SPxErrorCode GetNumSamples(unsigned int *numSamples,
				       int mutexAlreadyLocked=FALSE) const = 0;
    virtual SPxErrorCode GetNumSpokes(unsigned int *numSpokes,
				      int mutexAlreadyLocked=FALSE) const = 0;
    virtual SPxErrorCode GetStartRangeMetres(double *startRangeMetres,
					     int mutexAlreadyLocked=FALSE) const = 0;
    virtual SPxErrorCode GetEndRangeMetres(double *endRangeMetres,
					   int mutexAlreadyLocked=FALSE) const = 0;
    virtual SPxErrorCode GetMinBeamAngleDegs(double *minBeamAngleDegs,
					     int mutexAlreadyLocked=FALSE) const = 0;
    virtual SPxErrorCode GetMaxBeamAngleDegs(double *maxBeamAngleDegs,
					     int mutexAlreadyLocked=FALSE) const = 0;

    /* Blind sectors. */
    virtual int IsInBlindSector(UINT16 aziNum, int isMutexLocked) const = 0;

    /* Advanced configuration. */
    virtual  int IsUsingEarthCurvature(void) const = 0;
    virtual int IsAntiClockwise(void) const = 0;

    /* Change handlers. */
    virtual SPxErrorCode AddChangeFn(SPxSimTerrainRadarChangeFn_t fn, UINT32 flags, void *userArg) = 0;
    virtual SPxErrorCode RemoveChangeFn(SPxSimTerrainRadarChangeFn_t fn, UINT32 flags, void *userArg) = 0;

}; /* SPxSimTerrainRadarIface. */


#endif /* _SPX_SIM_TERRAIN_RADAR_IFACE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
