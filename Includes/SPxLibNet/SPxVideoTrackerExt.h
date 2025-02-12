/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxVideoTrackerExt class - base class
*	for external video trackers.
*
* Revision Control:
*   11/03/22 v1.2    AGC	Pass track altitude to SendTrack().
*
* Previous Changes:
*   04/03/22 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_VIDEO_TRACKER_EXT
#define _SPX_VIDEO_TRACKER_EXT

/* For base classes. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibNet/SPxNetAddr.h"

/* Forward declarations. */

class SPxVideoTrackerExt : public SPxObj, public SPxNetAddr
{
public:
    /* Construction and destruction. */
    SPxVideoTrackerExt(void);
    virtual ~SPxVideoTrackerExt(void);

    virtual int AlwaysSendSelectedTrack(void) const { return FALSE; }

    /* Video tracking control. */
    virtual int IsTrackingEnabled(UINT8 subSystemIndex) const = 0;
    virtual int IsTracking(UINT8 subSystemIndex) const = 0;
    virtual SPxErrorCode SendTrack(UINT32 id,
				   double rangeMetres,
				   double bearingDegs,
				   double elevationDegs,
				   double altitudeMetres,
				   const char *name,
				   int isRadarTrackingEnabled,
				   int isVideoTrackingEnabled) = 0;

private:

}; /* SPxVideoTrackerExt */

#endif /* _SPX_VIDEO_TRACKER_EXT */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
