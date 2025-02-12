/*********************************************************************
*
* (c) Copyright 2016 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxTiledMapSrcCoverage object which generates
*	Tiled Map tiles from terrain data showing radar coverage.
*
*
* Revision Control:
*   27/01/21 v1.15   AGC	Fix line endings.
*
* Previous Changes:
*   06/08/20 1.14   AGC	Add terrain database deletion handler.
*   04/08/20 1.13   AGC	Update coverage on terrain event.
*   24/04/20 1.12   AGC	Add GetNumRadars().
*   11/06/19 1.11   AGC	Add parameter support.
*   09/01/19 1.10   BTB	Allow radar info changes to be stored as pending.
*   02/10/18 1.9    SP 	Replace SPxTiledMap with SPxTiledMapDatabase.
*   20/04/18 1.8    SP 	Support setting radar height above sea level.
*   13/04/18 1.7    BTB	Support enabling/disabling per radar.
*   02/02/18 1.6    AGC	Improve multi-radar support.
*   02/09/16 1.5    AGC	Display contours between height regions.
*   28/07/16 1.4    AGC	Enhance callback function.
*   14/07/16 1.3    AGC	Support alpha.
*			Support change handlers.
*   08/07/16 1.2    AGC	Support beam angles.
*			Support height above ground.
*			Support independent colours.
*			Render key.
*   15/06/16 1.1    AGC	Initial version.
**********************************************************************/
#ifndef SPX_TILED_MAP_SRC_COVERAGE_H_
#define SPX_TILED_MAP_SRC_COVERAGE_H_

/*
 * Other headers required.
 */

/* For SPxTiledMapDatabase enums. */
#include "SPxLibUtils/SPxTiledMapDatabase.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxEventHandler. */
#include "SPxLibUtils/SPxEventHandler.h"

/* For SPxOptional. */
#include "SPxLibUtils/SPxOptional.h"

/* For base class. */
#include "SPxLibUtils/SPxTiledMapSrc.h"

/*
 * Types
 */

/* Forward declarations. */
class SPxTerrainDatabase;

/* Define our class. */
class SPxTiledMapSrcCoverage : public SPxTiledMapSrc
{
public:
    /* Public types. */
    static const unsigned int MAX_NUM_VIS_HEIGHTS = 8;
    enum ChangeFlags
    {
	CHANGE_NONE	    = 0x0000,
	CHANGE_RADAR_POS    = 0x0001,
	CHANGE_RADAR_HEIGHT = 0x0002,
	CHANGE_RADAR_RANGE  = 0x0004,
	CHANGE_COLOURS	    = 0x0008,
	CHANGE_VIS_HEIGHTS  = 0x0010,
	CHANGE_RESOLUTION   = 0x0020,
	CHANGE_CURVATURE    = 0x0040,
	CHANGE_BEAM_ANGLE   = 0x0080,
	CHANGE_RADAR_ENABLE = 0x0100,
	CHANGE_ALL	    = 0xFFFF
    };

    struct VisProps;
    struct RadarProps;
    struct VisInfo
    {
	float maxAlpha;
	UINT32 colours[MAX_NUM_VIS_HEIGHTS];
	float visHeightsMetres[MAX_NUM_VIS_HEIGHTS];
	int visHeightsAboveGround;
	int earthCurvature;
    };
    struct RadarInfo
    {
	int coverageEnabled;
	double latDegs;
	double longDegs;
	double heightMetres;
	int isHeightAboveSeaLevel;
	unsigned int numAzimuths;
	unsigned int numSamples;
	double rangeMetres;
	double minBeamAngleDegs;
	double maxBeamAngleDegs;
    };
    struct VisData;

    typedef void (*ChangeFn)(UINT32 flags,
	const VisInfo *visInfo,
	int index, const RadarInfo *radarInfo);

    /* Public functions. */
    explicit SPxTiledMapSrcCoverage(SPxTerrainDatabase *db);
    virtual ~SPxTiledMapSrcCoverage(void);

    virtual SPxTiledMapSrcType GetType(void) const { return SPX_TILED_MAP_SRC_TYPE_COVERAGE; }
    virtual const char *GetSrcName(void) const { return "Radar Coverage"; }
    virtual SPxTiledMapDatabase::TileImageFormat_t GetTileImageFormat(void) const;
    virtual int IsTileYInverted(void) const;
    virtual int IsTileExpired(SPxTiledMapDatabase *tiledMap,
			      int zoom, int tileX, int tileY) const;
    virtual SPxErrorCode GetTile(SPxTiledMapDatabase *tiledMap,
				 int zoom, int tileX, int tileY,
				 const char *filename);
    virtual unsigned int GetOutOfBoundsTileSize(void) const;
    virtual const unsigned char *GetOutOfBoundsTileData(void) const;
    virtual unsigned int GetPlaceholderTileSize(void) const;
    virtual const unsigned char *GetPlaceholderTileData(void) const;
    virtual SPxErrorCode RenderKey(SPxTiledMapRenderer::Iface *renderer);

    SPxErrorCode SetVisInfo(const VisInfo *visInfo);
    SPxErrorCode GetVisInfo(VisInfo *visInfo) const;

    unsigned int AddRadar(void);
    SPxErrorCode RemoveRadar(unsigned int index);
    SPxErrorCode SetRadarInfo(unsigned int index, const RadarInfo *radarInfo,
			      int doUpdate = TRUE);
    SPxErrorCode GetRadarInfo(unsigned int index, RadarInfo *radarInfo,
			      int getPending = FALSE) const;
    unsigned int GetNumRadars(void) const;

    template<typename F, typename O>
    SPxErrorCode AddChangeHandler(F fn, O *obj)
    {
	return m_changeFn.Add(fn, obj);
    }
    template<typename F, typename O>
    SPxErrorCode RemoveChangeHandler(F fn, O *obj)
    {
	return m_changeFn.Remove(fn, obj);
    }


    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);
    virtual SPxErrorCode SetConfigFromState(void);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxEventHandler<ChangeFn> m_changeFn;

    /* Private functions. */
    SPxOptional<VisData> getMaxHeight(double latDegs, double longDegs) const;
    SPxOptional<VisData> getHeight(const RadarProps& props,
				   double latDegs, double longDegs) const;
    int testIntersect(double latStart, double latEnd,
		      double longStart, double longEnd) const;
    static SPxErrorCode terrainEventFn(UINT32 flags, void *userArg);
    static int terrainDBDeletedFn(void *invokingObject,
				  void *userObject,
				  void *arg);

}; /* SPxTiledMapSrcCoverage */

#endif /* SPX_TILED_MAP_SRC_COVERAGE_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/

