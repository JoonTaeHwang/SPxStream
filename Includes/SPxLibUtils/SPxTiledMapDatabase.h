/*********************************************************************
*
* (c) Copyright 2012 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxTiledMapDatabase class which provides
*   map tiles for display by the SPxTiledMapRenderer class.
*
* Revision Control:
*   20/10/20 v1.31   REW	Support SkyVector aeronautical charts.
*
* Previous Changes:
*   13/10/20 1.30   AGC	Add more download queue query functions.
*   04/08/20 1.29   AGC	Handle map source deletion.
*   12/11/19 1.28   AGC	Fix data race on tile image format.
*   05/10/18 1.27   SP 	Add SetDownloadMaxQueueLength().
*   03/10/18 1.26   SP 	Support server specific min and max zoom.
*   02/10/18 1.25   SP 	Support larger tiles for Google Maps.
*   02/10/18 1.24   SP 	Move utility functions here from SPxTiledMap.h.
*   01/10/18 1.23   SP 	Add support for Google Maps.
*   28/09/18 1.22   SP 	Add support for Bing Maps.
*   08/03/17 1.21   AGC	Use atomic for area caching in progress flag.
*   16/11/16 1.20   SP 	Tag built-in MapQuest and Stamen Terrain maps
*                       as no longer available.
*   03/10/16 1.19   AGC	Add mutex protection.
*   21/09/16 1.18   AGC	Use atomic variable for download success.
*   02/09/16 1.17   AGC	Use atomic variables for cache progress flags.
*   08/07/16 1.16   AGC	Add file mutex lock to avoid race between database and renderer.
*   16/05/14 1.15   SP 	Many changes to simplify use of custom
*			    server URL and source objects.
*                       Add ListCacheAreaTiles().
*                       Add TILE_NAME_FORMAT_QUERY_XYZ format.
*   01/05/14 1.14   SP 	Support auto update of tiles in cache.
*                       Support manual update of entire cache.
*   04/10/13 1.13   AGC	Simplify headers.
*   01/08/13 1.12   AGC	Add parameter interface.
*   21/05/13 1.11   REW	Remove trailing comma from last enum entries.
*   21/05/13 1.10   SP 	Various changes to improve flexibility.
*			Support quadkey tile names.
*   25/01/13 1.9    AGC	Add GetServerObj() function.
*   09/11/12 1.8    AGC	Support tiles from object.
*   30/08/12 1.7    REW	Remove trailing comma from last enum entries.
*   30/08/12 1.6    SP 	Validate image files.
*			Support JPEG and TIF tile image format.
*			Support multiple cache directories.
*			Support a number of predefined servers.
*			Make SetConfigDir() private.
*   21/08/12 1.5    SP 	Derive class from SPxTiledMap.
*   18/07/12 1.4    SP 	Rename some variables.
*   13/07/12 1.3    SP 	Use SPxPath for cache directory.
*   21/06/12 1.2    SP 	Add support for Windows.
*   06/06/12 1.1    SP 	Initial version.
* 
**********************************************************************/

#ifndef _SPX_TILED_MAP_DATABASE_H
#define _SPX_TILED_MAP_DATABASE_H

/*
 * Other headers required.
 */

/* We need the base class. */
#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxAtomic.h"
#include "SPxLibUtils/SPxCallbackList.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxSysUtils.h"
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Debug flags. */
#define SPX_TILED_MAP_DEBUG_DOWNLOAD    0x00000001
#define SPX_TILED_MAP_DEBUG_CACHE       0x00000002
#define SPX_TILED_MAP_DEBUG_PLACEHOLDER 0x00000004

/* Default minimum and maximum tile zoom. Previously only these these
 * zoom levels were supported, however with the addition of support for 
 * Google Maps, higher zoom tiles can now be downloaded.
 */
#define SPX_TILED_MAP_DEFAULT_MIN_ZOOM        0
#define SPX_TILED_MAP_DEFAULT_MAX_ZOOM        19

/* Max/min latitude is +/- this value. The value of (2 * 85.0511) 
 * is the height of the world in degrees latitude using a 
 * Mercator projection. 
 */
#define SPX_TILED_MAP_MAX_LAT_DEGS    85.0511

/* Default tile size. Previously only this size was supported.
 * However with the addition of support for Google Maps, larger 
 * tiles can now be downloaded.
 */
#define SPX_TILED_MAP_DEFAULT_TILE_SIZE_PIXELS 256

/* Mxximum tile size. */
#define SPX_TILED_MAP_MAX_TILE_SIZE_PIXELS 2048

/*********************************************************************
*
*   Macros
*
**********************************************************************/

/* Maximum number of server source objects. 
 * Must match SERVER_OBJECT_n definitions!!
 */
#define SPX_TILED_MAP_MAX_SRC_OBJECTS   4

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxTiledMapSrc;
class SPxThread;

/*
 * Define our class, derived from standard base class.
 */
class SPxTiledMapDatabase : public SPxObj
{
public:
    /*
     * Public types.
     */

    /* Server presets. Don't change the value assigned to 
     * servers as the values are used in configuration files!!
     */
    typedef enum
    {
        /* Custom (user defined) server. */
        SERVER_CUSTOM = 0,

        /* Open street map tiles rendered using mapnik. */
        SERVER_OSM_MAPNIK = 1,

        /* MapQuest map tiles (discontinued mid-2016). */
        SERVER_MAPQUEST = 2,

        /* MapQuest aerial photograph tiles (discontinued mid-2016). */
        SERVER_MAPQUEST_OPEN_AERIAL = 3,

        /* Open sea map chart seamark overlays. There are
         * currently some problems displaying this as tiles
         * are only provided for sea areas. The server does not
         * provide tiles for land area, not even a placeholder.
         * Therefore the renderer reports tiles over land as
         * failed downloads.
         */
        SERVER_OPEN_SEA_MAP = 4,

        /* Stamen terrain map tiles (was North America only - now not available). */
        SERVER_STAMEN_TERRAIN = 5,

        /* Tiles from an SPxTileMapSrc object.
         * Must match SPX_TILED_MAP_MAX_SRC_OBJECTS value!!
         */
        SERVER_OBJECT_1 = 6,
        SERVER_OBJECT_2 = 7,
        SERVER_OBJECT_3 = 8,
        SERVER_OBJECT_4 = 9,

        /* Microsoft Bing road map - requires a license key. */
        SERVER_BING_ROAD = 10,

        /* Microsoft Bing aerial map - requires a license key. */
        SERVER_BING_AERIAL = 11,

        /* Microsoft Bing aerial and road map and labels - requires a license key. */
        SERVER_BING_AERIAL_HYBRID = 12,

        /* Google road map - requires a license key. */
        SERVER_GOOGLE_ROAD = 13,

        /* Google aerial map - requires a license key. */
        SERVER_GOOGLE_AERIAL = 14,

        /* Google aerial map and road map and labels - requires a license key. */
        SERVER_GOOGLE_AERIAL_HYBRID = 15,

        /* Google terrain map - requires a license key. */
        SERVER_GOOGLE_TERRAIN = 16,

	/* SkyVector aeronautical charts - require a license key. */
	SERVER_SKYVECTOR_BASIC = 17,
	SERVER_SKYVECTOR_LOW = 18,
	SERVER_SKYVECTOR_HIGH = 19,
	SERVER_SKYVECTOR_VFR = 20,

        /* Must be last entry!! */
        MAX_NUM_SERVERS 
        
    } Server_t;

    /* Tile name format. */
    typedef enum
    {
        /* Tile name is specified as <z>/<x>/<y>.<ext>. */
        TILE_NAME_FORMAT_ZXY = 1,

        /* Tile name is specified as a quadkey, e.g. 123.<ext>. */
        TILE_NAME_FORMAT_QUADKEY = 2,

        /* Tile name is specified as a query string in the
         * form x=<x>&y=<y>&z=<z>. Note that the query string
         * does not include a file extension.
         */
        TILE_NAME_FORMAT_QUERY_XYZ = 3,

        /* Tile format specific to Google Maps. The string
         * defines the lat/long centre of the tile, zoom,
         * size in pixels and other things.
         */
        TILE_NAME_FORMAT_GOOGLE = 4

    } TileNameFormat_t;

    /* Tile format. */
    typedef enum
    {
        /* PNG image format (".png" file extension). */
        TILE_IMAGE_FORMAT_PNG = 1,

        /* JPG image format (".jpg" file extension). */
        TILE_IMAGE_FORMAT_JPG = 2,

        /* JPEG image format (".jpeg" file extension). */
        TILE_IMAGE_FORMAT_JPEG = 3,

        /* TIF image format (".tif" file extension). */
        TILE_IMAGE_FORMAT_TIF = 4,

        /* TIFF image format (".tiff" file extension). */
        TILE_IMAGE_FORMAT_TIFF = 5

    } TileImageFormat_t;

    /* Tile download mode. */
    typedef enum
    {
        /* If a tile is not available it will be downloaded
         * in the background and the application notified
         * when it is available (default).
         */
        DOWNLOAD_MODE_BACKGROUND = 0,

        /* A call to retrieve a tile that is not in the cache
         * will block until the tile is downloaded or a timeout
         * occurs. Not recommended.
         */
        DOWNLOAD_MODE_BLOCK = 1
    
    } DownloadMode_t;

    /* Event types. */
    typedef enum
    {
        /* A tile is available in cache for the first
         * time or the tile image file in cache has been 
         * updated.
         */
        EVENT_TILE_UPDATED = 0,

        /* The tile sever and cache directory has changed. */
        EVENT_SERVER_CHANGED = 1,

        /* The cache directory has been deleted. */
        EVENT_CACHE_DELETED = 2
    
    } EventType_t;

    /* Generic event info - prefixes all events. */
    typedef struct
    {
        EventType_t type;
    
    } EventInfo_t;
            
    /* Tile event callback info. */
    typedef struct
    {
        EventInfo_t event;   /* Generic event info. */
        int x;               /* X index. */
        int y;               /* Y index. */
        int z;               /* Zoom level. */
            
    } TileEventInfo_t;

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxTiledMapDatabase(const char *configDir=NULL);
    virtual ~SPxTiledMapDatabase(void);

    /* General configuration. */
    char *SetName(const char *newName);
    void SetDebug(UINT32 flags) { m_debugFlags = flags; }
    UINT32 GetDebug(void) { return m_debugFlags; }
    SPxErrorCode GetShortConfigDir(char *buf, unsigned int bufSize);
    SPxErrorCode GetFullConfigDir(char *buf, unsigned int bufSize);
    SPxErrorCode GetFullCacheDir(char *buf, unsigned int bufSize);

    /* Currently selected server. */
    int IsServerAvailable(Server_t server);
    SPxErrorCode SetServer(Server_t server);
    Server_t GetServer(void) const { return m_server; }
    SPxErrorCode GetServerName(char *buf, unsigned int bufSize) const;
    SPxErrorCode GetURLPrefix(char *buf, unsigned int bufSize) const;
    SPxErrorCode GetURLSuffix(char *buf, unsigned int bufSize) const;
    TileNameFormat_t GetTileNameFormat(void) const { return m_tileNameFormat; }
    TileImageFormat_t GetTileImageFormat(void) const { return static_cast<TileImageFormat_t>(static_cast<UINT32>(m_tileImageFormat)); }
    int IsTileYInverted(void) const { return m_isTileYInverted; }
    SPxTiledMapSrc *GetServerObj(void) { return m_serverObj; }

    /* Bing Maps specific. */
    int IsBingServer(void);
    SPxErrorCode SetBingKey(const char *key);
    SPxErrorCode GetBingKey(char *buf, unsigned int bufSize) const;

    /* Google Maps specific. */
    int IsGoogleServer(void);
    SPxErrorCode SetGoogleKey(const char *key);
    SPxErrorCode GetGoogleKey(char *buf, unsigned int bufSize) const;
    SPxErrorCode SetGoogleTileSize(unsigned int pixels);
    unsigned int GetGoogleTileSize(void) const { return m_googleTileSizePixels; }

    /* SkyVector specific. */
    int IsSkyVectorServer(void);
    SPxErrorCode SetSkyVectorKey(const char *key);
    SPxErrorCode GetSkyVectorKey(char *buf, unsigned int bufSize) const;

    /* Custom server. */
    SPxErrorCode SetCustomName(const char *name);
    SPxErrorCode GetCustomName(char *buf, unsigned int bufSize) const;
    SPxErrorCode SetCustomURLPrefix(const char *urlPrefix);
    SPxErrorCode GetCustomURLPrefix(char *buf, unsigned int bufSize) const;
    SPxErrorCode SetCustomURLSuffix(const char *urlSuffix);
    SPxErrorCode GetCustomURLSuffix(char *buf, unsigned int bufSize) const;
    SPxErrorCode SetCustomTileNameFormat(TileNameFormat_t nameFormat);
    TileNameFormat_t GetCustomTileNameFormat(void) const { return m_customTileNameFormat; }
    SPxErrorCode SetCustomTileImageFormat(TileImageFormat_t imageFormat);
    TileImageFormat_t GetCustomTileImageFormat(void) const { return m_customTileImageFormat; }
    SPxErrorCode SetCustomTileYInverted(int isInverted);
    int IsCustomTileYInverted(void) const { return m_isCustomTileYInverted; }

    /* Server source objects. */
    SPxErrorCode AddServerSrc(SPxTiledMapSrc *src);
    SPxErrorCode RemoveServerSrc(SPxTiledMapSrc *src);

    /* Tile download and caching. */
    SPxErrorCode SetDownloadMode(DownloadMode_t mode);
    DownloadMode_t GetDownloadMode(void) const;
    SPxErrorCode SetDownloadTimeoutSecs(unsigned int secs);
    unsigned int GetDownloadTimeoutSecs(void) const;
    unsigned int GetDownloadPercentSuccess(void) const;
    unsigned int GetDownloadQueueLength(void) const { return m_reqTileListLen; }
    int IsDownloadQueueEmpty(void) const { return m_isReqTileListEmpty; }
    SPxErrorCode SetDownloadMaxQueueLength(unsigned int len);
    unsigned int GetDownloadMaxQueueLength(void) const { return m_reqTileListMaxLen; }
    SPxErrorCode SetCacheExpireSecs(UINT32 secs);
    UINT32 GetCacheExpireSecs(void) const { return m_cacheExpireSecs; }
    int IsPreserveCacheAllowed(void) const { return m_isPreserveCacheAllowed; }
    int IsAreaCacheAllowed(void) const { return m_isAreaCacheAllowed; }

    /* Tile requests. */
    SPxErrorCode GetTile(int x, int y, int z,
                         char *buf, 
                         unsigned int bufSize,
                         int *obtainedZ=NULL);

    SPxErrorCode GetTileFromServer(int x, int y, int z,
                                   char *buf, 
                                   unsigned int bufSize);

    SPxErrorCode GetTileFromCache(int x, int y, int z,
                                  char *buf, 
                                  unsigned int bufSize,
                                  int *obtainedZ=NULL);

    SPxErrorCode GetTilePlaceholder(char *buf, unsigned int bufSize);

    unsigned int GetTileRequestsPercentComplete(void);

    /* Area caching. */
    SPxErrorCode SetCacheArea(double latNDegs, double lonWDegs,
                              double latSDegs, double lonEDegs,
                              int minZ, int maxZ,
                              unsigned int maxNumTilesToDownload=9999);

    SPxErrorCode GetCacheArea(double *latNDegsRtn, double *lonWDegsRtn,
                              double *latSDegsRtn, double *lonEDegsRtn,
                              int *minZRtn, int *maxZRtn);
            
    SPxErrorCode ListCacheAreaTiles(FILE *fileHandle);

    SPxErrorCode StartCachingArea(void);

    SPxErrorCode StopCachingArea(void);

    unsigned int GetCacheAreaPercentComplete(void);

    unsigned int GetCacheAreaNumTiles(void) 
    { 
        return m_cacheAreaNumTiles; 
    }

    unsigned int GetCacheAreaNumTilesToDownload(void)
    {
        return m_cacheAreaNumTilesToDownload;
    }

    unsigned int GetCacheAreaNumTilesToUpdate(void)
    {
        return m_cacheAreaNumTilesToUpdate;
    }

    unsigned int GetCacheAreaNumTilesDownloaded(void)
    {
        return m_cacheAreaNumTilesDownloaded;
    }

    unsigned int GetCacheAreaNumTilesFailed(void)
    {
        return m_cacheAreaNumTilesFailed;
    }

    int IsAreaCachingInProgress(void) 
    { 
        return m_isAreaCachingInProgress; 
    }

    /* Cache deletion. */
    SPxErrorCode DeleteCache(void);
    SPxErrorCode StartDeletingCache(void);
    int IsCacheDeletionInProgress(void) 
    { 
        return m_isCacheDeletionInProgress; 
    }

    /* Cache updating. */
    SPxErrorCode UpdateCache(void);
    SPxErrorCode StartUpdatingCache(void);
    int IsCacheUpdateInProgress(void) 
    { 
        return m_isCacheUpdateInProgress; 
    }

    /* Event callbacks. */
    SPxErrorCode AddEventCallback(SPxCallbackListFn_t fn, void *userArg);
    SPxErrorCode RemoveEventCallback(SPxCallbackListFn_t fn, void *userArg);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* Utility functions.*/
    unsigned int GetTileSizePixels(void) { return m_tileSizePixels; }
    int GetNumXYTiles(int z);
    double GetNumXYTilesD(int z);

    int GetMinZoom(void) { return m_minZoom; }
    int GetMaxZoom(void) { return m_maxZoom; }

    int GetTileXFromLon(double lonDegs, int z);
    int GetTileYFromLat(double latDegs, int z);

    double GetLonWFromTileX(int x, int z);
    double GetLatNFromTileY(int y, int z);

    double GetLonCentreFromTileX(int x, int z);
    double GetLatCentreFromTileY(int y, int z);

    double GetTilesWidthDegs(int eastX, int westX, int z);
    double GetTilesHeightDegs(int northY, int southY, int z);

    int GetNumXTiles(double lonEDegs, 
                     double lonWDegs,
                     int z);
    int GetNumYTiles(double latNDegs, 
                     double latSDegs,
                     int z);

    void InflateTiles(int *x, int *y, 
                      int *numXTiles, int *numYTiles,
                      int z,
                      int extraXTiles, int extraYTiles);

    int GetClosestZoom(double degsLonPerPixel);

    int IsTileInView(int x, int y, int z,
                     double latNDegs, double lonWDegs,
                     double latSDegs, double lonEDegs);

    void GetTilesInView(double latNDegs, double lonWDegs,
                        double latSDegs, double lonEDegs,
                        int z,
                        int *xRtn, int *yRtn,
                        int *numXRtn, int *numYRtn);

    void GetTileSubArea(int zIn,
                        int xSub, int ySub, int zSub,
                        int *xPixelsRtn, int *yPixelsRtn,
                        double *scaleRtn);

    double MercatorLatToYPixels(int mapHeightPixels, 
                                double latNorthDegs, 
                                double latSouthDegs,
                                double latDegs);

    double MercatorYPixelsToLat(int mapHeightPixels, 
                                double latNorthDegs, 
                                double latSouthDegs,
                                double yPixels);

    double MercatorLatToLinearY(double latDegs);

    double MercatorLinearYToLat(double linearY);

    double LatAdd(double latDegs, double valDegs);

    double LongAdd(double longDegs, double valDegs);

    double CalculateRotation(double startDegs, double endDegs);

    double NormaliseAngle(double degs);

protected:
    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

private:
    /*
     * Private types.
     */

    /* Entries in tile download list. */
    typedef struct _Tile_t
    {
        _Tile_t *next;
        int x; /* X index. */
        int y; /* Y index. */
        int z; /* Zoom level. */
        unsigned int retryCount; /* No. of retries. */
            
    } Tile_t;

    /*
     * Private variables.
     */

    /* General mutex protection. */
    mutable SPxCriticalSection m_mutex;

    /* Debugging. */
    UINT32 m_debugFlags;
    FILE *m_logFile;

    /* Current server ID. */
    Server_t m_server;

    /* Current server name. */
    char *m_serverName;

    /* Current tile size. */
    unsigned int m_tileSizePixels;

    /* Current server URL prefix. */
    char *m_urlPrefix;

    /* Additional info to append to the current URL. */
    char *m_urlSuffix;

    /* Min and max zoom levels. */
    int m_minZoom;
    int m_maxZoom;

    /* Allow cache to kept once application has closed? */
    int m_isPreserveCacheAllowed;

    /* Allow bulk caching of an area? */
    int m_isAreaCacheAllowed;

    /* Bing Maps specific items. */
    char *m_bingKey;
    int m_isBingPreserveCacheAllowed;
    int m_isBingAreaCacheAllowed;
    UINT32 m_lastBingErrTime;

    /* Google Maps specific items. */
    char *m_googleKey;
    int m_isGooglePreserveCacheAllowed;
    int m_isGoogleAreaCacheAllowed;
    UINT32 m_lastGoogleErrTime;
    unsigned int m_googleTileSizePixels;

    /* SkyVector specific items. */
    char *m_skyVectorKey;
    UINT32 m_lastSkyVectorErrTime;

    /* Current object to generate map tiles. */
    SPxTiledMapSrc *m_serverObj;

    /* Current tile image format. */
    SPxAtomic<UINT32> m_tileImageFormat;

    /* Current tile name format. */
    TileNameFormat_t m_tileNameFormat;

    /* Is tile Y index inverted for currect server? */
    int m_isTileYInverted;

    /* Database 256x256 placeholder tile and config directory. */
    SPxPath m_placeholder256Path;

    /* Cache sub-directory in configuration directory. */
    char *m_cacheSubdir;

    /* Filename extension ".png", ".jpeg", etc. */
    char *m_filenameExt;

    /* Tile download mode. */
    DownloadMode_t m_downloadMode;

    /* Tile download timeout in seconds. */
    unsigned int m_downloadTimeoutSecs;

    /* Max times we can try to download a file. */
    unsigned int m_maxDownloadRetries;

    /* Current tile download success rate. */
    SPxAtomic<int> m_downloadSuccessPercent;

    /* Thread use to download tiles in the background. */
    SPxThread *m_backgroundThread;

    /* Callback list use to notify application
     * when events have occured.
     */
    SPxCallbackList *m_eventCallbackList;

    /* Mutex to protect file operations. */
    SPxCriticalSection m_fileMutex;
    
    /* List of requested tiles to download. */
    Tile_t *m_reqTileList;

    /* Number of requested tile downloads that are pending. */
    unsigned int m_reqTileListLen;

    /* Tile request list high water mark. */
    unsigned int m_reqTileListHighLen;

    /* Maximum number of tile requests to queue up. */
    unsigned int m_reqTileListMaxLen;

    /* Is tile list queue empty and any in-progress download complete? */
    int m_isReqTileListEmpty;

    /* Mutex to pretect access to requested tile list. */
    mutable SPxCriticalSection m_reqTileListMutex;

    /* Maximum age of tiles in cache in seconds since Epoch. */
    UINT32 m_cacheExpireSecs;

    /* Is cache area set? */
    int m_isCacheAreaSet;

    /* Is area caching currently in progress? */
    SPxAtomic<int> m_isAreaCachingInProgress;

    /* Lat/long area to cache. */
    double m_cacheAreaLatNDegs;
    double m_cacheAreaLatSDegs;
    double m_cacheAreaLonEDegs;
    double m_cacheAreaLonWDegs;

    /* Min and max zoom to use for area caching. */
    int m_cacheAreaMinZoom;
    int m_cacheAreaMaxZoom;

    /* Total number of tiles in cache area. */
    unsigned int m_cacheAreaNumTiles;

    /* Number of tiles to process in cache area. This defines
     * the estimated number to tiles that we will need process
     * until all tiles (or the maximum number of tiles) have
     * been downloaded. Processing a tile means checking to
     * see if it is already in the cache and if not then
     * downloading it. This parameter is used to estimate 
     * percentage completion of the area caching operation.
     */
    unsigned int m_cacheAreaNumTilesToProcess;

    /* Number of tiles processed so far in cache area. See above. */
    unsigned int m_cacheAreaNumTilesProcessed;

    /* Number of tiles to download in cache area. This may be
     * limited by the maximum number of tiles to download defined 
     * when the cache area was set.
     */
    unsigned int m_cacheAreaNumTilesToDownload;

    /* Number of tiles in the cache area that are currently
     * in the cache but expired and will be updated.
     */
    unsigned int m_cacheAreaNumTilesToUpdate;

    /* Number of tiles downloaded so far in cache area. */
    unsigned int m_cacheAreaNumTilesDownloaded;

    /* Number of tiles that could not be downloaded in cache area. */
    unsigned int m_cacheAreaNumTilesFailed;

    /* Next tile to download in the cache area. Note that the
     * X value is NOT normalised when crossing the meridian.
     * This must be performed when using the X value by wrapping
     * it to the value of GetNumXYTiles(m_cacheAreaNextTile.z).
     */
    Tile_t m_cacheAreaNextTile;

    /* Is cache deletion currently in progress? */
    SPxAtomic<int> m_isCacheDeletionInProgress;

    /* Is cache update currently in progress? */
    SPxAtomic<int> m_isCacheUpdateInProgress;

    /* Custom server settings. */
    char *m_customName;
    char *m_customUrlPrefix;
    char *m_customUrlSuffix;
    TileImageFormat_t m_customTileImageFormat;
    TileNameFormat_t m_customTileNameFormat;
    int m_isCustomTileYInverted;
    unsigned int m_customTileSizePixels;
    int m_customMinZoom;
    int m_customMaxZoom;

    /* Server source objects. */
    SPxTiledMapSrc *m_sources[SPX_TILED_MAP_MAX_SRC_OBJECTS];

    /*
     * Private functions.
     */

    /* Server selection. */
    SPxErrorCode SelectServerSrc(SPxTiledMapSrc *src);
    SPxErrorCode SelectServerURL(const char *name,
                                 const char *urlPrefix,
                                 const char *urlSuffix,
                                 TileNameFormat_t tileNameFormat,
                                 TileImageFormat_t tileImageFormat,
                                 int isTileYInverted,
                                 unsigned tileSizePixels,
                                 int minZoom,
                                 int maxZoom,
                                 int isPreserveCacheAllowed,
                                 int isAreaCacheAllowed);

    /* General. */
    SPxErrorCode BuildQuadkeyTileName(int x, int y, int z,
                                      char *buf, 
                                      unsigned int bufSize);
    SPxErrorCode BuildGoogleTileName(int x, int y, int z,
                                     char *buf, 
                                     unsigned int bufSize);
    SPxErrorCode BuildTileURL(int x, int y, int z,
                              char *buf, 
                              unsigned int bufSize);
    SPxErrorCode BuildTileFilename(int x, int y, int z,
                                   int appendPid,
                                   char *buf, 
                                   unsigned int bufSize);
    SPxErrorCode GetTileXYZFromFilename(const char *filename,
                                        int *xRtn, int *yRtn, int *zRtn);
    SPxErrorCode SetConfigDir(const char *dirName);
    SPxErrorCode DownloadTile(const char *tileURL, FILE *imgFilePtr);
    SPxErrorCode GetTileFromCacheInternal(int x, int y, int z,
                                          char *buf, unsigned int bufSize);
    int IsImageValid(const unsigned char *imgBuf, 
                     unsigned int imgBufSize);

    /* Server specific. */
    SPxErrorCode UpdateBingURL(void);
    SPxErrorCode UpdateGoogleURL(void);
    SPxErrorCode UpdateSkyVectorURL(void);

    /* Tile requests. */
    SPxErrorCode RequestTileForDownload(int x, int y, int z);
    void AddTileToRequestList(Tile_t *tile);
    void ClearTileRequestList(void);
    SPxErrorCode ServiceTileRequests(void);
    int IsTileRequestListEmpty(void) const;

    /* Caching. */
    SPxErrorCode ServiceCacheArea(void);
    int NextCacheAreaTile(void);
    SPxErrorCode UpdateCacheSubdir(const char *dir);
    SPxErrorCode DeleteCachesOnExit(void);

    /*
     * Private static functions.
     */
 
    static void *BackgroundThreadHandler(SPxThread *thread);
    static int SourceDeleted(void *invokingObject,
                             void *userObject,
                             void *arg);
    
}; /* SPxTiledMapDatabase */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_TILED_MAP_DATABASE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
