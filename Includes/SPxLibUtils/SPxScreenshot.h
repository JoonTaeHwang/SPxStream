/*********************************************************************
*
* (c) Copyright 2015, 2020, Cambridge Pixel Ltd.
*
* Purpose:
*   Header file for screenshot utilities.
*
*   Using this file in a Linux application will require that the
*   gdk-pixbuf-x11 library is linked.
*
* Revision Control:
*   07/08/20 v1.2    AGC	Improve support without X11.
*
* Previous Changes:
*   27/08/15 1.1    AGC	Initial version.
**********************************************************************/
#ifndef SPX_SCREENSHOT_H_
#define SPX_SCREENSHOT_H_

/* Include before testing if X11 supported. */
#include "SPxLibUtils/SPxCommon.h"

#if SPX_X11_SUPPORTED
#include "X11/Xlib.h"
#endif

/* Forward declarations. */
class SPxImage;

/* Take a screenshot. */
#if defined(WIN32) || defined(__BORLAND__)
extern SPxErrorCode SPxTakeScreenshot(SPxImage *image,
				      HWND hwnd=NULL,
				      int useScreenArea=TRUE,
				      int bringToFront=TRUE,
				      int captureDropShadow=TRUE);
#else
#if SPX_X11_SUPPORTED
extern SPxErrorCode SPxTakeScreenshot(SPxImage *image,
				      Display *display=NULL,
				      int screenNum=-1,
				      Drawable drawable=0);
#endif
#endif

#endif /* SPX_SCREENSHOT_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/
