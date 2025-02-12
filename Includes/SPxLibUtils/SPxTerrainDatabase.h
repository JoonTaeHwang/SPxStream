/*********************************************************************
*
* (c) Copyright 2012 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxTerrainDatabase object which manages and
*	downloads terrain tiles.
*
*
* Revision Control:
*   09/09/22 v1.29   AGC	Add GetFullRadarArea().
*
* Previous Changes:
*   14/07/22 1.28   AGC	Improve internal access to cache directory.
*   17/12/21 1.27   AGC	Support user specified caching.
*   24/11/21 1.26   AGC	Add GetRadarArea().
*   03/09/21 1.25   AGC	Add GetHeightsFromRadar() function.
*   15/02/21 1.24   AGC	Improve terrain tile validation.
*   12/02/21 1.23   AGC	Fix N/C #831 - default terrain server has gone.
*   04/08/20 1.22   AGC	Support custom sources.
*   24/04/20 1.21   AGC	Add SetRadarAreaMetres().
*   07/05/19 1.20   AGC	Support min/max latitude for server.
*   03/05/18 1.19   AGC	Add IsTiledSupported/Cached() functions.
*   31/10/16 1.18   AGC	Add flag to control cache time updates.
*   20/07/16 1.17   AGC	Avoid duplicate servers when migrating old parameters.
*   14/07/16 1.16   AGC	Add GetNumTilesInRadarAreas().
*   08/07/16 1.15   AGC	Make GetHeight functions virtual for testing.
*   16/03/16 1.14   AGC	Delete parameters when removing servers.
*   22/10/14 1.13   AGC	Allow tile size to be retrieved with height.
*   16/10/14 1.12   AGC	Support SRTM1 data.
*   15/10/14 1.11   AGC	Fix terrain generation at anti-meridian.
*			Fix terrain generation near SRTM limits.
*   14/10/14 1.10   AGC	Support multiple terrain servers.
*			Add GetIntersectDistance() function.
*   24/09/14 1.9    AGC	Add GetHeightInterpolate/NoInterpolate() functions.
*   08/05/14 1.8    AGC	Support not caching with GetHeight().
*   05/09/13 1.7    AGC	Add GetNumTilesToCache() function.
*   25/04/13 1.6    AGC	Use doubles for terrain heights.
*   08/04/13 1.5    AGC	Never lock terrain database while downloading tiles.
*   28/03/13 1.4    AGC	Improve performance.
*   25/01/13 1.3    AGC	Add flags to events.
*   09/11/12 1.2    AGC	Improve cache area management.
*			Allow blocking height retrieval.
*   26/10/12 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_TERRAIN_DATABASE_H
#define _SPX_TERRAIN_DATABASE_H

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For base classes. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxLock.h"

/*
 * Types
 */

/* Value indicating no data was available. */
#define SPX_TERRAIN_VAL_NO_SUPPORT (32767)
#define SPX_TERRAIN_VAL_INVALID (-32768)

/* Forward declarations. */
struct SPxLatLong_tag;
class SPxThread;
class SPxViewControl;
class SPxTerrainSrc;
class SPxTerrainSrcFactory;
struct SPxTerrainTile;


enum SPxTerrainDatabaseFlags
{
    SPX_TERRAIN_DB_FLAGS_NONE = 0x00,
    SPX_TERRAIN_DB_FLAGS_BLOCKING = 0x01,
    SPX_TERRAIN_DB_FLAGS_SERVER = 0x2
};

enum SPxTerrainServer
{
    SPX_TERRAIN_SERVER_SRTM1 = 0,
    SPX_TERRAIN_SERVER_SRTM3 = 1,
    SPX_TERRAIN_SERVER_DTED = 2,
    SPX_TERRAIN_SERVER_CUSTOM_1 = 16,
    SPX_TERRAIN_SERVER_CUSTOM_2 = 17,
    SPX_TERRAIN_SERVER_CUSTOM_3 = 18,
    SPX_TERRAIN_SERVER_CUSTOM_4 = 19,
    SPX_TERRAIN_SERVER_CUSTOM_5 = 20,
    SPX_TERRAIN_SERVER_CUSTOM_6 = 21,
    SPX_TERRAIN_SERVER_CUSTOM_7 = 22,
    SPX_TERRAIN_SERVER_CUSTOM_8 = 23
};

/* Callback types. */
typedef SPxErrorCode (*SPxTerrainDatabaseFn_t)(UINT32 flags, void *userArg);

/* Define our class. */
class SPxTerrainDatabase : public SPxObj, public SPxLock
{
public:
    /* Public types. */
    struct ServerInfo;

    /* Public functions. */
    SPxTerrainDatabase(void);
    virtual ~SPxTerrainDatabase(void);

    unsigned int AddServer(SPxTerrainServer server, const char *url=NULL);
    unsigned int AddServer(SPxTerrainServer server, const char *url,
			   unsigned int index, int reuseIndex=FALSE);
    SPxErrorCode RemoveServer(unsigned int serverPos);
    SPxErrorCode MoveServer(unsigned int serverPos, int change);
    unsigned int GetNumServers(void) const;
    int IsServerSet(unsigned int serverPos) const;
    SPxErrorCode SetServerType(unsigned int serverPos, SPxTerrainServer server);
    SPxErrorCode GetServerType(unsigned int serverPos, SPxTerrainServer *server) const;
    SPxErrorCode SetServerURL(unsigned int serverPos, const char *url);
    SPxErrorCode GetServerURL(unsigned int serverPos, char *buf, int bufLen) const;
    SPxErrorCode SetServerMinLatDegs(unsigned int serverPos, double minLatDegs);
    SPxErrorCode GetServerMinLatDegs(unsigned int serverPos, double *minLatDegs) const;
    SPxErrorCode SetServerMaxLatDegs(unsigned int serverPos, double maxLatDegs);
    SPxErrorCode GetServerMaxLatDegs(unsigned int serverPos, double *maxLatDegs) const;

    SPxErrorCode AddServerFactory(SPxTerrainServer server, SPxTerrainSrcFactory *obj);
    int IsServerFactoryAvailable(SPxTerrainServer server) const;
    SPxErrorCode GetServerFactory(SPxTerrainServer server, SPxTerrainSrcFactory **obj);

    /* Cache management. */
    SPxErrorCode SetCacheDir(const char *dir);
    SPxErrorCode GetShortCacheDir(char *buf, unsigned int bufLen);
    SPxErrorCode GetFullCacheDir(char *buf, unsigned int bufLen);
    SPxErrorCode DeleteCache(void);

    /* Settings. */
    SPxErrorCode SetInterpolated(int interpolated);
    int IsInterpolated(void) const;
    SPxErrorCode SetMinCacheSecs(unsigned int minCacheSecs);
    SPxErrorCode GetMinCacheSecs(unsigned int *minCacheSecs) const;

    /* Main data retrieval function. */
    virtual double GetHeight(double latDegs, double longDegs,
			     int cacheIfUnavail, UINT32 now,
			     int overrideInterpolate=FALSE);
    virtual double GetHeightInterpolate(double latDegs, double longDegs,
					UINT32 now, int *latIndRet, int *longIndRet,
					int *sizeX=NULL, int *sizeY=NULL,
					int updateCacheTime=TRUE) const;
    virtual double GetHeightInterpolate(double latDegs, double longDegs,
					int cacheIfUnavail,
					UINT32 now);
    virtual double GetHeightNoInterpolate(double latDegs, double longDegs,
					  int cacheIfUnavail,
					  UINT32 now);
    virtual double GetHeightNow(double latDegs, double longDegs, UINT32 now);
    SPxErrorCode GetHeightsFromRadar(double aziDegs, double spacingMetres,
				     char *buf, unsigned int bufLen,
				     unsigned int radarIndex=0);
    SPxErrorCode CacheTile(int latInd, int longInd);

    /* Other data retrieval functions. */
    SPxErrorCode GetIntersectDistance(double latDegs, double longDegs,
	double bearingDegs, double heightMetres,
	double intervalMetres, double maxDistMetres, double *retDistMetres);
    int IsTileSupported(int latInd, int longInd) const;
    int IsTileCached(int latInd, int longInd) const;

    /* External radar control. */
    SPxErrorCode SetRadarAreaMetres(const SPxLatLong_tag *latLong,
				    double endRangeMetres,
				    unsigned int radarIndex = 0);
    SPxErrorCode SetRadarArea(const SPxLatLong_tag *latLong,
			      double widthDegs,
			      unsigned int radarIndex=0);
    SPxErrorCode GetRadarArea(SPxLatLong_tag *latLong,
			      double *widthDegs,
			      unsigned int radarIndex=0) const;
    SPxErrorCode GetFullRadarArea(SPxLatLong_tag *latLong,
				  double *widthDegs) const;
    SPxErrorCode RemoveRadarArea(unsigned int radarIndex=0);
    unsigned int GetNumTilesInRadarAreas(void) const;
    unsigned int GetNumTilesToCache(void) const;

    /* Caching. */
    SPxErrorCode SetCacheArea(const SPxLatLong_tag *latLong,
			      double widthDegs, double heightDegs);
    SPxErrorCode GetCacheArea(SPxLatLong_tag *latLong,
			      double *widthDegs, double *heightDegs) const;
    SPxErrorCode StartCachingArea(void);
    SPxErrorCode StopCachingArea(void);
    unsigned int GetCacheAreaPercentComplete(void) const;
    int IsAreaCachingInProgress(void) const;

    const char *GetServerName(SPxTerrainServer server) const;

    /* Add/remove event callbacks. */
    SPxErrorCode AddEventCallback(SPxTerrainDatabaseFn_t fn, void *userArg);
    SPxErrorCode RemoveEventCallback(SPxTerrainDatabaseFn_t fn, void *userArg);

    /* Standard parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* SPxLock interface. */
    virtual SPxErrorCode Initialise(void);
    virtual SPxErrorCode Enter(void);
    virtual SPxErrorCode TryEnter(void);
    virtual SPxErrorCode Leave(void);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxTerrainTile *m_tileInfo[180][360];	/* Currently loaded tile information. */

    /* Private functions. */
    SPxErrorCode SetConfigFromState(void);
    SPxErrorCode setSvrParameter(unsigned int index,
				 const char *paramName,
				 const char *paramValue) const;
    SPxErrorCode deleteSvrParameter(unsigned int index,
				    const char *paramName) const;
    SPxErrorCode setServerURL(SPxTerrainServer server, const char *url);
    SPxErrorCode cacheData(void);
    static void *cacheFnWrapper(SPxThread *thread);
    void *cacheFn(SPxThread *thread);
    int getNextTileToCache(int *latInd, int *longInd);
    SPxErrorCode getTile(int latInd, int longInd,
	UINT32 signalFlags=SPX_TERRAIN_DB_FLAGS_NONE);
    SPxErrorCode getTileSRTM1(ServerInfo &serverInfo, int latInd, int longInd,
			      UINT32 signalFlags,
			      const char *cacheDir);
    SPxErrorCode getTileSRTM3(ServerInfo &serverInfo, int latInd, int longInd,
			      UINT32 signalFlags,
			      const char *cacheDir);
    SPxErrorCode getTileDTED(ServerInfo &serverInfo, int latInd, int longInd,
			     UINT32 signalFlags);
    SPxErrorCode getTileCustom(ServerInfo &serverInfo, int latInd, int longInd,
			       UINT32 signalFlags);
    SPxErrorCode getTileSRTM(ServerInfo& serverInfo,
	int latInd, int longInd, const char *url,
	const char *filename, UINT32 signalFlags, int tileSize,
	const char *cachePaths[], unsigned int numCachePaths,
	const char *srcDirs[], unsigned int numSrcDirs);
    SPxErrorCode downloadTileSRTM(ServerInfo& serverInfo,
	const char *url, const char *name,
	const char *srcDirs[], unsigned int numSrcDirs,
	unsigned char *buf, size_t bufSize, int saveTile=TRUE);
    SPxErrorCode unzipTile(unsigned char *destBuf, size_t destBufSize,
			   unsigned char *srcBuf, size_t srcBufSize,
			   size_t *destOutSize=NULL);
    SPxErrorCode signalEvent(UINT32 flags);
    unsigned int addServer(SPxTerrainServer server, const char *url,
	int selectIndex, unsigned int index, int reuseIndex);
    SPxErrorCode clear(void);
    int checkLatLong(double latDegs, double longDegs) const;
    int isTileInCache(int latInd, int longInd) const;
    int isTileInCacheArea(int latInd, int longInd) const;
    int isTileSRTM1Cached(int latInd, int longInd) const;
    int isTileSRTM3Cached(int latInd, int longInd) const;
    int isTileSRTMCached(const char *cachePaths[], unsigned int numCachePaths) const;

}; /* SPxTerrainDatabase. */

#endif /* _SPX_TERRAIN_DATABASE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
