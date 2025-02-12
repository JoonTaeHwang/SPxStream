/*********************************************************************
*
* (c) Copyright 2014, 2016, 2020, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxGdiplus class used to automatically initialise
*   and shutdown GDI+.
*
*
* Revision Control:
*   04/03/20 v1.4    AGC	Support colour replacement in SPxGdiplusScaleBitmap().
*
* Previous Changes:
*   09/01/20 1.3    AGC	Add SPxGdiplusSys class.
*   09/09/16 1.2    AGC	Add SPxGdiplusScaleBitmap().
*   20/06/14 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_GDIPLUS_H
#define	_SPX_GDIPLUS_H

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxGdiplusSys base class. */
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Automatically initialise/shutdown GDI+. */
class SPxGdiplus
{
public:
    SPxGdiplus(void);
    ~SPxGdiplus(void);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;
};

/* This version must be dynamically allocated, but will then
 * be auto-freed as part of SPxDeleteAllSystem().
 */
class SPxGdiplusSys : public SPxObj
{
public:
    SPxGdiplusSys(void);
    virtual ~SPxGdiplusSys(void) {}

    SPxAutoPtr<SPxGdiplus> m_gdi;
};

extern HBITMAP SPxGdiplusScaleBitmap(HBITMAP bitmap,
				     int widthPixels,
				     int heightPixels,
				     unsigned int numReplaceColours=0,
				     UINT32 *replaceColours=NULL);

#endif /* _SPX_GDIPLUS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
