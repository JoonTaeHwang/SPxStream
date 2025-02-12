/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for track utility functions.
*
* Revision Control:
*   14/06/22 v1.1    REW	Initial Version (from SPxRadarTrack.cpp).
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_TRACK_UTILS_H
#define _SPX_TRACK_UTILS_H

/*
 * Other headers required.
 */


/*********************************************************************
*
*   Constants
*
**********************************************************************/


/*********************************************************************
*
*   Macros
*
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/* Utility functions. */
extern const char *SPxTrackGetStringForSensorType(UINT32 sensorType);
extern int SPxTrackIsSensorPrimary(UINT32 sensorType);
extern int SPxTrackIsSensorSecondary(UINT32 sensorType);
extern int SPxTrackIsSensorCombined(UINT32 sensorType);
extern int SPxTrackIsSensorGPSBased(UINT32 sensorType);
extern int SPxTrackIsSensorMultiple(UINT32 sensorType);

#endif /* SPX_TRACK_UTILS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
