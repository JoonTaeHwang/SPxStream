/*********************************************************************
*
* (c) Copyright 2007 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for class to render scan converted data on Windows
*
* Revision Control:
*   21/06/18 v1.8    AGC	Add GetRequiredScaleFactor().
*
* Previous Changes:
*   15/06/18 1.7    AGC	Use SPxThread and set thread name.
*   08/03/17 1.6    AGC	Use correct calling convention for SPxWin32UpdateThread.
*   07/01/16 1.5    AGC	Make Set/GetUseIntermediateBitmap() virtual.
*   03/11/15 1.4    AGC	PatchSet() patch data now const.
*   04/10/13 1.3    AGC	Simplify headers.
*   22/11/12 1.2    REW	Add PanView().
*   13/01/12 1.1    AGC	Create from SPxScDestDisplayWin 1.37 without MFC
**********************************************************************/
#ifndef _SPX_SC_DEST_DISPLAY_WIN32_H
#define _SPX_SC_DEST_DISPLAY_WIN32_H

#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxThreads.h"
#include "SPxLibSc/SPxScDest.h"
#include "SPxLibSc/SPxScSource.h"
#include "SPxLibSc/SPxScDestDisplay.h"

void *SPxWin32UpdateThread(SPxThread *thread);

class SPxScDestDisplayWinRaw :public SPxScDestDisplay
{
protected:
    SPxCriticalSection m_bitmapAccess;

    /* Intermediate bitmap, optionally for doing 8-bit based fading. */
    UINT8 *m_intermediateBitmap;
    int m_useIntermediateBitmap;
    unsigned char *m_winBitmapBits;
    HBITMAP m_winDIBbitmap;
    SPxThread m_thread;			// Handle of update thread.
    BITMAPINFO *m_bitmapInfo;
    HWND m_underlay, m_overlay;	// Underlay and overlay
    HWND m_win;			// The radar window
    int m_screenWin;			// Screen for a specific window.
    DirtyBox m_dirtyBox;		// The dirty box that contains the patches for update.
    DirtyBox UpdateDirtyBox(int,int,int,int);
    void InitialiseDirtyBox(int x, int y, int w, int h);
    void UpdateWindow(DirtyBox *box);
    int m_constructOK;			// Set to 1 after successful Create();
    int CreateRadarWindow(BOOL isLayered);
    void DrawRandomBoxes(void);
    void DrawBox(int x, int y, int w, int h, UINT32 colour);
    // Initialise flags for update control.
    BOOL m_doUpdateWhenPatchListCompletes;
    BOOL m_patchesBeingUpdated;
    BOOL m_doFadeWhenPatchListCompletes;
    double m_fadeLevelsRemainder;
    unsigned int m_numFadesSinceLastPatch;
    int CreateBitmap(int maxScreenW, int maxScreenH);
    HDC m_dcMemory;


public:
    void FadeWin(void);
    SPxCriticalSection *GetBitmapAccessMutex(void) {return &m_bitmapAccess;}
    DWORD m_lastUpdateTime;		/* The time of the last update */
    DWORD m_lastFadeTime;		/* The time of the last real time fade */
    HWND GetOverlayHwnd(void)  {return m_overlay;}
    HWND GetUnderlayHwnd(void) {return m_underlay;}
    void SetDoFadeWhenPatchListCompletes(void) { m_doFadeWhenPatchListCompletes = TRUE;}
    void SetDoUpdateWhenPatchListCompletes(void) { m_doUpdateWhenPatchListCompletes = TRUE;}
    BOOL PatchesBeingUpdated(void) {return m_patchesBeingUpdated;}
    BOOL DoUpdateWhenPatchListCompletes(void){return m_doUpdateWhenPatchListCompletes;}
    BOOL DoFadeWhenPatchListCompletes(void){return m_doUpdateWhenPatchListCompletes;}
    virtual void Update(void);
    HWND GetRadarWindowHwnd(void) {return m_win;}
    void GenerateTestPattern(int, int);
    virtual void CopyBitmapToWindow(DirtyBox *box);
    SPxScDestDisplayWinRaw(void);	
    int Create(UCHAR screenID, UINT16 maxScreenW, UINT16 maxScreenH,
			HWND underlay = 0, HWND overlay = 0);
    virtual ~SPxScDestDisplayWinRaw(void);

    void ClearBitmap(UINT32 val=0);
    unsigned char *GetDisplayBits(void) {return m_winBitmapBits;}
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
    int SetRadarPosition(REAL32 /*rx*/, REAL32 /*ry*/, UCHAR /*t*/) {return 0;}

    /* Control over the use of an intermediate bitmap for fading. */
    virtual void SetUseIntermediateBitmap(int enable)
    {
	m_useIntermediateBitmap = (enable ? TRUE : FALSE);
    }
    virtual int GetUseIntermediateBitmap(void) { return(m_useIntermediateBitmap); }

    virtual int GetRequiredScaleFactor(void);
};

#endif /* _SPX_SC_DEST_DISPLAY_WIN32_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
