/*********************************************************************
 *
 * (c) Copyright 2015, Cambridge Pixel Ltd.
 *
 * File: $RCSfile: SPxSoftButtonQt.h,v $
 * ID: $Id: SPxSoftButtonQt.h,v 1.7 2015/12/04 10:17:14 rew Exp $
 *
 * Purpose:
 *  Header for soft button class under Qt.
 *
 * Revision Control:
 *  04/12/15 v1.7    REW	Support mouseOverBitmap.
 *
 * Previous Changes:
 *  12/06/15 1.6    AGC	Support user specified soft button sizes.
 *  20/02/15 1.5    AGC	Add mutex protection.
 *  12/02/15 1.4    JP 	Reordered variable declarations to match
 *			initialisation in the constructor.
 *  09/02/15 1.3    AGC	Homogenise soft button constructors.
 *  05/02/15 1.2    JP 	Add missing functionalities.
 *  26/01/15 1.1    JP 	Initial version.
 **********************************************************************/
#ifndef _SPX_SOFT_BUTTON_QT_H
#define _SPX_SOFT_BUTTON_QT_H

/*
 * Other headers required.
 */

/* For common types etc. */
#include "SPxLibUtils/SPxCommon.h"

/* For SPxAutoPtr. */

/* For base class. */
#include "SPxLibUtils/SPxSoftButton.h"

/*
 * Types.
 */

/*
 * Forward declarations.
 */
class QPaintDevice;

/* Define our class. */
class SPxSoftButtonQt : public SPxSoftButton
{
public:
    /* Public functions */
    explicit SPxSoftButtonQt(const char *bitmap, int width=-1, int height=-1,
				const char *moBitmap=NULL,
				int moWidth=-1, int moHeight=-1);
    virtual ~SPxSoftButtonQt(void);

    /* Set/get destination to render to */
    SPxErrorCode SetDestination(QPaintDevice *paintDevice);
    SPxErrorCode SetDestination(QPaintDevice *paintDevice, int doNotBlend);

    QPaintDevice* GetDestination(void) const;

protected:
    /* Protected functions. */
    virtual SPxErrorCode paint(double brightness);
    virtual int isMouseOver(int x, int y);

private:
    /* Private variables */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    int getDisplayWidth(void) const;
    int getDisplayHeight(void) const;

}; /* SPxSoftButtonQt */

#endif /* _SPX_SOFT_BUTTON_QT_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
