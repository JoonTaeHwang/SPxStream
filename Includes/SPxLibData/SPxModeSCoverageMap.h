/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxModeSCoverageMap class which supports loading
*   Eurocontrol Mode S coverage map files.
*
* Revision Control:
*   16/10/20 v1.3    AGC	Support retrieval for local node from filename.
*				Support retrieval of default cluster state.
*
* Previous Changes:
*   15/10/20 1.2    AGC	Restrict coverage to single station.
*   17/09/20 1.1    AGC	Initial version.
* 
**********************************************************************/
#ifndef _SPX_MODE_S_COVERAGE_MAP_H
#define _SPX_MODE_S_COVERAGE_MAP_H

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"

/* Forward declarations. */
struct SPxLatLong_tag;

class SPxModeSCoverageMap : public SPxObj
{
public:
    enum MapType
    {
        MAP_TYPE_SURVEILLANCE = 0,
        MAP_TYPE_DATALINK = 1,
        MAP_TYPE_LOCKOUT = 2
    };

    typedef SPxErrorCode (*DrawRectFn)(const SPxLatLong_tag *topLeft,
                                       const SPxLatLong_tag *bottomRight,
                                       UINT8 stateMask,
                                       void *userArg);

    SPxModeSCoverageMap(void);
    virtual ~SPxModeSCoverageMap(void);

    SPxErrorCode SetPath(const char *path);
    SPxErrorCode GetPath(char *buf, unsigned int bufLen) const;

    SPxErrorCode GetLocalNode(UINT8 *sac, UINT8 *sic) const;
    UINT8 GetDefaultClusterState(void) const;

    unsigned int GetNumMaps(void) const;
    SPxErrorCode GetMapReferences(UINT8 *buf, unsigned int bufLen) const;

    SPxErrorCode GetCoverage(UINT8 mapRef,
                             UINT8 sac, UINT8 sic,
                             double altitudeMetres,
                             const SPxLatLong_tag *topLeft,
                             const SPxLatLong_tag *bottomRight,
                             DrawRectFn drawRectFn,
                             void *userArg) const;

    int SetParameter(char *parameterName,
		     char *parameterValue);
    int GetParameter(char *parameterName,
                     char *valueBuf,
                     int bufLen);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    SPxErrorCode loadPath(const char *path);
    SPxErrorCode generateStrips(UINT8 mapRef,
                                UINT8 sac, UINT8 sic,
                                double altitudeMetres) const;

};

#endif /* _SPX_MODE_S_COVERAGE_MAP_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
