/*********************************************************************
*
* (c) Copyright 2007 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for class to provide graphics-independent bitmap handling
*
* Revision Control:
*   18/02/19 v1.31   AGC	Use dirty box for clearing.
*
* Previous Changes:
*   23/02/18 1.30   REW	Add SetStride() function.
*   11/05/16 1.29   AGC	Remove unimplemented UpdateWindow() function prototype.
*   12/02/16 1.28   AGC	Derive from SPxLock to allow bitmap locking with SPxAutoLock.
*   07/01/16 1.27   AGC	Make Set/GetUseIntermediateBitmap() virtual.
*   03/11/15 1.26   AGC	PatchSet() patch data now const.
*   17/03/15 1.25   AGC	Use base class m_maxW/H variables.
*   11/12/14 1.24   REW	Mark ClearBitmap() as virtual now that it's
*			also in the SPxScDestDisplay base class.
*   04/10/13 1.23   AGC	Simplify headers.
*   06/06/13 1.22   SP 	Add GetStride().
*   15/05/13 1.21   SP 	Add LockBitmap() and UnlockBitmap().
*   25/04/13 1.20   SP 	Make m_bitmapAccess protected.
*   22/11/12 1.19   REW	Add PanView().
*   07/07/11 1.18   AGC	Prevent warning in AV library.
*   04/02/11 1.17   AGC	Add ImplementsSweepLine() support.
*   28/01/11 1.16   AGC	Add GetIntermediateBitmap(), GetRadarPosition(),
*			and SetUseMainBitmap/GetUseMainBitmap() functions.
*   28/10/10 1.15   AGC	Add include guards.
*   21/09/10 1.14   REW	Interlock fading with patch updates.
*   15/09/10 1.14   REW	Make destructor virtual.
*   08/09/10 1.13   REW	Add m_fadeLevelsRemainder for bug #096.
*   22/03/10 1.12   SP 	Add SetUpdateCallback().
*   29/09/09 1.11   REW	FadeBitmap() handles its own timing rate.
*   06/03/09 1.10   REW	Add m_numFadesSinceLastPatch counter.
*   19/02/09 1.9    REW	Support fading at intermediate 8-bit level.
*   19/09/08 1.8    DGJ	Access functions for dirty box for incremental updates
*   01/02/08 1.7    REW	Revert previous change.
*   01/02/08 1.6    DGJ	Add SetBitmapMemory()
*   25/10/07 1.5    REW	Change SetSource() to use UINT32 args.
*   06/09/07 1.4    DGJ	Support SetSource to clear bitmap
*   03/07/07 1.3    DGJ	Comment out SetFade().
*   29/06/07 1.2    DGJ	Add m_radarUpdate.
*   21/06/07 1.1    DGJ	Initial Version
**********************************************************************/

#ifndef _SPX_SC_DEST_BITMAP_H
#define _SPX_SC_DEST_BITMAP_H

#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxLock.h"
#include "SPxLibSc/SPxScDest.h"
#include "SPxLibSc/SPxScSource.h"
#include "SPxLibSc/SPxScDestDisplay.h"

#include "stdio.h"

// Flags to indicate what has changed since the last update
#define SPX_BITMAP_CHANGE_POSITION  (1 << 0)
#define SPX_BITMAP_CHANGE_SIZE      (1 << 1)
#define SPX_BITMAP_CHANGE_VISIBLE   (1 << 2)
#define SPX_BITMAP_CHANGE_VIEW      (1 << 3)
#define SPX_BITMAP_CHANGE_FADE      (1 << 4)

// Bitmap types;
#define SPX_BITMAP_32BITS 1
#define SPX_BITMAP_8BITS 0

class SPxScDestBitmap :public SPxScDestDisplay, public SPxLock
{
    unsigned char *m_bitmap;
    int m_bitmapType;           // Type of bitmap (8 or 32 bits);
    int m_classAllocatedMemory; // Who allocated the memory
    int m_bitmapDepth;		// Depth of bitmap in bytes
    int m_bitmapStride;		// Stride of bitmap
    int m_screenWin;		// Screen for a specific window.
    DirtyBox m_dirtyBox;	// Dirty box that contains patches for update.
    DirtyBox m_clearDirtyBox;

    // Critical section object to allow access to the dirty box.
    SPxCriticalSection m_dirtyBoxAccess;

    // The view in the bitmap.
    REAL32 m_vx, m_vy, m_vw, m_vh;
    int m_viewHasBeenSet;       // Flag that is set to true when view is set.
    DirtyBox UpdateDirtyBox(int,int,int,int);
    void UpdateDirtyBox(DirtyBox& box, int, int, int, int);
    void InitialiseDirtyBox(int x, int y, int w, int h);
    void InitialiseDirtyBox(DirtyBox& box, int x, int y, int w, int h);
    void GenerateTestPattern(int, int);
    int m_constructOK;			// Set to 1 after successful Create();
    void DrawRandomBoxes(void);
    void ClearBitmapInternal(UINT32 val, int clearAll);

    // Radar position.
    REAL32 m_rx, m_ry;

    // Flag to indicate what has changed since the last update.
    UINT16 m_whatsChangedSinceLastUpdate;
    void DrawBox(int x, int y, int w, int h, UCHAR colour);
    static void (*m_userNotifyUpdate)(SPxScDestBitmap *, UINT16 changes,
					UINT16 firstAzimuth, UINT16 endAzimuth,
					DirtyBox box);

    /* Fields to control fading (as well as those in the base class). */
    unsigned int m_numFadesSinceLastPatch;
    UINT32 m_timeOfLastFadeMsecs;
    double m_fadeLevelsRemainder;
    int m_doFadeWhenPatchListCompletes;
    int m_patchesBeingUpdated;

    /* Intermediate bitmap, optionally for doing 8-bit based fading. */
    UINT8 *m_intermediateBitmap;
    int m_useIntermediateBitmap;

    int m_useMainBitmap;

    int m_implementsSweepLine;	/* Does this class (or a wrapper) implement its own
				 * sweep line?
				 */

    /* Update callback function type */
    typedef void (*SPxScDestBitmapUpdateFn)(SPxScDestBitmap *bitmap, 
                                            UINT16 changes,
                                            UINT16 firstAzimuth, 
                                            UINT16 endAzimuth,
                                            DirtyBox box,
                                            void *userPtr);

    
    /* Per-object update callback function and user arg */
    SPxScDestBitmapUpdateFn m_updateFn;
    void *m_updateFnUserPtr;

protected:
    SPxCriticalSection m_bitmapAccess;

public:

    SPxScDestBitmap(void);
    virtual ~SPxScDestBitmap(void);

    int Create(UINT16 maxScreenW, UINT16 maxScreenH,
		int bitMapType = SPX_BITMAP_8BITS, 
		unsigned char *bitmapMemory = 0, int bitmapStride = 0);
    int GetWidth(void);
    int GetHeight(void);
    int GetStride(void) {return m_bitmapStride;}
    SPxErrorCode SetStride(int stride);

    static void SetNotifyUpdate(void (*fn)(SPxScDestBitmap*, UINT16,
						UINT16, UINT16, DirtyBox)) 
    {
	m_userNotifyUpdate = fn;
    }

    /* Per object update callback */
    void SetUpdateCallback(SPxScDestBitmapUpdateFn fn, void *userPtr);
        
    /* Call the FadeBitmap() function frequently (at least every
     * GetFadeInterval() milliseconds) to implement real-time fading.
     * It handles timing checks internally and will do nothing if not
     * enough time has passed since the last fade, i.e. it is safe to
     * call this more often than necessary (or even if real-time fading
     * is disabled - it will do nothing).  The 'fadedPtr' argument can
     * be used to give the location of an optional flag that will be set
     * to zero if no fade was necesssary and non-zero if a fade was done.
     * The ForcedFadeBitmap() function is for internal use only.
     */
    int FadeBitmap(int *fadedPtr=NULL);
    int ForcedFadeBitmap(int levels);

    /* Access to the Dirty Box. Get access, read contents and clear it. */
    void ReadAndClearDirtyBox(int *x, int *y, int *w, int *h);
    void LockDirtyBox(void);
    void UnlockDirtyBox(void);
    void ReadClearDirtyBox(int *x, int *y, int *w, int *h);
    void ReadDirtyBox(int *x, int *y, int *w, int *h);
    void ClearDirtyBox(void);
    void ClearDirtyBox(DirtyBox& box);

    /* External access to the bitmap data. */
    SPxErrorCode LockBitmap(void);
    SPxErrorCode UnlockBitmap(void);

    /* SPxLock interface for locking bitmap with SPxAutoLock. */
    SPxErrorCode Enter(void) { return LockBitmap(); }
    SPxErrorCode Leave(void) { return UnlockBitmap(); }

    // Useful convenience functions that a user might need to access the bitmap
    unsigned char *GetBitmap(void) {return m_bitmap;}
    unsigned char *GetIntermediateBitmap(void) {return m_intermediateBitmap;}
    int GetView(REAL32 *vx, REAL32 *vy, REAL32 *vw, REAL32 *vh);  
    int GetShowState(void) {return m_radarUpdate;}  
    /* the following are available courtesy of the parent class
    int GetFadeType(void);
    int GetfadeRate(void);
    int GetW(void)
    int GetH(void)
    */
    
    virtual void ClearBitmap(UINT32 val=0);
    virtual void ClearWholeBitmap(UINT32 val=0);
    void DrawOutline(int x, int y, int w, int h, UCHAR colour);

    virtual int ImplementsSweepLine(void) { return m_implementsSweepLine; }

// SC API.      
    int SetWinPos(INT16 screenX, INT16 screenY);
    int SetWinGeom(INT16 screenX,INT16 screenY,UINT16 screenW,UINT16 screenH);
    int SetWinParam(UINT16 param, UINT32 arg1, UINT32 arg2, UINT32 arg3);
    int SetView(REAL32 vx, REAL32 vy, REAL32 vw, REAL32 vh);
    int PanView(REAL32 vx, REAL32 vy, UCHAR clearTrails=0);
    int ShowRadar(UCHAR rid, UCHAR state);
    virtual int PatchSet(UCHAR format, UINT16 numPatches,
                         const UINT16 *patchData, UINT16 patchDataLen);
//    int SetFade(UCHAR fadeType, UINT16 fadeRate);
    int SetSource(UINT32, UINT32, UINT32);
    int SetRadarPosition(REAL32 rx, REAL32 ry, UCHAR /*t*/)
    { 
	m_rx = rx; 
	m_ry = ry;
	return 0;
    }
    void GetRadarPosition(REAL32 *rx, REAL32 *ry)
    { 
	if(rx) *rx = m_rx;
	if(ry) *ry = m_ry;
    }

    /* Control over the use of an intermediate bitmap for fading. */
    virtual void SetUseIntermediateBitmap(int enable)
    {
	m_useIntermediateBitmap = (enable ? TRUE : FALSE);
    }
    virtual int GetUseIntermediateBitmap(void) { return(m_useIntermediateBitmap); }

    /* Control over use of main bitmap for use by SPxPPIwindowGlx. */
    void SetUseMainBitmap(int enable) 
    { 
	m_useMainBitmap = (enable ? TRUE : FALSE); 
    }
    int GetUseMainBitmap(void) { return m_useMainBitmap; }

    /* Control over overriding SC sweep line for use by SPxPPIwindowGlx. */
    void SetImplementsSweepLine(int enable) { m_implementsSweepLine = enable; }
};

#endif /* _SPX_SC_DEST_BITMAP_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
