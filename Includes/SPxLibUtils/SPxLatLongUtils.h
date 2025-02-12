/*********************************************************************
*
* (c) Copyright 2012 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*   LatLong utility functions public header file.
*
* Revision Control:
*   16/03/21 v1.19   AJH	Add retrieval functions for conversion settings.
*
* Previous Changes:
*   30/10/20 1.18   AGC	Add precision option for lat/long formatting.
*   09/10/20 1.17   REW	Add SPxParseLatLong().
*   16/09/20 1.16   AGC	Add SPxDegreesToDMS().
*   13/01/17 1.15   REW	Add SPxLatLongFormatPosition().
*   23/09/16 1.14   REW	Add SPxLatLongFormatDegs().
*   26/11/15 1.13   AGC	Move SPX_LATLONG_MEAN_EARTH_RADIUS_METRES
*			from source to header.
*   09/11/15 1.12   SP 	Change normalise flag default to 
*			TRUE in SPxLatLongAddMetres() and 
*			SPxLatLongAddBearingDist().
*   03/11/15 1.11   REW	Implement real Haversine option, plus
*			allow choice of mode per function call.
*   03/11/15 1.10   REW	HAVERSINE corrected to RHUMBLINE.
*   25/06/15 1.9    AGC	Add SPxLatLongAddBearingDist().
*   12/12/13 1.8    REW	Add DMS conversions.
*   15/10/13 1.7    AGC	Include SPxTypes.h instead of SPxCommon.h.
*   25/01/13 1.6    AGC	Add findShorter option to some lat/long functions.
*   09/11/12 1.5    REW	Add optional normalise flag to AddMetres().
*   26/10/12 1.4    AGC	Add SPxLatLongBearingDist() function.
*   29/02/12 1.3    REW	Avoid compiler warning about trailing commas.
*   29/02/12 1.2    SP 	Add Vincenty conversion.
*   20/02/12 1.1    SP 	Initial version.
*
**********************************************************************/

#ifndef _SPX_LAT_LONG_UTILS_H
#define _SPX_LAT_LONG_UTILS_H

/* Make sure we have the common definitions and headers. */
#include "SPxLibUtils/SPxTypes.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxLatLongUTM.h" /* For SPX_REF_ELLIPSOID_WGS84 */


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Approximate mean radius of the earth in metres to use for spherical
 * calculations (as opposed to ellipsoidal calculations).
 *
 * This value comes from the International Union of Geodesy and
 * Geophysics (IUGG) and is considered the "best" value to minimise RMS
 * errors from the WGS-84 ellipsoid.
 */
#define	SPX_LATLONG_MEAN_EARTH_RADIUS_METRES	6371008.8

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Conversion mode to use. */
typedef enum
{
    /* "Default" means the mode set with SPxLatLongSetConvMode(). */
    SPX_LATLONG_CONV_MODE_DEFAULT = 0,

    /* UTM: Included for reference only. Generally not used
     * as errors towards edges of zones and when crossing
     * zones is significant.
     */
    SPX_LATLONG_CONV_MODE_UTM = 1,

    /* RHUMBLINE: Use functions to calculate the rhumb line
     * between lat/long positions. Simple and fast but assumes 
     * spherical earth (ellipsoid model is igniored). 
     */
    SPX_LATLONG_CONV_MODE_RHUMBLINE = 2,

    /* VINCENTY: Use Vincenty function to calculate the great circle
     * line between lat/long positions. Makes use of the selected
     * reference ellipsoid.
     */ 
    SPX_LATLONG_CONV_MODE_VINCENTY = 3,

    /* HAVERSINE: Use Haversine function to calculate the great circle
     * line between lat/long positions.  Faster than Vincenty, but assumes
     * spherical earth.
     */
    SPX_LATLONG_CONV_MODE_HAVERSINE = 4

} SPxLatLongConvMode_t;

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

/* Configure global conversion settings. */
SPxErrorCode SPxLatLongSetConvMode(SPxLatLongConvMode_t mode);
SPxLatLongConvMode_t SPxLatLongGetConvMode(void);
SPxErrorCode SPxLatLongSetRefEllipsoid(int ref);
int SPxLatLongGetRefEllipsoid(void);

/* Add metres to a lat/long and get the new lat/long back. */
SPxErrorCode SPxLatLongAddMetres(double lat, double lon, 
				    double xMetres, double yMetres,
				    double *latRtn, double *lonRtn,
				    int normalise=TRUE,
				    SPxLatLongConvMode_t
				      convMode=SPX_LATLONG_CONV_MODE_DEFAULT);

/* Calculate the difference in metres between two lat/long points, 
 * i.e. how many metres to add to the first point to get to the 
 * second point.
 */
SPxErrorCode SPxLatLongDiffMetres(double lat1, double lon1, 
				    double lat2, double lon2, 
				    double *xMetresRtn, double *yMetresRtn,
				    int findShorter=FALSE,
				    SPxLatLongConvMode_t
				      convMode=SPX_LATLONG_CONV_MODE_DEFAULT);

/* Add bearing and distance to a lat/long and get new lat/long back. */
SPxErrorCode SPxLatLongAddBearingDist(double lat, double lon,
				    double bearingRads, double distMetres,
				    double *latRtn, double *lonRtn,
				    int normalise=TRUE,
				    SPxLatLongConvMode_t
				      convMode=SPX_LATLONG_CONV_MODE_DEFAULT);

/* Calculate the bearing and distance between two lat/long points. */
SPxErrorCode SPxLatLongBearingDist(double lat1, double lon1,
                                    double lat2, double lon2,
                                    double *bearingRadsRtn,
				    double *distMetresRtn,
                                    int findShorter=FALSE,
				    SPxLatLongConvMode_t
				      convMode=SPX_LATLONG_CONV_MODE_DEFAULT);

/*
 * Conversion from decimal degrees to DMS components.
 */
extern unsigned int SPxDegreesToDMSDegs(double degrees);
extern unsigned int SPxDegreesToDMSMins(double degrees);
extern double SPxDegreesToDMSSecs(double degrees);
extern void SPxDegreesToDMS(double degrees,
			    unsigned int *degsRtn,
			    unsigned int *minsRtn,
			    double *secsRtn,
			    int secsNumDp = -1);

/* Conversion from decimal degrees (either lat OR long) to formatted string
 * in SPX_FORMAT_LL_DEGS, DEGS_MINS or DEGS_MINS_SECS format (but not UTM
 * or MGRS).
 */
extern SPxErrorCode SPxLatLongFormatDegs(char *buf, int bufLen,
					 SPxFormatLatLong_t fmt,
					 double latLongDegs,
					 int latNotLong,
					 int precision=-1);

/* Conversion from decimal degrees (lat AND long) to formatted string
 * in SPX_FORMAT_LL_DEGS, DEGS_MINS, DEGS_MINS_SECS, UTM or MGRS format.
 */
extern SPxErrorCode SPxLatLongFormatPosition(char *buf, int bufLen,
					     SPxFormatLatLong_t fmt,
					     double latDegs, double longDegs,
					     const char *separator,
					     int precision=-1);

/* Parse a string to extract lat/long from various different formats. */
extern SPxErrorCode SPxParseLatLong(const char *str,
				    double *latDegsPtr,
				    double *longDegsPtr);

#endif /* _SPX_LAT_LONG_UTILS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
