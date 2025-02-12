/*********************************************************************
*
* (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for Direct2D bitmap destination class.
*
* Revision Control:
*   26/08/22 v1.10   AGC	Avoid cppcheck warning.
*
* Previous Changes:
*   18/02/19 1.9    AGC	Use dirty box for clearing.
*   30/11/18 1.8    AGC	Improve scale factor behaviour.
*			Add mutex protection for Direct2D bitmap.
*   19/11/18 1.7    AGC	High DPI improvements.
*   04/06/18 1.6    AGC	Override SetScaleFactor().
*   28/11/17 1.5    AGC	Add CreateD2D().
*   11/05/16 1.4    AGC	Add moving platform reference mode support.
*   05/02/15 1.3    AGC	Remove unused SPxRendererD2D header.
*   11/12/14 1.2    AGC	Add ClearBitmap() overload.
*   27/09/13 1.1    AGC	Initial version.
*
**********************************************************************/

#ifndef _SPX_SC_DEST_BITMAP_D2D_H
#define _SPX_SC_DEST_BITMAP_D2D_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxCriticalSection.h"

/* Required for SPX_SUPPORT_D2D define. */
#include "SPxLibWin/SPxWinD2DUtils.h"

/* Base class header. */
#include "SPxLibSc/SPxScDestBitmap.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
struct ID2D1RenderTarget;
struct ID2D1Bitmap;
struct D2D_RECT_F;

/* SPxScDestBitmapD2D class definition. */
class SPxScDestBitmapD2D : public SPxScDestBitmap
{
public:
    /* Constructor, destructor etc. */
    SPxScDestBitmapD2D(void);
    virtual ~SPxScDestBitmapD2D();
    int Create(UINT16 maxScreenW, 
               UINT16 maxScreenH,
               ID2D1RenderTarget *target=NULL);
    int CreateD2D(ID2D1RenderTarget *target);

    /* Get the D2D bitmap. */
    ID2D1Bitmap *GetBitmap(void);

    /* Render to D2D bitmap. */
    SPxErrorCode UpdateBitmap(void);

    /* Render D2D bitmap to target. */
    SPxErrorCode DrawBitmap(ID2D1RenderTarget *target, const D2D_RECT_F& rect);

    virtual void ClearBitmap(UINT32 val=0);
    virtual void ClearWholeBitmap(UINT32 val=0);

    virtual SPxErrorCode SetScaleFactor(int scaleFactor);
    int GetScaleFactor(void) const { return(1); }
    SPxErrorCode SetRequiredScaleFactor(int scaleFactor);
    virtual int GetRequiredScaleFactor(void) { return m_reqScaleFactor; }

private:
    SPxCriticalSection m_mutex;

#ifdef SPX_SUPPORT_D2D
    ID2D1Bitmap *m_bitmap;
#endif
    int m_reqScaleFactor;
    int m_scaleFactorD2D;

    void RenderBitmap(void);

}; /* SPxScDestBitmapD2D class */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_SC_DEST_BITMAP_D2D_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
