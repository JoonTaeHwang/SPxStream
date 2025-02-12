/*********************************************************************
 *
 * (c) Copyright 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPx Direct2D image with optional brightness and
 *   contrast adjustment.
 *
 * Revision Control:
 *  30/11/18 v1.1    AGC	Initial version.
 *
 * Previous Changes:
 **********************************************************************/

#ifndef _SPX_IMAGE_D2D_H
#define _SPX_IMAGE_D2D_H

/* Needed for general SPx types. */
#include "SPxLibUtils/SPxObj.h"

/* Forward declarations. */
struct ID2D1DeviceContext;
struct ID2D1Bitmap;
struct ID2D1Effect;

class SPxImageD2D : public SPxObj
{
public:
    /* Constructor and destructor */
    explicit SPxImageD2D(void);
    virtual ~SPxImageD2D(void);

    SPxErrorCode SetBitmap(ID2D1DeviceContext *device,
			   ID2D1Bitmap *bitmap);

    SPxErrorCode SetBrightness(int underlayBrightness);
    int GetBrightness(void) const;
    SPxErrorCode SetContrast(int underlayContrast);
    int GetContrast(void) const;

    ID2D1Effect *GetEffect(void);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    SPxErrorCode setBrightness(int underlayBrightness);
    SPxErrorCode setContrast(int underlayContrast);

};

#endif /* _SPX_IMAGE_D2D_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
