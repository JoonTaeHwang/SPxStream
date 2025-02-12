/*********************************************************************
*
* (c) Copyright 2012 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxSimTerrain object which generates simulated
*	returns of data due to the influence of terrain information
*	read from a terrain database object.
*
*
* Revision Control:
*   15/11/21 v1.29   AGC	Support use with SPxSimTerrainRadarIface interface.
*
* Previous Changes:
*   04/10/21 1.28   AGC	Support configurable sea level.
*   02/02/18 1.27   AGC	Fix N/C #519 - terrain outside blind sector.
*   11/01/18 1.26   AGC	Use 16-bit azimuths.
*   20/04/17 1.25   AGC	Stop using multiple threads.
*   24/02/17 1.24   AGC	Further mutex improvements.
*   06/02/17 1.23   AGC	Fix mutex issues.
*   23/08/16 1.22   AGC	Event function prototype updated.
*   03/03/16 1.21   AGC	Support various radar data types.
*   11/01/16 1.20   AGC	Add alwaysNearNoise option to GetAzimuthData().
*   23/12/15 1.19   AGC	Support multi byte azimuth data.
*   26/11/15 1.18   AGC	Support multiple visibility heights.
*   16/11/15 1.17   AGC	Move blind sectors to SPxSimRadar.
*   29/04/15 1.16   AGC	Add blind sector support.
*   07/11/14 1.15   AGC	Update radar sim event handler.
*   24/09/14 1.14   AGC	Improve beamwidth efficiency.
*			Multi-threaded terrain generation.
*   04/07/14 1.13   AGC	Add radar visibility map.
*   08/05/14 1.12   AGC	Support start range.
*   28/08/13 1.11   AGC	Improve buildings around antimeridian.
*   14/08/13 1.10   AGC	Improve updates when buildings change.
*   30/07/13 1.9    AGC	Add building support.
*   06/06/13 1.8    AGC	Reduce memory usage.
*   25/04/13 1.7    AGC	Use doubles for terrain heights.
*   28/03/13 1.6    AGC	Replace cliff mode with cliff (angle) mode.
*			Improve performance.
*   18/03/13 1.5    AGC	Add SPX_TERRAIN_MODE_CLIFF_ANGLE terrain mode.
*   08/03/13 1.4    AGC	Add GetSimRadar() function.
*   25/01/13 1.3    AGC	Only update data when radar terrain tiles updated.
*   09/11/12 1.2    AGC	Configurable gain and cliff resolution.
*			Support parameters.
*			Allow retrieval of visibility data.
*   26/10/12 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_SIM_TERRAIN_H
#define _SPX_SIM_TERRAIN_H

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxRadarSimHeight enumeration. */
#include "SPxLibData/SPxRadarSimulator.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/*
 * Types
 */

/* Terrain data generation modes. */
enum SPxTerrainMode
{
    SPX_TERRAIN_MODE_NONE = 0,
    SPX_TERRAIN_MODE_HEIGHT = 1,
    SPX_TERRAIN_MODE_CLIFF = 2
};

/* Forward declarations. */
class SPxTerrainDatabase;
class SPxBuildingDatabase;
class SPxSimTerrainRadarIface;
class SPxRadarSimulator;
class SPxThread;
struct SPxRadarSimEventParams;

/* Define our class. */
class SPxSimTerrain : public SPxObj
{
public:
    /* Public functions. */
    explicit SPxSimTerrain(SPxTerrainDatabase *tdb,
			   SPxBuildingDatabase *bdb,
			   SPxSimTerrainRadarIface *radar,
			   SPxRadarSimulator *radarSim);
    virtual ~SPxSimTerrain(void);

    /* Set display options. */
    SPxErrorCode EnableTerrain(int enabled);
    int IsTerrainEnabled(void) const;
    SPxErrorCode EnableBuildings(int enabled);
    int AreBuildingsEnabled(void) const;
    SPxErrorCode SetTerrainMode(SPxTerrainMode terrainMode);
    SPxErrorCode GetTerrainMode(SPxTerrainMode *terrainMode) const;
    SPxErrorCode SetHiddenObscured(int hiddenObscured);
    int IsHiddenObscured(void) const;
    SPxErrorCode EnableAziSpreading(int enabled);
    int IsAziSpreadingEnabled(void) const;
    SPxErrorCode SetGain(double gain);
    SPxErrorCode GetGain(double *gain) const;
    SPxErrorCode SetCliffResolution(double cliffResolution);
    SPxErrorCode GetCliffResolution(double *cliffResolution) const;
    SPxErrorCode SetSeaLevelMetres(double seaLevelMetres);
    SPxErrorCode GetSeaLevelMetres(double *seaLevelMetres) const;

    /* Get data for a spoke. */
    SPxErrorCode GetSpokeData(UINT16 aziNum16,
	unsigned int beamwidthSpokes,
	const UINT16 *noiseData,
	const UINT16 *nearNoiseData,
	unsigned int nearNoiseSamples,
	unsigned char *spokeData,
	UCHAR packing,
	SPxPackingChannel channel,
	float *visibleData,
	int alwaysNearNoise);
    SPxErrorCode GetVisibleData(UINT16 aziNum16,
	float *visHeightMetres, unsigned int numHeights,
	SPxRadarSimHeight heightAbove,
	unsigned char *spokeData,
	UCHAR packing,
	SPxPackingChannel channel);
    SPxErrorCode GetVisibleHeightMetres(double latDegs, double longDegs,
					float *visHeightMetres);

    SPxErrorCode GetTerrainDatabase(SPxTerrainDatabase **tdb);
    SPxErrorCode GetBuildingDatabase(SPxBuildingDatabase **bdb);
    SPxErrorCode GetSimRadar(SPxSimTerrainRadarIface **simRadar);

    /* Set/get parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    SPxErrorCode calcData(unsigned int spokeNum);
    template<typename T>
    SPxErrorCode calcReturnData(UINT16 aziNum16, unsigned int spokeNum, T *returnData);
    SPxErrorCode calcVisibleData(unsigned int spokeNum);
    template<typename T>
    SPxErrorCode calcSpreadData(unsigned int spokeNum, unsigned int beamwidthSpokes,
				T *spreadData, const T *returnData);
    unsigned int calcNumSamplesFromZeroRange(void) const;
    static SPxErrorCode viewChangeFnWrapper(UINT32 flags, void *userArg);
    SPxErrorCode viewChangeFn(UINT32 flags);
    static SPxErrorCode buildChangeFnWrapper(double minLatDegs, double maxLatDegs,
	double minLongDegs, double maxLongDegs, int crossZeroLong, void *userArg);
    SPxErrorCode buildChangeFn(double minLatDegs,
			       double maxLatDegs,
			       double minLongDegs,
			       double maxLongDegs,
			       int crossZeroLong);
    static SPxErrorCode radarChangeFnWrapper(SPxSimTerrainRadarIface *radar, UINT32 flags, void *userArg);
    SPxErrorCode radarChangeFn(SPxSimTerrainRadarIface *radar, UINT32 flags);
    static SPxErrorCode eventFnWrapper(UINT32 flags,
				       const SPxRadarSimEventParams *params, void *userArg);
    SPxErrorCode eventFn(UINT32 flags);
    SPxErrorCode radarPosRangeChanged(unsigned int radarIndex,
				      double endRangeMetres,
				      const SPxLatLong_t *latLong,
				      double heightAboveSeaMetres,
				      int heightAboveSeaValid,
				      unsigned int beamwidthSpokes=1);
    SPxErrorCode buildingsChanged(void);

}; /* SPxSimTerrain */

#endif /* _SPX_SIM_TERRAIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
