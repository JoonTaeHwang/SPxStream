/*********************************************************************
 *
 * (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx Video Simulator SPxVidSimScene class header.
 *
 * Revision Control:
 *   09/09/22 v1.15  SP 	Rework display object types.
 *
 * Previous Changes:
 *   30/08/22 1.14  SP 	Make GetTerrainDB() const.
 *   22/07/22 1.13  SP 	Changes to simplify creation and deletion.
 *   14/07/22 1.12  SP 	Add GetPlatformByID() and tidy up code.
 *   20/12/19 1.11  AGC	Add extrapolate track position option.
 *   02/07/18 1.10  AGC	Make various functions const.
 *   27/06/18 1.9   SP 	Support multiple platforms.
 *   31/10/16 1.8   SP 	Support terrain tiled map.
 *   07/10/16 1.7   SP 	Derive from SPxVidSimMovable.
 *   14/10/15 1.6   SP 	Add Set/GetNearClipMetres().
 *   06/10/15 1.5   SP 	Allow track metres to be used in preference to lat/long.
 *   01/10/15 1.4   SP 	Support configurable land & sea colours.
 *   26/08/15 1.3   SP 	Add Initialise().
 *   19/07/13 1.2   SP 	Further development.
 *   15/07/13 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_VID_SIM_SCENE_H
#define _SPX_VID_SIM_SCENE_H

/* Other headers required. */
#include "SPxLibUtils/SPxVidSimMovable.h"
#include "SPxLibUtils/SPxCriticalSection.h"

/*********************************************************************
 *
 *  Constants
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Macros
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Type definitions
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Class definitions
 *
 *********************************************************************/

/* Forward declare any classes required. */
class SPxTerrainDatabase;
class SPxTiledMapDatabase;
class SPxVidSimPlatform;
class SPxVidSimModelDatabase;
class SPxTrackDatabase;
class SPxTiledMapRendererWin;
class SPxRemoteServer;
class SPxBitmapWin;

class SPxVidSimScene : public SPxVidSimMovable
{
public:

    /*
     * Public types.
     */

    typedef enum
    {
        DETAIL_LEVEL_LOW,
        DETAIL_LEVEL_MEDIUM,
        DETAIL_LEVEL_HIGH,
        _DETAIL_LEVEL_LAST /* Must be last entry. */

    } DetailLevel_t;

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxVidSimScene(void);
    virtual ~SPxVidSimScene(void);
    virtual SPxErrorCode Initialise(void);

    /* Object access. */
    unsigned int GetNumDisplays(void) { return GetNumChildren(OBJECT_TYPE_DISPLAY); }
    SPxVidSimDispGL *GetDisplay(unsigned int index);
    unsigned int GetNumStreams(void) { return GetNumChildren(OBJECT_TYPE_STREAM); }
    SPxVidSimDispGL *GetStream(unsigned int index);
    unsigned int GetNumPlatforms(void) { return GetNumChildren(OBJECT_TYPE_PLATFORM); }
    SPxVidSimPlatform *GetPlatform(unsigned int index);
    SPxTerrainDatabase *GetTerrainDB(void) const { return m_terrainDB; }
    SPxTiledMapDatabase *GetTiledMapDB(void) { return m_tiledMapDB; }
    SPxVidSimModelDatabase *GetModelDB(void) { return m_modelDB; }
    SPxTrackDatabase *GetTrackDB(void) { return m_trackDB; }

    /* Scene boundaries. */
    SPxErrorCode SetLatLong(double latDegs, double lonDegs);
    SPxErrorCode SetSizeMetres(double width, 
                               double length,
                               double height);
    double GetWidthMetres(void) const { return m_sceneWidthMetres; }
    double GetLengthMetres(void) const { return m_sceneLengthMetres; }
    double GetHeightMetres(void) const { return m_sceneHeightMetres; }

    void ShowFloor(int state);
    int IsFloorVisible(void) const { return m_showFloor; }

    void ShowCeiling(int state);
    int IsCeilingVisible(void) const { return m_showCeiling; }

    void ShowNorthWall(int state);
    int IsNorthWallVisible(void) const { return m_showNorthWall; }

    void ShowSouthWall(int state);
    int IsSouthWallVisible(void) const { return m_showSouthWall; }

    void ShowEastWall(int state);
    int IsEastWallVisible(void) const { return m_showEastWall; }
    
    void ShowWestWall(int state);
    int IsWestWallVisible(void) const { return m_showWestWall; }

    void SetWallRGB(UINT32 rgb);
    UINT32 GetWallRGB(void) const { return m_wallRGB; }

    void SetCeilingRGB(UINT32 rgb);
    UINT32 GetCeilingRGB(void) const { return m_ceilingRGB; }

    void SetFloorRGB(UINT32 rgb);
    UINT32 GetFloorRGB(void) const { return m_floorRGB; }

    void SetBackgroundRGB(UINT32 rgb);
    UINT32 GetBackgroundRGB(void) const { return m_backgroundRGB; }

    /* Terrain. */
    SPxErrorCode SetTerrainLandThreshMetres(double metres);
    double GetTerrainLandThreshMetres(void) const { return m_terrainLandThreshMetres; }

    SPxErrorCode SetTerrainZCorrectionMetres(double metres);
    double GetTerrainZCorrectionMetres(void) const { return m_terrainZCorrectionMetres; }

    SPxErrorCode SetTerrainTileSizeMetres(double metres);
    double GetTerrainTileSizeMetres(void) const { return m_terrainTileSizeMetres; }

    void ShowTerrain(int state);
    int IsTerrainVisible(void) const { return m_showTerrain; }

    void SetTerrainLandRGB(UINT32 rgb);
    UINT32 GetTerrainLandRGB(void) const { return m_terrainLandRGB; }
    
    void SetTerrainSeaRGB(UINT32 rgb);
    UINT32 GetTerrainSeaRGB(void) const { return m_terrainSeaRGB; }

    /* Terrain map. */
    void ShowTerrainMap(int state);
    int IsTerrainMapVisible(void) const { return m_showTerrainMap; }
    void CheckUpdateTerrainMapBitmap(void);
    unsigned int GetTerrainMapBitmapWidthPixels(void) const;
    unsigned int GetTerrainMapBitmapHeightPixels(void) const;
    const UINT8 *GetTerrainMapBitmapDataAndLock(void);
    void UnlockTerrainMapBitmap(void);
    double GetTerrainMapXScale(void) const;
    double GetTerrainMapYScale(void) const;

    /* Terrain radar. */
    void ShowTerrainRadar(int state);
    int IsTerrainRadarVisible(void) const { return m_showTerrainRadar; }

    /* Lighting. */
    SPxErrorCode SetAmbientBrightness(double level);
    double GetAmbientBrightness(void) const { return m_ambLightBrightness; }

    SPxErrorCode SetDiffuseBrightness(double level);
    double GetDiffuseBrightness(void) const { return m_difLightBrightness; }

    SPxErrorCode SetDiffuseDirection(double pitchDegs, double yawDegs);
    double GetDiffusePitchDegs(void) const { return m_difLightPitchDegs; }
    double GetDiffuseYawDegs(void) const { return m_difLightYawDegs; }

    /* Fog density control. */
    SPxErrorCode SetFogDensity(double density);
    double GetFogDensity(void) const { return m_fogDensity; }
    SPxErrorCode SetFogRGB(UINT32 rgb);
    UINT32 GetFogRGB(void) const { return m_fogRGB; }

    /* Detail level control. */
    SPxErrorCode SetDetailLevel(DetailLevel_t level);
    DetailLevel_t GetDetailLevel(void) const { return m_detailLevel; }
    SPxErrorCode SetNearClipMetres(double metres);
    double GetNearClipMetres(void) const { return m_nearClipMetres; }

    /* Tracks. */
    SPxErrorCode SetTrackLatLongEnabled(int isEnabled);
    int IsTrackLatLongEnabled(void) const { return m_isTrackLatLongEnabled; }
    SPxErrorCode SetTrackPosExtrapolateEnabled(int isEnabled);
    int IsTrackPosExtrapolatEnabled(void) const { return m_isTrackPosExtrapolateEnabled; }

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxVidSimModelDatabase *m_modelDB; /* Model database. */
    SPxRemoteServer *m_remoteServer;   /* Used to receive track reports. */
    SPxTrackDatabase *m_trackDB;       /* Track database. */
    DetailLevel_t m_detailLevel;       /* Level of detail to display. */
    double m_nearClipMetres;           /* Near clippling distance. */

    /* Scene boundaries. */
    double m_sceneWidthMetres;         /* Scene width (east/west). */
    double m_sceneLengthMetres;        /* Scene length (north/south). */
    double m_sceneHeightMetres;        /* Scene height (up/down). */
    int m_showFloor;                   /* Show floot. */
    int m_showCeiling;                 /* Show ceiling. */
    int m_showNorthWall;               /* Show north wall. */
    int m_showSouthWall;               /* Show south wall. */
    int m_showEastWall;                /* Show east wall. */
    int m_showWestWall;                /* Show west wall. */
    UINT32 m_wallRGB;                  /* Wall colour. */
    UINT32 m_ceilingRGB;               /* Ceiling colour. */
    UINT32 m_floorRGB;                 /* Floor colour. */

    /* Terrain. */
    int m_showTerrain;                 /* Show terrain. */
    double m_terrainZCorrectionMetres; /* Terrain altitude correction. */
    double m_terrainLandThreshMetres;  /* Terrain land/sea threshold. */
    UINT32 m_terrainLandRGB;           /* Land colour. */
    UINT32 m_terrainSeaRGB;            /* Sea colour. */
    double m_terrainTileSizeMetres;    /* Tile size, zero for auto. */
    SPxTerrainDatabase *m_terrainDB;   /* Terrain database. */
    int m_forceTiledMapRedraw;     /* Force a redraw of the terrain map. */

    /* Terrain tiled map. */
    int m_showTerrainMap;                         /* Show terrain map. */
    SPxTiledMapDatabase *m_tiledMapDB;            /* Tiled map database. */
    SPxCriticalSection m_terrainMapBitmapMutex;   /* Terrain image bitmap mutex. */
    UINT32 m_lastTerrainMapBitmapUpdateTime;      /* Last bitmap update time. */
#ifdef _WIN32
    SPxBitmapWin *m_terrainMapBitmap;             /* Terrain image bitmap. */
    SPxTiledMapRendererWin *m_tiledMapRenderer;   /* Tiled map renderer. */
#else
    /* Add Linux objects here. */
#endif

    /* Terrain radar video. */
    int m_showTerrainRadar;            /* Show radar video. */

    /* Tracks. */
    int m_isTrackLatLongEnabled;       /* Use lat/long in track reports? */
    int m_isTrackPosExtrapolateEnabled; /* Extrapolate track positions? */

    /* Lighting. */
    double m_ambLightBrightness;       /* Ambient light brightness. */
    double m_difLightBrightness;       /* Diffuse light brightness. */
    double m_difLightPitchDegs;        /* Diffuse light pitch (0 down). */
    double m_difLightYawDegs;          /* Diffuse light yaw (0 north). */

    /* Weather effects. */
    double m_fogDensity;               /* Amount of fog to display. */
    UINT32 m_fogRGB;                   /* Fog colour. */

    /* Background. */
    UINT32 m_backgroundRGB;            /* Background colour. */

    /*
     * Private functions.
     */

}; /* SPxVidSimScene */

#endif /* _SPX_VID_SIM_SCENE_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
