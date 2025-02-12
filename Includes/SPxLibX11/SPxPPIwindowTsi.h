/*********************************************************************
*
* (c) Copyright 2010 - 2011, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxPPIwindowTsi.h,v $
* ID: $Id: SPxPPIwindowTsi.h,v 1.4 2013/10/04 15:31:08 rew Exp $
*
* Purpose:
*   Header file for SPxPPIwindowTsi class. 
*
*   Note that this class is derived from SPxPPIwindow and overrides 
*   several functions in the base class.
*
* Revision Control:
*   04/10/13 v1.4    AGC	Simplify headers.
*
* Previous Changes:
*   28/01/11 1.3    AGC	Rename file from SPxPPIwindowGLX.h to SPxPPIwindowTsi.h
*   15/09/10 1.2    REW	Make destructor virtual.
*   22/03/10 1.1    SP 	Initial version.
**********************************************************************/

#ifndef _SPX_PPI_WINDOW_TSI_H
#define	_SPX_PPI_WINDOW_TSI_H

/* Header files */
#include <X11/Xlib.h>
#include "SPxLibX11/SPxPPI.h"

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

/* Forward declarations. */
class SPxGlxMixer;
class SPxSc;
class SPxScDestBitmap;
class SPxScFollowX11;
class SPxRunProcess;
struct DirtyBox;

/*
 * SPxPPIwindowTsi class.
 */
class SPxPPIwindowTsi : public SPxPPIwindow
{
public:

    /* Public functions */
    SPxPPIwindowTsi(void);
    virtual ~SPxPPIwindowTsi(void);

    int SetGLXFadeType(unsigned int fadeType);
    unsigned int GetGLXFadeType(void);
    int SetGLXFadeRate(double fadePercent);
    double GetGLXFadeRate(void);

    /* Public override functions */
    int SetSPx(SPxSc *sc,
               SPxScDestBitmap *destBitmap,
               SPxRunProcess *scProcess=NULL,
               SPxScFollowX11 *followWin=NULL);

    int AddSPx(SPxSc *sc,
               SPxScDestBitmap *destBitmap,
               SPxRunProcess *scProcess=NULL);

    int SetDrawable(Drawable drawable);

    void SetRadarView(double cx, double cy, double w, double h);
    int SetRadarPosition(int index, double x, double y, UCHAR clear=0);

    void PixmapAChanged(void);
    void PixmapBChanged(void);

    UINT32 CompositeDisplay(void);

protected:

    /* Protected override functions */
    int CommonCreate(Display *display, int screen, 
                     int maxWidth, int maxHeight, 
                     int numRadarVideos);

private:

    /* Private functions */
    void updatePeriodEstimate(UINT16 firstAzimuth,
                             UINT16 lastAzimuth);

    static void bitmapUpdateHandler(SPxScDestBitmap *bitmap,
                                    UINT16 changes,
                                    UINT16 firstAzimuth,
                                    UINT16 lastAzimuth,
                                    DirtyBox box,
                                    void *userPtr);

    /* Used for GLX compositing */
    SPxGlxMixer *m_glxMixer;

    /* Used for scan period estimation */
    UINT32 m_lastUpdateTimeMs;
    UINT32 m_scanPeriodMs;

}; /* SPxPPIwindowTsi class */

#endif /* _SPX_PPI_WINDOW_TSI_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
