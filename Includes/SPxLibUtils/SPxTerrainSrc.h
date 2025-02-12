/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxTerrainSrc object which is the base class for
*	external sources of terrain data to the terrain database.
*
*
* Revision Control:
*   06/10/20 v1.2    AGC	Pass cache info to GetHeight().
*
* Previous Changes:
*   04/08/20 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_TERRAIN_SRC_H
#define _SPX_TERRAIN_SRC_H

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

class SPxTerrainSrc : public SPxObj
{
public:
    SPxTerrainSrc(void) {}
    virtual ~SPxTerrainSrc(void) {}

    virtual SPxErrorCode SetURL(const char *url) = 0;
    virtual SPxErrorCode GetURL(char *url, unsigned int urlLen) const = 0;

    virtual double GetHeight(double latDegs, double longDegs,
			     int updateCacheTime,
			     unsigned int minCacheSecs) = 0;

    virtual double GetMinLatDegs(void) const = 0;
    virtual double GetMaxLatDegs(void) const = 0;
    virtual double GetMinLongDegs(void) const = 0;
    virtual double GetMaxLongDegs(void) const = 0;

}; /* SPxTerrainSrc */

/* Factory for generating terrain source objects. */
class SPxTerrainSrcFactory : public SPxObj
{
public:
    /* Public functions. */
    SPxTerrainSrcFactory(void) {}
    virtual ~SPxTerrainSrcFactory(void) {}
    virtual int IsSupported(void) const { return TRUE; }

    virtual const char *GetSrcName(void) const = 0;

    virtual const char *GetDefFileExtension(void) const = 0;
    virtual const char *GetFileExtensionList(void) const = 0;

    virtual SPxTerrainSrc *CreateSrc(void) = 0;

}; /* SPxTerrainSrcFactory. */

#endif /* _SPX_TERRAIN_SRC_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
