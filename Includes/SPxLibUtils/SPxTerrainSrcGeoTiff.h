/*********************************************************************
*
* (c) Copyright 2020, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxTerrainSrcGeoTiff object which allows terrain
*	data to be read from GeoTIFF files by the terrain database.
*
*
* Revision Control:
*   15/03/21 v1.5    AGC	Add manual ClearCache() function.
*
* Previous Changes:
*   05/03/21 1.4    AGC	Support configurable clear cache interval.
*   06/10/20 1.3    AGC	Support multiple GeoTIFF files.
*			Support caching.
*   03/09/20 1.2    AGC	Support more GeoTIFF files.
*   04/08/20 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_TERRAIN_SRC_GEOTIFF_H
#define _SPX_TERRAIN_SRC_GEOTIFF_H

/* For base class. */
#include "SPxLibUtils/SPxTerrainSrc.h"

/* Forward declarations. */
struct tiff;

/* Define our class. */
class SPxTerrainSrcGeoTiff : public SPxTerrainSrc
{
public:
    /* Public functions. */
    explicit SPxTerrainSrcGeoTiff(unsigned int clearCacheIntervalMS=15000);
    virtual ~SPxTerrainSrcGeoTiff(void);

    virtual SPxErrorCode SetURL(const char *url);
    virtual SPxErrorCode GetURL(char *url, unsigned int urlLen) const;

    virtual double GetHeight(double latDegs, double longDegs,
			     int updateCacheTime,
			     unsigned int minCacheSecs);

    virtual double GetMinLatDegs(void) const;
    virtual double GetMaxLatDegs(void) const;
    virtual double GetMinLongDegs(void) const;
    virtual double GetMaxLongDegs(void) const;

    void ClearCache(void);

private:
    struct FileInfo;
    struct impl;
    SPxAutoPtr<impl> m_p;

    SPxErrorCode loadFile(const char *fullPath,
			  FileInfo *fileInfo);

    template<typename dataType>
    SPxErrorCode readTiffTiles(tiff *tiff,
			       unsigned int width,
			       unsigned int height,
			       float *data);

    template<typename dataType>
    SPxErrorCode readTiffLines(tiff *tiff,
			       unsigned int width,
			       unsigned int height,
			       float *data);

    void clearCache(void);

}; /* SPxTerrainSrcGeoTiff. */

class SPxTerrainSrcFactoryGeoTiff : public SPxTerrainSrcFactory
{
public:
    /* Public functions. */
    SPxTerrainSrcFactoryGeoTiff(void) {}
    virtual ~SPxTerrainSrcFactoryGeoTiff(void) {}

    virtual int IsSupported(void) const { return (_MSC_VER >= 1900); }

    virtual const char *GetSrcName(void) const { return "GeoTIFF"; }

    virtual const char *GetDefFileExtension(void) const { return ".tiff"; }
    virtual const char *GetFileExtensionList(void) const { return "TIFF Files (*.tiff;*.tif)|*.tiff;*.tif|Zip Files (*.zip)|*.zip|"; }

    virtual SPxTerrainSrcGeoTiff *CreateSrc(void) { return new SPxTerrainSrcGeoTiff(); }

}; /* SPxTerrainSrcFactoryGeoTiff. */

#endif /* _SPX_TERRAIN_SRC_GEOTIFF_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
