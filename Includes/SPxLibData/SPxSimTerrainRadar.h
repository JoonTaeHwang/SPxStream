/*********************************************************************
*
* (c) Copyright 2021, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for radar interface for use with SPxSimTerrain.
*
*
* Revision Control:
*   09/09/22 v1.4    AGC	Support index.
*
* Previous Changes:
*   07/04/22 1.3    AGC	Move from SPxLibRst to SPxLibData and rename.
*   16/11/21 1.2    AGC	Improve position updates.
*   15/11/21 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_SIM_TERRAIN_RADAR_H
#define _SPX_SIM_TERRAIN_RADAR_H

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxTypes.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibData/SPxSimTerrainRadarIface.h"

/* Forward declarations. */
class SPxNavData;
struct SPxLatLong_tag;

class SPxSimTerrainRadar : public SPxSimTerrainRadarIface
{
public:
    /* Public functions. */
    explicit SPxSimTerrainRadar(SPxNavData *navData,
				unsigned int index=0);
    virtual ~SPxSimTerrainRadar(void);

    SPxErrorCode SetLatLong(const SPxLatLong_tag *latLong);
    SPxErrorCode SetHeightAboveSeaMetres(double heightAboveSeaMetres);
    SPxErrorCode SetEndRangeMetres(double endRangeMetres);

    virtual SPxErrorCode GetIndex(unsigned int *index) const;

    /* Radar position. */
    virtual int IsStatic(int mutexAlreadyLocked=FALSE) const;
    virtual SPxErrorCode GetLatLong(SPxLatLong_tag *latLong,
				    int mutexAlreadyLocked=FALSE) const;
    virtual SPxErrorCode GetHeightAboveSeaMetres(double *heightAboveSeaMetres,
						 int mutexAlreadyLocked=FALSE) const;

    /* Radar properties. */
    virtual SPxErrorCode GetNumSamples(unsigned int *numSamples,
				       int mutexAlreadyLocked=FALSE) const;
    virtual SPxErrorCode GetNumSpokes(unsigned int *numSpokes,
				      int mutexAlreadyLocked=FALSE) const;
    virtual SPxErrorCode GetStartRangeMetres(double *startRangeMetres,
					     int mutexAlreadyLocked=FALSE) const;
    virtual SPxErrorCode GetEndRangeMetres(double *endRangeMetres,
					   int mutexAlreadyLocked=FALSE) const;
    virtual SPxErrorCode GetMinBeamAngleDegs(double *minBeamAngleDegs,
					     int mutexAlreadyLocked=FALSE) const;
    virtual SPxErrorCode GetMaxBeamAngleDegs(double *maxBeamAngleDegs,
					     int mutexAlreadyLocked=FALSE) const;

    /* Blind sectors. */
    virtual int IsInBlindSector(UINT16 aziNum, int isMutexLocked) const;

    /* Advanced configuration. */
    virtual int IsUsingEarthCurvature(void) const;
    virtual int IsAntiClockwise(void) const;

    /* Change handlers. */
    virtual SPxErrorCode AddChangeFn(SPxSimTerrainRadarChangeFn_t fn, UINT32 flags, void *userArg);
    virtual SPxErrorCode RemoveChangeFn(SPxSimTerrainRadarChangeFn_t fn, UINT32 flags, void *userArg);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    void callChangeFns(UINT32 flags);

}; /* SPxSimTerrainRadar. */

#endif /* _SPX_SIM_TERRAIN_RADAR_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
