/*********************************************************************
*
* (c) Copyright 2009 - 2015, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxLatLongUTM.h,v $
* ID: $Id: SPxLatLongUTM.h,v 1.8 2015/11/09 12:10:12 rew Exp $
*
* Purpose:
*   LatLong <-> UTM conversion public header file.
*
* Revision Control:
*   09/11/15 v1.8    SP 	Add optional normalise flag to 
*                               SPxLatLongAddUTM() and 
*                               SPxLatLongAddBearingDistUTM().
*
* Previous Changes:
*   25/06/15 1.7    SP	Add SPxLatLongAddBearingDistUTM().
*   15/10/13 1.6    AGC	Include SPxTypes.h instead of SPxCommon.h.
*   26/10/12 1.5    AGC	Add SPxLatLongBearingDistUTM().
*   10/03/10 1.4    REW	Add SPxLatLongDiffUTM().
*   04/03/10 1.3    REW	Add SPxLatLongAddUTM().
*   02/10/09 1.2    REW	Don't need Constants.h any more.
*   02/10/09 1.1    SP 	Initial version
**********************************************************************/

#ifndef _SPX_LAT_LONG_UTM_H
#define _SPX_LAT_LONG_UTM_H

/* Make sure we have the common definitions and headers. */
#include "SPxLibUtils/SPxTypes.h"
#include "SPxLibUtils/SPxError.h"

/* Local headers */
#include "SPxLibUtils/SPxLatLongUTMConversion.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Reference ellipsoid to use for WGS-84 */
#define SPX_REF_ELLIPSOID_WGS84 23

/* Bytes required to store UTM zone code and null terminator */
#define SPX_UTM_ZONE_BYTES      4

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/*********************************************************************
*
*   Macros
*
**********************************************************************/

/*********************************************************************
*
*   Variable definitions
*
**********************************************************************/

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/* Convert lat/long to UTM */
SPxErrorCode SPxLatLongToUTM(int refEllipsoid, 
                             const double lat, 
                             const double lon, 
                             double *utmNorthingRtn, 
                             double *utmEastingRtn,
                             char *utmZoneRtn);

/* Convert UTM to lat/long */
SPxErrorCode SPxUTMToLatLong(int refEllipsoid, 
                             double utmNorthing, 
                             double utmEasting,
                             const char *utmZone,
                             const double *latRtn, 
                             const double *lonRtn);

/* Add a UTM offset to a lat/long and get the new lat/long back. */
SPxErrorCode SPxLatLongAddUTM(int refEllipsoid,
                             const double lat, 
                             const double lon, 
                             double utmNorthing, 
                             double utmEasting,
                             double *latRtn, 
                             double *lonRtn,                             
                             int normalise=TRUE);

/* Calculate the difference in UTM between two lat/long points, i.e. how
 * many metres to add to the first point to get to the second point.
 */
SPxErrorCode SPxLatLongDiffUTM(int refEllipsoid,
                             const double firstLat, 
                             const double firstLon, 
                             const double secondLat, 
                             const double secondLon, 
                             double *xMetresRtn, 
                             double *yMetresRtn);

SPxErrorCode SPxLatLongAddBearingDistUTM(int refEllipsoid,
					 double lat,
					 double lon,
					 double bearingRads,
					 double distMetres,
					 double *latRtn,
					 double *lonRtn,
                                         int normalise=TRUE);

SPxErrorCode SPxLatLongBearingDistUTM(int refEllipsoid,
				      double firstLat,
				      double firstLon,
				      double secondLat,
				      double secondLon,
				      double *bearingRadsRtn,
				      double *distMetresRtn);

#endif /* _SPX_LAT_LONG_UTM_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
