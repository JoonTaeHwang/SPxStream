/*********************************************************************
 *
 * (c) Copyright 2010 - 2012, Cambridge Pixel Ltd.
 *
 * File: $RCSfile: SPxBitmapWin.h,v $
 * ID: $Id: SPxBitmapWin.h,v 1.7 2013/10/04 15:31:08 rew Exp $
 *
 * Purpose:
 *   Header for SPx bitmap class.
 *
 * Revision Control:
 *   04/10/13 v1.10   AGC	Simplify headers.
 *
 * Previous Changes:
 *   30/07/13 1.9    AGC    Improve Unicode support.
 *   30/04/12 1.8    AGC    Make GetHBitmap() const.
 *   29/03/12 1.7    SP     Add mutex for synchronisation.
 *   12/04/11 1.3    SP     Add ClearBitmap().
 *   14/09/10 1.2    SP     Change GetName() to GetBitmapName()
 *                          to avoid masking baseclass function.
 *                          Make destructor virtual.
 *   28/07/10 1.1    SP     Initial version.
 **********************************************************************/

#ifndef _SPX_BITMAP_WIN_H
#define _SPX_BITMAP_WIN_H

/* We need SPxLibUtils for common types. */
#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
 *
 *   Constants
 *
 **********************************************************************/

/*********************************************************************
 *
 *   Type definitions
 *
 **********************************************************************/

/*
 * SPx Shared Bitmap class
 */
class SPxBitmapWin : public SPxObj
{
private:
    /*
     * Private fields.
     */

    /* Bitmap info block. For shared bitmaps this block is written to
     * start of shared memory. The size of this structure MUST be a
     * multiple of 32-bits. Fields should only be appended to this
     * structure, not inserted. Alternatively use one of the reserved
     * fields. When a change is made the version number should be
     * incremented. See BITMAP_INFO_VERSION for details.
     */
    typedef struct
    {
        UINT32 magic;           /* Magic value */
        UINT32 sizeBytes;       /* Size of this structure in bytes */
        UINT32 version;         /* Version number of this structure */
        UINT32 reserved32_1;    /* Reserved for future use */
        UINT32 reserved32_2;    /* Reserved for future use */
        UINT32 reserved32_3;    /* Reserved for future use */
        UINT32 reserved32_4;    /* Reserved for future use */
        UINT32 reserved32_5;    /* Reserved for future use */
        BITMAPV5HEADER bmi;     /* Windows V5 bitmap info structure */

    } BitmapInfo_t;

    const char *m_name;         /* Bitmap name */
    HANDLE m_hMap;              /* Handle to memory mapping */
    HBITMAP m_hBitmap;          /* Handle to DIB section */
    HDC m_hDC;                  /* Handle to bitmap drawing context */
    BitmapInfo_t *m_infoPtr;    /* Pointer to info block */
    unsigned char *m_bitmapPtr; /* Pointer to start of bitmap data */
    BitmapInfo_t _m_bitmapInfo; /* Info block memory used for local bitmap */
    HANDLE m_mutex;             /* Handle to mutex used for synchronisation */

    /*
     * Private functions.
     */

    SPxErrorCode CreateDIB(BOOL isShared);
    SPxErrorCode CheckSharedMagic(void);
    SPxErrorCode InitSharedMem(unsigned int width,
                               unsigned int height,
                               unsigned int bitsPerPixel);

public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxBitmapWin(void);
    virtual ~SPxBitmapWin(void);

    /* Create a local memory bitmap */
    SPxErrorCode CreateLocal(unsigned int width,
                             unsigned int height,
                             unsigned int bitsPerPixel=32);

    /* Create a shared memory bitmap */
    SPxErrorCode CreateShared(const char *name,
                              unsigned int width,
                              unsigned int height,
                              unsigned int bitsPerPixel=32);

    /* Attach to an existing bitmap */
    SPxErrorCode Attach(const char *name);

    /* Access functions */
    const char *GetBitmapName(void) { return m_name; }
    HBITMAP GetHBitmap(void) const { return m_hBitmap; }
    HDC GetHDC(void) { return m_hDC; }
    void *GetBufferPtr(void) { return m_bitmapPtr; }
    unsigned int GetWidth(void);
    unsigned int GetHeight(void);
    unsigned int GetBitsPerPixel(void);
    void ClearBitmap(void);
    SPxErrorCode Lock(int timeoutMs=-1);
    SPxErrorCode Unlock(void);

    /* Static functions */
    static BOOL IsSharedBitmap(const char *name);
};

/*********************************************************************
 *
 *   Function prototypes
 *
 **********************************************************************/

#endif /* _SPX_BITMAP_WIN_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
