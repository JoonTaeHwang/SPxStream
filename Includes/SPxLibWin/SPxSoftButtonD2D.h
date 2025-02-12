/*********************************************************************
 *
 * (c) Copyright 2013 - 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *  Header for soft button class under Windows.
 *
 * Revision Control:
 *  19/11/18 v1.8    AGC	High DPI improvements.
 *
 * Previous Changes:
 *  04/12/15 1.7    REW	Support mouseOverBitmap.
 *  12/06/15 1.6    AGC	Support user specified soft button sizes.
 *  20/02/15 1.5    AGC	Add mutex protection.
 *  09/02/15 1.4    AGC	Use string for constructor argument.
 *  26/01/15 1.3    AGC	SoftButton base class no longer declares SetDestination().
 *  11/12/14 1.2    AGC	SetDestination() not longer requires factory.
 *  27/09/13 1.1    AGC	Initial version.
 *
 **********************************************************************/
#ifndef _SPX_SOFT_BUTTON_D2D_H
#define _SPX_SOFT_BUTTON_D2D_H

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For base class. */
#include "SPxLibUtils/SPxSoftButton.h"

/*
 * Types.
 */

/* Forward declarations. */
struct ID2D1Factory;
struct ID2D1RenderTarget;
struct ID2D1Bitmap;

/* Define our class. */
class SPxSoftButtonD2D : public SPxSoftButton
{
public:
    /* Public functions */
    explicit SPxSoftButtonD2D(const char *bitmap, int width=-1, int height=-1,
				const char *mouseOverBitmap=NULL,
				int mouseOverWidth=-1, int mouseOverHeight=-1);
    virtual ~SPxSoftButtonD2D(void);

    /* Set/get destination to render to */
    SPxErrorCode SetDestination(ID2D1RenderTarget *target);
    ID2D1Factory *GetFactory(void) const;
    ID2D1RenderTarget *GetDestination(void) const;

protected:
    /* Protected functions. */
    virtual SPxErrorCode paint(double alpha);
    virtual int isMouseOver(int x, int y);

private:
    /* Private variables */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    int getDisplayWidth(int drawMo) const;
    int getDisplayHeight(int drawMo) const;

}; /* SPxSoftButtonD2D */

#endif /* _SPX_SOFT_BUTTON_D2D_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
