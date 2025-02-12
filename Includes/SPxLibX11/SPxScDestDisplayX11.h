/*********************************************************************
*
* (c) Copyright 2007 - 2016, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxScDestDisplayX11.h,v $
* ID: $Id: SPxScDestDisplayX11.h,v 1.33 2016/01/07 14:19:54 rew Exp $
*
* Purpose:
*   Header for class to render scan converted data on X11.
*
* Revision Control:
*   07/01/16 v1.33   AGC	Make Set/GetUseIntermediateBitmap() virtual.
*
* Previous Changes:
*   03/11/15 1.32   AGC	PatchSet() patch data now const.
*   02/04/14 1.31   AGC	Remove unused radar colour variables.
*   04/10/13 1.30   AGC	Simplify headers.
*   22/11/12 1.29   REW	Add PanView().
*   26/11/10 1.28   AGC	Add SPxFindPseudoColorVisual().
*			Add optional checkVisualFormat argument to
*			SPxFindRgbVisual().
*   20/09/10 1.27   REW	Use m_bitmapAccess to protect fades.
*   17/09/10 1.26   REW	Add m_fadeLevelsRemainder.
*   15/09/10 1.25   REW	Make destructor virtual.
*   06/03/09 1.24   REW	Add m_numFadesSinceLastPatch counter.
*   19/02/09 1.23   REW	Support fading at intermediate 8-bit level.
*   13/10/08 1.22   DGJ	Add SPxFindRgbVisual().
*   05/12/07 1.21   DGJ	Add UpdateFullWindow() and ThreadFunc().
*   22/11/07 1.20   REW	Use time functions from SPxLibUtils.
*   19/11/07 1.19   DGJ	Do stacking order check outside of main update
*   29/10/07 1.18   DGJ	Call atexit only once. On delete set deleted
*			objects to 0. Separate Update and UpdateObject
*   25/10/07 1.17   REW	Change SetSource() to use UINT32 args.
*   11/10/07 1.16   DGJ	Support multiple radar windows sharing an overlay
*   25/09/07 1.15   REW	Add m_doFadeWhenPatchListCompletes.
*   06/09/07 1.14   DGJ	Implement SetSource to clear bitmaps
*   29/08/07 1.13   DGJ	Added SPxNoWindowDecorate()
*   13/07/07 1.12   DGJ	Separate screen updates from fades
*   03/07/07 1.11   DGJ	Add SPxFindArgbVisual().
*   29/06/07 1.10   DGJ	Remove more unused/unnecessary code.
*   27/06/07 1.9    DGJ	Remove unused/unnecessary code.
*   21/06/07 1.8    DGJ	Moved common code to DestDisplay.  Changed
*			update strategy and renamed Fade() to Update().
*   18/06/07 1.7    REW	Removed radarSource from Create() function.
*   15/06/07 1.6    DGJ	Add m_restackAtom and more layering funcs.
*   13/06/07 1.5    DGJ	Many changes, including the base class type.
*   11/06/07 1.4    REW	SPxLibSC changed to SPxLibSc.
*   08/06/07 1.3    DGJ	Tweaked SPxSc name capitalisation etc.
*   07/06/07 1.2    REW	Tweak include names.
*   04/06/07 1.1    DGJ	Initial Version
**********************************************************************/

#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibSc/SPxScDest.h"
#include "SPxLibSc/SPxScSource.h"
#include "SPxLibSc/SPxScDestDisplay.h"

#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/XShm.h>
#include "stdio.h"

// Colour modes for a window.
#define SPX_RADAR_COLOUR_MODE_TRUE 100

// Threshold level for the density of the dirty box before the display is updated.
#define SPX_DIRTY_BOX_UPDATE_THRESHOLD 30

// The maximum number of radar windows
#define SPX_MAX_NUM_RADAR_WINDOWS 4

Visual *SPxFindArgbVisual(Display *, int);
Visual *SPxFindRgbVisual(Display *dpy, int scr, int checkVisualFormat=TRUE);
Visual *SPxFindPseudoColorVisual(Display *dpy, int scr);

SPxErrorCode SPxNoWindowDecorate(Display *display, Window window);

/* Forward declarations. */
class SPxThread;

// Define the class for X11 display of radar video
//
// For each of the functions provided by the parent class SPxSCDest (the scan-conversion
//  destination), this class provides the X11-specific implementation of the display functions
//
class SPxScDestDisplayX11 :public SPxScDestDisplay
{
    static int m_scr;			// X Screen, before Window is created.
    static Display *m_dpy;		// X Display
    static Visual *m_visual;		// X Visual.
    static int m_numWindows;		// Total number of scan-conversion windows open.
    static int m_winDepth;		// The number of bits per pixel (eg 32).
    static int m_priorityCount;         // Priority allocated to next window.
    static SPxScDestDisplayX11* m_firstObject;
					// The head of a linked list of instances.
    static SPxScDestDisplayX11* m_lastObject;
					// The tail of a linked list of instances.
    static int m_firstTime;             // First time through allow initialisation
    UINT32 m_timeLastUpdate;
    UINT32 m_timeLastFade;
    double m_fadeLevelsRemainder;
    UINT32 m_timeLastStackingCheck;
    Atom m_restackAtom;
    int m_beingDeleted;                 // Flag to say that object is being deleted.
    XImage *m_image;			// The client-side image structure
    Window m_win;			// The Window
    Pixmap m_pixmap;			// The server-side pixmap.
    GC m_gc;				// Graphics context
    int m_screenWin;			// Screen for a specific window.
    XShmSegmentInfo m_shmInfo;		// Shared-memory info.
    Colormap m_colourmap;
    Window m_olay, m_ulay;		// Underlay and Overlay X Windows.
    Window m_olayRoot, m_ulayRoot;	// The parents of m_ulay and o_lay that are children of RootWIndow
    int m_canUseWindow;			// Normal operation has m_canUseWindow=1, but 
					//  if 0 it measn the window is not used - just the Pixmap.
    int m_canUsePixmap;			// As above, but relates to use of pixmap.
    int m_patchesBeingUpdated;  // Set to 1 PatchSet is received, and cleared when "update now" patch
                                // is received.
    int m_doUpdateWhenPatchListCompletes;
                                // Set to 1 to request an update when the "update now" patch is received.
    int m_doFadeWhenPatchListCompletes;
                                // Set to 1 to request a fade when the "update now" patch is received.
    unsigned int m_numFadesSinceLastPatch;	/* Counter */
    DirtyBox m_dirtyBox;		// The dirty box that contains the patches for update.
    SPxScDestDisplayX11 *m_next;	// Next one in the linked list.
    DirtyBox UpdateDirtyBox(int,int,int,int);	
    void InitialiseDirtyBox(int x, int y, int w, int h);	
    Window FindRootWindow(Window win);
    void UpdateWindow(DirtyBox *box);	
    void DrawRandomBoxes(void);	
    void ClearImage(void);
    int m_constructOK;			       // Set to 1 after successful Construct();
    int CheckAndChangeGeom(UINT16 screenX, UINT16 screenY, UINT16 screenW, UINT16 screenH);
    void StackAboveUlay(void);
    void StackBelowOlay(void);
    SPxScDestDisplayX11* TestObjectAbove(void);
    SPxScDestDisplayX11* TestObjectBelow(void);
    int m_priority;                     /* Used when we have multiple radar windows sharing an overlay
                                        The priority allows the radar windows to be stacked. A larger 
                                        number puts the window on top. */

    /* Intermediate bitmap, optionally for doing 8-bit based fading. */
    UINT8 *m_intermediateBitmap;
    int m_useIntermediateBitmap;
    SPxCriticalSection m_bitmapAccess;

public:
    void *ThreadFunc(SPxThread*);
    void UpdateObject(void);
    void GenerateTestPattern(int, int);
    SPxScDestDisplayX11(void);          // Constructor;
    int Create(UCHAR screenID,	
		UINT16 maxScreenW, UINT16 maxScreenH,	
		UINT32 ulayWindow, UINT32 olayWindow,
		UINT32 flags = 0);	
    virtual ~SPxScDestDisplayX11(void);
    static void Update(void);
    void UpdateFullWindow(void);
    static void DeleteAll(void);	// Delete all instances of the class
    static SPxScDestDisplayX11 *FindObjectForWindow(Window win);   
    static Display *OpenXDisplay(char *displayString);
    static void HandleXEvents(void);
    static int UseDisplay(void *clientData);
    static Visual *FindARGBVisual(void);
    Pixmap GetPixmap(void) {return m_pixmap;}
    XImage *GetImage(void) {return m_image;}
    Window GetWindow(void) {return m_win;}
    void UseWindow(int state) {m_canUseWindow = state;}
    void UsePixmap(int state) {m_canUsePixmap = state;}		
    void CheckStacking(void);									
    void DrawRandomRectangles(Drawable drawable);
    void DrawBox(int x, int y, int w, int h, UINT32 colour);
    void CopyPixmapToWindow(void);
    void FadeWin(void);		// Fade the window.
    void DrawOutline(int x, int y, int w, int h, UINT32 colour);
    // SC API.      
    int SetWinPos(INT16 screenX, INT16 screenY);
    int SetWinGeom(INT16 screenX, INT16 screenY, UINT16 screenW, UINT16 screenH);
    int SetWinParam(UINT16 param, UINT32 arg1, UINT32 arg2, UINT32 arg3);		
    int SetView(REAL32 vx, REAL32 vy, REAL32 vw, REAL32 vh);
    int PanView(REAL32 viewX, REAL32 viewY, UCHAR clearTrails=0);
    int ShowRadar(UCHAR rid, UCHAR state);
    int PatchSet(UCHAR format, UINT16 numPatches, const UINT16 *patchData, UINT16 patchDataLen);
    int SetSource(UINT32, UINT32, UINT32);
    int SetRadarPosition(REAL32 rx, REAL32 ry, UCHAR t) {return 0;}

    /* Control over the use of an intermediate bitmap for fading. */
    virtual void SetUseIntermediateBitmap(int enable)
    {
        m_useIntermediateBitmap = (enable ? TRUE : FALSE);
    }
    virtual int GetUseIntermediateBitmap(void) { return(m_useIntermediateBitmap); }
};


/*********************************************************************
*
* End of file
*
**********************************************************************/
