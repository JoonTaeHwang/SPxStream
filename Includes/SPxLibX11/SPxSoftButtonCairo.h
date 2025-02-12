/*********************************************************************
 *
 * (c) Copyright 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *  Header for soft button class on Linux using Cairo.
 *
 * Revision Control:
 *  06/08/21 v1.1    SP 	Initial version based on SPxSoftButtonGtk.
 *
 * Previous Changes:
 *
 **********************************************************************/
#ifndef _SPX_SOFT_BUTTON_CAIRO_H
#define _SPX_SOFT_BUTTON_CAIRO_H

/*
 * Other headers required.
 */

/* For common types etc. */
#include "SPxLibUtils/SPxCommon.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For base class. */
#include "SPxLibUtils/SPxSoftButton.h"

/*
 * Types.
 */

/* Forward declarations. */
struct _cairo;

/* Define our class. */
class SPxSoftButtonCairo : public SPxSoftButton
{
public:
    /* Public functions */
    explicit SPxSoftButtonCairo(const char *bitmap, int width=-1, int height=-1,
				const char *moBitmap=NULL,
				int moWidth=-1, int moHeight=-1);
    virtual ~SPxSoftButtonCairo(void);

    /* Set/get Cairo context */
    SPxErrorCode SetCairo(struct _cairo *cr);
    struct _cairo *GetCairo(void) const;

protected:
    /* Protected functions. */
    virtual SPxErrorCode paint(double brightness);
    virtual int isMouseOver(int x, int y);

private:
    /* Private variables */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    int getDisplayWidth(int mo) const;
    int getDisplayHeight(int mo) const;

}; /* SPxSoftButtonCairo */

#endif /* _SPX_SOFT_BUTTON_CAIRO_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
