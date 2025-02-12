/*********************************************************************
*
* (c) Copyright 2008 - 2013, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxScreen.h,v $
* ID: $Id: SPxScreen.h,v 1.7 2013/10/04 15:31:08 rew Exp $
*
* Purpose:
*	SPx Screen Record/Replay common header file.
*
*
* Revision Control:
*   04/10/13 v1.7    AGC	Simplify headers.
*
* Previous Changes:
*   13/02/13 1.6    SP 	Move generic SR stuff to SPxPackets.h.
*   21/12/12 1.5    SP 	Add SPX_SR_SCREEN2 and SPX_MAX_NUM_SCREENS.
*                       Add screen 2 fields to SPxSRfileMasterBlock.
*   21/07/09 1.4    REW	Add x/y to SPxSRframeHeader for partial grabs.
*   08/07/09 1.3    SP 	New members required for XDamage support.
*   27/11/08 1.2    REW	Use new file format with packet headers.
*   27/11/08 1.1    DGJ	Initial checked-in version.
**********************************************************************/

#ifndef _SPX_SCREEN_H
#define	_SPX_SCREEN_H

/*********************************************************************
*
*   Constants
*
**********************************************************************/


/*********************************************************************
*
*   Headers
*
**********************************************************************/

/* Dependencies in SPxScreenRecordScreen */
#include <X11/Xlib.h>
#include <X11/extensions/XShm.h>
#include <X11/extensions/Xdamage.h>
#include <X11/extensions/Xfixes.h>

#include "SPxLibUtils/SPxTime.h"


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

/*
 * Type used within record/replay classes for descriptions of X screens.
 */
class SPxScreenRecordScreen
{
public:	
    int m_xScreenNumber;	/* X Windows screen number */
    int m_width;		/* Width of screen in pixels */
    int m_height;		/* Height of screen in lines */
    int m_depth;		/* Bits per pixel */
    int m_scale;		/* Scale factor to record at */
    int m_available;		/* Is screen available */
    XImage *m_image;		/* Image for this screen */
    Visual *m_visual;		/* The display visual for this screen */
    Pixmap m_pixmap;            /* Shared memory pixmap */
    GC m_pixmapGC;              /* Graphics context for pixmap */
    XShmSegmentInfo m_shminfo;	/* Shared memory info for this screen */
    Damage m_damage;            /* Handle for XDamage object */
    XserverRegion m_damagedRegion; /* Used to store damaged areas */
    SPxTime_t lastRefFrameTime;	/* Time of last reference frame */
};


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_SCREEN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
