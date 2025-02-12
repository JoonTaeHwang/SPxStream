/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *  Header for soft button class using WPF.
 *
 * Revision Control:
 *  07/02/20 v1.1    AGC	Initial version.
 *
 * Previous Changes:
 *
 **********************************************************************/
#ifndef _SPX_SOFT_BUTTON_WPF_H
#define _SPX_SOFT_BUTTON_WPF_H

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
class SPxImage;

/* Define our class. */
class SPxSoftButtonWpf : public SPxSoftButton
{
public:
    typedef SPxErrorCode(*PaintFn)(SPxImage *image, double alpha,
                                   int x, int y, int width, int height);

    /* Public functions */
    explicit SPxSoftButtonWpf(const char *bitmap, int width=-1, int height=-1,
			      const char *mouseOverBitmap=NULL,
			      int mouseOverWidth=-1, int mouseOverHeight=-1);
    virtual ~SPxSoftButtonWpf(void);

    void InstallPaintFn(PaintFn paintFn);

protected:
    /* Protected functions. */
    virtual SPxErrorCode paint(double alpha);
    virtual int isMouseOver(int x, int y);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    int getDisplayWidth(void) const;
    int getDisplayHeight(void) const;

}; /* SPxSoftButtonWPF */

#endif /* _SPX_SOFT_BUTTON_WPF_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
