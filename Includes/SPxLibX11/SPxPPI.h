/*********************************************************************
*
* (c) Copyright 2008 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header file for SPxPPIwindow class.
*
*
* Revision Control:
*   25/03/22 v1.37   AGC	Add UseShm option.
*
* Previous Changes:
*   15/06/21 1.36   AGC	Actually remove GetRadarColour().
*   15/06/21 1.35   BTB	Remove GetRadarColour().
*   15/06/21 1.34   BTB	Add GetRadarColour().
*   02/02/18 1.33   AGC	Fix N/C #525 - use view control to set/get view.
*   26/06/16 1.32   AJH	Add GetWinWidth/Height().
*   24/09/14 1.31   JP	Add GetViewControl(), GetNumRadarVideos(), GetSPxSCs(),
*			GetSPxRunProcesses(), GetSPxScDestBitmaps() and
*			GetFollowWin().
*   04/10/13 1.30   AGC	Simplify headers.
*   12/02/13 1.29   SP 	Add per-channel SetRadarBrightness().
*   26/11/12 1.28   REW	Add DeparentWindow().
*   21/06/12 1.27   SP 	Support external SPxViewControl.
*			Add PanView().
*   21/10/11 1.26   SP 	Add functions to set/get radar position.
*   07/10/11 1.25   AGC	Add support for sub-areas.
*   27/09/11 1.24   AGC	Add 32-bit blend mode option.
*   26/03/11 1.23   SP 	Add SetTopmostPixmap().
*   25/02/11 1.22   REW	Add GetNextIdxToUse() and GetNumVideos().
*   11/02/11 1.21   AGC	Add PixmapADrawing() and PixmapBDrawing().
*			Fix history back to v1.17.
*   04/02/11 1.20   AGC	Move accelarion mode enum here.
*			Make GetImage() safe.
*			Make GetAccelerated() function public.
*			Remove need for storing sweep line settings.
*			Add GetDestWinDepth() function.
*   28/01/11 1.19   AGC	Make some functions virtual, others protected
*			for SPxPPIwindowGlx compatibility.
*			Add SetAccelerated() function for SPxPPIwindowGlx.
*			Add Sweep Line set/get functions.
*			Make GetFadeType()/GetFadeTime() functions safe.
*   26/11/10 1.18   AGC	Add SetAvoidRender(), SetDepth(), GetDrawable().
*   17/09/10 1.17   SP 	Add UpdateRadarView().
*			Fix #090 - window compositing on top-level window.
*   15/09/10 1.16   REW	Make destructor virtual.
*   12/08/10 1.15   REW	Add GetIdxForBitmap() support.
*   11/08/10 1.14   REW	Add RemoveSPx() support.
*   27/05/10 1.13   SP 	Add GetParameter() & SetParameter().
*   22/03/10 1.12   SP 	Enable class to be sub-classed.
*   18/12/09 1.11   DGJ	Support pixmaps with window.
*			Rename pictures to A/B.
*   16/12/09 1.10   DGJ	Support GetRadarObject() function.
*   29/09/09 1.9    REW	Rely on FadeBitmap() to simplify timing.
*   29/09/09 1.8    REW	Add deletionHandler().
*   24/05/09 1.7    DGJ	Add ClearImage() and ClearAllImages().
*   21/04/09 1.6    DGJ	Support fading per channel
*   31/03/09 1.5    REW	Fix bug #027, real-time fade of second radars.
*   13/10/08 1.4    DGJ	New compositing method using redirections.
*			Change SetSPx() function (no view).
*   14/09/08 1.3    DGJ	Implement incremental compositing
*			updates based on changed areas.
*   03/03/08 1.2    DGJ	CompositeDisplay now returns an activityCode
*   12/02/08 1.1    DGJ	Initial Version.
**********************************************************************/


/* SPxPPIwindow Class */

#ifndef PPI_H_
#define PPI_H_

/* Standard headers. */
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/XShm.h>

/* For base class. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibSc/SPxSc.h"

/* Activity codes/masks that the CompositeDisplay returns. */
#define SPX_COMPOSITE_VIDEOS 		(0xf)
#define SPX_COMPOSITE_VIDEOS_VISIBLE 	(0xf0)
#define SPX_COMPOSITE_DID_FADE 		(1 << 8)
#define SPX_COMPOSITE_DID_UNDERLAY_COPY (1 << 9)
#define SPX_COMPOSITE_DID_RADAR_COPY 	(1 << 10)
#define SPX_COMPOSITE_DID_OVERLAY_COPY 	(1 << 11)
#define SPX_COMPOSITE_DID_OUTPUT_COPY 	(1 << 12)

/* Forward declarations. */
class SPxRunProcess;
class SPxViewControl;
class SPxScDestBitmap;
class SPxScFollowX11;
class SPxSc;

/* Acceleration modes. */
enum SPxPPIwindowAccel_t
{
    SPX_PPI_WIN_ACCEL_OFF   = 0,    /* Never use accelerated fading. */
    SPX_PPI_WIN_ACCEL_AUTO  = 1,    /* Use accelerated fading if the hardware supports
				     * it, otherwise use non-accelerated fading
				     * with no warning or error. */
    SPX_PPI_WIN_ACCEL_GLX    = 2    /* Use GLX accelerated fading if the hardware supports
				     * it, otherwise throw an error. */
};

class SPxPPIwindow :public SPxObj
{
    SPxRunProcess **m_scProcess;	/* List of scan conv processes */
    int m_width;			/* Display width */
    int m_height;			/* Display height */
    int m_lastWidth;			/* Last used display width */
    int m_lastHeight;			/* Last used display height */
    int m_maxWidth;			/* Maximum width */
    int m_maxHeight;			/* Maximum height */
    XShmSegmentInfo *m_radarImageSHMinfo;  /* List of shared-mem segments */
    Display *m_display;			/* X Display */
    int m_screen;			/* X Screen number */
    Picture *m_radarPicture;		/* List of radar pictures */
    Pixmap *m_radarPixmap;		/* List of radar pixmaps */
    Picture m_combinePicture;		/* Intermediate composition Pic */
    Picture m_pictureA;			/* Graphics overlay Picture */
    Picture m_pictureB;			/* Graphics underlay/overlay Picture */
    Pixmap m_combinePixmap;		/* Intermediate pixmap */
    Pixmap m_topmostPixmap;             /* An additional topmost pixmap */
    Picture m_topmostPicture;           /* Mapped to topmost pixmap */
    GC m_gc;				/* X GC for pixmap manipulation */
    GC m_gcScreen;			/* X GC for copies to screen */
    GC m_gcCombine;
    Visual *m_argbVisual;		/* 32-bit ARGB Display visual */
    Visual *m_rgbVisual;		/* 24-bit RGB Display visual */
    XImage *CreateImage(int);	
    double m_viewX, m_viewY, m_viewW, m_viewH; /* Current view */
    int m_subAreaX, m_subAreaY, m_subAreaW, m_subAreaH; /* Current sub-area. */
    int m_subAreaEnabled;		/* Is sub-area display enabled? */
    int m_subAreaChanged;		/* Has the sub-area changed. */
    int m_useShm;			/* Use X11 SHM if available? */
    int *m_fadeType;			/* Fade type */
    double *m_fadeTime;			/* Seconds if real-time, or numSweeps */
    SPxScFollowX11 *m_followWin;
    void (*m_olayRedrawFn)(Pixmap, int, int, double, double, double, double);
    void (*m_ulayRedrawFn)(Pixmap, int, int, double, double, double, double);
    int m_enablePixmapBDrawing;
    int m_enablePixmapADrawing;	
    int *m_radarVisibleState;		/* Is radar visible for each channel? */
    int m_visibleRadarCount;		/* Number of visible radars */
    double *m_radarXMetres;             /* List of radar X positions */
    double *m_radarYMetres;             /* List of radar Y positions */

    int m_accelerated;			/* Accelerated fading using derived class? */

    /* Flags to indicate if pixmaps have changed for compositing */
    UCHAR m_pixmapBchanged;
    UCHAR m_pixmapAchanged;
    int GetDepthOfDrawable(Display *display, Drawable drawable);
    int m_depthOfUnderlay;		/* The depth of the underlay pixmap */	
	
    /* Member variables for redirected window (Composite From Window) */
    Window m_destInWin, m_destOutWin, m_destParentWin;
    int m_destWinWidth, m_destWinHeight, m_destWinX, m_destWinY;
    int m_destWinInputDepth;
    int m_destWinForceSizeAlignment;
    GC m_destGC;
    Picture m_destInPicture, m_destOutPicture;
    int m_destWinOrigX, m_destWinOrigY;

    int m_avoidRender;
    int m_depth;
    int m_32BitBlendMode;
    int m_deparented;			/* Has window been Deparented? */

    int m_pixmapCompositing;	    /* Is pixmap compositing being used? */

    SPxViewControl *m_viewCtrl;     /* Installed view control object. */
    
    /* The composition functions for Redirected-Window and Pixmap
     * application compositing.
     */
    UINT32 CompositeDisplayWindow(void);
    UINT32 CompositeDisplayPixmaps(void);
    int PushRadarsToServer(int *updateX, int *updateY,
				int *updateW, int *updateH);
    int FadeRadars(void);

    static int m_defaultFadeType;
    static double m_defaultFadeTime;
    static int m_defaultFadingIntervalMS;

    static UINT32 m_defaultSweepLineColour;
    static SPxScSweepLineType_t m_defaultSweepLineType;
    static SPxScSweepLineColMode_t m_defaultSweepLineColMode;
    
    /* Image clearing. */
    void ClearImage(int videoIndex);
    void ClearAllImages(void);

    /* Picture creation. */
    int CreatePictureAfromPixmapA(void);
    int CreatePictureBfromPixmapB(void);

    /* Create a window for rendering radar. */
    virtual Drawable CreateRadarWindow(void);


    /* View control */
    void UpdateRadarView(void);
    
    /* Function to install as a handler for when some of the objects
     * we've been given to use (bitmaps etc.) are deleted behind our backs.
     */
    static int deletionHandler(void *invokingObject, 
                               void *userPtr, void *arg);

protected:

    /* Protected functions */
    virtual int CommonCreate(Display *display, int screen, 
			     int maxWidth, int maxHeight, 
			     int numRadarVideos);
    void SetAccelerated(int accelerated) { m_accelerated = accelerated; }
    Window GetAppGraphics(void) { return (m_createdOK) ? m_destInWin : 0; }
    Window GetParentWin(void)	{ return m_destParentWin; }
    int GetDestWinX(void)	{ return m_destWinX; }
    int GetDestWinY(void)	{ return m_destWinY; }
    int GetDestWinW(void)	{ return m_destWinWidth; }
    int GetDestWinH(void)	{ return m_destWinHeight; }
    int GetDestWinDepth(void)	{ return m_destWinInputDepth; }
    int RedirectWindow(Window window);
    int UsingPixmapComposite(void) { return m_pixmapCompositing; }

    /* Test if a drawable is valid */
    int IsDrawableValid(Drawable xid);

    /* Protected variables */
    SPxSc **m_sc;			/* List of scan converters */
    int m_createdOK;			/* Was the object created ok? */
    int m_numRadarVideos;		/* Max number of radar videos */
    int m_numVideoDefined;		/* The number currently defined. */
    SPxScDestBitmap **m_destBitmap;	/* List of destination for SC */
    Drawable m_displayDrawable;		/* Destination (Window/Pixmap) */
    Pixmap m_pixmapA;			/* Pixmap A (underlay) */
    Pixmap m_pixmapB;			/* Pixmap B (underlay/overlay) */
    XImage **m_radarImage;		/* List of radar Images */

public:
    void SetAvoidRender(int avoid){ m_avoidRender = avoid; }
    void Set32BitBlendMode(int op) { m_32BitBlendMode = op; }

    /* Get whether accelerated fading and compositing is in use. */
    int GetAccelerated(void)	{ return m_accelerated; }

    /* Functions for the client to use to report that it is about
     * to start drawing to the pixmaps.
     */
    virtual void PixmapADrawing(void) { return; }
    virtual void PixmapBDrawing(void) { return; }
    /* Functions for the client to use to report that it has changed
     * the pixmaps.
     */
    virtual void PixmapAChanged(void);
    virtual void PixmapBChanged(void);

    /* Set an output drawable for Pixmap compositing. */
    virtual int SetDrawable(Drawable drawable);
    Drawable GetDrawable(void){ return m_displayDrawable; }

    void SetDepth(int depth){ m_depth = depth; }

    /* Is radar video enabled for a channel index ? */
    int GetEnableRadarVideo(int channel);
    
    /* Get the maximum width/height of the window */
    int GetMaxWidth(void) {return m_maxWidth;}
    int GetMaxHeight(void) {return m_maxHeight;}

    /* Get the width/height of the window */
    int GetWinWidth(void) {return m_width;}
    int GetWinHeight(void) {return m_height;}

    /* Get the display */
    Display *GetDisplay(void){ return m_display; }
    
    /* Destructor */
    virtual ~SPxPPIwindow(void);
    
    /* Add/remove SPx Objects */
    virtual int AddSPx(SPxSc *sc, SPxScDestBitmap *destBitmap, SPxRunProcess * = 0);
    virtual int RemoveSPx(SPxScDestBitmap *destBitmap);

    /* See how many videos are currently in the window. */
    virtual int GetNumVideos(void) { return(m_numVideoDefined); }

    /* Get the Bitmap for the specified channel index and vice versa. */
    SPxScDestBitmap *GetBitmap(int index);
    virtual int GetIdxForBitmap(SPxScDestBitmap *destBitmap);	/* -1 => err */
    
    /* Get the index that will be used for the next channel added. */
    virtual int GetNextIdxToUse(void);		/* -1 => error */

    /* Set the fading interval in ms */
    void SetFadingIntervalMS(int channel, int fadeInterval);
    void SetFadingIntervalMS(int fadeInterval);
   
    /* Set the radar colour for all channels */
    void SetRadarColour(UINT32 colour);
    
    /* Set the radar colour for a specific channel */
    void SetRadarColour(int channelIndex, UINT32 colour);

    /* Get the radar object (eg SPxSourceLocal) */
    SPxSc *GetRadarObject(int index);

    void EnableRadarVideo(int state);
    void EnableRadarVideo(int channel, int state);
    void EnablePixmapADrawing(int state) {m_enablePixmapADrawing = state;}
    void EnablePixmapBDrawing(int state) {m_enablePixmapBDrawing = state;}
    int GetPixmapBEnable(void) {return m_enablePixmapBDrawing;}
    int GetPixmapAEnable(void) {return m_enablePixmapADrawing;}
    void RedrawPixmapA(void);
    void RedrawPixmapB(void);
    void ConvertWindowToWorld(int x, int y, double *wx, double *wy);
    void RegisterPixmapARedraw(
		void (*)(Pixmap, int, int, double, double, double, double));
    void RegisterPixmapBRedraw(
		void (*)(Pixmap, int, int, double, double, double, double));
    virtual void SetView(double cx, double cy, double w, double h);
    virtual void PanView(double cx, double cy, int clearTrails);
    int CheckWindow(void);
    
    /* Support GetFadeTime with no channel (assume 0) - legacy support only */
    double GetFadeTime(void);
    double GetFadeTime(int channel);

    /* Support GetFadeType with no channel (assume 0) - legacy support only */
    int GetFadeType(void);
    int GetFadeType(int channel);

    UINT32 GetSweepLineColour(int channel);
    SPxScSweepLineType_t GetSweepLineType(int channel);
    SPxScSweepLineColMode_t GetSweepLineColMode(int channel);

    double GetViewX(void);
    double GetViewY(void);
    double GetViewW(void);
    double GetViewH(void);
    SPxErrorCode GetView(double *cxRtn, double *cyRtn,
			 double *wRtn, double *hRtn);
    void SetFadeTime(double fadeRate);
    void SetFadeTime(int channel, double fadeRate);
    void SetFade(int channel, UCHAR fadeType);
    void SetFade(UCHAR fadeType);
    void SetSweepLineColour(UINT32 colour);
    void SetSweepLineColour(int channel, UINT32 colour);
    void SetSweepLineType(SPxScSweepLineType_t type);
    void SetSweepLineType(int channel, SPxScSweepLineType_t type);
    void SetSweepLineColMode(SPxScSweepLineColMode_t mode);
    void SetSweepLineColMode(int channel, SPxScSweepLineColMode_t mode);
    SPxPPIwindow(void);

    /* Radar position */
    void SetRadarPosition(double xMetres, 
			  double yMetres, 
			  int clearTrails=0);
    void SetRadarPosition(int channel, 
			  double xMetres, 
			  double yMetres, 
			  int clearTrails=0);
    void GetRadarPosition(int channel, 
			  double *xMetresRtn, 
			  double *yMetresRtn);
    
    /* Creation methods */
    virtual int Create(Display *display, int screen, Drawable displayWindow,
		       int maxWidth, int maxHeight, Pixmap underlayPixmap,
		       Pixmap overlayPixmap, int numRadarVideos, int createDrawable=FALSE);

    virtual int Create(Display *display, int screen, 
		       Window appGraphics, 
		       int maxWidth, int maxHeight,
		       int numRadarVideos, 
		       Pixmap pixmapA = 0, Pixmap pixmapB = 0,
		       int createDrawable=FALSE);

    /* Set the input window to be used for Window Compositing. This can
     * only be used if the Window was not initially set in the Create
     * call - it can't be changed.
     */
    virtual int SetWindow(Window appGraphics);

    /* Remove the input window and all links to it, putting it back to
     * its original parent etc.  This should only be used shortly before
     * destroying the object, although it is not normally necessary - it will
     * be called internally by the destructor anyway.
     */
    int DeparentWindow(void);

    virtual int SetSPx(SPxSc *sc, SPxScDestBitmap *destBitmap,
		SPxRunProcess *scProcess = NULL,
		SPxScFollowX11 *followWin = NULL);	
	
    unsigned char * GetRadarImageMemory(int index);
    int GetRadarImageStride(int index);
    Pixmap GetCombinePixmap(void) {return m_combinePixmap;}
    int GetWidth(void) 		{return m_width;}
    int GetHeight(void) 	{return m_height;}
    XImage* GetImage(int index);
    virtual UINT32 CompositeDisplay(void);
    virtual void SetRadarView(double cx, double cy, double w, double h);
    virtual void PanRadarView(double cx, double cy, int clearTrails);
    int SetRadarSubArea(int x, int y, int w, int h);
    int GetRadarSubArea(int *x, int *y, int *w, int *h);
    void EnableSubArea(int enable);
    int GetSubAreaEnable(void) { return m_subAreaEnabled; }
    void SetUseShm(int useShm) { m_useShm = useShm; }
    void SetRadarBrightness(int brightness);
    void SetRadarBrightness(int index, int brightness);
    SPxErrorCode SetTopmostPixmap(Pixmap pixmap);
    SPxErrorCode SetViewControl(SPxViewControl *obj);
    SPxViewControl *GetViewControl(void) {return m_viewCtrl;}
    int GetNumRadarVideos(void) {return m_numRadarVideos;}
    SPxSc** GetSPxSCs(void) {return m_sc;}
    SPxRunProcess** GetSPxRunProcesses(void) {return m_scProcess;}
    SPxScDestBitmap** GetSPxScDestBitmaps(void) {return m_destBitmap;}
    SPxScFollowX11* GetFollowWin(void) {return m_followWin;}
    
	/* Parameter assignment. */
    int SetParameter(char *parameterName, char *parameterValue);
    int GetParameter(char *parameterName, char *valueBuf, int bufLen);
};

#endif /* PPI_H_*/

/*********************************************************************
*
*	End of file
*
**********************************************************************/
