/*********************************************************************
 *
 * (c) Copyright 2012 - 2015, 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *  Header for soft button class under Windows.
 *
 * Revision Control:
 *  06/08/21 v1.8    SP 	Derive from SPxSoftButtonCairo.
 *
 * Previous Changes:
 *  04/12/15 1.7    REW	Support mouseOverBitmap.
 *  12/06/15 1.6    AGC	Support user specified soft button sizes.
 *  20/02/15 1.5    AGC	Add mutex protection.
 *  09/02/15 1.4    AGC	Use string for constructor argument.
 *  26/01/15 1.3    AGC	SoftButton base class no longer declares SetDestination().
 *  14/08/13 1.2    SP 	Use modified paint() function.
 *  09/11/12 1.1    AGC	Initial version.
 *
 **********************************************************************/
#ifndef _SPX_SOFT_BUTTON_GTK_H
#define _SPX_SOFT_BUTTON_GTK_H

/*
 * Other headers required.
 */

/* For common types etc. */
#include "SPxLibUtils/SPxCommon.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For base class. */
#include "SPxLibX11/SPxSoftButtonCairo.h"

/*
 * Types.
 */

/* Forward declarations. */
struct _GdkDrawable;

/* Define our class. */
class SPxSoftButtonGtk : public SPxSoftButtonCairo
{
public:
    /* Public functions */
    explicit SPxSoftButtonGtk(const char *bitmap, int width=-1, int height=-1,
				const char *moBitmap=NULL,
				int moWidth=-1, int moHeight=-1);
    virtual ~SPxSoftButtonGtk(void);

    /* Set/get destination to render to */
    SPxErrorCode SetDestination(struct _GdkDrawable *drawable);
    struct _GdkDrawable *GetDestination(void) const;

protected:
    /* Protected functions. */

private:
    /* Private variables */
    struct impl;
    SPxAutoPtr<impl> m_p;

}; /* SPxSoftButtonGtk */

#endif /* _SPX_SOFT_BUTTON_GTK_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
