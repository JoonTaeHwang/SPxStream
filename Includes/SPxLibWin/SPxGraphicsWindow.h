/*********************************************************************
 *
 * (c) Copyright 2010 - 2017, Cambridge Pixel Ltd.
 *
 * File: $RCSfile: SPxGraphicsWindow.h,v $
 * ID: $Id: SPxGraphicsWindow.h,v 1.8 2017/02/21 15:21:01 rew Exp $
 *
 * Purpose:
 *   Header for SPx graphics window class.
 *
 * Revision Control:
 *   21/02/17 v1.8    AGC	Support specifying DPI for PaintWindow().
 *
 * Previous Changes:
 *   04/10/13 1.7    AGC    Simplify headers.
 *   24/05/13 1.6    AGC    Fix WINVER undefined warning.
 *   19/04/12 1.5    SP     Set key colour as RGB (as documented) not BGR.
 *   25/11/11 1.4    SP     Improve debug control.
 *   14/09/10 1.3    SP     Add event redirection.
 *                          Add extra debug.
 *                          Make destructor virtual.
 *   20/08/10 1.2    SP     Fix problems with attached windows.
 *   28/07/10 1.1    SP     Initial version.
 **********************************************************************/

#ifndef _SPX_GRAPHICS_WINDOW_H
#define _SPX_GRAPHICS_WINDOW_H

/* Windows headers. */
#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

/* Needed for general SPx types. */
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
 *
 *   Constants
 *
 **********************************************************************/

/* Debug flags.  Keep in sync with SPxRadarWindow debug flags. */
#define SPX_GFX_WIN_DEBUG_CONFIG    0x00000001   /* Configuration */
#define SPX_GFX_WIN_DEBUG_UPDATE    0x00000002   /* Window update */

/*********************************************************************
 *
 *   Type definitions
 *
 **********************************************************************/

/* Forward declarations. */
class SPxBitmapWin;
class SPxThread;
class SPxCriticalSection;
namespace Gdiplus
{
    class Graphics;
};

/*
 * SPxGraphicsWindow class
 */
class SPxGraphicsWindow : public SPxObj
{
private:

    /*
     * Private Classes
     */

    /* Wrapper around CWnd */
    class GfxWnd : public CWnd
    {
    public:
        GfxWnd(void);
        virtual ~GfxWnd(void);
        SPxErrorCode RedirectMouseEvents(HWND hWnd);

    protected:
        BOOL OnWndMsg(UINT message, WPARAM wParam, 
                      LPARAM lParam, LRESULT* pResult);

    private:
        HWND m_mouseEventHWnd;
    };

    /*
     * Private types.
     */

    /* Control state. The unchanged state is the default state.
     * It is used for attached windows and prevents the current
     * window settings from being modified unless explicitly set.
     */
    typedef enum
    {
        STATE_UNCHANGED = 0,      /* Must be zero */
        STATE_DISABLED,
        STATE_ENABLED

    } State_t;

    /* All window status flags and fields */
    typedef struct
    {
        UINT32 updateFlags;       /* Flags set when object is modified */
        State_t alphaBlend;       /* Alpha blending state */
        int sca;                  /* Source constant alpha */
        State_t chromaKey;        /* Chromakeying state */
        UINT32 crKeyRGB;          /* Chromakey colour */
        State_t transparent;      /* Event transparency state */ 
        HWND ownerHWnd;           /* New owner of the window */
        BOOL forcePopup;          /* Force this window to be a pop-up */       

    } WindowStatus_t;
    
    /*
     * Private fields.
     */

    GfxWnd *m_win;                  /* The window object */
    unsigned int m_maxWidth;        /* Max window width */
    unsigned int m_maxHeight;       /* Max window height */
    BOOL m_isAttached;              /* Is this an attached window? */
    LONG_PTR m_origStyle;           /* Original window style flags */
    LONG_PTR m_origExStyle;         /* Original window exStyle flags */
    HWND m_origOwnerHWnd;           /* Original owner of the window */
    SPxThread *m_updateThread;      /* Thread used to update attached windows */
    SPxCriticalSection *m_updateMutex; /* Mutex used in update thread */
    WindowStatus_t m_winStatus;     /* Window status fields */
    BOOL m_updateLayeredWindowEnabled; /* Can we use UpdateLayeredWindow()? */
    UINT32 m_debug;                 /* Debug flags */
    HWND m_clipHWnd;                /* Window to clip to */
    BOOL m_includeClipWin;          /* Include m_clipHWnd itself? */
    BOOL m_includeClipWinInferiors; /* Include inferiors of m_clipHWnd? */
    SPxBitmapWin *m_bitmap;         /* Offscreen bitmap */
    Gdiplus::Graphics *m_bitmapGfx;          /* GDI+ to draw to bitmap */
    
    /*
     * Private functions.
     */

    HDC BlankClipWindowRegions(HDC srcHdc, 
                               int srcX, int srcY,
                               int width, int height);

    SPxErrorCode CheckCreateOffscreenBitmap(void);

protected:

    /*
     * Protected fields.
     */

    /*
     * Protected functions.
     */

public:

    /*
     * Public fields.
     */

    /*
     * Public functions.
     */

    SPxGraphicsWindow(unsigned int maxOffscreenWidth=0, 
                      unsigned int maxOffscreenHeight=0);
    virtual ~SPxGraphicsWindow(void);

    SPxErrorCode Create(HWND ownerHWnd);
    SPxErrorCode Attach(HWND hWnd, HWND ownerHWnd, BOOL forcePopup);
    BOOL IsAttached(void) { return m_isAttached; }
    SPxErrorCode SetAlphaBlending(BOOL enable);
    SPxErrorCode SetSourceConstantAlpha(int sca);
    SPxErrorCode SetChromaKeying(BOOL enable);
    SPxErrorCode SetChromaKeyColour(UINT32 crKeyRGB);
    SPxErrorCode SetTransparency(BOOL enable);
    SPxErrorCode RedirectMouseEvents(HWND hWnd);
    SPxErrorCode SetOwner(HWND ownerHWnd);
    SPxErrorCode SetClipWindow(HWND hWnd,  
                               BOOL includeWinInferiors=FALSE,
                               BOOL includeWin=TRUE);
    CWnd *GetCWnd(void) { return m_win; }
    HWND GetHWND(void) { return (m_win ? m_win->GetSafeHwnd() : NULL); }
    HWND GetTopLevelHWND(void);
    SPxErrorCode PaintWindow(HDC srcHdc, 
                             int srcX, int srcY,
                             int width,
                             int height,
			     int srcDpi=96, int dstDpi=96);
    void SetDebug(UINT32 debug) { m_debug = debug; }
    UINT32 GetDebug(void) { return m_debug; }

    /* These functions are public that that they can 
     * be called from the thread used to configure 
     * attached windows. 
     */
    SPxErrorCode UpdateWindow(void);
    UINT32 GetUpdateFlags(void) { return m_winStatus.updateFlags; }
    LONG_PTR GetOrigStyle(void) { return m_origStyle; }
    LONG_PTR GetOrigExStyle(void) { return m_origExStyle; }
    HWND GetOrigOwnerHWnd(void) { return m_origOwnerHWnd; }
    void SetIsAttached(BOOL state) { m_isAttached = state; }
    
    /* Static functions */
    static void SetDefaultDebug(UINT32 level);
    static void SPxGraphicsWindowDeleteAll(void);
};

#endif /* _SPX_GRAPHICS_WINDOW_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
