/*********************************************************************
 *
 * (c) Copyright 2010 - 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPx radar window class.
 *
 * Revision Control:
 *  02/02/18 v1.17   AGC	Remove unused view control handler.
 *
 * Previous Changes:
 *  21/02/17 1.16   AGC	Update window size on background thread.
 *  04/10/13 1.15   AGC	Simplify headers.
 *  24/05/13 1.14   AGC	Fix WINVER undefined warning.
 *  13/07/12 1.13   AGC	Support sub-areas.
 *  19/04/12 1.12   SP 	Support external compositing.
 *			Remove bitmap update flags.
 *			Add PanView().
 *  20/02/12 1.11   SP 	Add CreateOlayBottomBitmap() and
 *			CreateOlayBottomWindow().
 *			Rename some functions.
 *  08/02/12 1.10   SP 	Support update from SPxViewControl.
 *  13/01/12 1.9    SP 	Add CreateUlayTopBitmap() & 
 *			CreateBottomBitmap().
 *  25/11/11 1.8    SP 	Improve debug control.
 *  21/10/11 1.7    SP 	Add functions to set/get radar position.
 *  12/04/11 1.6    SP 	Add CreateUlayTopWin().
 *  04/02/11 1.5    AGC	Use individual sweep line option set/get functions.
 *  28/01/11 1.4    AGC	Add Sweep Line set/get functions.
 *  14/09/10 1.3    SP 	Add event redirection.
 *			Make destructor virtual.
 *			Add SPX_RW_NO_UNDERLAY flag.
 *  20/08/10 1.2    SP 	Fix problems with attached windows.
 *  28/07/10 1.1    SP 	Initial version.
 **********************************************************************/

#ifndef _SPX_RADAR_WINDOW_H
#define _SPX_RADAR_WINDOW_H

/* Needed for ganeral SPx types. */
#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxObj.h"

#include "SPxLibSc/SPxSc.h" /* For SPxScSweepLineType_t enum etc. */

/*********************************************************************
 *
 *   Constants
 *
 **********************************************************************/

/* 
 * Underlay and overlay control flags (bitwise). Some flags are
 * only applicable to either underlay or overlay windows. Some 
 * flags are used internally with the class and not intended
 * to be set by a user (and therefore not documented in the manual).
 * In general the default flags set by the class will be correct
 * for the mode in which an object is being used. Under certain
 * circumstances a user may wish to change the behaviour of an
 * object. In this situtation the following flags may be useful:
 *
 * Underlay window:
 *  SPX_RW_NO_MOUSE_REDIRECT
 *  SPX_RW_NO_OBSCURE_BACKGROUND
 *
 * Overlay window:
 *  SPX_RW_ALPHABLEND
 *  SPX_RW_NO_TRANSPARENT
 *  SPX_RW_NO_CLIP_TO_APP_INFERIORS
 *
 * Radar window:
 *  SPX_RW_NO_CLIP_TO_APP_INFERIORS
 */

/* 
 * Disable flags are bits 0 to 15. In general bits 0 to 7 are
 * used for flags that are documented in the API manual and
 * bits 8 to 15 are advanced or internal flags.
 */

/* Don't redirect mouse events to the application window */
#define SPX_RW_NO_MOUSE_REDIRECT           0x0001

/* This flag may be set when the application window is used
 * to provide overlay graphics and there is no underlay source.
 * By default a black window will be inserted at the bottom
 * of the stack so that the desktop is not visible. Set this 
 * flag to prevent this.
 */
#define SPX_RW_NO_OBSCURE_BACKGROUND       0x0002

/* Don't make window transparent to events */
#define SPX_RW_NO_TRANSPARENT              0x0004

/* Don't use per-pixel alpha-blending in the window */
#define SPX_RW_NO_ALPHABLEND               0x0008

/* Don't clip window contents to inferiors of 
 * the application window.
 */
#define SPX_RW_NO_CLIP_TO_APP_INFERIORS    0x0010

/* Don't use chromakeying in the window */
#define SPX_RW_NO_CHROMAKEY                0x0100

/* Don't force attached window to be a borderless pop-up */
#define SPX_RW_NO_BORDERLESS               0x0200

/* 
 * Enable flags are bits 16 to 31 and copy the bit 
 * pattern of the disable flags.
 */

/* Redirect mouse events to the application window */
#define SPX_RW_MOUSE_REDIRECT           (SPX_RW_NO_MOUSE_REDIRECT << 16)

/* Ensure background (desktop) is always obscured */
#define SPX_RW_OBSCURE_BACKGROUND       (SPX_RW_NO_OBSCURE_BACKGROUND << 16)

/* Make window transparent to events */
#define SPX_RW_TRANSPARENT              (SPX_RW_NO_TRANSPARENT << 16)

/* Use per-pixel alpha-blending in the window. If enabled
 * then chromakeying (SPX_RW_CHROMAKEY) cannot be used.
 */
#define SPX_RW_ALPHABLEND               (SPX_RW_NO_ALPHABLEND << 16)

/* Don't clip window to inferiors of the application window. */
#define SPX_RW_CLIP_TO_APP_INFERIORS    (SPX_RW_NO_CLIP_TO_APP_INFERIORS << 16)

/* Use chromakeying in the window. If enabled then 
 * per-pixel alpha blending (SPX_RW_ALPHABLEND) cannot be used.
 */
#define SPX_RW_CHROMAKEY                (SPX_RW_NO_CHROMAKEY << 16)

/* Force attached window to be a borderless pop-up */
#define SPX_RW_BORDERLESS               (SPX_RW_NO_BORDERLESS << 16)

/* 
 * Debug flags. 
 * Keep in sync with SPxGraphicsWindow debug flags. 
 */
#define SPX_RADAR_WIN_DEBUG_CONFIG      0x00000001  /* Configuration */
#define SPX_RADAR_WIN_DEBUG_UPDATE      0x00000002  /* Window update */


/*********************************************************************
 *
 *   Type definitions
 *
 **********************************************************************/

/* Forward declarations. */
class SPxSc;
class SPxScDestBitmap;
class SPxRunProcess;
class SPxScFollowWin;
class SPxGraphicsWindow;
class SPxBitmapWin;
class SPxViewControl;
class SPxEvent;
class SPxThread;

/*
 * SPx Shared Bitmap class
 */
class SPxRadarWindow : public SPxObj
{
public:

    /*
     * Public types.
     */

    /* Compositing mode. */
    typedef enum
    {
        /* Compositing mode not set. */
        COMP_MODE_INVALID,

        /* Replace contents of the application window with radar 
         * video and graphics provided in shared memory bitmaps. 
         */
        COMP_MODE_APP_REPLACE,

        /* Combine application window with radar video and graphics
         * displayed in layered windows. Graphics may be provided in
         * layered windiows or shared memory bitmaps.
         */
        COMP_MODE_APP_WINDOW,

        /* Output radar video and graphics into shared memory bitmaps
         * so that they can be composited externally by the application.
         */
        COMP_MODE_EXTERNAL,
    
    } CompMode_t;

    /*
     * Public fields.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor */
    explicit SPxRadarWindow(void);
    virtual ~SPxRadarWindow(void);

    /* Overloaded create functions */
    SPxErrorCode Create(HWND appHWin,
                        unsigned int maxWidth,
                        unsigned int maxHeight,
                        unsigned int maxRadarVideos,
                        const char *ulayBitmapName,
                        const char *olayBitmapName,
                        UINT32 ulayFlags=0,
                        UINT32 olayFlags=0,
                        UINT32 radarFlags=0);

    SPxErrorCode Create(HWND appHWin,
                        unsigned int maxWidth,
                        unsigned int maxHeight,
                        unsigned int maxRadarVideos,
                        BOOL appWinOverlay,
                        const char *bitmapName,
                        UINT32 ulayFlags=0,
                        UINT32 olayFlags=0,
                        UINT32 radarFlags=0);

    SPxErrorCode Create(HWND appHWin,
                        unsigned int maxWidth,
                        unsigned int maxHeight,
                        unsigned int maxRadarVideos,
                        BOOL appWinOverlay,
                        HWND hWin,
                        UINT32 ulayFlags=0,
                        UINT32 olayFlags=0,
                        UINT32 radarFlags=0);

    SPxErrorCode Create(const char *radarBitmapName,
                        unsigned int maxWidth,
                        unsigned int maxHeight,
                        unsigned int maxRadarVideos,
                        const char *ulayBitmapName,
                        const char *olayBitmapName,
                        UINT32 ulayFlags=0,
                        UINT32 olayFlags=0,
                        UINT32 radarFlags=0);

    /* Legacy function. */
    BOOL IsBitmapCompositing(void) 
    { 
        return((m_compMode == COMP_MODE_APP_REPLACE) ? TRUE : FALSE); 
    }

    CompMode_t GetCompMode(void) { return m_compMode; }

    unsigned char *GetRadarImageMemory(int radarIndex);

    unsigned int GetRadarImageStride(int radarIndex);

    SPxErrorCode SetSPx(SPxSc *sc,
                        SPxScDestBitmap *scBitmap,
                        SPxRunProcess *scProc,
                        SPxScFollowWin *followWin);

    SPxErrorCode AddSPx(SPxSc *sc,
                        SPxScDestBitmap *scBitmap,
                        SPxRunProcess *scProc);

    SPxGraphicsWindow *CreateOlayTopWin(SPxErrorCode *errRtn);
    SPxBitmapWin *CreateOlayTopBitmap(SPxErrorCode *errRtn);

    SPxGraphicsWindow *CreateOlayBottomWin(SPxErrorCode *errRtn);
    SPxBitmapWin *CreateOlayBottomBitmap(SPxErrorCode *errRtn);

    SPxGraphicsWindow *CreateUlayTopWin(SPxErrorCode *errRtn);
    SPxBitmapWin *CreateUlayTopBitmap(SPxErrorCode *errRtn);

    SPxGraphicsWindow *CreateUlayBottomWin(SPxErrorCode *errRtn);
    SPxBitmapWin *CreateUlayBottomBitmap(SPxErrorCode *errRtn);

    void CompositeDisplay(void);

    SPxErrorCode EnableRadarVideo(int radarIndex, BOOL state);
    SPxErrorCode EnableRadarVideo(BOOL state);
    BOOL GetEnableRadarVideo(int radarIndex);

    SPxErrorCode SetView(double cx, double cy, 
                         double w, double h);
    SPxErrorCode GetView(double *cxRtn, double *cyRtn, 
                         double *wRtn, double *hRtn);

    SPxErrorCode PanView(double cx, double cy, int clearTrails=0);

    SPxErrorCode SetRadarColour(int radarIndex, UINT32 rgb);
    SPxErrorCode SetRadarColour(UINT32 rgb);

    SPxErrorCode SetFade(int radarIndex, UCHAR fadeType);
    SPxErrorCode SetFade(UCHAR fadeType);
    UCHAR GetFade(int radarIndex);

    SPxErrorCode SetFadeTime(int radarIndex, double fadeTime);
    SPxErrorCode SetFadeTime(double fadeTime);
    double GetFadeTime(int radarIndex);

    SPxErrorCode SetSweepLineColour(int radarIndex, UINT32 colour);
    SPxErrorCode SetSweepLineColour(UINT32 colour);
    UINT32 GetSweepLineColour(int radarIndex);

    SPxErrorCode SetSweepLineType(int radarIndex, SPxScSweepLineType_t type);
    SPxErrorCode SetSweepLineType(SPxScSweepLineType_t type);
    SPxScSweepLineType_t GetSweepLineType(int radarIndex);

    SPxErrorCode SetSweepLineColMode(int radarIndex, 
                                     SPxScSweepLineColMode_t mode);
    SPxErrorCode SetSweepLineColMode(SPxScSweepLineColMode_t mode);
    SPxScSweepLineColMode_t GetSweepLineColMode(int radarIndex);

    SPxErrorCode SetRadarPosition(double xMetres, 
                                  double yMetres, 
                                  int clearTrails=0);
    SPxErrorCode SetRadarPosition(int radarIndex, 
                                  double xMetres, 
                                  double yMetres, 
                                  int clearTrails=0);
    SPxErrorCode GetRadarPosition(int radarIndex, 
                                  double *xMetresRtn, 
                                  double *yMetresRtn);

    SPxErrorCode SetRadarSubArea(int x, int y, int w, int h);
    SPxErrorCode GetRadarSubArea(int *x, int *y, int *w, int *h);
    void EnableSubArea(int enable);
    int GetSubAreaEnable(void);

    unsigned int GetMaxWidth(void) { return m_maxWidth; }
    unsigned int GetMaxHeight(void) { return m_maxHeight; }

    void EnablePixmapADrawing(int state) { m_ulayEnabled = state; }
    void EnablePixmapBDrawing(int state) { m_olayEnabled = state; }

    SPxErrorCode SetOlayKeyColour(UINT32 crKey);
    COLORREF GetOlayKeyColour(void);

    SPxErrorCode SetMouseEventWindow(HWND hWnd);
    HWND GetMouseEventWindow(void);

    void SetDebug(UINT32 debug) { m_debug = debug; }
    UINT32 GetDebug(void) { return m_debug; }

    unsigned int GetWinWidth(void) { return m_winWidth; }
    unsigned int GetWinHeight(void) { return m_winHeight; } 

    SPxErrorCode SetViewControl(SPxViewControl *obj);
 
    /* The following function names are used for
     * compatibility with the SPxPPIwindow class.
     */
    void PixmapAChanged(void) { /* Do nothing at present. */ }
    void PixmapBChanged(void) { /* Do nothing at present. */ }

    SPxErrorCode SetWindowSize(unsigned int width, 
                               unsigned int height,
                               int forceUpdate=FALSE);

    /* 
     * Public static functions. 
     */

    static void SetDefaultDebug(UINT32 level);

protected:
    /*
     * Protected fields.
     */

    /*
     * Protected functions.
     */

    /* Parameter setting and retrieval (overrides baseclass functions) */
    int SetParameter(char *parameterName, char *parameterValue);
    int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:

    /*
     * Private types.
     */

    /* Overlay and underlay windows (see cpp file header). */
    typedef enum
    {
        OLAY_TOP_GFX_WIN = 0,
        OLAY_GFX_WIN,
        OLAY_BOTTOM_GFX_WIN,
        ULAY_TOP_GFX_WIN,
        ULAY_GFX_WIN,
        ULAY_BOTTOM_GFX_WIN

    } GfxWin_t;

    /* Radar video info */
    typedef struct
    {
        SPxBitmapWin *bitmap;       /* Bitmap with allocated memory */
	SPxBitmapWin *bitmapAux;
        SPxSc *sc;                  /* Scan converter object */
        SPxScDestBitmap *scBitmap;  /* Radar scan conversion bitmap */
        SPxRunProcess *scProc;      /* Scan conversion process */
        SPxGraphicsWindow *radarWin;/* Radar window used for win compositing */
        BOOL isEnabled;             /* TRUE if radar video is enabled */
        UCHAR fadeType;             /* Current radar fade type */
        double fadeTime;            /* Current fade time */
        double radarXMetres;        /* Current radar X position */
        double radarYMetres;        /* Current radar Y position */

    } RadarInfo_t;

    /* Function pointer types for DPI queries. */
    typedef UINT(WINAPI *GetDpiForWindowFn)(HWND);
    typedef UINT(WINAPI *GetDpiForSystemFn)();

    /*
     * Private fields.
     */

    unsigned int m_maxWidth;        /* Maximum width in pixels */
    unsigned int m_maxHeight;       /* Maximum height in pixels */
    unsigned int m_maxRadarVideos;  /* Maximum number of radar videos */
    unsigned int m_numRadarVideos;  /* Current number of radar videos */
    CompMode_t m_compMode;          /* Compositing mode. */
    BOOL m_appWinOverlay;           /* Is the application win an overlay? */
    HWND m_appHWnd;                 /* Application window */
    SPxGraphicsWindow *m_appWin;    /* Application window after attach */
    SPxGraphicsWindow *m_olayWin;   /* Overlay window */
    SPxGraphicsWindow *m_olayTopWin;/* Top overlay window */
    SPxGraphicsWindow *m_olayBottomWin; /* Bottom overlay window */
    SPxGraphicsWindow *m_ulayWin;   /* Underlay window */
    SPxGraphicsWindow *m_ulayTopWin;/* Top underlay window */
    SPxGraphicsWindow *m_ulayBottomWin;/* Bottom underlay window */
    SPxBitmapWin *m_olayBitmap;     /* Overlay shared memory bitmap */
    SPxBitmapWin *m_olayTopBitmap;  /* Top overlay shared memory bitmap */
    SPxBitmapWin *m_olayBottomBitmap; /* Bottom overlay shared mem bitmap */
    SPxBitmapWin *m_ulayBitmap;     /* Underlay shared memory bitmap */
    SPxBitmapWin *m_ulayTopBitmap;  /* Top underlay shared memory bitmap */
    SPxBitmapWin *m_ulayBottomBitmap; /* Bottom ulay shared memory bitmap */
    SPxBitmapWin *m_combBitmap;     /* Combined local memory bitmap */
    RadarInfo_t *m_radarInfo;       /* Radar info array */
    BOOL m_ulayEnabled;             /* Drawing underlay enabled? */
    BOOL m_olayEnabled;             /* Drawing overlay enabled? */
    SPxScFollowWin *m_follow;       /* Used to track application window */
    int m_slaveWinIndex;            /* Index of next slave window */
    COLORREF m_olayCrKey;           /* Overlay chromakey colour */
    HWND m_mouseEventHWnd;          /* Window to redirect mouse events to */
    UINT32 m_debug;                 /* Debug flags */
    SPxGraphicsWindow *m_firstWin;  /* This variable is used to set the 
                                     * first graphics window in the z-order 
                                     * stack. 
                                     */
    unsigned int m_winWidth;        /* Current application window width */
    unsigned int m_winHeight;       /* Current application window height */
    SPxViewControl *m_viewCtrl;     /* Installed view control object. */
    SPxEvent *m_event;		    /* Event for background window size updates. */
    SPxThread *m_thread;	    /* Thread for background window size updates. */

    int m_subAreaX, m_subAreaY, m_subAreaW, m_subAreaH; /* Current sub-area. */
    int m_subAreaEnabled;               /* Is sub-area display enabled? */

    /* DPI matching. */
    HMODULE m_userMod;
    GetDpiForWindowFn m_getDpiForWindow;
    GetDpiForSystemFn m_getDpiForSystem;
    UINT m_winDpi;

    /*
     * Private functions.
     */

    SPxErrorCode CreateInternal(HWND appHWin,
                                unsigned int maxWidth,
                                unsigned int maxHeight,
                                unsigned int maxRadarVideos,
                                const char *ulayBitmapName,
                                const char *olayBitmapName,
                                HWND ulayHWin,
                                HWND olayHWin,
                                UINT32 userUlayFlags,
                                UINT32 userOlayFlags,
                                UINT32 userRadarFlags);

    SPxErrorCode CreateExternal(const char *radarBitmapName,
                                unsigned int maxWidth,
                                unsigned int maxHeight,
                                unsigned int maxRadarVideos,
                                const char *ulayBitmapName,
                                const char *olayBitmapName,
                                UINT32 ulayFlags,
                                UINT32 olayFlags,
                                UINT32 radarFlags);

    void DoReplaceCompositing(unsigned int width, unsigned int height);

    void DoWindowCompositing(unsigned int width, unsigned int height);

    void UpdateExternalBitmaps(unsigned int width, unsigned int height);

    void FadeRadarBitmaps(void);

    SPxGraphicsWindow *CreateWin(HWND ownerHWnd,
                                 UINT32 flags, 
                                 SPxErrorCode *errRtn);

    SPxGraphicsWindow *AttachWin(HWND hWnd,
                                 HWND ownerHWnd,
                                 UINT32 flags, 
                                 SPxErrorCode *errRtn);

    SPxErrorCode ConfigureWin(SPxGraphicsWindow *win, 
                              UINT32 flags);

    SPxErrorCode ValidateRadarIndex(const char *funcName, 
                                    int index, 
                                    BOOL useMaxNum);

    static void *ThreadFnWrapper(SPxThread *thread);
    void *ThreadFn(SPxThread *thread);

    SPxGraphicsWindow *GetNextGraphicsWindow(GfxWin_t gfxWin);
    SPxGraphicsWindow *GetPrevGraphicsWindow(GfxWin_t gfxWin);

    /*
     * Private static functions.
     */

};

/*********************************************************************
 *
 *   Function prototypes
 *
 **********************************************************************/

#endif /* _SPX_RADAR_WINDOW_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
