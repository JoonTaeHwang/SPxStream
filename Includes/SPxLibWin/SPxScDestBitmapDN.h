/*********************************************************************
*
* (c) Copyright 2019 - 2020 Cambridge Pixel Ltd.
*
* Purpose:
*	Header for DN bitmap destination class.
*
* Revision Control:
*   08/12/20 v1.2    JGB    Renamed to DN instead of WPF.
*
* Previous Changes:
*   20/12/19 1.1    AGC	Initial version.
*
**********************************************************************/

#ifndef _SPX_SC_DEST_BITMAP_DN_H
#define _SPX_SC_DEST_BITMAP_DN_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxCriticalSection.h"

/* Base class header. */
#include "SPxLibSc/SPxScDestBitmap.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */


/* SPxScDestBitmapDN class definition. */
class SPxScDestBitmapDN : public SPxScDestBitmap
{
public:
    typedef SPxErrorCode(*ClearBitmapFn)(UINT32 val, int x, int y, int w, int h);
    typedef SPxErrorCode(*ClearWholeBitmapFn)(UINT32 val);

    /* Constructor, destructor etc. */
    SPxScDestBitmapDN(void);
    virtual ~SPxScDestBitmapDN();

    virtual void ClearBitmap(UINT32 val = 0);
    virtual void ClearWholeBitmap(UINT32 val = 0);

    virtual SPxErrorCode SetScaleFactor(int scaleFactor);
    int GetScaleFactor(void) const { return(1); }
    SPxErrorCode SetRequiredScaleFactor(int scaleFactor);
    virtual int GetRequiredScaleFactor(void);

    void InstallClearBitmapFn(ClearBitmapFn fn);
    void InstallClearWholeBitmapFn(ClearWholeBitmapFn fn);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

}; /* SPxScDestBitmapDN class */

#endif /* _SPX_SC_DEST_BITMAP_DN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
