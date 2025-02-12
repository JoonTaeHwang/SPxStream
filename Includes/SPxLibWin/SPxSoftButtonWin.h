/*********************************************************************
 *
 * (c) Copyright 2012 - 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *  Header for soft button class under Windows.
 *
 * Revision Control:
 *  19/11/18 v1.11   AGC	High DPI improvements.
 *
 * Previous Changes:
 *  26/09/17 1.10   AGC	Support changing bitmap.
 *  04/12/15 1.9    REW	Support mouseOverBitmap.
 *  12/06/15 1.8    AGC	Support user specified soft button sizes.
 *  20/02/15 1.7    AGC	Add mutex protection.
 *  09/02/15 1.6    AGC	Use string for constructor argument.
 *  05/02/15 1.5    AGC	Avoid warnings from newer Windows SDKs.
 *  26/01/15 1.4    AGC	SoftButton base class no longer declares SetDestination().
 *  14/08/13 1.3    SP 	Support rendering without alpha.
 *  24/05/13 1.2    AGC	Fix WINVER undefined warning.
 *  09/11/12 1.1    AGC	Initial version.
 *
 **********************************************************************/
#ifndef _SPX_SOFT_BUTTON_WIN_H
#define _SPX_SOFT_BUTTON_WIN_H

/*
 * Other headers required.
 */

/* For common types etc. */
#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

/* For base class. */
#include "SPxLibUtils/SPxSoftButton.h"

/*
 * Types.
 */

/* Define our class. */
class SPxSoftButtonWin : public SPxSoftButton
{
public:
    /* Public functions */
    explicit SPxSoftButtonWin(const char *bitmap, int width=-1, int height=-1,
				const char *mouseOverBitmap=NULL,
				int mouseOverWidth=-1, int mouseOverHeight=-1);
    virtual ~SPxSoftButtonWin(void);

    /* Set/get destination to render to */
    SPxErrorCode SetDestination(HDC hdc, HWND hwnd=NULL);
    SPxErrorCode SetDestination(HDC hdc, int doNotBlend, HWND hwnd=NULL);
    HDC GetDestination(void) const;

    /* Set bitmap to use. */
    SPxErrorCode SetBitmap(const char *bitmap, int width=-1, int height=-1);

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

}; /* SPxSoftButtonWin */

#endif /* _SPX_SOFT_BUTTON_WIN_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
