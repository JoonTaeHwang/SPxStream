/*********************************************************************
*
* (c) Copyright 2007 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for the scan conversion destination base class.
*
* Revision Control:
*   20/03/19 v1.10   AGC	Support scan converters sharing destination.
*
* Previous Changes:
*   21/06/18 1.9    AGC	Add GetRequiredScaleFactor().
*   01/05/14 1.8    AGC	Remove historical unused static functions.
*   04/10/13 1.7    AGC	Simplify headers.
*   04/02/11 1.6    AGC	Add ImplementsSweepLine() function.
*   25/06/07 1.5    REW	Add Pause/Resume() support.
*   08/06/07 1.4    DGJ	Tweaked name capitalisation. Also class
*			is now derived from SPxSc base.
*   07/06/07 1.3    REW	Headers in subdirs.
*   04/06/07 1.2    DGJ	Moved class here from SourceLocal.h
*   24/05/07 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPxScDest_h
#define _SPxScDest_h

/*
 * Other headers required.
 */
/* For base class. */
#include "SPxSc.h"

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

/* Base class for all scan conversion destinations (network, windows). */
class SPxScDest :public SPxSc
{
public:
    SPxScDest();		// Abstract class, cannot instantiate.
    virtual ~SPxScDest();

    /* Control code for pausing/resuming updates (e.g. during calcs). */
    int m_paused;
    virtual void Pause(void)	{ m_paused++; }
    virtual void Resume(void)	{ m_paused--; }
    virtual int IsPaused(void)	{ return(m_paused > 0); }

    /* Does the dest display implement its own sweep line? */
    virtual int ImplementsSweepLine(void) { return FALSE; }

    /* Required scaling factor based on DPI. */
    virtual int GetRequiredScaleFactor(void) { return 1; }
};

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPxScDest_h */
/*********************************************************************
*
* End of file
*
**********************************************************************/
