/*********************************************************************
*
* (c) Copyright 2010, 2013 - 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxGlxMixer.h,v $
* ID: $Id: SPxGlxMixer.h,v 1.5 2014/07/18 14:03:59 rew Exp $
*
* Purpose:
*   Header for SPx GLX Mixer.
*
* Revision Control:
*   18/07/14 v1.5    AGC	Remove using namespace Tsi.
*
* Previous Changes:
*   04/10/13 1.4    AGC	Simplify headers.
*   15/09/10 1.3    REW	Make destructor virtual.
*   23/04/10 1.2    SP 	Fix N/C #082. Remove dependency on TsiFade.h
*			from public headers.
*   22/03/10 1.1    SP 	Initial version.
**********************************************************************/

#ifndef _SPX_GLX_MIXER_H
#define	_SPX_GLX_MIXER_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xdamage.h>

#include "SPxLibUtils/SPxObj.h"

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
struct DirtyBox;
class SPxCriticalSection;

/*
 * The following is a forward declaration of the TsiFadeWindow
 * class contained in TsiFade.h. This file is only included in the
 * source code to avoid placing a dependency on the headers which
 * form part of the SPx library release. In this file we use 
 * 'unsigned int' in place of HFADELAYER for the same reason.
 */
namespace Tsi
{
    class TsiFadeWindow;
}


/*
 * GLX mixer class.
 */
class SPxGlxMixer : public SPxObj
{
public:
    /* 
     * Public functions.
     */
    SPxGlxMixer(UINT32 debugFlags=0);
    virtual ~SPxGlxMixer(void);

    SPxErrorCode Create(Display *display,
                        int screenNum,
                        int maxWidth,
                        int maxHeight,
                        int numRadarVideos,
                        Drawable drawable=0,
                        Pixmap ulayPixmap=0,
                        Pixmap olayPixmap=0);

    XVisualInfo *GetVisual(void);

    SPxErrorCode SetRadarImage(int index, XImage *xImage);

    SPxErrorCode SetDrawable(Drawable drawable);
    SPxErrorCode SetUnderlayPixmap(Pixmap ulayPixmap);
    SPxErrorCode SetOverlayPixmap(Pixmap olayPixmap);

    SPxErrorCode SetRadarPeriod(double periodSecs);
    SPxErrorCode SetView(double x, double y, double width, double height);
    SPxErrorCode SetRadarPosition(int index, double x, double y);
    SPxErrorCode SetFadeType(unsigned int fadeType);
    unsigned int GetFadeType(void);
    SPxErrorCode SetFadeRate(double fadePercent);
    double GetFadeRate(void);

    SPxErrorCode UpdateRadar(int index,
                             UINT16 firstAzimuth,
                             UINT16 lastAzimuth,
                             DirtyBox *box);
    SPxErrorCode UpdateUnderlay(void);
    SPxErrorCode UpdateOverlay(void);
    SPxErrorCode RedrawWindow(void);
    SPxErrorCode ReportXEvent(XEvent *event);

    /*
     * Public fields.
     */  

private:
    /* 
     * Types 
     */
    typedef struct
    {
        unsigned int tsiRadarLayer; /* Fade window radar layer (HFADELAYER) */
        XImage *xImage;             /* Radar XImage */
        int radarUpdated;           /* Update pending flag */
        double xPos;                /* Radar X position in metres */
        double yPos;                /* Radar Y position in metres */
        int updatesPerSweep;        /* Number of updates per sweep */
        int updateCount;            /* Use to count updates per sweep */
    
    } RadarInfo_t;

    /*
     * Private functions.
     */
    SPxErrorCode FlushRadarUpdates(RadarInfo_t *radarInfo);
    SPxErrorCode FlushGraphicsUpdates(void);
    void QueryDrawable(void);
    void UpdateFadeRegionRetireValue(void);
    void UpdateFadeRegionRetireCount(int index);

    /*
     * Private fields.
     */

    /* X11 fields */
    Display *m_display;                 /* X display */
    Drawable m_drawable;                /* PPI window or pixmap */
    int m_screenNum;                    /* Screen number */
    Pixmap m_ulayPixmap;                /* Optional underlay pixmap */
    Pixmap m_olayPixmap;                /* Optional overlay pixmap */
    XVisualInfo *m_tsiFadeWinVisual;    /* Fade window visual info */
    Damage m_ulayDamage;                /* Used to track ulay damage */
    Damage m_olayDamage;                /* Used to track olay damage */    

    /* TSI Fader fields */
    Tsi::TsiFadeWindow *m_tsiFadeWin;   /* TSI fade window (TsiFadeWindow) */
    unsigned int m_tsiUlayLayer;        /* TSI base layer (HFADELAYER) */
    unsigned int  m_tsiOlayLayer;       /* TSI top layer (HFADELAYER) */
    SPxCriticalSection *m_tsiFadeWinMutex; /* Fade window mutex */

    /* Misc fields */
    UINT32 m_debugFlags;                /* Debug flags */
    RadarInfo_t *m_radarInfoList;       /* Array of radar info */
    int m_maxWidth;                     /* Max window width */
    int m_maxHeight;                    /* Max window height */
    int m_numRadarVideos;               /* Max num radar videos */
    double m_radarPeriodSecs;           /* Rotation period in secs */
    UINT32 m_lastRedrawTime;            /* Last time window was redrawn */
    int m_olayUpdated;                  /* Overlay update flag */
    int m_ulayUpdated;                  /* Underlay updated flag */
    int m_drawableWidth;                /* Current drawable width */
    int m_drawableHeight;               /* Current drawable height */
    int m_purgeFadeRegions;             /* Purge fade regions if true */
    double m_fadePercent;               /* % to fade in one sweep */ 
    unsigned int m_fadeType;            /* Fade type */
    double m_fadeDelta;                 /* Amount to fade each update */

    /* Current view */
    double m_viewX;
    double m_viewY;
    double m_viewW;
    double m_viewH;

    /* Event base IDs for XServer extensions */
    int m_damageEventBase;              /* XDamage event base */
    int m_fixesEventBase;               /* XFixes event base */
    
}; /* SPxGlxMixer class */

#endif /* _SPX_GLX_MIXER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
